package ls.unshell;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import ls.hook.Root;
import ls.unshell.MainActivity.MyListAdapter;
import ls.unshell.MainActivity.ViewHolder;
import ls.util.L_IO;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.Signature;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class ChangeDumpSO extends Activity{	
	//定义UI
	private static ListView mlistview;
	private static MyListAdapter myListAdapter;
	Button btn_cancel;
	//
	List<String> listAutoDump;  
/**
 * 
 */
	private void AssetsCopyEnv()
	{
	
	}
/**
 * 			Find_AllFile
 * 开始查找文件
 */
	private void Find_AllFile(String inPath,List<String> inlist)
	{
		if(inlist ==  null)
			return;
		String[] files = null;  
        try {  
            // 获得Assets一共有几多文件  
            files = this.getResources().getAssets().list(inPath);  
        } catch (IOException e1) {  
            return ;  
        }  
        for (String fileName : files) 
        {
            // 根据路径判断是文件夹还是文件  
            if (!fileName.contains(".")) {  
                if (0 == inPath.length()) {  
                	Find_AllFile(fileName, inlist);  
                } else {  
                	Find_AllFile(inPath + "/" + fileName, inlist);  
                }  
                continue;  
            }  

            inlist.add(inPath+"/"+fileName);
		}
	}
/**
 * 			AssetforeachSO
 * 在Assets 的AutoDump目录下搜索SO文件
 */
	private List<String> Assetforeach()
	{
		List<String> mlist = new ArrayList<String>();
		List<String> mresult = new ArrayList<String>();
		Find_AllFile("AutoDump",mlist);
		for (String mfilepath : mlist) {
			if(mfilepath.contains(".so"))
			{
				 Log.i("LsShell", mfilepath);
				 mresult.add(mfilepath);
				 
			}
		}
		mlist.clear();
		return mresult;		
	}
	/**
	 * 
	 * @param inAsster
	 * @param inSaveName
	 */
	private void CopyFile(String inAsster,String inSaveName)
	{
		Root.del("/data/local/MHookDemo/"+inSaveName);
		Root.del("/data/data/ls.mhookdemo/"+inSaveName);
		L_IO.Asstercopy(getApplicationContext(),inAsster,"/data/data/ls.mhookdemo/"+inSaveName);	
		Root.copy("/data/local/MHookDemo/"+inSaveName,"/data/data/ls.mhookdemo/"+inSaveName);
		Root.SUchmod_777("/data/local/MHookDemo/"+inSaveName);
	}
	/**
	 * 
	 * @param inAsster
	 * @param inSaveName
	 */
	private void CopyAutoDump()
	{
		Root.del("/data/local/MHookDemo/AutoDump.so");
		Root.copy("/data/local/MHookDemo/AutoDump.so","/data/data/ls.mhookdemo/lib/libAutoDump.so");
		Root.SUchmod_777("/data/local/MHookDemo/AutoDump.so");
	}
	/**
	 *		Init_UI
	 * 	初始化UI* 
	 */
	private void Init_UI()
	{
		mlistview = (ListView)findViewById(R.id.list_view);
		btn_cancel = (Button)findViewById(R.id.button_Chancel);
		btn_cancel.setOnClickListener(onh);
		listAutoDump = Assetforeach();
		myListAdapter = new MyListAdapter(listAutoDump);
		mlistview.setAdapter(myListAdapter);
		mlistview.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				ViewHolder viewHolder = (ViewHolder)view.getTag();
				CopyFile(viewHolder.name.getText()+"","AutoDump.so");
				Log.i("LsShell", viewHolder.name.getText()+"");
				finish();
			}
		});
	}
	/*
	 *					onCreate
	 * (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_changeso);
		Init_UI();
	}
	
/**
 * 按键监控
 */	
    OnClickListener onh = new OnClickListener(){
		@Override
		public void onClick(View inView) {
			if(btn_cancel == inView)
			{
				//复制自动识别SO到MHookDemo目录
				CopyAutoDump();
				CopyFile("AutoDump/Ali.so","Ali.so");
				CopyFile("AutoDump/NAGA.so","NAGA.so");
				CopyFile("AutoDump/QH360.so","QH360.so");
				CopyFile("AutoDump/Baidu.so","Baidu.so");
				CopyFile("AutoDump/PayEgis.so","PayEgis.so");
				CopyFile("AutoDump/Bangcle.so","Bangcle.so");
				finish();
			}
		}
    };
    
    static class ViewHolder{  
        TextView name;  
    }  
    class MyListAdapter extends BaseAdapter
    {	
    	List<String> mList;
    	public MyListAdapter(List<String> list){  
    		mList = new ArrayList<String>();  
    		mList = list;  
		}    

		@Override
		public int getCount() {
			// TODO Auto-generated method stub
		      return mList.size();  
		}

		@Override
		public Object getItem(int position) {
			// TODO Auto-generated method stub
		      return mList.size();  
		}

		@Override
		public long getItemId(int position) {
			// TODO Auto-generated method stub
		      return mList.size();  
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			ViewHolder holder = null;  	         
        	if (convertView == null) 
        	{  
        		LayoutInflater mInflater = (LayoutInflater)   
	                      getSystemService(Context.LAYOUT_INFLATER_SERVICE);  
        		convertView = mInflater.inflate(R.layout.listitem, null);  
        		holder = new ViewHolder();  
        		holder.name = (TextView)convertView.findViewById(R.id.list_name);  
        		convertView.setTag(holder);  
        	}else{  
        		holder = (ViewHolder)convertView.getTag();  
        	}  
     
        	holder.name.setText(mList.get(position));      
        	return convertView;  
		}  
    	
    }
}

package ls.unshell;
import java.io.*;
import java.util.*;

import ls.hook.Root;
import ls.unshell.R;
import ls.util.ConfigINFO;
import android.view.*;
import android.view.View.OnClickListener;
import android.widget.*;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.widget.AdapterView.OnItemClickListener;

public class MainActivity extends Activity {

	//定义UI
	private static List<PackageInfo> listdatas = new ArrayList<PackageInfo>();
	private static ListView mlistview ;
	private static MyListAdapter myListAdapter;
	private Button button_Substrace;
	private Button button_Mode;
	private Button button_Auto;
	//
	PackageManager pManager; 
	public final static String Dir_PATH = "/data/local/MHookDemo/";
	public final static String FILE_PATH = "/data/local/MHookDemo/Config";
	public ConfigINFO Config = null;
/**
 *		Init_UI
 * 	初始化UI* 
 */
	private void Init_UI(){
		Activity mAct = new Activity();
		mlistview = (ListView)findViewById(R.id.list_view);
		button_Substrace = (Button)findViewById(R.id.button_Substrace);
		button_Mode = (Button)findViewById(R.id.button_Mode);
		button_Auto = (Button)findViewById(R.id.button_Auto);
		button_Mode.setText(Config.getModeString());
		
		listdatas = getAllApps(MainActivity.this);
		myListAdapter = new MyListAdapter(listdatas);
		mlistview.setAdapter(myListAdapter);
		mlistview.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				ViewHolder viewHolder = (ViewHolder)view.getTag();
				viewHolder.selected.toggle();
				myListAdapter.map.put(position, viewHolder.selected.isChecked());				
			}
		});
		button_Substrace.setOnClickListener(onh);
		button_Mode.setOnClickListener(onh);
		button_Auto.setOnClickListener(onh);
		
		if(Config.getModeString().equals(ConfigINFO.MODE_AUTODUMP))
		{
			button_Auto.setEnabled(true);
		}else{
			button_Auto.setEnabled(false);
		}
	}
	/*
	 * 				onCreate
	 * (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		File mfile = new File(Dir_PATH);
		if(!mfile.exists())
		{
			//申请Root权限进行创建目录
			Root.SUmkdir(Dir_PATH);
		}
		//文件权限应该在创建时候就应该初始化完毕
		Config = new ConfigINFO(FILE_PATH);
		pManager = getPackageManager();
		Init_UI();	
	}
/**
 *	获取手机内所有应用
 * @param context
 */
	public static List<PackageInfo> getAllApps(Context context) 
	{
		 
        List<PackageInfo> apps = new ArrayList<PackageInfo>();
        PackageManager pManager = context.getPackageManager();
        List<PackageInfo> packlist = pManager.getInstalledPackages(0);
        for (int i = 0; i < packlist.size(); i++) {
            PackageInfo pak = (PackageInfo) packlist.get(i);
            // 判断是否为非系统预装的应用程序
            // 这里还可以添加系统自带的，这里就先不添加了，如果有需要可以自己添加
            // if()里的值如果<=0则为自己装的程序，否则为系统工程自带
            if ((pak.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) <= 0) 
            {
                apps.add(pak);
            }            
        }
        return apps;
    }
/**
 * 按键监控
 */
    OnClickListener onh = new OnClickListener(){
		@Override
		public void onClick(View inView) {
			
			System.loadLibrary("SubstrateHook.cy");   
			Root.del("/data/local/MHookDemo/libSubstrateHook.cy.so");
			Root.copy("/data/local/MHookDemo/libSubstrateHook.cy.so","/data/data/ls.mhookdemo/lib/libSubstrateHook.cy.so");
			Root.SUchmod_777("/data/local/MHookDemo/libSubstrateHook.cy.so");
			if(inView == button_Mode){
				Config.setNextMode();
				button_Mode.setText(Config.getModeString());
				if(Config.getModeString().equals(ConfigINFO.MODE_AUTODUMP))
				{
					button_Auto.setEnabled(true);
				}else{
					button_Auto.setEnabled(false);
				}
				return ;
			}else if(inView == button_Auto)
			{
				if(!Config.getModeString().equals(ConfigINFO.MODE_AUTODUMP))
				{
					Toast.makeText(MainActivity.this, "模式失败！", Toast.LENGTH_SHORT).show();
					return ;
				}
				//
				Intent intent = new Intent();  
                intent.setClass(MainActivity.this, ChangeDumpSO.class);  
                //启动Activity  
                startActivity(intent);  
			}
			//创建文件并且设置权限
			Root.SUcreateFile(FILE_PATH);
			//设置文件内容				
			StringBuilder mBuilder = new StringBuilder();
			Config.App.clear();
			for (int i = 0; i < listdatas.size(); i++) {
				if (myListAdapter.map.get(i)) {
					Config.App.add(listdatas.get(i).packageName);
				}
			}
			System.out.println(mBuilder.toString());
			try {		
				Config.writeConfig(FILE_PATH);
				Toast.makeText(MainActivity.this, "写入成功", Toast.LENGTH_SHORT).show();
				if(Config.getModeString().equals(ConfigINFO.MODE_TestDUMP)){
					Root.copy("/data/local/MHookDemo/TestDump.so","/data/data/ls.mhookdemo/lib/libShell_TestDump.so");
					Root.SUchmod_777("/data/local/MHookDemo/TestDump.so");
				}else if(Config.getModeString().equals(ConfigINFO.MODE_DEBUG)){
					Root.copy("/data/local/MHookDemo/Shell_Debug.so","/data/data/ls.mhookdemo/lib/libShell_Debug.so");
					Root.SUchmod_777("/data/local/MHookDemo/Shell_Debug.so");
				}
			} catch (IOException e) {
				Toast.makeText(MainActivity.this, "文件写入失败", Toast.LENGTH_SHORT).show();
				e.printStackTrace();
			}
		}  	
    };
/**
 * 自定义ListView适配器  
 */
    static class ViewHolder{  
        CheckBox selected;  
        TextView name;  
    }  
    class MyListAdapter extends BaseAdapter
    {  
    	List<PackageInfo> listPerson;  
    	public  HashMap<Integer,Boolean> map;   
  		public MyListAdapter(List<PackageInfo> list){  
  			listPerson = new ArrayList<PackageInfo>();  
  			listPerson = list;  
  			map = new HashMap<Integer,Boolean>();
  			for (int i = 0; i < list.size(); i++) {
  				if(Config.App.contains(list.get(i).packageName))
  					map.put(i, true);
  				else {
  					map.put(i, false);
  				}
  			}
  		}    
        @Override  
        public int getCount() {  
            return listPerson.size();  
        }    
        @Override  
        public Object getItem(int position) {  
            return listPerson.get(position);  
        }  
        @Override  
        public long getItemId(int position) {  
            return position;  
        }  
        /*
         * (non-Javadoc)
         * @see android.widget.Adapter#getView(int, android.view.View, android.view.ViewGroup)
         */
        @Override  
        public View getView(int position, View convertView, ViewGroup parent) {  
        	ViewHolder holder = null;  	         
        	if (convertView == null) 
        	{  
        		LayoutInflater mInflater = (LayoutInflater)   
	                      getSystemService(Context.LAYOUT_INFLATER_SERVICE);  
        		convertView = mInflater.inflate(R.layout.checkbox_listitem, null);  
        		holder = new ViewHolder();  
        		holder.selected = (CheckBox)convertView.findViewById(R.id.list_CheckBox);  
        		holder.name = (TextView)convertView.findViewById(R.id.list_name);  
        		convertView.setTag(holder);  
        	}else{  
        		holder = (ViewHolder)convertView.getTag();  
        	}  
        	holder.selected.setChecked(map.get(position));  
        	holder.name.setText(pManager.getApplicationLabel(listPerson.get(position).applicationInfo));      
        	return convertView;  
        }      
	}
/*
 * 加载库，
 */
	static 
	{  
		//System.loadLibrary("SubstrateHook.cy");   
	}
}

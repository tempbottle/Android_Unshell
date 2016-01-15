package ls.util;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class ConfigINFO {
	//
	public int Mode = 0;
	public String Info = "";
	public static List<String> App = null;
/**
 * 初始化配置文件	
 * @param inPath 配置文件路径
 */
	public ConfigINFO(String inPath)
	{
		readConfig(inPath);
	}
/**
 * 读取配置文件
 * @param inPath 配置文件路径
 */
	public void readConfig(String inPath)
	{
		App = new ArrayList<String>();
		Info = L_IO.readString(inPath);
		if(Info == null){
			Info = "";
			return ; 
		}
		String[] m_str = Info.split(",");		
		for(int m_i = 0;m_i < m_str.length;m_i++){
			if(m_str[m_i].contains("MODE:"))
				setMode(m_str[m_i]);				
			else if(m_str[m_i].contains("APP:"))
			{
				String mPackName = m_str[m_i].substring("APP:".length());
				App.add(mPackName);
			}
		}
	}
/**
 * 
 * @param inPath
 * @throws IOException
 */
	public void writeConfig(String inPath) throws IOException
	{
		String mStr  = "";
		mStr = "MODE:" +getModeString()+",";
		for(int m_i = 0;m_i < App.size();m_i++)
		{
			mStr += "APP:" + App.get(m_i)+",";
		}
		L_IO.writeString(mStr, inPath);
	}
/**
 * 获取模式字符串
 * @return 模式字符串
 */
	public String getModeString()
	{
		if(Mode == 0)
			return MODE_CLOSE;
		else if(Mode == 1)
			return MODE_DEBUG;
		else if(Mode == 2)
			return MODE_TestDUMP;
		else if(Mode == 3)
			return MODE_AUTODUMP;
		return MODE_CLOSE;
	}
/**
 * 设置下一个模式
 */
	public void setNextMode()
	{
		Mode++;
		if(Mode >= MaxMode)
		{
			Mode = 0;
		}
	}
	
/**
 * 设置模式
 * @param inStr
 */
	public void setMode(String inStr)
	{
		if(inStr.contains(MODE_CLOSE))
			Mode = 0; 
		else if(inStr.contains(MODE_DEBUG))
			Mode = 1; 
		else if(inStr.contains(MODE_TestDUMP))
			Mode = 2;
		else if(inStr.contains(MODE_AUTODUMP))
			Mode = 3;
		else 
			Mode = 0; 
	}
//
	private final static int MaxMode = 4;
	public final static String MODE_CLOSE = "CLOSE";
	public final static String MODE_DEBUG = "DEBUG";
	public final static String MODE_TestDUMP =  "TestDump";
	public final static String MODE_AUTODUMP = "AutoDump";
}

package ls.Debug;
import android.util.Log;
/**
 * 				DebugLog
 * 打印日志
 * @author LsMouse
 */
public class DebugLog {
	public static final String TAG = "Lslog";
	
	public static void Out(boolean inEn){
		Log.i(TAG,inEn+"");
	}
	public static void Out(Boolean inEn){
		Log.i(TAG,inEn+"");
	}
	public static void Out(int inInt){
		Log.i(TAG,inInt+"");
	}
	public static void Out(long inInt){
		Log.i(TAG,inInt+"");
	}
	public static void Out(String inStr){
		Log.i(TAG,inStr);
	}
	public static void Out(Object inObject){
		if(inObject == null){
			Log.i(TAG,"inObject is null");
			return ;
		}
		Log.i(TAG,"inObject"+inObject.getClass());
		Log.i(TAG,"inObject"+inObject.toString());
	}
	public static void Out(Object[] inObject){
		if(inObject == null){
			Log.i(TAG,"inObject[] is null");
			return ;
		}else if(0 == inObject.length){
			Log.i(TAG,"inObject[] size is 0");
			return ;
		}
		Log.i(TAG,"inObject[] size is "+inObject.length);
		int m_i = 0;
		while(m_i < inObject.length){
			Out(inObject[m_i]);
			m_i++;
		}		
	}
}

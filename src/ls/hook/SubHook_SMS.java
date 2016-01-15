package ls.hook;
import java.lang.reflect.Method;
import android.app.PendingIntent;
import android.util.Log;
import com.saurik.substrate.MS;  

public class SubHook_SMS implements MS.ClassLoadHook{
	private static SubHook_SMS smsHookClassLoader;  
    public  SubHook_SMS() {  
        super();  
    }  
    public static SubHook_SMS getInstance(){  
        if (smsHookClassLoader == null) {  
            smsHookClassLoader = new SubHook_SMS();  
        }  
        return smsHookClassLoader;  
    }  
      
    @SuppressWarnings({ "unchecked", "rawtypes" })  
    public void classLoaded(Class<?> SmsManager) {  
        //code to modify the class when loaded  
        Method sendTextMessage;  
        try {  
            sendTextMessage = SmsManager.getMethod("sendTextMessage",   
                    new Class[]{String.class,String.class,String.class,PendingIntent.class,PendingIntent.class});  
              
        } catch (NoSuchMethodException e) {  
            sendTextMessage = null;  
        }  
        MS.hookMethod(SmsManager, sendTextMessage, new MS.MethodAlteration() {  
            public Object invoked(Object _this,Object... _args) throws Throwable{  
                Log.i("MHook","SEND_SMS");  
                Log.i("MHook","destination:"+_args[0]);  
                Log.i("MHook","source:"+_args[1]);  
                Log.i("MHook","text:"+_args[2]);  
                return invoke(_this, _args);  
            }  
        });  
    }  
}

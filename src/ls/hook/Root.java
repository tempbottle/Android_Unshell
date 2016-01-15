package ls.hook;

import java.io.DataOutputStream;
import java.io.IOException;

public class Root {
/**
*		upgradeRootPermission
* 应用程序运行命令获取 Root权限，设备必须已破解(获得ROOT权限)
* 将路径设置为777权限
* @param 文件路径
* @return 应用程序是/否获取Root权限
*/
	public static boolean SUchmod_777(String inFilePath) {
		try {
			SUShell("chmod 777 "+inFilePath);
		} catch (Exception e1) {
			e1.printStackTrace();
			return false;
		}
        return true ;
	}
/**
 * 		SuCopyShell
 * 使用Root权限进行复制文件
 * @param inDest -> 目的路径
 * @param inSrc　-> 源路径
 */
	public static boolean copy(String inDest,String inSrc){
		try {
			SUShell("cp "+inSrc+" "+ inDest);
		} catch (Exception e1) {
			e1.printStackTrace();
			return false;
		}
		return true ;
	}
/**
 * 				SUcreateFile
 * 使用root创建文件
 * @param inPathName
 */
	public static boolean SUcreateFile(String inPathName){
		try {
			SUShell("> " + inPathName);
		} catch (Exception e1) {
			e1.printStackTrace();
			return false;
		}
		SUchmod_777(inPathName);
        return true;
	}
/**	
 * 				SUmkdir
 * 使用Root权限，创建目录，并且将目录设置RWX权限。
 * @param inDirPath -> 文件目录
 */
	public static boolean SUmkdir(String inDirPath)
	{
		try {
			SUShell("mkdir " + inDirPath);
		} catch (Exception e1) {
			e1.printStackTrace();
			return false;
		}
		SUchmod_777(inDirPath);
        return true;
	}
/**	
 * 				del
 * 使用Root权限删除文件或目录
 * 
 */	
	public static boolean del(String inPath){
		try {
			SUShell("rm -rf " + inPath);
		} catch (Exception e1) {
			e1.printStackTrace();
			return false;
		}
        return true;
	}
/**
 * 
 * @param inShellCode
 * @throws IOException 
 * @throws InterruptedException 
 */
	public static void SUShell(String inShellCode) throws Exception
	{
		Process process = null ;
        DataOutputStream os = null ;
        process = Runtime.getRuntime().exec( "su");
        os = new DataOutputStream(process .getOutputStream());
        os.writeBytes(inShellCode + "\n");
        os.writeBytes("exit\n" );
        os.flush();
        process.waitFor();
	}
}

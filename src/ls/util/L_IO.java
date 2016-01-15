package ls.util;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;

import ls.hook.Root;

@TargetApi(Build.VERSION_CODES.GINGERBREAD)
public class L_IO {
 /**
 * 
 * @param fileName
 * @return
 */
	public  static String readString(String fileName) {
		File file = new File(fileName);
		if (!file.exists()) {
			try {
				file.createNewFile();
				file.setReadable(true, false);
				file.setWritable(true, false);
				
			} catch (IOException e) {

				e.printStackTrace();
			}
		}
		BufferedReader reader = null;
		StringBuilder builder = new StringBuilder();
		try {
			reader = new BufferedReader(new FileReader(file));
			String tempString = null;
			while ((tempString = reader.readLine()) != null) {
				builder.append(tempString);// +System.getProperties().getProperty("line.separator")
			}
			reader.close();
			return builder.toString();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (reader != null) {
				try {
					reader.close();
				} catch (IOException e1) {
				}
			}
		}
		return null;
	}
/**
 * 
 * @param string
 * @param filename
 * @throws IOException
 */
	public static void writeString(String string, String filename) throws IOException {

		FileWriter writer = new FileWriter(filename, false);
		writer.write(string);
		writer.close();
	}
/**
  * 
  * @param myContext
  * @param ASSETS_NAME 要复制的文件名
  * @param savePath 要保存的路径
  * @param saveName 复制后的文件名
  *  testCopy(Context context)是一个测试例子。
  */
	public static void Asstercopy(Context myContext, String ASSETS_NAME,
			String saveName) 
	{
		try {
			if (!(new File(saveName)).exists()) 
			{
				InputStream is;
				is = myContext.getResources().getAssets().open(ASSETS_NAME);
				if(is == null)return ;
				FileOutputStream fos = new FileOutputStream(saveName);
				byte[] buffer = new byte[7168];
				int count = 0;
				while ((count = is.read(buffer)) > 0) {
					fos.write(buffer, 0, count);
				}
				fos.close();
				is.close();
			}	
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

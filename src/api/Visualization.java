package api;

import java.io.IOException;
import java.util.Map;
import java.util.TreeMap;

import org.vde.database.Database;

public class Visualization {
	

	String [] datasets = {"flight","sensordata"};//"flight","sensordata"};//,"taxitrip2011","sensordata"};//"flight",,"airline","taxitrip2011"};
	public Map<String,Database> dbSets;
	public Visualization()
	{
		dbSets = new TreeMap<String,Database>();
		String schemafile;
		String datafile;
		for(String data:datasets)
		{
			if(getOsName().toLowerCase().contains("windows"))
			{
				schemafile = "C:\\Users\\srahman7\\workspace\\datarepo\\" + data + "_schema.csv";
				datafile = "C:\\Users\\srahman7\\workspace\\datarepo\\" + data + ".csv";
			}
			else
			{
				schemafile = "/media/srahman7/OS/Users/srahman7/workspace/datarepo/" + data + "_schema.csv";
				datafile = "/media/srahman7/OS/Users/srahman7/workspace/datarepo/" + data + ".csv";
				
			}
			
			System.out.println("-------------------------" + data + "-------------------------");
			
			try {
				if(data.equals("airline"))
					dbSets.put(data, new Database(data, schemafile, datafile, 25000000));
				else
					dbSets.put(data, new Database(data, schemafile, datafile, 15000000));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		
		
		
	}
	

	   public  String getOsName()
	   {
		String OS = null;
	      if(OS == null) {
	    	  OS = System.getProperty("os.name"); 
	    	  }
	      return OS;
	   }
	   
	
	
	
}

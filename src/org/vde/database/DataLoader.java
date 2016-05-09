package org.vde.database;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.text.NumberFormat;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.SerializationConfig.Feature;

public class DataLoader {
	
	private Database database;
//	private String schemafilename="src/main/resources/schema-test.csv";
//	private String datafilename="src/main/resources/data-test.csv";
	private static String schemafilename="src/student_schema.txt";
	private static String datafilename="src/student_data.csv";
//	private String schemafilename="src/main/resources/schema1.txt";
// 	private String datafilename="src/main/resources/2008.csv";
	private ObjectMapper mapper = new ObjectMapper();
	

    
    public static Database createDatabase(String name,String schemafile,String datafile) throws IOException, InterruptedException{
    	Database database = new Database(name,schemafile,datafile);
    	return database;

  
    }
    
    public static Database createDatabase(String name) throws IOException, InterruptedException{
    	Database database = new Database(name,schemafilename,datafilename);
    	return database;

  
    }
	

}

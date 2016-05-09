package org.vde.database;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.roaringbitmap.RoaringBitmap;
import org.vde.database.Column;
import org.vde.database.DatabaseMetaData;
import org.vde.database.Query.FilterPredicate;
import  org.vde.database.ColumnMetadata;

public class Database {
	private String name;
	private Map<String,Column> columns= new HashMap<String,Column>();
	public DatabaseMetaData databaseMetaData= new DatabaseMetaData();
	public static long rowCount;
	
	public Database(String name,String schemafilename,String datafilename) throws IOException, InterruptedException{
		this.name=name;
		readSchema(schemafilename);
		loadData(datafilename);
		DatabaseCatalog.addDatabase(name, this);		
	}
	
	public Database(String name,String schemafilename,String datafilename,int limit) throws IOException, InterruptedException{
		this.name=name;
		readSchema(schemafilename);
		loadData(datafilename,limit);
		DatabaseCatalog.addDatabase(name, this);		
	}
	
	public Database(String name,String schemafile) throws IOException, InterruptedException{
		this.name=name;
		readSchema(schemafile);
		DatabaseCatalog.addDatabase(name, this);	
	}
                                                              
	
	public Map<String, Column> getColumns() {
		return columns;
	}

	public void addValue(String columnName,int row,String value){
		
		Column column=columns.get(columnName);
		column.add(row, value);
 	}
	
	
	private void readSchema(String schemafilename) throws IOException, InterruptedException{
   	 BufferedReader bufferedReader = new BufferedReader(new FileReader(schemafilename));
	 String line;
	 while ((line = bufferedReader.readLine()) != null){
			 ColumnMetadata columnMetadata= new ColumnMetadata();
			 String[] sections=line.split(":");
			 columnMetadata.name=sections[0];
			 String[] terms=sections[1].split(",(?=([^\"]*\"[^\"]*\")*[^\"]*$)", -1);
			 columnMetadata.isIndexed=true;			 
			 columnMetadata.dataType=terms[0];
			 if("indexed".equals(terms[1])){
				 columnMetadata.isIndexed=true;
			 }
			 else{
				 columnMetadata.isIndexed=false;
				 columnMetadata.c_min = Float.parseFloat(terms[6].trim());
			       columnMetadata.c_max = Float.parseFloat(terms[7].trim());
			 }		 
			 
		       if(terms[2].equals("T")){
		    	   databaseMetaData.xAxisColumns.put(columnMetadata.name,columnMetadata);	    	   
		       }
		       if(terms[3].equals("T")){
		    	   databaseMetaData.yAxisColumns.put(columnMetadata.name,columnMetadata);	    	   
		       }
		       
		       if(terms[4].equals("T")){
		    	   databaseMetaData.zAxisColumns.put(columnMetadata.name,columnMetadata);	    	   
		       }
		       
		       if(terms[5].trim().equals("T")){
		    	   databaseMetaData.predicateColumns.put(columnMetadata.name,columnMetadata);	    	   
		       }
		       if(terms[8].equals("T")){
		    	   databaseMetaData.trendDimensions.put(columnMetadata.name,columnMetadata);	    	   
		       }
		       if(terms[9].equals("T")){
		    	   databaseMetaData.heatDimensions.put(columnMetadata.name,columnMetadata);	    	   
		       }
		       
		       
		       
		       Column column = new Column(columnMetadata, this);		 
			 
		 }
		 
		bufferedReader.close();		
	}
	
	

    private void loadData(String datafilename) throws IOException{
      	BufferedReader bufferedReader = new BufferedReader(new FileReader(datafilename));
		String line;
		line = bufferedReader.readLine();
		String[] header=line.split(",");
		int count=0;
		 String[] terms;
		while ((line = bufferedReader.readLine()) != null){
			 terms=line.split(",(?=([^\"]*\"[^\"]*\")*[^\"]*$)", -1);
            for(int i=0;i<header.length;i++){
            	//System.out.println(line);
           	     addValue(header[i].trim(), count, terms[i].trim());
                }
            count=count+1;	
            if(count%1000000==0)
            	System.out.println("Loaded "+count+" number of rows");
           
		 }
		this.rowCount=count;
		 
		bufferedReader.close();
	}
    
    private void loadData(String datafilename, int limit) throws IOException{
      	BufferedReader bufferedReader = new BufferedReader(new FileReader(datafilename));
		String line;
		line = bufferedReader.readLine();
		//System.out.println(line);
		String[] header=line.split(",(?=([^\"]*\"[^\"]*\")*[^\"]*$)", -1);
		int count=0;
		 String[] terms;
		 while ((line = bufferedReader.readLine()) != null){
			 terms=line.split(",(?=([^\"]*\"[^\"]*\")*[^\"]*$)", -1);
			 if(header.length==terms.length)
			 {
	            for(int i=0;i<header.length;i++){
	            	//System.out.println(line+"--"+i+"--"+header[i]);//+"---"+terms[i]);
	            	
	            	 	addValue(header[i].trim(), count, terms[i].trim());
	            		
	            	 
	                }
	            count=count+1;
			 }
            if(count%1000000==0)
            	System.out.println("Loaded "+count+" number of rows");
           
            if(count >limit){
            	break;
            }
		 }
		this.rowCount=count;
		 
		bufferedReader.close();
	}
    
    public Object getColumns(FilterPredicate filterPredicate){
		Column column=columns.get(filterPredicate.getPropertyName());
		return column.getValues(filterPredicate);
	 }
 
    
    public Object getColumns(String columnName){
		Column column = columns.get(columnName);
		
		return column.getValues();
	 }
 
   
    public ColumnMetadata getColumnMetaData(String columnName){
		Column column = columns.get(columnName);
		return column.columnMetadata;
			
     }
 
   public DatabaseMetaData getFormMetdaData(){
	  return databaseMetaData;
      }
   
	public  Map<String,RoaringBitmap> getIndexedColumnValues(String columnName) {
		
	    return (Map<String,RoaringBitmap>)getColumns(columnName);
	}
	
	public  String[]  getUnIndexedColumnValues(String columnName) {
		//System.out.println(columnName);
	    return (String[])getColumns(columnName);
	}

}

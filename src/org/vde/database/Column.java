package org.vde.database;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.BitSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.roaringbitmap.RoaringBitmap;
import org.vde.database.ColumnMetadata;
import org.vde.database.Query.FilterOperator;
import org.vde.database.Query.FilterPredicate;

public class Column {
	public ColumnMetadata columnMetadata= new ColumnMetadata();
	private Object columnValue;
	public Column(ColumnMetadata columnMetadata,Database database){
		this.columnMetadata=columnMetadata;
		database.getColumns().put(columnMetadata.name, this);
		if(columnMetadata.isIndexed==true){
			columnValue=new IndexedColumnValues();
		}
		else{
			columnValue=new UnIndexedColumnValues();
		}
	}
	
	public String getName() {
		return columnMetadata.name;
	}
	
	public ColumnMetadata getColumnMetadata() {
		return columnMetadata;
	}
	
	public void setColumnMetadata(ColumnMetadata columnMetadata) {
		this.columnMetadata = columnMetadata;
	}
	
	public Object getColumnValue() {
		return columnValue;
	}
	
	public void setColumnValue(Object columnValue) {
		this.columnValue = columnValue;
	}

	
	//TODO: Change from string to value
	//TODO: Change from List to array for values;
	public void add(int row,String value){
		if(columnMetadata.isIndexed){
			addIndexedValue(row,value);
		}
		else{
			addUnIndexedValue(row,value);
		}

		 if(columnMetadata.dataType.equals("int") || columnMetadata.dataType.equals("float") ){
			 Float num=Float.parseFloat(value);	 		 
			 if (num<columnMetadata.min)
				 columnMetadata.min=num;
			 if (num>columnMetadata.max)
				 columnMetadata.max=num;		 
		 }
		 
			
		
	}
	
	
	private void addUnIndexedValue(int row, String value) {
		UnIndexedColumnValues unIndexedColumnValues = (UnIndexedColumnValues)columnValue;
		unIndexedColumnValues.insertValue(row, value);

	}
	
	
	private void addIndexedValue(int row, String value) {
		IndexedColumnValues indexedColumnValues = (IndexedColumnValues)columnValue;
		indexedColumnValues.add(row,value);			 
	}

	  
	 
	public Object getValues(){
			if(columnMetadata.isIndexed){
				return getIndexedValues();
			}
			else{
				return getUnIndexedValues();
			}
				
			
		}

	public Object getValues(FilterPredicate filterPredicate){
		if(columnMetadata.isIndexed){
			return getIndexedValues(filterPredicate);
		}
		else{
			return getUnIndexedValues(filterPredicate);
		}			
		
	}
	
	private Object getIndexedValues(){
		
		IndexedColumnValues indexedColumnValues = (IndexedColumnValues)columnValue;
		Map<String,RoaringBitmap> valuesCopy = new HashMap<String, RoaringBitmap>(); 
		return indexedColumnValues.columnValues;
	/*	for(String key:indexedColumnValues.columnValues.keySet()){
			valuesCopy.put(key,(RoaringBitmap)indexedColumnValues.columnValues.get(key).clone());
		}	
		return valuesCopy;*/
	}
	
	private Object getUnIndexedValues(){
		 UnIndexedColumnValues columnValues = (UnIndexedColumnValues)columnValue;
		 return columnValues.getColumnValues();
		// String[] columnValuesCopy= Arrays.copyOf(columnValues.getColumnValues(), columnValues.getColumnValues().length);
	//	 return columnValuesCopy;
		
	}
	
	private Object getIndexedValues(FilterPredicate filterPredicate){
		IndexedColumnValues indexedColumnValues = (IndexedColumnValues)columnValue;
		String value=String.valueOf(filterPredicate.getValue());
		RoaringBitmap values=null;
		for(String key: indexedColumnValues.columnValues.keySet()){
			if(filterPredicate.getOperator()==FilterOperator.EQUAL&& key==value)
				values=(RoaringBitmap)indexedColumnValues.columnValues.get(key).clone();
			 
		}
		return values;
	}

	
	private Object getUnIndexedValues(FilterPredicate filterPredicate){
		return null;
	}
	


	

}

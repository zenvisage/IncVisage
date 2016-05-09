package org.vde.database;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class UnIndexedColumnValues {
	private String[] columnValues;
	private int initial_capacity=1000000;
	private int length;

	public UnIndexedColumnValues() {
		columnValues=new String[initial_capacity];
		length=initial_capacity;
	}

	public String[] getColumnValues() {
		return columnValues;
	}

	public void setColumnValues(String[] columnValues) {
		this.columnValues = columnValues;
	}
	
	public void insertValue(int row,String value){
		if(row >= this.length){
			extendArray();
		}
		this.columnValues[row]=value;
		
	}
	
	 protected void extendArray() {
		  this.columnValues = Arrays.copyOf(this.columnValues, this.columnValues.length+initial_capacity);
	      this.length=this.columnValues.length;	        
	    }

}

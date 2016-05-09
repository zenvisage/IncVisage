package org.vde.database;

import java.io.IOException;
import java.util.List;
import java.util.Map;

import org.roaringbitmap.IntIterator;
import org.roaringbitmap.RoaringBitmap;
import org.roaringbitmap.SampledResult;
import org.roaringbitmap.SampledResult.SampledResultIntIterator;
import org.vde.database.Query.FilterPredicate;

public class NewExecutor {
	private static  Database database;
	
	public NewExecutor(Database database){
		this.database=database;
	}
	
	private static RoaringBitmap getIndexedColumnValues(FilterPredicate filterPredicate1) {
	    return (RoaringBitmap)database.getColumns(filterPredicate1);
	}
	
	private static Map<String,RoaringBitmap> getIndexedColumnValues(String columnName) {
	    return (Map<String,RoaringBitmap>)database.getColumns(columnName);
	}
	
	private static List<String>  getUnIndexedColumnValues(String columnName) {
		//System.out.println(columnName);
	    return (List<String>)database.getColumns(columnName);
	}

	
	public static double getAggregate(String groupbyatrribute, String value, String measureAttribute, int sample_count, String aggregate){
		
		List<String> measureValues=getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getSampledGroup(groupbyatrribute,value,sample_count);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate);
	    return result;
		
	}
			
  private static SampledResult getSampledGroup(String groupbyatrribute, String value, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps = getIndexedColumnValues(groupbyatrribute);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps.get(value);
		SampledResult sampledResult = RoaringBitmap.sampleBits(roaringBitmap, sample_count,(int)database.rowCount);
	    return sampledResult;
		}
	
	
 private static double doAggregateOnSampledBits(SampledResult sampledResult,
		 List<String> measureValues,String aggregate) {
		 double sum=0;
		 int count=0;
		 SampledResultIntIterator iterator = sampledResult.getIntIterator();
		 while(iterator.hasNext()){
		    	int i=iterator.next();
		    	count++;
		    	sum=sum+Float.valueOf(measureValues.get(i));
		 }
		 float average=(float) (sum/count);
		 if (count == 0)
		    average = 0;
		return average;
	}


	

}

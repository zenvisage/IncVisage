package org.vde.database;

import java.io.IOException;
import java.util.List;
import java.util.Map;

import org.roaringbitmap.IntIterator;
import org.roaringbitmap.RoaringBitmap;
import org.roaringbitmap.SampledResult;
import org.roaringbitmap.SampledResult.SampledResultIntIterator;
import org.vde.database.Query.FilterPredicate;

public class SamplingBasedExecutor {
	private static  Database database;

	public SamplingBasedExecutor(Database database){
		this.database=database;
	}
	
	private static RoaringBitmap getIndexedColumnValues(FilterPredicate filterPredicate1) {
	    return (RoaringBitmap)database.getColumns(filterPredicate1);
	}
	

/*
	public static double getSimpleAggregate(String groupbyatrribute, String value, String measureAttribute, String aggregate){
		
		String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		RoaringBitmap roaringBitmap = getSimpleGroup(groupbyatrribute,value);
		double result= doAggregateOnRoaringBitmap(roaringBitmap,measureValues,aggregate);
	    return result;
		
	}
	
   public static double getSimpleAggregate(RoaringBitmap roaringBitmap, String measureAttribute, String aggregate){
		
		String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		double result= doAggregateOnRoaringBitmap(roaringBitmap,measureValues,aggregate);
	    return result;
		
	}
	
    public static double getANDAggregate(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2,String measureAttribute, String aggregate, int sample_count){
		
		String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		RoaringBitmap roaringBitmap = getANDGroup(groupbyatrribute1, value1, groupbyatrribute2, value2);
		double result= doAggregateOnRoaringBitmap(roaringBitmap,measureValues,aggregate);
	    return result;
		
	}
    
    public static double getORAggregate(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2,String measureAttribute, String aggregate, int sample_count){
		
  		String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
  		RoaringBitmap roaringBitmap = getORGroup(groupbyatrribute1, value1, groupbyatrribute2, value2, sample_count);
  		double result= doAggregateOnRoaringBitmap(roaringBitmap,measureValues,aggregate);
  	    return result;
  		
  	}
      
	
	public static double getSimpleSampledAggregate(String groupbyatrribute, String value, String measureAttribute, int sample_count, String aggregate){
		
		String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getSampledSimpleGroup(groupbyatrribute,value,sample_count);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate).average;
	    return result;
		
	}
	
	public double getSimpleSampledAggregateWithoutReplacement(String groupbyatrribute, String value, String measureAttribute, int sample_count, String aggregate, short previousKey){
		String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getSampledSimpleGroupWithoutReplacement(groupbyatrribute,value,sample_count,previousKey);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate).average;
	    return result;
		
	}
	
	public double getSimpleSampledAggregateWithoutReplacement(String groupbyatrribute, String value, String measureAttribute, int sample_count, String aggregate){
		String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getSampledSimpleGroupWithoutReplacement(groupbyatrribute,value,sample_count);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate).average;
	    return result;
		
	}
	
	public static double getANDSampledAggregateWithoutReplacement(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2,String measureAttribute, String aggregate, int sample_count) {
	  	String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getANDSampledGroup(groupbyatrribute1, value1, groupbyatrribute2, value2, sample_count);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate).average;
	    return result;
	}
	
	public static double getANDSampledAggregateWithoutReplacement(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2,String measureAttribute, String aggregate, int sample_count,short previousKey) {
	  	String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getANDSampledGroup(groupbyatrribute1, value1, groupbyatrribute2, value2, sample_count);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate).average;
	    return result;
	}
	
	public static double getANDSampledAggregate(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2,String measureAttribute, String aggregate, int sample_count) {
	  	String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getANDSampledGroup(groupbyatrribute1, value1, groupbyatrribute2, value2, sample_count);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate).average;
	    return result;
	}
	
	public static double getORSampledAggregateWithoutReplacement(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2,String measureAttribute, String aggregate, int sample_count) {
	  	String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getORSampledGroup(groupbyatrribute1, value1, groupbyatrribute2, value2, sample_count);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate).average;
	    return result;
	}
	
	public static double getORSampledAggregateWithoutReplacement(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2,String measureAttribute, String aggregate, int sample_count,short previousKey) {
	  	String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getORSampledGroup(groupbyatrribute1, value1, groupbyatrribute2, value2, sample_count);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate).average;
	    return result;
	}
	
	public static double getORSampledAggregate(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2,String measureAttribute, String aggregate, int sample_count) {
	  	String[] measureValues=database.getUnIndexedColumnValues(measureAttribute);
		SampledResult sampledResult = getORSampledGroup(groupbyatrribute1, value1, groupbyatrribute2, value2, sample_count);
		double result= doAggregateOnSampledBits(sampledResult,measureValues,aggregate).average;
	    return result;
	}
	*/		
	
  public static RoaringBitmap getSimpleGroup(String groupbyatrribute, String value) {
		  	Map<String, RoaringBitmap> groupbyRoaringBitmaps = database.getIndexedColumnValues(groupbyatrribute);
			RoaringBitmap roaringBitmap= groupbyRoaringBitmaps.get(value);
	//		System.out.println(roaringBitmap.getCardinality());
			return roaringBitmap;
  }
  
  public static RoaringBitmap getANDGroup(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		RoaringBitmap result = RoaringBitmap.and(roaringBitmap, roaringBitmap1);
		return result;
	}
  
  public static RoaringBitmap getORGroup(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		RoaringBitmap result = RoaringBitmap.or(roaringBitmap, roaringBitmap1);
		return result;
	}
			
  public static SampledResult getSampledSimpleGroup(String groupbyatrribute, String value, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps = database.getIndexedColumnValues(groupbyatrribute);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps.get(value);
		SampledResult sampledResult = RoaringBitmap.sampleBits(roaringBitmap, sample_count,(int)database.rowCount);
	    return sampledResult;
	}
  
  public static SampledResult getSampledSimpleGroupWithoutReplacement(String groupbyatrribute, String value, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps = database.getIndexedColumnValues(groupbyatrribute);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps.get(value);
		SampledResult sampledResult = RoaringBitmap.sampleBitsWithoutReplacement(roaringBitmap, sample_count,(int)database.rowCount);
	    return sampledResult;
	}
  
  public static SampledResult getSampledSimpleGroupWithoutReplacement(String groupbyatrribute, String value, int sample_count,short previouskey) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps = database.getIndexedColumnValues(groupbyatrribute);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps.get(value);
		SampledResult sampledResult = RoaringBitmap.sampleBitsWithoutReplacement(roaringBitmap, sample_count, (int)database.rowCount, previouskey);
	    return sampledResult;
	}
	
  
  public static SampledResult getANDSampledGroup(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		SampledResult sampledResult = RoaringBitmap.sampledAnd(roaringBitmap,roaringBitmap1,sample_count,(int)database.rowCount);
	    return sampledResult;
	}
  
  public static SampledResult getFilterSampledGroup(String groupbyatrribute1, String value1,String filterAttribute, String value2, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(filterAttribute);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		SampledResult sampledResult = RoaringBitmap.sampledAnd(roaringBitmap,roaringBitmap1,sample_count,(int)database.rowCount);
	    return sampledResult;
	}
  
  public static SampledResult getFilterAndSampledGroup(String groupbyatrribute1, String value1, String groupbyatrribute2, String value2, String filterAttribute, String value3, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		
		Map<String, RoaringBitmap> groupbyRoaringBitmaps3 = database.getIndexedColumnValues(filterAttribute);
		RoaringBitmap roaringBitmap2= groupbyRoaringBitmaps3.get(value3);
		
		SampledResult sampledResult = RoaringBitmap.sampledAndThree(roaringBitmap,roaringBitmap1,roaringBitmap2,sample_count,(int)database.rowCount);
	    return sampledResult;
	}
  
  /*public SampledResult getFilterSampledSimpleGroupWithoutReplacement(String groupbyAttribute, String value1,
			String filterAttribute, String filterValue, int sample_count) {
	    Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyAttribute);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(filterAttribute);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(filterValue);
		SampledResult sampledResult = RoaringBitmap.sampledAndWithoutReplacement(roaringBitmap, roaringBitmap1, sample_count);
	    return sampledResult;
	}

public SampledResult getFilterSampledSimpleGroupWithoutReplacement(String groupbyAttribute, String value1,
			String filterAttribute, String filterValue, int sample_count, short previous_key) {
	    Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyAttribute);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(filterAttribute);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(filterValue);
		SampledResult sampledResult = RoaringBitmap.sampledAndWithoutReplacement(roaringBitmap, roaringBitmap1, sample_count, previous_key);
	    return sampledResult;
	}
  public static SampledResult getFilterAndSampledGroupWithoutReplacement(String groupbyatrribute1, String value1, String groupbyatrribute2, String value2, String filterAttribute, String value3, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		
		Map<String, RoaringBitmap> groupbyRoaringBitmaps3 = database.getIndexedColumnValues(filterAttribute);
		RoaringBitmap roaringBitmap2= groupbyRoaringBitmaps3.get(value3);
		
		SampledResult sampledResult = RoaringBitmap.sampledAndThreeWithoutReplacement(roaringBitmap,roaringBitmap1,roaringBitmap2,sample_count);
	    return sampledResult;
	}
  
  
  public static SampledResult getFilterAndSampledGroupWithoutReplacement(String groupbyatrribute1, String value1, String groupbyatrribute2, String value2, String filterAttribute, String value3, int sample_count, short previuos_key) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		
		Map<String, RoaringBitmap> groupbyRoaringBitmaps3 = database.getIndexedColumnValues(filterAttribute);
		RoaringBitmap roaringBitmap2= groupbyRoaringBitmaps3.get(value3);
		
		SampledResult sampledResult = RoaringBitmap.sampledAndThreeWithoutReplacement(roaringBitmap,roaringBitmap1,roaringBitmap2,sample_count,previuos_key);
	    return sampledResult;
	}*/
  

  
  public static SampledResult getANDSampledGroupWithoutReplacement(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2, int sample_count,short previous_key) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		SampledResult sampledResult = RoaringBitmap.sampledAndWithoutReplacement(roaringBitmap, roaringBitmap1, sample_count, previous_key);
	    return sampledResult;
	}
  
  public static SampledResult getANDSampledGroupWithoutReplacement(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		SampledResult sampledResult = RoaringBitmap.sampledAndWithoutReplacement(roaringBitmap, roaringBitmap1, sample_count);
	    return sampledResult;
	}
  
  
  public static SampledResult getORSampledGroup(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2, int sample_count) {
    
	  Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		SampledResult sampledResult = RoaringBitmap.sampledOR(roaringBitmap,roaringBitmap1,sample_count,(int)database.rowCount);
	    return sampledResult;
	}
  
  public static SampledResult getORSampledGroupWithoutReplacement(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2, int sample_count,short previous_key) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		SampledResult sampledResult = RoaringBitmap.sampledORWithoutReplacement(roaringBitmap, roaringBitmap1, sample_count, previous_key);
	    return sampledResult;
	}

 public static SampledResult getORSampledGroupWithoutReplacement(String groupbyatrribute1, String value1,String groupbyatrribute2, String value2, int sample_count) {
	  	Map<String, RoaringBitmap> groupbyRoaringBitmaps1 = database.getIndexedColumnValues(groupbyatrribute1);
		RoaringBitmap roaringBitmap= groupbyRoaringBitmaps1.get(value1);
		Map<String, RoaringBitmap> groupbyRoaringBitmaps2 = database.getIndexedColumnValues(groupbyatrribute2);
		RoaringBitmap roaringBitmap1= groupbyRoaringBitmaps2.get(value2);
		SampledResult sampledResult = RoaringBitmap.sampledORWithoutReplacement(roaringBitmap, roaringBitmap1, sample_count);
	    return sampledResult;
	}
  
	
 public static ResultObject doAggregateOnSampledBits(SampledResult sampledResult,
		 String[] measureValues,String aggregate, int sample_count) {
		 double sum=0;
		 int count=0;
		 //int count1=0;
		 SampledResultIntIterator iterator = sampledResult.getIntIterator();
		 while(iterator.hasNext() && count < sample_count){
		    	int i=iterator.next();
		    	count+=iterator.getCount();
		    	
		    	sum=sum+iterator.getCount()*Float.valueOf(measureValues[i]);
		 }
		 float average=(float) (sum/count);
		 //float average1=(float) (sum/count1);
		 if (count == 0){
			    average = 0;
		 }
		 
		
	    //System.out.println("count: "+count+" avg: "+average); 
		 
		return new ResultObject(average, count);
	}

 public static double doAggregateOnRoaringBitmap(RoaringBitmap roaringBitmap,
		 String[] measureValues,String aggregate) {
		 double sum=0;
		 int count=0;
		 IntIterator iterator = roaringBitmap.getIntIterator();
		 while(iterator.hasNext()){
		    	int i=iterator.next();
		    	count++;
		    	sum=sum+Float.valueOf(measureValues[i]);
		 }
		 float average=(float) (sum/count);
		 if (count == 0)
		    average = 0;
		return average;
	}

	

}

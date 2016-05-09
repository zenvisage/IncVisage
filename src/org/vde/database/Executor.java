package org.vde.database;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import org.roaringbitmap.IntIterator;
import org.roaringbitmap.RoaringBitmap;
import org.vde.database.Query;
import org.vde.database.Query.CompositeFilter;
import org.vde.database.Query.CompositeFilterOperator;
import org.vde.database.Query.Filter;
import org.vde.database.Query.FilterPredicate;

// Author:  Tarique Siddiqui

public class Executor {
	private  Database database;
	
	public Executor(Database database){
		this.database=database;
	}
	
	public Node getData(Query query) throws InterruptedException{
		int n = (int) database.rowCount;
		//System.out.println("Number of rows:"+n);
		RoaringBitmap bitSet;		
		if(query.getFilter()!=null){			
		  bitSet=applyFilters(query);
		}
		else{
			bitSet=new RoaringBitmap();
			bitSet.add(0,n-1);
		}			
		Node node =applyGroupBy(query, bitSet);
		return node;	
	}

	
	// Assuming no nesting for now, Assuming filters on indexed columns only
	public RoaringBitmap applyFilters(Query query){
		    //System.out.println("Applying Filters");
			Query.Filter filter= query.getFilter();
			RoaringBitmap result;					
			if(filter.isComposite()){
				CompositeFilter compositeFilter = (CompositeFilter) filter;
				List<Filter> subFilters=compositeFilter.getSubFilters();
				FilterPredicate filterPredicate1=(FilterPredicate) subFilters.get(0);
				result= getIndexedColumnValues(filterPredicate1);			
				for(int i=1;i<subFilters.size();i++){
					RoaringBitmap column=getIndexedColumnValues((FilterPredicate) subFilters.get(i));
					if(CompositeFilterOperator.AND==compositeFilter.getOperator()){
						result.and(column);
					}
					if(CompositeFilterOperator.OR==compositeFilter.getOperator()){
						result.or(column);
					}				
				}		
			}		
			else
				result= getIndexedColumnValues((FilterPredicate) filter);	
			//System.out.println("Ending Filters");
			return result;
	}
		
	private RoaringBitmap getIndexedColumnValues(FilterPredicate filterPredicate1) {
	    return (RoaringBitmap)database.getColumns(filterPredicate1);
	}
	
	private Map<String,RoaringBitmap> getIndexedColumnValues(String columnName) {
	    return (Map<String,RoaringBitmap>)database.getColumns(columnName);
	}
	
	private List<String>  getUnIndexedColumnValues(String columnName) {
		//System.out.println(columnName);
	    return (List<String>)database.getColumns(columnName);
	}


	//works only for two levels, and indexed values
	public Node applyGroupBy(Query query, RoaringBitmap bitSet) throws InterruptedException{
	   String aggregationVariable=query.getAggregationVarible();
	   List<String> measureValues=getUnIndexedColumnValues(aggregationVariable);
	   List<String> groupBys= Arrays.asList(query.getGroupBy().split(","));
	   List<Map<String,RoaringBitmap>> groupByValues = new ArrayList<Map<String,RoaringBitmap>>();	   
	   for(String groupBy:groupBys){
		   groupByValues.add(getIndexedColumnValues(groupBy));
	   }
	   Node root= new Node();
	   doGroupBys(root, bitSet,0, groupByValues, measureValues);  
	   return root;   
	}
	
	
private void doGroupBys(Node node,RoaringBitmap result, int index, List<Map<String,RoaringBitmap>> groupByValues,List<String> measureValues){
	Map<String,RoaringBitmap> values=groupByValues.get(index);
	for(String key:values.keySet()){
		Node n = new Node();
		RoaringBitmap bitmap= RoaringBitmap.and(result, values.get(key));	
		//System.out.println(key);
		node.chilNodes.put(key,n);
		if(index<groupByValues.size()-1){			
			doGroupBys(n,bitmap,index+1,groupByValues,measureValues);
			
		}
		else{			
		    double sum=0;
		    int count=0;
		    IntIterator iterator = bitmap.getIntIterator();
		    while(iterator.hasNext()){
		    	int i=iterator.next();
		    	count++;
		    	sum=sum+Float.valueOf(measureValues.get(i));
		    }
			  float average=(float) (sum/count);
			  if (count == 0)
				  average = 0;
			  //System.out.println(sum+":"+count+":"+average);
			  n.aggregate=average;	
			  

		}   		
	}
}


public  LinkedHashMap<String, LinkedHashMap<Integer, Float>>  changeFromNodeToLinkedList(Node node){
	LinkedHashMap<String, LinkedHashMap<Integer, Float>> output = new LinkedHashMap<String, LinkedHashMap<Integer,Float>>();
	List<Integer> ordinals = new ArrayList<Integer>();
	for(String key1 :node.chilNodes.keySet()){
		Node second=node.chilNodes.get(key1);
		for(String key2:second.chilNodes.keySet()){
			ordinals.add(Integer.parseInt(key2));			
		}		
	}
	
	Collections.sort(ordinals);
	
	for(String key1 :node.chilNodes.keySet()){
		LinkedHashMap<Integer, Float> element = new LinkedHashMap<Integer,Float>();
		output.put(key1,element);
		Node second=node.chilNodes.get(key1);
		for(Integer i:ordinals){
			Node n=second.chilNodes.get(Integer.toString(i));
			element.put(i,(float) n.aggregate);
			//System.out.println(key1+":"+i+":"+n.aggregate+"hey");
					
		}		
	}
	
		
	return output;
	
}

public static void main(String[] args) throws IOException, InterruptedException {
	  Database db=DataLoader.createDatabase("test");
   	  Query q = new Query("query").setGrouby("Major,NoOfCourses").setAggregationFunc("avg").setAggregationVaribale("GPA");
	  Executor executor = new Executor(db);
	  executor.getData(q);
}


}
	
	


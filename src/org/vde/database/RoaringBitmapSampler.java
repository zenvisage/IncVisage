package org.vde.database;

import java.util.Arrays;
import java.util.BitSet;
import java.util.Iterator;
import java.util.Random;

import org.junit.Assert;
import org.roaringbitmap.Container;
import org.roaringbitmap.FastAggregation;
import org.roaringbitmap.IntIterator;
import org.roaringbitmap.RoaringBitmap;
import org.roaringbitmap.SampledResult;
import org.roaringbitmap.SampledResult.SampledResultIntIterator;


public class RoaringBitmapSampler {
//	int n=1000000000; // 1 billion
	int n = 1000000;  // 1 million
	int batch_Size=10000; 
//	int sample_count=250000000;   // 250 million - 25%
	
	int sample_count=250000;
	Random randomGenerator = new Random();
	Integer[] indices = new Integer[batch_Size];
   
	
	public void sampleAnd(){
    	System.out.println("Testing massive logical AND (all timings are in milliseconds)");
		RoaringBitmap[] rrm1 = new RoaringBitmap[2];
		for (int k = 0; k < rrm1.length; ++k)
			rrm1[k] = new RoaringBitmap();
		int howmany = n;
		long start= System.currentTimeMillis();
		for (int k = 0; k < howmany; ++k) {
			double r=Math.random();
		  if (r < 0.3){
				rrm1[0].add(k);
			}
		  else if(r < 0.7){
			  rrm1[1].add(k);
		  }
			else{
				rrm1[0].add(k);
				rrm1[1].add(k);
			}
		}
	  long end= System.currentTimeMillis();
	  long diff=end-start;
	  System.out.println("Total no. of bits: "+n);
	  int c1=rrm1[0].getCardinality();
	  int c2=rrm1[1].getCardinality();
	  int i=c1+c2-n;
	  System.out.println("Loading time:"+diff);
	  System.out.println("Cardinality of RBitmap1 :"+rrm1[0].getCardinality());
	  System.out.println("Cardinality of RBitmap2 :"+rrm1[1].getCardinality()); 
	  System.out.println("Size (RBitmap1 intersection RBitmap2):"+ i); 
	  System.out.println("Sample Size:"+sample_count);
		
	//	RoaringBitmap result = RoaringBitmap.and(rrm1[0], rrm1[1]);	
	    unsampledAnd(rrm1);
//	    approach1(rrm1);
//	    approach2(rrm1);
	    containersampledAnd(rrm1);
		  
    }
	
	public void  approach2(RoaringBitmap[] rrm1){
		 int count=0;		
    	    long start= System.currentTimeMillis();
		    while(count < sample_count){
			Integer[] indices = sampleIndices(n,batch_Size);
			BitSet bitSet0=sample(rrm1[0],indices);
			BitSet bitSet1=sample(rrm1[1],indices);
		//	System.out.println(bitSet0.cardinality());
		//	System.out.println(bitSet1.cardinality());
			bitSet0.and(bitSet1);
			int c=bitSet0.cardinality();
			count=count+c;
	//		System.out.println(count);
		    }
		    long end= System.currentTimeMillis();
		    long diff=end-start;
		    System.out.println("Approach2 - (segment level) with a btach size of "+batch_Size+":"+diff);
	}
	
	
	public void  approach1(RoaringBitmap[] rrm1){
		    int count=0;		
		    long start= System.currentTimeMillis();
		    Random randomGenerator = new Random();
		    while(count < sample_count){
		     int randomInt = randomGenerator.nextInt(n);
		     if(rrm1[0].contains(randomInt) && rrm1[1].contains(randomInt))
		    	 count=count+1;
		    }
		    long end= System.currentTimeMillis();
		    long diff=end-start;
		    System.out.println("Approach1- Sampling each point i.e No batching :"+diff);
	}
	
	public void unsampledAnd(RoaringBitmap[] rrm1){
		  long start= System.currentTimeMillis();
		RoaringBitmap result = RoaringBitmap.and(rrm1[0], rrm1[1]);	
		 long end= System.currentTimeMillis();
		 long diff=end-start;
		 IntIterator iterator= result.getIntIterator();
		 int count=0;
		 while(iterator.hasNext()){
			    	int i=iterator.next();
	//		        System.out.println(count+" : "+i);
			        count=count+1;
			 }
		 System.out.println("No sampling i.e Complete AND:"+diff);
		
	}
	
	public void containersampledAnd(RoaringBitmap[] rrm1){
	//	System.out.println("Starting Container Level sampling");
		long start= System.currentTimeMillis();
	   	SampledResult sampleResult = RoaringBitmap.sampledAnd(rrm1[0], rrm1[1],sample_count,n);	
		long end= System.currentTimeMillis();
		long diff=end-start;
		SampledResultIntIterator iterator= sampleResult.getIntIterator();
		int count=0;
		while(iterator.hasNext()){
		    	int i=iterator.next();
		        System.out.println(count+" : "+i);
		        count=count+1;
		 }
			
		System.out.println("Approach3- Container Level sampling:"+diff);
		
	}
   
    public BitSet sample(RoaringBitmap rr1, Integer[] indices){
    	BitSet bitSet = new BitSet(indices.length);
    	 for (int idx = 0; idx <  indices.length; ++idx){
    		  bitSet.set(idx,rr1.contains(indices[idx]));
           }   
    	return bitSet;
    	
    }
    
    
    public Integer[] sampleIndices(int n, int k){
  //  	Random randomGenerator = new Random();
  
        for (int idx = 0; idx <  k; ++idx){
          int randomInt = randomGenerator.nextInt(n);
          indices[idx]=randomInt;
        }   
        return indices;
     }
	
    
    public static void main(String[] args) {
    	RoaringBitmapSampler s = new RoaringBitmapSampler();
    	s.sampleAnd();    	
	}

}

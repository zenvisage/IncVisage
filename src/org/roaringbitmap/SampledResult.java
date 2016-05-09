package org.roaringbitmap;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class SampledResult {
 public	ArrayList<SampledContainer> sampledContainers;
 public Map<Integer,Integer> counts;
 public int totalContainers;
 public short lastIndex;
 
  public SampledResultIntIterator getIntIterator() {
	        return new SampledResultIntIterator();	       
	    }
	

   public  SampledResult(){
		 counts= new HashMap<Integer,Integer>();
		sampledContainers = new ArrayList<SampledContainer>();
	}
   
   public short getLastIndex(){
	   return lastIndex;
   }
	
	public final class SampledResultIntIterator{
	        private int hs = 0;

	        private ShortIterator iter;

	        private int pos = 0;
	     
	        private int count=0;

	        private int x;
	        private int size=0;
	        


	        private SampledResultIntIterator() {
	   	            nextContainer();
	            size=sampledContainers.size();
	        }

	  
	        public boolean hasNext() {
	            return pos < size;
	        }

	        private void nextContainer() {
	            if (pos < sampledContainers.size()) {
	                 iter =sampledContainers.get(pos).container.getShortIterator();
	                short index = sampledContainers.get(pos).index;
	                count=counts.get((int)index);
	                hs = Util.toIntUnsigned(index) << 16;
	            }
	        }

	        public int next() {
	            x = Util.toIntUnsigned(iter.next()) | hs;
	            if (!iter.hasNext()) {
	                ++pos;
	                nextContainer();
	            }
	               return x;
	        }
	        
	        public int getCount() {
	            return count;
	        }

	    
	        public SampledResultIntIterator clone() {
	            try {
	            	SampledResultIntIterator x = (SampledResultIntIterator) super.clone();
	                x.iter =  this.iter.clone();
	                return x;
	            } catch (CloneNotSupportedException e) {
	                return null;// will not happen
	            }
	        }

	    }

}

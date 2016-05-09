/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package test;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

import org.json.simple.JSONObject;

import api.*;


/**
 *
 * @author srahman7
 */
public class PreliminaryTests {

    /**
     * @param args the command line arguments
     */
      
    
    public static void main(String[] args) {
        // TODO code application logic here
    	Visualization viz= new Visualization();
    	ApproxHistoSamp ah = new ApproxHistoSamp();//ApproxHeatMap();
    	ApproxHeatMapSamp am = new ApproxHeatMapSamp();
    	ah.setDbSets(viz.dbSets);
    	am.setDbSets(viz.dbSets);
        //ah.withoutReplacement = false;
        JSONObject jobj = new JSONObject();
        
        jobj.put("dataset", "flight");//airline,sensordata,epa
        jobj.put("dimension", "Day");
        jobj.put("dimension1", "Year");//Day Of Month
        jobj.put("dimension2", "Month");
        jobj.put("predicate", "Carrier");//Carrier,Year
        jobj.put("predicatevalue", "AA");//1987, JetBlue Airways
        jobj.put("measure", "Arrival Delay (minutes)");//ArrDelay,Temperature,PPM
        jobj.put("sampling", "with");
        
        double [] epsilon = {1};
        try { 
        	String result = am.loadData(jobj.toJSONString());
        	
        	result = am.loadAxesData(jobj.toJSONString());
        	//long start = System.nanoTime();
        	
        	for(int i=0;i<epsilon.length;i++)
        	{
        		am.epsilon=epsilon[i];
        		am.getData(result);
        		am.reset();
        		 jobj.put("dataset", "flight");//airline,sensordata,epa
        	        jobj.put("dimension", "Day");
        	        jobj.put("dimension1", "Day Of Month");//Day Of Month
        	        jobj.put("dimension2", "Month");
        	        jobj.put("predicate", "Carrier");//Carrier.Year
        	        jobj.put("predicatevalue", "American Eagle Airlines");//American Eagle Airlines,1987
        	        jobj.put("measure", "Arrival Delay (minutes)");//ArrDelay,Temperature,PPM
        	        jobj.put("sampling", "with");
        	        result = am.loadAxesData(jobj.toJSONString());
        	        am.getData(result);
        	}
        	//String result = ah.loadData(jobj.toJSONString());
        	//result = ah.loadAxesData(jobj.toJSONString());
        	//ah.reset();
        	//ah.getData(result);
        	//ah.reset();
        	//ah.getData(result);
        	//BufferedWriter timedata;
    		//if(ah.withoutReplacement)
    		//	timedata = new BufferedWriter(new FileWriter(".\\result\\airline\\runtimes_wo.csv",true));
    		//else
    		//	timedata = new BufferedWriter(new FileWriter(".\\result\\airline\\runtimes_w.csv",true));
            //System.out.println("Total Time:"+(System.nanoTime()-start)/1000000);
        	//timedata.write("Total Time,"+(System.nanoTime()-start)/1000000+"\n");
        	//timedata.close();
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
        
//        Map<Integer, TemporalObject> NT=null;
//        
//        String[] data = {"airline"}; //"ds3_all_equal", "ds1","airline","airline_delay", "yahoo_bid","yahoo_click", "ds3","synth3"
//        int [] k = {100};  //8,12,15,20,25,28,30,50,60,70,80,90,100---4,6,8,10,12,14,15
//        double [] epsilon = {0.1};//0.1,0.2,0.3 
//        int run = 15;
//        
//        
//        
//        File file;
//        IHisto ih;
//        BufferedWriter bih;
//        
//        for (int dta=0;dta<data.length;dta++) {
//            System.out.println("-------------------------" + data[dta] + "-------------------------");
//            
//            
//            NT = ah.createNT(data[dta]);
//            
//            long seed = System.nanoTime();
//            for(int key:NT.keySet())
//            {
//                Collections.shuffle(NT.get(key).instanceArray, new Random(seed));
//            }
//                
//                
//            file = new File(".\\result\\out\\"+data[dta]+"\\");
//            if (!file.exists()) {
//                    if (file.mkdirs()) {
//                            System.out.println("Directory is created!");
//                    } else {
//                            System.out.println("Failed to create directory!");
//                    }
//            }
//
//
//            for(int j=0;j<epsilon.length;j++)
//            {
//                for(int i=0;i<k.length;i++)
//                {
//                    file = new File(".\\result\\out\\"+data[dta]+"\\"+(int)(epsilon[j]*100)+"\\"+k[i]+"\\");
//                    if (!file.exists()) {
//                        if (file.mkdirs()) {
//                                System.out.println("Directory is created!");
//                        } else {
//                                System.out.println("Failed to create directory!");
//                        }
//                    }
//
//                    ih = new IHisto();
//
//                    ih.genHisto(NT, epsilon[j], 0.05, k[i],".\\result\\out\\"+data[dta]+"\\"+(int)(epsilon[j]*100)+"\\"+k[i]+"\\");
//
//                    
//                                   
//                   
//
//                }
//
//
//
//
//                }  
//                
//
//            
//   
//        
//               
//        
//        }
    }
    
}

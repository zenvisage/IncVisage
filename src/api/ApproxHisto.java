/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package api;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Objects;
import java.util.Random;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.json.simple.*;
import org.json.simple.parser.JSONParser;
import org.roaringbitmap.RoaringBitmap;
import org.roaringbitmap.SampledResult;
import org.vde.database.Database;
import org.vde.database.ResultObject;
import org.vde.database.SamplingBasedExecutor;

/**
 *
 * @author srahman7
 */
public class ApproxHisto{

	/**
	 * @param args
	 *            the command line arguments
	 */

	public static int totalInstances;
	// Map<String,RoaringBitmap> NeedleTail;
	Map<Integer, TemporalObject> groupAGG;
	int xMinF=-1,xMaxF=-1;
	long returnTime;
	public Database db;
	public Map<String,Database> dbSets = new TreeMap<String,Database>();
	public HashMap<Integer, Short> withoutReplacementIndex = new HashMap<Integer, Short>();
	SamplingBasedExecutor sbe;

	int [] sampledInstancePerIter;
	double [] groupAVGbase;
	int [] samplingCountBase;
	
	TreeMap<Integer, ArrayList<Segment>> kBest;
	JSONObject jObjF;
	JSONArray jArrF;
	int[] groups;
	public int[] samplingcount;
	double[] groupsAVG;
	String dir;
	String groupbyAttribute;
	String measureAttribute;
	String filterAttribute;
	String filterValue;
	public String dataset;
	String[] measureValues;
	String samplingType;
	public boolean withoutReplacement = false;
	public boolean firstIteration = true;
	double[][] cArray = { { -53, 280 }, { -27, 135 }, { -7, 11 } };
	double c = 0;
	public double epsilon=8,delta=0.05,alpha=1;
	// BufferedWriter timedata;
	
	public void setDbSets(Map<String,Database> dbSets)
	{
		
		this.dbSets = dbSets;
		
		System.out.println(this.dbSets.size());
		System.out.println(dbSets.size());
		
		//for(String key:dbSets.keySet())
			//System.out.println(key);
	}
	
	
	public String loadData(String info)
			throws IOException, InterruptedException, org.json.simple.parser.ParseException {
		System.out.println(info);
		JSONParser parser = new JSONParser();
		JSONObject jsonobj = (JSONObject) parser.parse(info);
		dataset = jsonobj.get("dataset").toString(); // "ds3_all_equal",
														// "ds1","airline","airline_delay",
														// "yahoo_bid","yahoo_click",
														// "ds3","synth3"
		//groupbyAttribute = jsonobj.get("dimension").toString();
		//measureAttribute = jsonobj.get("measure").toString();
		//samplingType = jsonobj.get("sampling").toString();

		//c = cArray[2][1] - cArray[2][0];
		System.out.println("-------------------------" + dataset + "-------------------------");

		//kBest = new TreeMap<Integer, ArrayList<Segment>>();
		jObjF = new JSONObject();
		jArrF = new JSONArray();
		dir = ".\\result\\" + dataset + "\\";

		
		// if (withoutReplacement)
		// timedata = new BufferedWriter(new FileWriter(dir +
		// "runtimes_wo.csv"));
		// else
		// timedata = new BufferedWriter(new FileWriter(dir +
		// "runtimes_w.csv"));

		long start = System.nanoTime();
		db = null;
		db = this.dbSets.get(dataset);

		
		System.out.println(db.databaseMetaData.getxAxisColumns().keySet());
		System.out.println(db.databaseMetaData.getyAxisColumns().keySet());
		System.out.println(db.databaseMetaData.getPredicateColumns().keySet());
		JSONObject meta = new JSONObject();
		
		String xAxisStr = "";
		String yAxisStr = "";
		String predStr = "";
		int i=0;
		for(String key:db.databaseMetaData.getxAxisColumns().keySet())
		{
			
			if(db.databaseMetaData.getTrendColumns().containsKey(key))
			{
				i++;
				
				xAxisStr+=key;
				if(i<db.databaseMetaData.getTrendColumns().keySet().size())
				{
					xAxisStr+=",";
				}
			}
			
		}
		
		i=0;
		for(String key:db.databaseMetaData.getyAxisColumns().keySet())
		{
			i++;
			yAxisStr+=key;
			if(i<db.databaseMetaData.getyAxisColumns().keySet().size())
			{
				yAxisStr+=",";
			}
		}
		
		i = 0;
        for (String key : db.databaseMetaData.getPredicateColumns().keySet()) {
            i++;
            predStr += key;
            if (i < db.databaseMetaData.getPredicateColumns().keySet().size()) {
            	predStr += ",";
            }
        }
        meta.put("dimension", xAxisStr);
        meta.put("measure", yAxisStr);
        meta.put("predicate", predStr);
		return meta.toJSONString();

	}

	public String loadPredicateData(String info)
			throws IOException, InterruptedException, org.json.simple.parser.ParseException {
		JSONParser parser = new JSONParser();
		JSONObject jsonobj = (JSONObject) parser.parse(info);
		filterAttribute = jsonobj.get("predicate").toString();
		
		Map<String, RoaringBitmap> indexedbitmaps = db.getIndexedColumnValues(filterAttribute);
		
		JSONObject meta = new JSONObject();
		
		String predStr = "";
		
		int i = 0;
        for (String key : indexedbitmaps.keySet()) {
            i++;
            predStr += key;
            if (i < indexedbitmaps.keySet().size()) {
            	predStr += ",";
            }
        }
        if(filterAttribute.toLowerCase().contains("city"))
        {
        	predStr = "Atlanta- GA"+","+"Chicago- IL"+","+"Denver- CO"+","+"Houston- TX"+","+"Los Angeles- CA"+","+"New York- NY"+","+"Oklahoma- OH"; ;
        }
        else if(filterAttribute.toLowerCase().contains("state"))
        {
        	predStr = "AK,CA,FL,GA,IL,LA,MN,NY,OH,PA,TX";
        }
        else if(filterAttribute.toLowerCase().contains("airport"))
        {
        	predStr = "ATL,DEN,DFW,JFK,LAX,LGA,ORD,SFO,CTL,LAS,PHX";
        }
        else if(filterAttribute.toLowerCase().contains("carrier"))
        {
        	predStr = "AA,AS,CO,OH(1),UA,US";
        }
        meta.put("predicate", predStr);
		return meta.toJSONString();
		
	}
	public String loadAxesData(String info)
			throws IOException, InterruptedException, org.json.simple.parser.ParseException {
		System.out.println(info);
		JSONParser parser = new JSONParser();
		JSONObject jsonobj = (JSONObject) parser.parse(info);
		
		groupbyAttribute = jsonobj.get("dimension").toString();
		measureAttribute = jsonobj.get("measure").toString();
		filterAttribute = jsonobj.get("predicate").toString();
		filterValue = jsonobj.get("predicatevalue").toString();
		samplingType = jsonobj.get("sampling").toString();
		kBest = new TreeMap<Integer, ArrayList<Segment>>();

		firstIteration = true;

				
		Map<String, RoaringBitmap> indexedbitmaps = db.getIndexedColumnValues(groupbyAttribute);

		measureValues = db.getUnIndexedColumnValues(measureAttribute);

		// System.out.println(db.getIndexedColumnValues(groupbyAttribute).keySet());

		org.vde.database.ColumnMetadata cmd1 = db.getColumnMetaData(groupbyAttribute);
		System.out.println("xMin:" + cmd1.min + "xMax:" + cmd1.max);
		
		int xMin,xMax;
		
		xMin = (int) cmd1.min;
		xMax = (int) cmd1.max;
		
		org.vde.database.ColumnMetadata cmd2 = db.getColumnMetaData(measureAttribute);
		
		c = cmd2.c_max - cmd2.c_min;
		
		epsilon = 1;
		
		Object[] hObj = indexedbitmaps.keySet().toArray();
		groups = new int[xMax-xMin+1];//[hObj.length];
		groupAGG = new HashMap<Integer, TemporalObject>();
		samplingcount = new int[xMax-xMin+1];//[hObj.length];
		
		
		sampledInstancePerIter = new int[xMax-xMin+1];
		
		samplingCountBase = new int[xMax-xMin+1];
		
		groupAVGbase = new double[xMax-xMin+1];
		
		try
		{
			BufferedReader br = new BufferedReader(new FileReader(groupbyAttribute+",csv"));
			String line;
			int count = 0;
			while((line=br.readLine())!=null)
			{
				if(count >0)
				{
					sampledInstancePerIter[count-1] = Integer.parseInt(line.split(",")[1]);
				}
				count++;
			}
		}
		catch(Exception e)
		{
			
		}
		
		
		sbe = new SamplingBasedExecutor(db);

		for (int i = 0,j=xMin; j <= xMax; i++,j++) {
			groups[i] = j;

			if(indexedbitmaps.keySet().contains(Integer.toString(j)))
				samplingcount[i] = 0;
			else
				samplingcount[i] = -1;
			groupAGG.put(groups[i], new TemporalObject(groups[i]));

			//System.out.println(i+"=="+groups[i]+"=="+samplingcount[i]);
		}
		
		/*for (int i = 0; i < hObj.length; i++) {
			groups[i] = Integer.parseInt(hObj[i].toString());

			samplingcount[i] = 0;

			groupAGG.put(groups[i], new TemporalObject(groups[i]));

		}*/

		
		Arrays.sort(groups);

		

		// org.vde.database.ColumnMetadata cmd2 =
		// db.getColumnMetaData(measureAttribute);
		// System.out.println("yMin:" + cmd2.min + "yMax:" + cmd2.max);

		
		JSONObject meta = new JSONObject();
		JSONObject yRange = new JSONObject();

		yRange = y_range();

		
		meta.put("xMin", xMin);

		meta.put("xMax", xMax);

		meta.put("yMin", Double.parseDouble(yRange.get("yMin").toString()));
		meta.put("yMax", Double.parseDouble(yRange.get("yMax").toString()));

		return meta.toJSONString();

	}

	public JSONObject y_range() {

		double max = -10000000;
		double min = 10000000;

		for (int key : groupAGG.keySet()) {
			if (groupAGG.get(key).avg > max)

				max = groupAGG.get(key).avg;
			if (groupAGG.get(key).avg < min)
				min = groupAGG.get(key).avg;
			// groupAGG.get(key).avg = 0;

		}

/*		double avg = 0; 
		for (int key : groupAGG.keySet()) {
			avg += groupAGG.get(key).avg;
		
		 }
		 
		 avg /= groupAGG.keySet().size();
		 
		 double stdev = 0;
		 
		 for (int key : groupAGG.keySet()) { 
			 stdev += (avg - groupAGG.get(key).avg) * (avg - groupAGG.get(key).avg);
		 // groupAGG.get(key).avg = 0;
		  
		 // groupAGG.get(key).count = 0; 
			 }
		  
		  stdev /= groupAGG.keySet().size();
		  
		  stdev = Math.sqrt(stdev);*/
		 

		JSONObject meta = new JSONObject();
		//NumberFormat formatter = new DecimalFormat("#0.00");
		//System.out.println(min + "--" + max);
		//meta.put("yMin", Double.parseDouble(formatter.format(min)));// avg -
																	// stdev
		//meta.put("yMax", Double.parseDouble(formatter.format(max)));// avg + 2 *
																	// stdev
		
		meta.put("yMin", min);// avg -
																			// stdev
		meta.put("yMax", max);// avg + 2 *
																			// stdev
		
		return meta;
	}

	public void getData(String query) throws org.json.simple.parser.ParseException {

		System.out.println(query);
		String result = "";
		JSONParser parser = new JSONParser();
		JSONObject jsonobj = (JSONObject) parser.parse(query);

		// System.out.println(query);
		long iteration = (long) jsonobj.get("xMax");

		try {
			BufferedWriter timedata;
    		//if(ah.withoutReplacement)
    		timedata = new BufferedWriter(new FileWriter(".\\result\\"+dataset+"_runtimes.csv"));
    		timedata.write("Iteration,Time(ms)\n");
			for (long i = xMinF; i <= iteration; i++) {

				// 
				long start = System.nanoTime();
				//System.out.println("---------------Iteration ----------------" + i);
				result = genHisto(Long.toString(i), "0");
				
				if(i==-1)
					i=xMinF;
				timedata.write(i + "," +(System.nanoTime()-start)/1000000+"\n");
				// timedata.write((System.nanoTime() - returnTime) / 1000000 +
				// "," );
				// System.out.println("Histo Returns");
				// Time:"+(System.nanoTime()-start)/1000000);
				// timedata.write((System.nanoTime() - start) / 1000000 + "\n");
			}
			 timedata.close();
		} catch (IOException ex) {
			Logger.getLogger(ApproxHisto.class.getName()).log(Level.SEVERE, null, ex);
		}

		// System.out.println(groupAGG.get(1).agg.avg);

		// System.out.println(result);
		

	}

	public void reset() {
		kBest = new TreeMap<Integer, ArrayList<Segment>>();

		firstIteration = true;

		for (int i=0;i<groupAGG.keySet().size();i++) {
			groupAGG.get(groups[i]).avg = 0;
			if(samplingcount[i]!=-1)
				samplingcount[i]=0;
			
		}
		
		for(int i=0; i < sampledInstancePerIter.length;i++)
			sampledInstancePerIter[i]  = 0;
		
		for(int i=0; i < groups.length;i++)
		{
			samplingCountBase[i]  = 0;
			groupAVGbase[i] = 0;
		}
		
		sbe = new SamplingBasedExecutor(db);
		jObjF = new JSONObject();
		jArrF = new JSONArray();
		xMinF = -1;
		xMaxF = -1;

	}

	public void genJSON(int k, String jsonSave) throws IOException {
		//NumberFormat formatter = new DecimalFormat("#0.00");
		JSONObject jObjTmp;
		JSONObject jObjSeg;
		JSONArray jArrSeg;
		JSONObject jObjEst;
		JSONArray jArrEst;
		// segmentVal = new BufferedWriter(new FileWriter(dir+k+".csv"));
		// segmentVal.write("start,end,average"+"\n");
		BufferedWriter segmentJSON = null;
		
		jObjSeg = new JSONObject();
		jArrSeg = new JSONArray();
		jObjEst = new JSONObject();
		jArrEst = new JSONArray();

		for (int s = 0; s < kBest.get(k).size(); s++) {
			if (kBest.get(k).get(s).isNew) {
				jObjSeg.put("n", s);
				jObjTmp = new JSONObject();
				jObjTmp.put("b", groups[kBest.get(k).get(s).start]);
				jObjTmp.put("e", groups[kBest.get(k).get(s).end]);
				//System.out.println(k+" "+kBest.get(k).get(s).mean);
				jObjTmp.put("a", kBest.get(k).get(s).mean);//Double.parseDouble(formatter.format(kBest.get(k).get(s).mean))
				// System.out.println(Double.parseDouble(formatter.format(kBest.get(k).get(s).mean)));
				jObjSeg.put("s", jObjTmp);

				jArrSeg.add(jObjSeg);

				jObjSeg = new JSONObject();

				for (int groupi = kBest.get(k).get(s).start; groupi <= kBest.get(k).get(s).end; groupi++) {

					jObjEst.put("a", groupAGG.get(groups[groupi]).avg);//Double.parseDouble(formatter.format(groupAGG.get(groups[groupi]).avg))
					jObjEst.put("g", groups[groupi]);

					jArrEst.add(jObjEst);

					jObjEst = new JSONObject();
				}
				
				

			}

			// segmentVal.write(groups[kBest.get(k).get(s).start]+","+groups[kBest.get(k).get(s).end]+","+kBest.get(k).get(s).mean+"\n");
		}

		jObjF.put("sl", jArrSeg);

		// segmentVal.close();

		// avgApprox = new BufferedWriter(new
		// FileWriter(dir+"estimates_"+k+".csv"));
		// avgApprox.write("group,average\n");

		jObjF.put("gl", jArrEst);

		JSONObject yRange = new JSONObject();

		yRange = y_range();

		jObjF.put("y", yRange);
		
		if(firstIteration)
		{
			jObjF.put("xMin", groups[xMinF]);
			jObjF.put("xMax", groups[xMaxF]);
		}

		// jArrF.add(jObjF);
		/*if (jsonSave.equals("1")) {
			segmentJSON = new BufferedWriter(new FileWriter(dir + "currentIt.json"));
			segmentJSON.write(JSONObject.toJSONString(jObjF));
			segmentJSON.close();
		}

		// approx.close();
		if (k == 78) {
			// System.out.println(jArrSeg);
			// System.out.println(jArrEst);
			System.out.println(jObjF);
		}*/
		// return jObjF;
	}

	public String genHisto(String str, String jsonSave) throws IOException {
		int k = Integer.parseInt(str);

		System.out.println("ep: "+epsilon+", alpha: "+alpha);
		int seg_start, seg_end;

		Arrays.sort(groups);

		// incrementally approx histogram

		int sampledInstances = 0;

		seg_start = 0;
		seg_end = groups.length - 1;

		// BufferedWriter avgApprox=null;
		// BufferedWriter segmentVal = null;
		// BufferedWriter segmentJSON = null;
		//BufferedWriter best = new BufferedWriter(new FileWriter("estDifference.csv",true));
		JSONObject jObj;
		JSONArray jArr;
		BufferedWriter segmentJSON = null;

		// for k=1 sample from 1 to m

		// for k > 1, retieve segments from kBest(k-1)
		// for each segment find best split and calc sum avg of new found
		// segments
		// keep track of the best-k segment, i.e. maximizing sum
		// insert that in to kBest[k]

		if (firstIteration) {
			// approx = new BufferedWriter(new
			// FileWriter(dir+"approx_"+k+".csv"));

			
			ArrayList<Segment> tempSeg = new ArrayList<Segment>();
			// sample from m groups and estimate mean
			double alpha_ = 0;
			SampledResult sampledResult;

			long samplingTime = 0;
			long aggregationTime = 0;
			long sampAggTime = System.nanoTime();
			ResultObject rObj=null;
			for (int i = seg_start, grp = 1; i <= seg_end; i++, grp++) {
				//System.out.println(i+"=="+groups[i]+"=="+samplingcount[i]);
				if(samplingcount[i]!=-1)
				{
					// count the amount of sampling to do
					
						sampledInstances = (int) Math.ceil(36 * c * c * (seg_end - seg_start + 1)
								* (Math.log((4.0 * (groups.length)) / delta))
								* (1.0 / (groups.length * groups.length * epsilon * epsilon))); 
						/*sampledInstances = (int) Math.ceil((Math.log((1.0 * (seg_end - seg_start + 1)) / delta) / Math.log(2))
										* (1.0*c*c / ((Math.min(grp*grp, (seg_end - seg_start + 2 - grp)*(seg_end - seg_start + 2 - grp))) * epsilon * epsilon)));*/
						
						
	
					// double avg;
					
					if (this.withoutReplacement) {
						// System.out.println("In without");
						// System.out.println("Start sampling");
						long start = System.nanoTime();
						sampledResult = sbe.getSampledSimpleGroupWithoutReplacement(groupbyAttribute,
								Integer.toString(groups[i]), sampledInstances);
						// System.out.println("num instances: "+sampledInstances);
	
						samplingTime += (System.nanoTime() - start);
						// long start = System.nanoTime();
						start = System.nanoTime();
						// System.out.println("Start aggregation");
						rObj = sbe.doAggregateOnSampledBits(sampledResult, measureValues, "avg",sampledInstances);
						groupAGG.get(groups[i]).avg += rObj.average; 
						
						aggregationTime += (System.nanoTime() - start);
						// System.out.println("Sampling
						// Time:"+(System.nanoTime()-start)/1000000);
						withoutReplacementIndex.put(groups[i], sampledResult.getLastIndex()); // save
																								// it
																								// in
																								// the
																								// map
	
					}
	
					else {
	
						// groupAGG.get(groups[i]).avg +=
						// sbe.getSimpleSampledAggregate(groupbyAttribute,
						// Integer.toString(groups[i]), measureAttribute,
						// sampledInstances, "avg");
						// System.out.println("Start sampling");
						long start = System.nanoTime();
						
						if(filterAttribute.equals("")) 
							sampledResult = sbe.getSampledSimpleGroup(groupbyAttribute, Integer.toString(groups[i]),sampledInstances);
						else
							sampledResult = sbe.getFilterSampledGroup(groupbyAttribute, Integer.toString(groups[i]),filterAttribute,filterValue,sampledInstances);
	
						samplingTime += (System.nanoTime() - start);
						// long start = System.nanoTime();
						// System.out.println("Start aggregation");
						start = System.nanoTime();
						rObj = sbe.doAggregateOnSampledBits(sampledResult, measureValues, "avg",sampledInstances);

						if(rObj.count>0)
						{
							if(xMinF==-1)
							{
								xMinF = i;
								k=groups[i];
							}
							if(i > xMaxF)
								xMaxF= i;
							groupAGG.get(groups[i]).avg += rObj.average;
						}
						
						else
						{
							groupAGG.get(groups[i]).avg = 0;
							samplingcount[i] = -1;
							
						}
						
						aggregationTime += (System.nanoTime() - start);
					}
	
					
					alpha_ += groupAGG.get(groups[i]).avg;
					samplingcount[i]+=rObj.count;
				}
				//System.out.println(xMinF+"---"+xMaxF);
			}
			
			

			// timedata.write(samplingTime/ 1000000 + "," +aggregationTime/
			// 1000000+"," );
			// timedata.write((System.nanoTime()-sampAggTime)/ 1000000+",");

			long histoTime = System.nanoTime();
			seg_start = xMinF;
			seg_end = xMaxF;
			alpha_ = (1.0 / (seg_end - seg_start + 1)) * alpha_;

			tempSeg.add(new Segment(seg_start, seg_end, alpha_));

			// timedata.write((System.nanoTime()-histoTime)/ 1000000+",");
			// returnTime = System.nanoTime();

			if (!jsonSave.equals("2")) {
				kBest.put(k, tempSeg);
				genJSON(k, jsonSave);
				firstIteration = false;
				return JSONObject.toJSONString(jObjF);
			}
			firstIteration = false;
			return "";

		} else {
			// approx = new BufferedWriter(new
			// FileWriter(dir+"approx_"+k+".csv"));
			// approxGrp = new BufferedWriter(new
			// FileWriter(dir+"approx_"+k+"_grp.csv"));
			//best.write("Iteration,"+k+"\n");
			//best.write("Group, Difference\n");
			epsilon /= alpha;
			ArrayList<Segment> tempSeg = new ArrayList<Segment>();
			ArrayList<Segment> prevSeg = new ArrayList<Segment>();
			Segment segTemp;
			prevSeg = kBest.get(k - 1);

			double global_seg_mean = 0;
			int split_segment_no = -1;
			int split_segment_maxSplit = -1;
			double global_max = -1000000000;
			double global_best_mean1 = 0, global_best_mean2 = 0;
			long samplingTime = 0;
			long aggregationTime = 0;
			long sampAggTime = System.nanoTime();

			for (int s = 0; s < prevSeg.size(); s++) {

				segTemp = prevSeg.get(s);
				seg_start = segTemp.start;
				seg_end = segTemp.end;

				if (seg_end - seg_start < 1) {
					// System.out.println("here0");
					continue;
				}
				// sample from m groups and estimate mean
				int newSamp = 0;

				if (!segTemp.isNew) {
					// System.out.println("hereOld");

					// approx.write("\n\n\nSeg No,start,end,split,Global
					// Estimate, Segment_left Score, Segment_right Score\n");
					// approx.write((s+1)+","+groups[seg_start]+","+groups[seg_end]+",-,-\n\n\n");
					// continue;
				} else {

					// System.out.println("hereNew");
					double segment_tmp_avg = 0;

					SampledResult sampledResult;
					ResultObject rObj=null;
					for (int i = seg_start, grp = 1; i <= seg_end; i++, grp++) {
						if(samplingcount[i]!=-1)
						{
							// count the amount of sampling to do
							
								sampledInstances = (int) Math.ceil(36 * c * c * (seg_end - seg_start + 1)
										* (Math.log((4.0 * (groups.length)) / delta))
										* (1.0 / (groups.length * groups.length * epsilon * epsilon))); 
								/*sampledInstances = (int) Math.ceil((Math.log((1.0 * (seg_end - seg_start + 1)) / delta) / Math.log(2))
										* (1.0*c*c / ((Math.min(grp*grp, (seg_end - seg_start + 2 - grp)*(seg_end - seg_start + 2 - grp))) * epsilon * epsilon)));*/
								
							
	
							if (this.withoutReplacement) {
								// System.out.println("Start sampling");
								long start = System.nanoTime();
								sampledResult = sbe.getSampledSimpleGroupWithoutReplacement(groupbyAttribute,
										Integer.toString(groups[i]), sampledInstances,
										withoutReplacementIndex.get(groups[i]));
								// System.out.println("num instances:
								// "+sampledInstances);
								samplingTime += (System.nanoTime() - start);
								// long start = System.nanoTime();
								// System.out.println("Start aggregation");
								start = System.nanoTime();
								rObj = sbe.doAggregateOnSampledBits(sampledResult, measureValues,"avg",sampledInstances);
								groupAGG.get(groups[i]).avg = samplingcount[i]*groupAGG.get(groups[i]).avg + rObj.count*rObj.average;
								groupAGG.get(groups[i]).avg = groupAGG.get(groups[i]).avg / (samplingcount[i] + rObj.count);
								aggregationTime += (System.nanoTime() - start);
								// System.out.println("Sampling
								// Time:"+(System.nanoTime()-start)/1000000);
								withoutReplacementIndex.put(groups[i], sampledResult.getLastIndex()); // save
																										// it
																										// in
																										// the
																										// map
	
							}
	
							else {
	
								// groupAGG.get(groups[i]).avg +=
								// sbe.getSimpleSampledAggregate(groupbyAttribute,
								// Integer.toString(groups[i]), measureAttribute,
								// sampledInstances, "avg");
								// System.out.println("Start sampling");
								long start = System.nanoTime();
								if(filterAttribute.equals("")) 
									sampledResult = sbe.getSampledSimpleGroup(groupbyAttribute, Integer.toString(groups[i]),sampledInstances);
								else
									sampledResult = sbe.getFilterSampledGroup(groupbyAttribute, Integer.toString(groups[i]),filterAttribute,filterValue,sampledInstances);
			
								samplingTime += (System.nanoTime() - start);
								// long start = System.nanoTime();
								// System.out.println("Start aggregation");
								start = System.nanoTime();
								rObj = sbe.doAggregateOnSampledBits(sampledResult, measureValues, "avg",sampledInstances);
								
								//double oldAvg = groupAGG.get(groups[i]).avg;
								groupAGG.get(groups[i]).avg = samplingcount[i] * groupAGG.get(groups[i]).avg
										+ rObj.count * rObj.average;// *******count
																																// might
																																// not
																																// equal
																																// sampledinstances
	
								groupAGG.get(groups[i]).avg = groupAGG.get(groups[i]).avg / (samplingcount[i] + rObj.count);
								aggregationTime += (System.nanoTime() - start);
								
								//best.write(groups[i]+","+Math.abs(oldAvg-groupAGG.get(groups[i]).avg)+"\n"); 
							}
							
							
							segment_tmp_avg += groupAGG.get(groups[i]).avg;
							samplingcount[i]+=rObj.count;
						}
					}

					prevSeg.get(s).mean = segment_tmp_avg * (1.0 / (seg_end - seg_start + 1));

				}
			}

			// timedata.write(samplingTime/ 1000000 + "," +aggregationTime/
			// 1000000+"," );
			//best.close();
			// timedata.write((System.nanoTime()-sampAggTime)/ 1000000+",");
			long histoTime = System.nanoTime();
			for (int s = 0; s < prevSeg.size(); s++) {

				segTemp = prevSeg.get(s);
				seg_start = segTemp.start;
				seg_end = segTemp.end;

				if (seg_end - seg_start < 1) {

					continue;
				}

				// calculate segment means for different choice of split and
				// find best split
				double alphaT = 0;
				double alphaU = 0;
				double seg_mean = 0;
				double best_mean1 = 0;
				double best_mean2 = 0;
				double tmp1, tmp2;
				double max = -1000000000;
				int maxSplit = -1;

				// estimate segment means means. calc alpha(i,j) for i=2 to m-1
				for (int i = seg_start; i <= seg_end; i++) {
					for (int j = seg_start; j <= i; j++) {
						alphaT += groupAGG.get(groups[j]).avg;

					}

					alphaT = (1.0 / (i - seg_start + 1)) * alphaT; // V_s
					tmp1 = alphaT;
					//seg_mean += alpha * alpha * (i - seg_start + 1);

					
					for (int j = i + 1; j <= seg_end; j++) {
						alphaU += groupAGG.get(groups[j]).avg;

					}

					alphaU = (1.0 / (seg_end - (i + 1) + 1)) * alphaU; // V_s
					if (i == seg_end)
						alphaU = 0;
					tmp2 = alphaU;
					//seg_mean += alpha * alpha * (seg_end - (i + 1) + 1); // |S|sum(V_s)
					seg_mean = (i - seg_start + 1)*(seg_end - (i + 1) + 1)*(alphaT-alphaU)*(alphaT-alphaU)/(groups.length*(seg_end-seg_start+1));
							
					if (seg_mean > max) {
						max = seg_mean;
						maxSplit = i;
						best_mean1 = tmp1;
						best_mean2 = tmp2;

						
					}

					seg_mean = 0;
					alphaT = 0;
					alphaU=0;
				}

				// find best GLOBAL histogram

				//global_seg_mean += (maxSplit - seg_start + 1) * best_mean1 * best_mean1;
				//global_seg_mean += (seg_end - (maxSplit + 1) + 1) * best_mean2 * best_mean2;

				global_seg_mean = max;
				
				/*for (int g = 0; g < prevSeg.size(); g++) {
					if (g != s) {
						global_seg_mean += (prevSeg.get(g).end - prevSeg.get(g).start + 1) * prevSeg.get(g).mean
								* prevSeg.get(g).mean;

					}

				}*/

				if (global_seg_mean > global_max) {
					global_max = global_seg_mean;

					split_segment_no = s;
					split_segment_maxSplit = maxSplit;

					global_best_mean1 = best_mean1;
					global_best_mean2 = best_mean2;

				}

				global_seg_mean = 0;

			}

			// find best histogram. can be improved by bunary search to log(n)

			for (int s = 0; s < prevSeg.size(); s++) {
				if (s == split_segment_no) {
					// System.out.println("Split Group:
					// "+groups[split_segment_maxSplit]);
					if (split_segment_maxSplit == prevSeg.get(s).end) {
						tempSeg.add(new Segment(prevSeg.get(s).start, split_segment_maxSplit - 1, global_best_mean1));
						tempSeg.add(new Segment(split_segment_maxSplit, prevSeg.get(s).end, global_best_mean2));
					} else {
						tempSeg.add(new Segment(prevSeg.get(s).start, split_segment_maxSplit, global_best_mean1));
						tempSeg.add(new Segment(split_segment_maxSplit + 1, prevSeg.get(s).end, global_best_mean2));
					}

				} else {
					prevSeg.get(s).isNew = false;
					tempSeg.add(prevSeg.get(s));
				}
			}

			kBest.put(k, tempSeg);

			// timedata.write((System.nanoTime()-histoTime)/ 1000000+",");
			returnTime = System.nanoTime();
			genJSON(k, jsonSave);
			return JSONObject.toJSONString(jObjF);
		}

		// return "";

	}

	public String genHistoBaseline(String str, String jsonSave) throws IOException {
		int k = Integer.parseInt(str);

		System.out.println("ep: "+epsilon+", alpha: "+alpha);
		int seg_start, seg_end;

		Arrays.sort(groups);

		// incrementally approx histogram

		int sampledInstances=0;

		seg_start = 0;
		seg_end = groups.length - 1;

		// BufferedWriter avgApprox=null;
		// BufferedWriter segmentVal = null;
		// BufferedWriter segmentJSON = null;
		//BufferedWriter best = new BufferedWriter(new FileWriter("estDifference.csv",true));
		JSONObject jObj;
		JSONArray jArr;
		BufferedWriter segmentJSON = null;

		// for k=1 sample from 1 to m

		// for k > 1, retieve segments from kBest(k-1)
		// for each segment find best split and calc sum avg of new found
		// segments
		// keep track of the best-k segment, i.e. maximizing sum
		// insert that in to kBest[k]

		if (firstIteration) {
			
			SampledResult sampledResult;

			ResultObject rObj=null;
			for (int i = seg_start, grp = 1; i <= seg_end; i++, grp++) {
				//System.out.println(i+"=="+groups[i]+"=="+samplingcount[i]);
				if(samplingcount[i]!=-1)
				{
					// count the amount of sampling to do
					
						sampledInstances = (int) (sampledInstancePerIter[k-1]/groups.length); 
						/*sampledInstances = (int) Math.ceil((Math.log((1.0 * (seg_end - seg_start + 1)) / delta) / Math.log(2))
										* (1.0*c*c / ((Math.min(grp*grp, (seg_end - seg_start + 2 - grp)*(seg_end - seg_start + 2 - grp))) * epsilon * epsilon)));*/
						
						
	
					// double avg;
					
					if (this.withoutReplacement) {
						sampledResult = sbe.getSampledSimpleGroupWithoutReplacement(groupbyAttribute,
								Integer.toString(groups[i]), sampledInstances);
						rObj = sbe.doAggregateOnSampledBits(sampledResult, measureValues, "avg",sampledInstances);
						groupAVGbase[i] += rObj.average; 
						
						withoutReplacementIndex.put(groups[i], sampledResult.getLastIndex()); // save
																							
					}
	
					else {
	
						if(filterAttribute.equals("")) 
							sampledResult = sbe.getSampledSimpleGroup(groupbyAttribute, Integer.toString(groups[i]),sampledInstances);
						else
							sampledResult = sbe.getFilterSampledGroup(groupbyAttribute, Integer.toString(groups[i]),filterAttribute,filterValue,sampledInstances);
	
						rObj = sbe.doAggregateOnSampledBits(sampledResult, measureValues, "avg",sampledInstances);

						if(rObj.count>0)
						{
							if(xMinF==-1)
							{
								xMinF = i;
								k=groups[i];
							}
							if(i > xMaxF)
								xMaxF= i;
							groupAVGbase[i] += rObj.average;
						}
						
						else
						{
							groupAVGbase[i] = 0;
							samplingCountBase[i] = -1;
							
						}
						
					
					}
	
					
					samplingCountBase[i]+=rObj.count;
				}
				//System.out.println(xMinF+"---"+xMaxF);
			}
			
			
			seg_start = xMinF;
			seg_end = xMaxF;
			
			if (!jsonSave.equals("2")) {
				genJSON(k, jsonSave);
				firstIteration = false;
				return JSONObject.toJSONString(jObjF);
			}
			firstIteration = false;
			return "";

		} else {
			
			
			
			sampledInstances = (int) (sampledInstancePerIter[k-1]/groups.length); 
			
				SampledResult sampledResult;
					ResultObject rObj=null;
					for (int i = seg_start, grp = 1; i <= seg_end; i++, grp++) {
						if(samplingcount[i]!=-1)
						{
							if (this.withoutReplacement) {
								sampledResult = sbe.getSampledSimpleGroupWithoutReplacement(groupbyAttribute,
										Integer.toString(groups[i]), sampledInstances,
										withoutReplacementIndex.get(groups[i]));
							
								rObj = sbe.doAggregateOnSampledBits(sampledResult, measureValues,"avg",sampledInstances);
								withoutReplacementIndex.put(groups[i], sampledResult.getLastIndex()); // save
																										
							}
	
							else {
	
								
								if(filterAttribute.equals("")) 
									sampledResult = sbe.getSampledSimpleGroup(groupbyAttribute, Integer.toString(groups[i]),sampledInstances);
								else
									sampledResult = sbe.getFilterSampledGroup(groupbyAttribute, Integer.toString(groups[i]),filterAttribute,filterValue,sampledInstances);
			
								
								rObj = sbe.doAggregateOnSampledBits(sampledResult, measureValues, "avg",sampledInstances);
								
								//double oldAvg = groupAGG.get(groups[i]).avg;
								groupAVGbase[i]  = samplingCountBase[i] * groupAVGbase[i] 
										+ rObj.count * rObj.average;// *******count
																																// might
																																// not
																																// equal
																																// sampledinstances
	
								groupAVGbase[i]  = groupAVGbase[i]  / (samplingCountBase[i] + rObj.count);
								
							}
							
							
							samplingCountBase[i]+=rObj.count;
						}
					}

					

			genJSON(k, jsonSave);
			return JSONObject.toJSONString(jObjF);
		}

		// return "";

	}

}

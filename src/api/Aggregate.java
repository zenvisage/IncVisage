/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package api;

/**
 *
 * @author srahman7
 */
public class Aggregate {
    double sum;
    int count;
    double avg;
    public Aggregate()
    {
        this.sum=0;
        this.count=0;
        this.avg=0;
    }
    
    public void aggreCalc(String case_, double value)
    {
        
    	if(case_.equals("avg"))
    	{
    		
    		this.avg += value;
    	}
    	else if(case_.equals("sum"))
    	{
    		
    		this.sum+= value;
    	}
    	else
    	{
    		
    		this.count++;
            
    	}
    	
    	    
    }
}

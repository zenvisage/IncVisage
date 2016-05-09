/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package api;

import java.util.ArrayList;

/**
 *
 * @author srahman7
 */
public class CellObject {
    public int value;
    double sum;
    int count;
    double avg;
    
    
    public CellObject(int value)
    {
        this.value =value;
        
        this.sum=0;
        this.count=0;
        this.avg=0;
       
    }
    
       
}

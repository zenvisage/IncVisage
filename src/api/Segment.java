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
public class Segment {
    int start;
    int end;
    double mean;
    boolean isNew;
    
    public Segment(int start, int end, double mean)
    {
        this.start = start;
        this.end = end;
        this.mean = mean;
        this.isNew = true;
    }
}

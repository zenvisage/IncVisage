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
public class Quadrant {
    int xStart;
    int xEnd;
    int yStart;
    int yEnd;
    double mean;
    boolean isNew;
    
    public Quadrant(int xstart, int xend, int ystart, int yend, double mean)
    {
        this.xStart = xstart;
        this.xEnd = xend;
        this.yStart = ystart;
        this.yEnd = yend;
        this.mean = mean;
        this.isNew = true;
    }
}

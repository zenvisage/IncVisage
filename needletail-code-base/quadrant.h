#ifndef _QUADRANT_H_
#define _QUADRANT_H_

class Quadrant
{
public:
	Quadrant(int x_start, int x_end, int y_start, int y_end, float mean);
	Quadrant();

	void set_xStart(int s){x_start = s;}
	void set_xEnd(int s){x_end = s;}
	void set_yStart(int s){y_start = s;}
	void set_yEnd(int s){y_end = s;}
	void set_mean(float s){mean = s;}
	int get_xStart() { return x_start;}
	int get_xEnd() { return x_end;}
	int get_yStart() { return y_start;}
	int get_yEnd() { return y_end;}
	float get_mean() { return mean;}
	
protected:	
	int x_start;
	int x_end;
	int y_start;
	int y_end;
	float mean;
};

#endif /* _QUADRANT_H_ */
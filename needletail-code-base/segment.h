#ifndef _SEGMENT_H_
#define _SEGMENT_H_

class Segment
{
public:
	Segment(int start, int end,float mean);
	Segment();

	void set_start(int s){start = s;}
	void set_end(int s){end = s;}
	void set_mean(float s){mean = s;}
	int get_start() { return start;}
	int get_end() { return end;}
	float get_mean() { return mean;}
	
protected:	
	int start;
	int end;
	float mean;
};

#endif /* _SEGMENT_H_ */
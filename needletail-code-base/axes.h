#ifndef _AXES_H_
#define _AXES_H_

class Axes
{
public:
	
	Axes(int xMin,int xMax);
	Axes(int xMin,int xMax,int yMin,int yMax);
	Axes();
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	
};

#endif /* _AXES_H_ */
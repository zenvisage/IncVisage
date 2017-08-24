#include "quadrant.h"

using namespace std;

Quadrant::Quadrant(int x_start, int x_end, int y_start, int y_end, float mean)
{
	this->x_start = x_start;
	this->x_end = x_end;
	this->y_start = y_start;
	this->y_end = y_end;
	this->mean = mean;
	
}

Quadrant::Quadrant()
{

	
}

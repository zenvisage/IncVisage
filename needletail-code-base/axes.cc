#include "axes.h"

using namespace std;

Axes::Axes(int xMin,int xMax)
{
	this->xMin = xMin;
	this->xMax = xMax;
}

Axes::Axes(int xMin,int xMax,int yMin,int yMax)
{
	this->xMin = xMin;
	this->xMax = xMax;
	this->yMin = yMin;
	this->yMax = yMax;	
}

Axes::Axes()
{
	
}
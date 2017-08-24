#ifndef _HEAT_H_
#define _HEAT_H_
#include <map>
#include "axes.h"
#include "resultobject.h"
#include "quadrant.h"
#include "sdb.h"
#include "schema.h"
#include "swift_index.h"
class Heat
{
public:
	std::map<std::string, std::string> dimMap;
	std::map<std::string, std::string> measMap;
	std::map<std::string, std::string> filterMap;
	std::map<std::string,int> filterValueMap;
	std::map<int, int> groupXMap;
	std::map<int, int> groupYMap;
	
	std::map<std::string,float> groupAvgMap;
	std::string dataset;
	std::string dimXAttr;
	std::string dimYAttr;
	std::string measAttr;
	std::string filterAttr;
	int filterVal;
	bool firstIteration;
	bool isTwoSeg;
	bool isOneCell;
	int cardX;
	int cardY;
	int split_quad;
	Quadrant firstNewQuadrant;
	Quadrant secondNewQuadrant;
	Quadrant thirdNewQuadrant;
	Quadrant fourthNewQuadrant;
	int queryType;
	int isContinue;
	int xMax;
	int xMin;
	int yMax;
	int yMin;
	float avgMin;
	float avgMax;
	SwiftIndex& swi_heat;
	Heat(SwiftIndex& swi_heat);
	void setMaps(std::string dataset);
	void loadAxesData(std::string dimX,std::string dimY,std::string meas,std::string filter,std::string filterValue);
	void genHeat(int iteration,SDB& db11,Schema& schema11);
	void genHeatBase(int iteration,SDB& db11,Schema& schema11);
	void reset();
	int toContinue();
	bool validCells(int gX,int gY);
	
};

#endif /* _HEAT_H_ */
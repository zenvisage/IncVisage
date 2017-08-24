#ifndef _TREND_H_
#define _TREND_H_
#include <map>
#include "axes.h"
#include "resultobject.h"
#include "segment.h"
#include "sdb.h"
#include "schema.h"
#include "swift_index.h"
class Trend
{
public:
	std::map<std::string, std::string> dimMap;
	std::map<std::string, std::string> measMap;
	std::map<std::string, std::string> filterMap;
	std::map<std::string,int> filterValueMap;
	std::map<int, int> groupMap;
	std::map<int,float> groupAvgMap;
	std::string dataset;
	std::string dimAttr;
	std::string measAttr;
	std::string filterAttr;
	int filterVal;
	bool firstIteration;
	int card;
	int split_seg;
	Segment firstNewSegment;
	Segment secondNewSegment;
	int queryType;
	int isContinue;
	int xMin;
	int xMax;
	float avgMin;
	float avgMax;
	SwiftIndex& swi;
	Trend(SwiftIndex& swi);
	void setMaps(std::string dataset);
	void loadAxesData(std::string dim,std::string meas,std::string filter,std::string filterValue);
	void genHisto(int iteration,SDB& db1,Schema& schema1);
	void genHistoBase(int iteration,SDB& db1,Schema& schema1);
	void reset();
	int toContinue();
	
};

#endif /* _Trend_H_ */
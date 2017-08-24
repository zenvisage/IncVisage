#ifndef _RESULTOBJECT_H_
#define _RESULTOBJECT_H_
#include "segment.h"
class ResultObject
{
public:

	int seg_no;
	Segment& seg;

	ResultObject(int seg_no,Segment& seg);

	
};

#endif /* _RESULTOBJECT_H_ */
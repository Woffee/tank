#ifndef T_UTIL_H
#define T_UTIL_H

#include<stdlib.h>

class T_Util
{
public:
	static void GetBevelSpeed(IN POINT startPT,IN POINT destPT,IN int Speed,OUT int& xRatio,OUT int& yRatio);
};

#endif
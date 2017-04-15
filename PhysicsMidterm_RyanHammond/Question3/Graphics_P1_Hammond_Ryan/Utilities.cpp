#include "Utilities.h"

float getRandFloat(float LO, float HI)
{
	float r3 = LO +
		static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return r3;
}
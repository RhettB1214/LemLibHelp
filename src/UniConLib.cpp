#include <iostream>
#include "UniConLib.h"

namespace UniCon
{
	float Convert::mToIn(float input)
	{
		return input * 39.3701;
	}

	float Convert::inToM(float input)
	{
		return input / 39.3701;
	}

	float Convert::mmToIn(float input)
	{
		return input / 25.4;
	}

	float Convert::inToMm(float input)
	{
		return input * 25.4;
	}

	float Convert::mToFt(float input)
	{
		return input * 3.28084;
	}

	float Convert::ftToM(float input)
	{
		return input / 3.28084;
	}

	float Convert::mmToM(float input)
	{
		return input / 1000;
	}

	float Convert::mToMm(float input)
	{
		return input * 1000;
	}
}
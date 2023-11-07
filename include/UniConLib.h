#pragma once


namespace UniCon
{

	class Convert
	{
	public:

		/*Converts input from meters to inches*/
		static float mToIn(float input);

		/*Converts input from inches to meters*/
		static float inToM(float input);

		/*Converts input from millimeters to inches*/
		static float mmToIn(float input);

		/*Converts input from inches to millimeters*/
		static float inToMm(float input);

		/*Converts input from meters to feet*/
		static float mToFt(float input);

		/*Converts input from feet to meters*/
		static float ftToM(float input);

		/*Converts input from millimeters to meters*/
		static float mmToM(float input);

		/*Converts input meters to millimeters*/
		static float mToMm(float input);
	};
}

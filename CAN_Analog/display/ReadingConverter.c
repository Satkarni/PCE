/*
 * ReadingConverter.c
 *
 *  Created on: 17-Aug-2016
 *      Author: piyush
 */

#include "ReadingConverter.h"

void readingConverter_getRealValue(char *name, uint32_t rawValue, char* realValue)
{
	double temp;
	if (strcmp(name,"RPM") == 0)
	{

		if(checkFailure(name))
		{
			strcpy(realValue, "Fail");
		}
		else
		{
			temp = (double)rawValue * 0.125;
			temp = ceil(temp);
			dtostrf(temp,1,0,realValue);
		}
	}
	else if(strcmp(name,"LOP") == 0)
	{
		if(checkFailure(name))
		{
			strcpy(realValue, "Fail");
		}
		else
		{
			temp = (double)rawValue * 0.04;
			dtostrf(temp,4,2,realValue);
		}
	}

	else if(strcmp(name,"LOT") == 0)
	{
		if(checkFailure(name))
		{
			strcpy(realValue, "Fail");
		}
		else
		{
			temp = (double)rawValue * 0.03125;
			temp -= 273;
			dtostrf(temp,1,0,realValue);
		}
	}

	else if(strcmp(name,"ECT") == 0)
	{
		if(checkFailure(name))
		{
			strcpy(realValue, "Fail");
		}
		else
			itoa(rawValue-40,realValue,10);
	}
	else if(strcmp(name,"BattVGE") == 0)
	{
		if(checkFailure(name))
		{
			strcpy(realValue, "Fail");
		}
		else
		{
			temp = (double)rawValue * 0.05;
			dtostrf(temp,3,2,realValue);
		}
	}
	else if(strcmp(name, "Hours") == 0)
	{
		if(checkFailure(name))
		{
			strcpy(realValue, "Fail");
		}
		else
		{
			temp = (double)rawValue * 0.05;
			dtostrf(temp,3,2,realValue);
		}
	}
	else if(strcmp(name, "ExTmp") == 0)
	{
		if(checkFailure(name))
		{
			strcpy(realValue, "Fail");
		}
		else
		{
			temp = (double)rawValue*0.9766; 				//rawValue	* (5000/1024) * 1/5
			temp = ceil(temp);
			dtostrf(rawValue,1,0,realValue);
		}
	}
	else if(strcmp(name,"GrBxTmp") == 0)
	{
		if(checkFailure(name))
		{
			strcpy(realValue, "Fail");
		}
		else
		{
//			temp = (double)rawValue * 0.4883;							//rawValue * (5000/1024) * 1/10  *FOR PT100 on SPARE CHANNEL
			temp = ((double)rawValue - 196.608) * 0.190735;		//((rawValue-196.608) / (983.04-196.608)) * 150    *FOR 4-20 on ORIGINAL CHANNEL
			temp = ceil(temp);
			dtostrf(temp,1,0,realValue);
		}
	}
	else if(strcmp(name,"GrBxPr") == 0)
	{
		if(checkFailure(name))
		{
			strcpy(realValue, "Fail");
		}
		else
		{
			temp = ((double)rawValue - 196.608) / 19.6608;
			dtostrf(temp,2,1,realValue);
		}

	}
	else if(strcmp(name, "Linear") == 0)
	{
		itoa(rawValue,realValue,10);
	}
	else
	{
		strcpy(realValue,"No Data");
	}
}

/*
 * AlarmPage.c
 *
 *  Created on: 12-Aug-2016
 *      Author: piyush
 */


#include "AlarmPage.h"

void alarmPage_initAlarms()
{
	Enter_Critical_Section
	for(uint8_t i=0; i<ALARM_LIST_LENGTH; i++)
	{
		alarms[i].enabled = 0;
		trips[i].enabled = 0;
	}

	 strcpy(alarms[RPM_ALARM].value, "Overspeed");
	 strcpy(alarms[LOP_ALARM].value, "Lub Oil Pr. Low");
	 strcpy(alarms[LOT_ALARM].value, "Lub Oil Tmp. High");
	 strcpy(alarms[ECT_ALARM].value, "Water Tmp High");
	 strcpy(alarms[GBP_ALARM].value,"GearBox Oil Pr. High");
	 strcpy(alarms[GBT_ALARM].value,"GearBox Oil Tmp. High");
	 strcpy(alarms[EXT_ALARM].value,"Exhaust Tmp. High");

	 strcpy(trips[RPM_TRIP].value, "Overspeed");
	 strcpy(trips[LOP_TRIP].value, "Lub Oil Pr. Trip");
	 strcpy(trips[LOT_TRIP].value, "Lub Oil Tmp. Trip");
	 strcpy(trips[ECT_TRIP].value, "Water Tmp Trip");
	 strcpy(trips[GBP_TRIP].value, "GearBox Oil Pr. Trip");
	 strcpy(trips[GBT_TRIP].value, "GearBox Oil Tmp Trip");
	 strcpy(trips[EXT_TRIP].value, "Exhaust Tmp. Trip");
	 Leave_Critical_Section
}

void alarmPage_clearList()
{
//	uint8_t line_index, i, page_number;
//	page_number = 0;
//	line_index = 0;
//	alarms_count = 0;
//	trips_count = 0;
//	for (i=0; i < ALARM_LIST_LENGTH; i++)
//	{
//		alarmp_page[page_number].lineText[line_index] = 0;
//		line_index += 1;
//
//		if(line_index == ALARM_ENTRIES-1)
//		{
//			line_index = 0;
//			page_number += 1;
//		}
//	}
	for(uint8_t i=0; i<5; i++)
		activeAlarmList[i] = ALARM_LIST_LENGTH;
}

void alarmPage_generatePages()
{
	Enter_Critical_Section
	alarmPage_clearList();
	uint8_t line_index, i;
	line_index = 0;
	for (i=0; i < ALARM_LIST_LENGTH; i++)
	{
		if(trips[i].enabled == 1)
		{
			trips_count += 1;
//			alarmp_page[page_number].lineText[line_index] = trips[i].value;
			activeAlarmList[line_index] = i;
			line_index += 1;
		}
		if(alarms[i].enabled == 1 && trips[i].enabled == 0)
		{
			alarms_count += 1;
//			alarmp_page[page_number].lineText[line_index] = alarms[i].value;
			activeAlarmList[line_index] = i;
			line_index += 1;
		}
//		if(line_index == ALARM_ENTRIES-1)
//		{
//			line_index = 0;
//			page_number += 1;
//		}
	}
	max_alarm_page = (alarms_count+trips_count)/5;
	Leave_Critical_Section
}

uint8_t alarmPage_resetHead()
{
	alarmpage_head = 0;
	alarmPage_generatePages();
	return alarmpage_head;
}

uint8_t alarmPage_nextPage()
{
	alarmpage_head = (alarmpage_head >= max_alarm_page) ? 0 : alarmpage_head+1;
	return alarmpage_head;
}

uint8_t alarmPage_prevPage()
{
	alarmpage_head = (alarmpage_head <= 0) ? max_alarm_page : alarmpage_head-1;
	return alarmpage_head;
}

void alarmPage_drawStaticPage()
{
	Enter_Critical_Section

	ks0108ClearScreen();
	ks0108SelectFont(sanserif_9,ks0108ReadFontData,BLACK);
	ks0108Puts_mod_align_center("Active Warnings",0,127,1,100);
	ks0108InvertRect(0,0,127,10);
	Leave_Critical_Section
}

void alarmPage_drawDynamicPage()
{
	Enter_Critical_Section
	uint8_t j;
	ks0108FillRect(0,11,127,54,WHITE);
	ks0108SelectFont(sanserif_9,ks0108ReadFontData,BLACK);

	for(j=0; j < 5; j++)
	{
//		ks0108Puts_mod_align_center(alarmp_page[alarmpage_head].lineText[i],0,50,((i+1)*11)+1,50);
		if(activeAlarmList[j] == ALARM_LIST_LENGTH)
			break;
		if(trips[activeAlarmList[j]].enabled == 1)
		{
//			ks0108Puts_mod_align_center(trips[activeAlarmList[j]].value,0,127,((j+1)*11)+1,127);
			ks0108Puts_mod(trips[activeAlarmList[j]].value,0,((j+1)*11)+1,127);
		}

		if(alarms[activeAlarmList[j]].enabled == 1 && trips[activeAlarmList[j]].enabled == 0)
		{
//			ks0108Puts_mod_align_center(alarms[activeAlarmList[j]].value,0,127,((j+1)*11)+1,127);
			ks0108Puts_mod(alarms[activeAlarmList[j]].value,0,((j+1)*11)+1,127);
		}
	}
//	if (alarms_count > ALARM_ENTRIES)
//		{
//			if(alarmpage_head == 0)
//				ks0108DrawPageArrow(DOWN);
//
//			else if (alarmpage_head == max_alarm_page)
//				ks0108DrawPageArrow(UP);
//
//			else
//			{
//				ks0108DrawPageArrow(DOWN);
//				ks0108DrawPageArrow(UP);
//			}
//		}

	Leave_Critical_Section
}

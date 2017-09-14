/*
ioo * DTCPage.c
 *
 *  Created on: 19-Sep-2016
 *      Author: piyush
 */

#include "DTCPage.h"

void dtcPage_drawStaticPage()
{
	Enter_Critical_Section
	ks0108ClearScreen();
	ks0108SelectFont(sanserif_9, ks0108ReadFontData, BLACK);

	ks0108Puts_mod_align_center("Under Construction",0,127,15,100);
	Leave_Critical_Section
}

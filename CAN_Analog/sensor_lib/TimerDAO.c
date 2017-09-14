/*
 * TimerDAO.c
 *
 *  Created on: 16-Jul-2016
 *      Author: piyush
 */

#include "TimerDAO.h"
Timer *timer_first = NULL;

void showTimerQueue() //TESTING FUNCTION ONLY!! REMOVE IN FINAL BUILD!
{
	Timer *p = timer_first;
	printf("\nSTART -> ");
	for(;p != NULL;p = p->next)
		printf("%s -> ",p->name);
	printf("END");
}

uint8_t addTimer(char *name)
{
	if(timer_first == NULL)
	{
		timer_first = (Timer*)malloc(sizeof(Timer));

		if(timer_first == NULL)
			return 0;
		strcpy(timer_first->name,name);
		timer_first->next = NULL;
	}

	else
	{
		Timer *p = timer_first;

		while (p->next != NULL)
			p = p->next;

		p->next = (Timer*)malloc(sizeof(Timer));

		if(p->next == NULL)
			return 0;

		p = p->next;
		strcpy(p->name,name);
		p->next = NULL;
	}
	return 1;
}

uint8_t removeTimer(char *name)
{
	if (timer_first == NULL)
		return 0;
	Timer *p = timer_first, *q = p->next;
	if(strcmp(name,p->name) == 0)
	{
		timer_first = q;
		free(p);
		return 1;
	}
	while(q != NULL)
	{
		if(strcmp(name,q->name) == 0)
		{
			p->next = q->next;
			free(q);
			return 1;
		}
		p = p->next;
		q = q->next;
	}
	return 0;
}

Timer* getTimerByName (char *name)
{
	if(timer_first == NULL)
		return NULL;
	Timer *p = timer_first;
	while(p != NULL)
	{
		if(0 == strcmp(p->name,name))
			return p;
		else
			p = p->next;
	}
	return NULL;
}

Timer* getTimerByPosition (uint8_t position)
{
	if(timer_first == NULL)
		return NULL;
	Timer *p = timer_first;
	for(uint8_t i=0; i < position; i++)
	{
		p = p->next;
		if(p == NULL)
			return NULL;
	}
	return p;
}


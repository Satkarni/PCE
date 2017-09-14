/*
 * modbusDaemon.c
 *
 *  Created on: 03-Nov-2016
 *      Author: piyush
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <modbus/modbus.h>

#define DAEMON_NAME	"modbusd"

#define WAIT_PERIOD		60									///<Wait period (in seconds) between every log attempt
#define PLC_IP				"192.168.10.42"				///<IP Address of the PLC (Applicable for Ethernet)
#define PORT_NO			502								///<Port number of the PLC	(Applicable for Ethernet)
#define WORKING_DIR	"/home/piyush/plc_logs"	///<Working directory for the data logger files
#define USER_DIR			"/home/piyush"				///<Home directory of the current user

//#define MODBUS_ETHERNET								///<Device will use Ethernet (Ensure RS485 is disabled)
#define MODBUS_RS485										///<Device will use RS485	(Ensure Ethernet is disabled)

#define SLAVE_ADDRESS	1								///<Slave address of PLC (Applicable for RS485)
#define BAUD_RATE			115200UL					///<Serial baudrate. Default is 115200. (Applicable for RS485)
#define PARITY					'N'								///<Set to 'N' for None, 'E' for Even, 'O' for Odd. (Applicable for RS485)
#define DATA_BITS				8								///<Data bits per byte (Applicable for RS485)
#define STOP_BITS			1								///<Number of stop bits (Applicable for RS485)

FILE  *fp;
modbus_t *ctx;
time_t rawTime;
struct tm *timeInfo;

/**
 *	@brief Handles system signals (eg. SIGTERM)
 *	@param signal Value of the signal
 *
 *	This is an internal function which handles any signals sent by a *nix system to the daemon.
 *	DO NOT EDIT THIS FUNCTION.
 */
void signal_handler(int signal);

/**
 * @brief Prints a log entry to events.log.
 * @param string Constant string to be printed.
 *
 * Prints the string argument along with the current date/time on a new line. All log entries are automatically appended
 * to the events.log file in WORKING_DIR.
 */
void printlog(const char *string);

/**
 * @brief Generates a folder structure in WORKING_DIR and opens a file for data logging.
 *
 * The default folder structure is <current_year>/<current_month>-<current_date>.csv.
 * A file pointer is created in variable pointer *fp. Note that the file needs to be closed by the user.
 */
void generateFileStructure(void);

/**
 * @brief Main process function
 *
 * All modbus requests, responses and data logging is handled by this function. It generally handles all
 * the required processing to be done within the inifinite loop.
 */
void process(void);

/**
 * @brief Initialises the program as a daemon on a *nix system
 *
 * This function is called only once before the infinite loop. It fulfils all requirements required
 * for a daemon to run. The main program is forked, and all interactive file descriptors are closed.
 * Note that logging does not work if forking fails, or if the daemon cannot find the user directory.
 */
void daemonize(void);

void signal_handler(int signal)
{
	switch (signal)
	{
		case SIGTERM :
			printlog("Stopped Daemon");
			if(fp != NULL)
				fclose(fp);
			if(ctx != NULL)
			{
				modbus_close(ctx);
				modbus_free(ctx);
			}
			exit(EXIT_SUCCESS);
			break;
	}
}

void printlog(const char *string)
{
	FILE *logfile;
	if(chdir(WORKING_DIR) < 0)
	{
		exit(EXIT_FAILURE);
	}
	logfile = fopen("events.log","a");
	if(logfile == NULL)
	{
		exit(EXIT_FAILURE);
	}
	setvbuf(logfile,NULL,_IOLBF,BUFSIZ);
	static char dateTime[20];
	strftime(dateTime,sizeof(dateTime),"%Y-%m-%d %H:%M:%S", timeInfo);

	fprintf(logfile,"%s\t%s\n",dateTime,string);
	fclose(logfile);
}

void generateFileStructure(void)
{
	static char year[8], date[20],	time[20];
	char *pwd;
	char buf[PATH_MAX + 1];

	strftime(year,sizeof(year),"%Y",timeInfo);
	strftime(date,sizeof(date),"%m-%d",timeInfo);
	strftime(time,sizeof(time),"%H:%M:%S", timeInfo);

	pwd = getcwd(buf,PATH_MAX+1);

	if(strstr(pwd,year) == NULL)
	{
		if(strstr(pwd,WORKING_DIR) < 0)
		{
			printlog("Invalid working directory");
			if(ctx != NULL)
			{
				modbus_close(ctx);
				modbus_free(ctx);
			}
			exit(EXIT_FAILURE);
		}
		if(chdir(year) < 0)
		{
			if(mkdir(year,0700) < 0)
			{
				printlog("Unable to create directory for current year");
				if(ctx != NULL)
				{
					modbus_close(ctx);
					modbus_free(ctx);
				}
				exit(EXIT_FAILURE);
			}
			else
			{
				printlog("Created log directory for current year");
			}
			if(chdir(year) < 0)
			{
				printlog("Unable to change present working directory");
				if(ctx != NULL)
				{
					modbus_close(ctx);
					modbus_free(ctx);
				}
				exit(EXIT_FAILURE);
			}
		}
	}

	strcat(date,".csv");
	fp = fopen(date,"a");
	if(fp == NULL)
	{
		printlog("Failed to open log file");
		if(ctx != NULL)
		{
			modbus_close(ctx);
			modbus_free(ctx);
		}
		exit(EXIT_FAILURE);
	}
	setvbuf(fp,NULL,_IOLBF,BUFSIZ);
	fprintf(fp,"\"%s\"",time);
}

void process(void)
{
	if(ctx == NULL)
	{
#if defined(MODBUS_ETHERNET) && !defined(MODBUS_RS485)
		ctx = modbus_new_tcp(PLC_IP,PORT_NO);
		if(ctx == NULL)
		{
			printlog("Failed to initialise TCP Socket");
			return;
		}
		else
		{
			printlog("Modbus TCP socket initialised");
		}
#elif defined(MODBUS_RS485) && !defined(MODBUS_ETHERNET)
		ctx = modbus_new_rtu("/dev/serial0",BAUD_RATE,PARITY,DATA_BITS,STOP_BITS);
		if(ctx == NULL)
		{
			printlog("Failed to initialise RTU Socket");
			return;
		}
		else
		{
			printlog("Modbus RTS socket initialised");
		}

		if(modbus_set_slave(ctx,SLAVE_ADDRESS) == -1)
		{
			printlog("Invalid Slave Address");
			return;
		}
		else
		{
			printlog("Slave Address Set");
		}
#else
		printlog("Invalid connection parameters.");
		return;

#endif

		if(modbus_connect(ctx) == -1)
		{
			printlog("Connection refused by PLC");
			ctx = NULL;
			return;
		}
		else
		{
			printlog("Connection Established");
		}
	}

	uint16_t *read_reg = NULL;
	int reg_count, i;

	generateFileStructure();

	read_reg = (uint16_t *)malloc(20 * sizeof(uint16_t));
	if(read_reg == NULL)
	{
		printlog("Segmentation error. Unable to allocate sufficient memory.");
		return;
	}

	reg_count = modbus_read_registers(ctx,100,8,read_reg);						///<Reading holding registers 40100 to 40107
	if(reg_count != 20)
	{
		printlog("Registers read does not match requested range");
		return;
	}

	for(i = 0; i < reg_count; i++)
	{
		fprintf(fp,",\"%d\"",read_reg[i]);
	}
	printlog("Sensor readings written to file");

	free(read_reg);
	read_reg = (uint16_t *)malloc(sizeof(uint16_t));
	if(read_reg == NULL)
	{
		printlog("Segmentation error. Unable to allocate sufficient memory.");
		return;
	}

	reg_count = modbus_read_registers(ctx,116,1,read_reg);						///<Reading holding register 40116
	if(reg_count != 1)
	{
		printlog("Registers read does not match requested range");
		return;
	}

	fprintf(fp,",\"%d\"",*read_reg);
//	printlog("Active alarms written to file");

	reg_count = modbus_read_registers(ctx,117,1,read_reg);						///<Reading holding register 40117
	if(reg_count != 1)
	{
		printlog("Registers read does not match requested range");
		return;
	}

	fprintf(fp,",\"%d\"",*read_reg);
	//printlog("Active alarms and trips written to file");


	fprintf(fp,",\"%d\",\"%d\"",read_reg[0],read_reg[1]);
	printlog("Active start block values written to file");
	fprintf(fp,"\n");
	fclose(fp);
}

void daemonize(void)
{
	pid_t pid, sid;

	pid = fork();
	if(pid < 0)
		exit(EXIT_FAILURE);
	else if (pid > 0)
		exit(EXIT_SUCCESS);

	umask(0);

	if(chdir(USER_DIR) < 0)
	{
		exit(EXIT_FAILURE);
	}
	else
	{
		if(chdir(WORKING_DIR) < 0)
		{
			if(mkdir(WORKING_DIR, 0700) < 0)
			{
				exit(EXIT_FAILURE);
			}
			if(chdir(WORKING_DIR) < 0)
			{
				exit(EXIT_FAILURE);
			}
		}
	}

	time(&rawTime);
	timeInfo = localtime(&rawTime);
	printlog("Successfully started Daemon");

	sid = setsid();
	if(sid < 0)
	{
		printlog("Failed to create process group");
		exit(EXIT_FAILURE);
	}


	signal(SIGTERM, signal_handler);

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

int main(void)
{
	daemonize();
	ctx = NULL;

	while(1)
	{
		time(&rawTime);
		timeInfo = localtime(&rawTime);
		process();
		sleep(WAIT_PERIOD);
	}
}


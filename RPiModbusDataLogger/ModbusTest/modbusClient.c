/*
 * modbusClient.c
 *
 *  Created on: 30-Sep-2016
 *      Author: piyush
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <modbus/modbus.h>
#include <string.h>

#define PORT_NO		502

#define BAUDRATE	115200
#define PARITY			'N'
#define STOP_BITS	1
#define DATA_BITS 	8

int useRTU;

void printUsage(void)
{
		fprintf(stderr, "\nUsage: modbusClient [-r [Slave Address] | -e [IP Address]]  <Register Address> [Range]");

		fprintf(stderr,"\n\n -r \tUse modbus over RTU. Requires [Slave Address]");
		fprintf(stderr,"\n -e \tUse modbus over Ethernet. Requires [IP Address].");

		fprintf(stderr, "\n\n<Register Address>\t\t: Start address of holding register");
		fprintf(stderr, "\n[Range]\t\t\t\t: Number of registers to read");
		fprintf(stderr, "\n\nIf Range is not specified, single register will be read\n");
		fprintf(stderr, "\nExample (RTU): ./modbusClient -r 1 16\n");
		fprintf(stderr, "\nThe above example will read register 40016 from slave having address 1. Slave address 0 will broadcast the request\n");
		fprintf(stderr, "\n\nExample (Ethernet): ./modbusClient -e 192.168.10.150 30 5\n");
		fprintf(stderr, "\nThe above example will read registers 40030 to 40035 from device with IP address 192.168.10.150\n\n");
}

int main(int argc, char **argv)
{
	modbus_t *ctx = NULL;
	uint16_t *read_reg;
	uint8_t i, reg_count;
	int address, range = 1;
	long real_address;
	char	*ip_address;

	if(argc >= 2)
	{
		if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1], "--help") == 0)
		{
			printUsage();
			return 0;
		}
		else if(strcmp(argv[1], "-r") == 0)
		{
			useRTU = 1;
		}
		else if(strcmp(argv[1], "-e") == 0)
		{
			useRTU = 0;
		}
		else
		{
			fprintf(stderr, "\nERROR: Invalid Parameter '%s'\n",argv[1]);
			fprintf(stderr, "\nUse -h or --help to show usage\n");
			return -1;
		}
	}
	else
	{
		fprintf(stderr, "\nERROR: Insufficient Arguments.");
		fprintf(stderr, "\nUse -h or --help to show usage\n");
		return -1;
	}

	switch(argc)
	{
	case 2:
		fprintf(stderr, "\nERROR: Insufficient Arguments.");
		fprintf(stderr, "\nUse -h or --help to show usage\n");
		return -1;
		break;

	case 3:
		fprintf(stderr, "\nERROR: Insufficient Arguments.");
		fprintf(stderr, "\nUse -h or --help to show usage\n");
		return -1;
		break;

	case 4:
		break;

	case 5:
		if(strtol(argv[4],NULL,10) <= 0)
		{
			fprintf(stderr, "\nERROR: Range must be greater than zero\n");
			return -1;
		}
		else if (strtol(argv[4],NULL,10) > 260)
		{
			fprintf(stderr, "\nERROR: Range cannot exceed 260\n");
			return -1;
		}
		range = strtol(argv[4],NULL,10);
		break;

	default:
		fprintf(stderr,"\nERROR: Too many arguments\n");
		fprintf(stderr,"\nUse -h or --help to show usage\n");
		return -1;

	}

	if(strtol(argv[2],NULL,10) < 0)
	{
		fprintf(stderr,"\nERROR: Address cannot be negative\n");
		return -1;
	}

	if(useRTU == 0)
	{
		ip_address = argv[2];
		address = strtol(argv[3],NULL,10);
		printf("\n Socket Address: "); fputs(ip_address,stdout); printf(":%d",PORT_NO);
	}
	else
	{
		address = strtol(argv[3],NULL,10);
		printf("\n RTU Parameters....");
		printf("\n Slave Address: "); fputs(argv[2],stdout);
		printf("\n Baudrate: %d",BAUDRATE);
		printf("\n Parity: %c", PARITY);
		printf("\n Stop Bits: %d",STOP_BITS);
	}
	real_address = address + 40000;
	printf("\n\n Register Address: %ld", real_address);
	printf("\n Range: %d\n\n", range);

	if(useRTU == 1)
	{
		ctx = modbus_new_rtu("/dev/serial0",BAUDRATE,PARITY,DATA_BITS,STOP_BITS);
		if (ctx == NULL)
		{
			fprintf(stderr, "\nERROR: Unable to create RTU socket\n");
			return -1;
		}
		if(modbus_set_slave(ctx,strtol(argv[2],NULL,10)) == -1)
		{
			fprintf(stderr, "\nERROR: Invalid Slave Address\n");
			return -1;
		}
	}
	else
	{
		ctx = modbus_new_tcp(ip_address,PORT_NO);
		if (ctx == NULL)
		{
			fprintf(stderr, "\nERROR: Unable to create modbus TCP socket\n");
			return -1;
		}
	}

	if(modbus_connect(ctx) == -1)
	{
		fprintf(stderr, "\nERROR: Connection Refused\n");
		return -1;
	}

	read_reg = (uint16_t *)malloc(range * sizeof(uint16_t));

	reg_count = modbus_read_registers(ctx,address,range,read_reg);
	if(reg_count != range)
	{
		fprintf(stderr, "\nERROR: Register count does not match registers to read: %d\n", reg_count);
	}
	else
	{
		for (i=0; i < range; i++)
		{
			printf("\nAddress = %ld, Value = %d",(real_address + i),read_reg[i]);
		}
		printf("\n");
	}
	modbus_close(ctx);
	modbus_free(ctx);
	return 0;

}

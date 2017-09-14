
#!/bin/bash

gcc -I/usr/local/lib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"modbusClient.d" -MT"modbusClient.o" -o "modbusClient.o" "modbusClient.c"
gcc -L/usr/local/lib/ -o "modbusClient" ./modbusClient.o -lmodbus

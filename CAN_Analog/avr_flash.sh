#!/bin/bash

cd Debug/
#avrdude -pc128 -cusbasp -B0.25 -u -Uflash:w:PCE_CAN.hex:a

avrdude -pc128 -catmelice_isp -u -Uflash:w:PCE_CAN.hex:a

#avrdude -pc128 -catmelice -u -Uflash:w:PCE_CAN.hex:a

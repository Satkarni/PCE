#!/bin/bash

cd Debug/
avrdude -pm128 -catmelice_isp -u -Uflash:w:FTController_Local.hex:a

#!/bin/bash
######################################
#12/12/18
#Dana Olcott
#
#Simple flash command for stm8
#output file located in /Debug/output.ihx
#examples for stm8flash utility
#
# stm8flash -c <stlink|stlinkv2|espstlink> -p <partname> [-s flash|eeprom|0x8000] [-r|-w|-v] <filename>
# ./stm8flash -c stlinkv2 -p stm8s003f3 -w blinky.ihx
# options:
# -d port - port = /dev/port device, default = /dev/ttyUSB0
# For this case, we use /dev/stlinkv2_2

PROCESSOR="stm8s103f3"
PORT="/dev/stlinkv2_2"
DEVICE="STM8S103"
TARGET="output"
DEBUG_DIRECTORY="Debug"
OUTPUT_FILE="$DEBUG_DIRECTORY/$TARGET.ihx"


stm8flash -c stlinkv2 -d $PORT -p $PROCESSOR -w $OUTPUT_FILE



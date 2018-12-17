#!/bin/bash
######################################
#12/12/18
#Dana Olcott
#
#Simple build script for sdcc
#Small Device C Compiler
#STM8 8-bit Processor

#name of input file as arg, if no arg,
#defaults to main.c.  Generates a Debug 
#directory for output.ihx
#
NUM=$#
INPUT_FILE="main.c"
TARGET="output"
DEBUG_DIRECTORY="Debug"
OUTPUT_FILE="$DEBUG_DIRECTORY/$TARGET.ihx"

if test "$NUM" -eq "1";
then
    INPUT_FILE=$1
fi

echo "Input file: $INPUT_FILE"

#if debug directory is there, delete it
if [ -d $DEBUG_DIRECTORY ];
then
    rm -rf $DEBUG_DIRECTORY
fi

mkdir $DEBUG_DIRECTORY

######################################################
#commandline for sdcc - STM8
#include path
PROCESSOR="stm8s103f3"
PORT="stm8"
DEVICE="STM8S103"

CC_OPTIONS="-p$PROCESSOR -m$PORT -V --verbose --std-sdcc99 --debug --out-fmt-ihx -o $OUTPUT_FILE"
DEFS="-D$DEVICE"

#include paths
I_PATH1="./"
I_PATH2="/usr/share/sdcc/include"
I_PATH3="/usr/share/sdcc/lib/stm8"

I_PATH="-I$I_PATH1 -I$I_PATH2 -I$I_PATH3" 


sdcc $CC_OPTIONS $DEFS $INPUT_FILE $I_PATH





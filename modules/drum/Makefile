#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := DrumMachine

include $(IDF_PATH)/make/project.mk

datapart:
	cd drumkit && tar -cf ../data.tar *

partitiontable:
	gen_esp32part.py partitions.csv partitions.bin

#!/bin/sh

amidi -p hw:2,0,0 -S "90 7F 7F"
sleep 1
amidi -p hw:2,0,0 -S "90 7F 00"

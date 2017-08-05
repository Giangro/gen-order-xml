#!/bin/bash

PATH=.:$PATH

echo "starting genordxml...see genordxml.log for details."

genordxml.exe -l genordxml.log -d nelist.db -f NGSTE00000002_2017-08-04_15-00-00 

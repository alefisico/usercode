#!/bin/bash
ls -1 . | while read line
do
	 convert -verbose -density 150 -trim ${line} -quality 100 -sharpen 0x1.0 ${line/.pdf/}.png
done

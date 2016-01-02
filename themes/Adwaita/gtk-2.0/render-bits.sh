#! /bin/bash
#
# Yeah this script is pretty bad and ugly, so?
#
INKSCAPE=/usr/bin/inkscape
OPTIPNG=/usr/bin/optipng
SVG=adwaita-bits.svg
LISTFILE=bits-list.txt
for filename in `cat $LISTFILE`
do
	DIR=`echo $filename | cut -f1 -d '/'`
	if [ '!' -d $DIR ]; 
		then mkdir $DIR; 
	fi
	ID=`echo $filename | tr '/' '_'`
	$INKSCAPE $SVG -i $ID -e $filename.png
	$OPTIPNG -o7 --quiet $filename.png
done

#!/bin/bash
if [[ $# -eq 4 ]]; then
    width=$(identify -format "%w" "$1")> /dev/null
    step=$3
    echo "Creating images..."
    for (( i=0; i<=$width; i=i+$step ))
    do 
        name1="${i}_$1"
        name2="${i}_temp.ppm"
        `./scroll ${i} 0 $1 ${name1}`
        `./sphere ${name1} ${name2}`
        `rm ${name1}`
    done
    echo "Converting to gif..."
    list=`ls -v | grep _temp.ppm`
    `convert -delay $4 -loop 0 ${list} $2`
    `rm *_temp.ppm`

else
    echo "Usage : ./animate.sh <ims> <imd> <px> <delay between each frame>"
    echo "A px value of 5 means that each frame will be computed by scrolling by 5 pixels from the previous frame"
fi

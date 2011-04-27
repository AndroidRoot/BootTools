#!/bin/sh
if [ "$1x" == x ];
then
  echo "Error";
  exit 1;
fi
bootunpack "$1"
mkdir -p "$1-ramdisk"
CPIONAME=`basename $1-ramdisk.cpio.gz`
cd "$1-ramdisk"
gunzip -c "../$CPIONAME" | cpio -i 

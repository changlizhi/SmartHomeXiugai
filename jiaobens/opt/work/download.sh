#!/bin/bash
# downfilelist.sh
url=$1 #����ļ���url�ĸ���
cd /tmp/
if [ ! -f "shock.mp3" ]; then
	rm  shock.mp3
fi
echo "try to download... $url....."
wget -t 0 -c -O shock.mp3 $url
#if [ ! -f "shock.mp3" ]; then
	cp -fr shock.mp3 /tmp/mounts/SD-P1/shock.mp3
#fi
echo "download.finshed......."
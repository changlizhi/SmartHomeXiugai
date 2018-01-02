#!/bin/bash

mac1=`ifconfig|grep 'eth0'|grep -v '\.'|grep HWaddr|cut -d'r' -f 3|cut -d':' -f 1`
mac2=`ifconfig|grep 'eth0'|grep -v '\.'|grep HWaddr|cut -d'r' -f 3|cut -d':' -f 2`
mac3=`ifconfig|grep 'eth0'|grep -v '\.'|grep HWaddr|cut -d'r' -f 3|cut -d':' -f 3`
mac4=`ifconfig|grep 'eth0'|grep -v '\.'|grep HWaddr|cut -d'r' -f 3|cut -d':' -f 4`
mac5=`ifconfig|grep 'eth0'|grep -v '\.'|grep HWaddr|cut -d'r' -f 3|cut -d':' -f 5`
mac6=`ifconfig|grep 'eth0'|grep -v '\.'|grep HWaddr|cut -d'r' -f 3|cut -d':' -f 6`
macall=$mac1$mac2$mac3$mac4$mac5$mac6
echo $macall
tmp=`echo $macall | sed 's/:/ /g'`
       echo -n '' > dec_mac
        
       for hex in $tmp
       do
         dec=`printf "%d" 0x$hex`
         echo $dec >> dec_mac
      done
       
      cat dec_mac | xargs | sed 's/ /:/g; s/:$//g'


#!/bin/bash
malware_db='/home/rakshith/Desktop/malwaredatabase.txt'
scan_db='/home/rakshith/Desktop/newdb.txt'
 
      ./a.out
    i=1  
    find "/home" -type f -exec md5sum {} + > sums.txt
    > location.txt 
    while read line; do  
      
    #Reading each line 
    
    var=$( grep -c $line $malware_db ) 
    if [ $var == 1 ]
    then
    echo "ALERT!! $i Malwares has been detected, Fetching malware location......."
   grep $line "sums.txt" >> location.txt
    i=$((i+1))  
    fi
    done < $scan_db  
    
    i=1
    while read line; do
        echo "Location of Malware $i is"
    echo ${line:34}
    

    i=$((i+1))
    
    
    
    done< 'location.txt'
    
    echo "Do you want to delete all the Vulnerabilities permanently?[1/0]"
    read choice
    if [ $choice == 1 ]
    then
    while read line; do
        
   rm -rf ${line:34}
   
    done < "location.txt"
    fi
    echo "You are now completely safe"
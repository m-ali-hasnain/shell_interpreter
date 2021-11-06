#!/bin/bash
ls -l
read -s -n 1 key  
if [[ $key = "" ]]; then 
    exit 1;
else
    echo "You pressed '$key'"
fi
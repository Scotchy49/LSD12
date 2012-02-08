#!/bin/bash

txtrst=$(tput sgr0) # Text reset
txtred=$(tput setaf 1) # Red
txtgrn=$(tput setaf 2) # Green

exec<tests/test_suite
while IFS=' ' read -ra TOKENS; do
    result=`./lsd12 < ${TOKENS[1]} 2>&1 | head -1`
    if [ "$result" = "${TOKENS[0]}" ] 
    then
        ./lsd12 < ${TOKENS[1]} 2>/dev/null > output/${TOKENS[1]}
        if [ "$result" = "KO" ] 
        then
            echo "${txtgrn}pass${txtrst}: ${TOKENS[1]} KO `./lsd12 < ${TOKENS[1]} 2>&1 | tail -1` "
        else
            echo "${txtgrn}pass${txtrst}: ${TOKENS[1]} OK"
        fi
    else
    	if [ "$result" = "KO" ] 
        then
        	# KO, show the error
            echo "${txtred}fail${txtrst}: ${TOKENS[1]} $result, expected ${TOKENS[0]}. `./lsd12 < ${TOKENS[1]} 2>&1 | tail -1` "
        else
        	# OK
            echo "${txtred}fail${txtrst}: ${TOKENS[1]} $result, expected ${TOKENS[0]}."
        fi
        exit 1
    fi
done

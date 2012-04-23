#!/bin/bash

txtrst=$(tput sgr0) # Text reset
txtred=$(tput setaf 1) # Red
txtgrn=$(tput setaf 2) # Green
LSD12=$1

exec<tests/test_suite
while IFS=' ' read -ra TOKENS; do
    result=`${LSD12} < ${TOKENS[1]} 2>&1 | head -1`
    if [ "$result" = "${TOKENS[0]}" ] 
    then
        ${LSD12} < ${TOKENS[1]} > output/${TOKENS[1]} 2>output/${TOKENS[1]}_stderr
        if [ "$result" = "KO" ] 
        then
            echo "${txtgrn}pass${txtrst}: ${TOKENS[1]} KO `${LSD12} < ${TOKENS[1]} 2>&1 | tail -1` "
        else
            (cat ${TOKENS[1]} | grep "INPUT" | grep -oh "[0-9][0-9]*") > .input
            (cat ${TOKENS[1]} | grep "EXPECT" | grep -oh "[0-9][0-9]*") > .expected
            (cat .input | java -jar ../statement/gpmachine.jar -nogui -n output/${TOKENS[1]}) | grep -oh "[0-9][0-9]*" > .output
            DIFF=`diff .output .expected -q`
            if [ "$DIFF" = "" ]
            then
                echo "${txtgrn}pass${txtrst}: ${TOKENS[1]} OK"
                rm .input .output .expected
            else
                echo "${txtred}fail${txtrst}: ${TOKENS[1]} machine output differs from expected"
                exit 1
            fi
        fi
    else
    	if [ "$result" = "KO" ] 
        then
        	# KO, show the error
            echo "${txtred}fail${txtrst}: ${TOKENS[1]} $result, expected ${TOKENS[0]}. `${LSD12} < ${TOKENS[1]} 2>&1 | tail -1` "
        else
        	# OK
            echo "${txtred}fail${txtrst}: ${TOKENS[1]} $result, expected ${TOKENS[0]}."
        fi
        exit 1
    fi
done

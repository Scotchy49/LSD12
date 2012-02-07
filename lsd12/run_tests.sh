#!/bin/bash

exec<tests/test_suite
while IFS=' ' read -ra TOKENS; do
    result=`./lsd12 < ${TOKENS[1]} 2>&1 | head -1`
    if [ "$result" = "${TOKENS[0]}" ] 
    then
        if [ "$result" = "KO" ] 
        then
            echo "${TOKENS[1]} passed with KO `./lsd12 < ${TOKENS[1]} 2>&1 | tail -1` $error"
        else
            echo "${TOKENS[1]} passed with OK"
        fi
    else
        echo "${TOKENS[1]} failed: Got $result, expected ${TOKENS[0]}."
    fi
done

#!/bin/bash
echo "This script runs Solve with a few kinds of example input"
echo

echo "Press enter to begin"
read
clear


echo "===================================================="
echo "---------------- NORMAL EXPRESSIONS ----------------"
echo "===================================================="
echo "./Solve \"4+2*3\""
../Solve "4+2*3"
echo 

echo "./Solve \"6+7*2\""
../Solve "6+7*2"
echo 

echo "Press enter to continue"
read
clear

echo "===================================================="
echo "---------------- INVALID EXPRESSIONS ---------------"
echo "===================================================="
echo "./Solve \"4+2t*3\""
../Solve "4+2t*3"
echo 

echo "./Solve \"4++4\""
../Solve "4++4"
echo 

echo "./Solve \"4+\""
../Solve "4+"
echo 

echo "./Solve \"4 6\""
../Solve "4 6"
echo 

echo "Press enter to continue"
read
clear

echo "===================================================="
echo "----------------- NORMAL EQUATIONS -----------------"
echo "===================================================="
echo "./Solve -v \"2x+3\" 7"
../Solve -v "2x+3" 7
echo 

echo "./Solve -v \"2(x+1)\" 6"
../Solve -v "2(x+1)" 6
echo 

echo "Press enter to continue"
read
clear

echo "===================================================="
echo "----------------- INVALID EQUATIONS ----------------"
echo "===================================================="
echo "./Solve -v \"2y+3\" 7"
../Solve -v "2y+3" 7
echo 

echo "./Solve -v \"2x+3x\" 7"
../Solve -v "2x+3x" 7
echo 

echo "./Solve -v \"2x+3\""
../Solve -v "2x+3"
echo 

echo "./Solve -v \"2x+3x\" 5"
../Solve -v "2x+3x" 5
echo

echo "Press enter to end"
read
clear

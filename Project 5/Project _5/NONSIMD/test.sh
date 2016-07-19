#!/bin/csh

#Number of arrays:
foreach t( 10000 100000 1000000 10000000 100000000 320000000)
	echo "\n";
#	echo NUM = $t
	g++ -DNUM=$t arraymult.cpp -o arraymult -lm -fopenmp
	./arraymult
end

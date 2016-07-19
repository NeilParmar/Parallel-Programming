#!/bin/csh

#Number of arrays:
foreach t( 10000 100000 1000000 10000000 100000000)
	echo "\n";
	echo NMB = $t
	foreach s(104 192 240 512)
		echo LOCAL_SIZE =$s
		g++ -DNMB=$t -DLOCAL_SIZE=$s first first.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp
		./first
	end
end

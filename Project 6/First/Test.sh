#!/bin/csh

#Number of arrays:
foreach t( 2 4 8 16 1024)
	echo "\n";
	echo NMB = $t
	foreach s(8 128 256 512)
		echo LOCAL_SIZE =$s
		g++ first first.cpp  -DNMB=$t -DLOCAL_SIZE=$s /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp
		./first
	end
end

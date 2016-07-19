simd.p5.o:	simd.p5.h simd.p5.cpp
		g++ -c simd.p5.cpp -o simd.p5.o
arraymult:	arraymult.cpp simd.p5.o
		g++ -o arraymult arraymult.cpp simd.p5.o -lm -fopenmp	


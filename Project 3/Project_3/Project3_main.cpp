#include <omp.h>
#include <stdio.h>
#include <math.h>

#define NUMT	     4							//number of threads
#define NUM	        15							//while applying Fix#1
struct s
{
	float value;
	int pad[NUM];								//while applying Fix#1
} Array[4];


int main( )
{
#ifndef _OPENMP
    fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
    return 1;
#endif	

	double time0 = omp_get_wtime(); 
	//start time

	omp_set_num_threads( NUMT );
	fprintf( stderr, "Using %d threads\n", NUMT );				
	fprintf( stderr, "and %d pad\n", NUM );					//while applying Fix#1

	unsigned int someBigNumber = 1000000000;				// if > 4B, use "long unsigned int"

	#pragma omp parallel for
	for( int i = 0; i < 4; i++ )
	{
			
//		float tmp = Array[ i ].value;					//while applying Fix #2
		for( unsigned int j = 0; j < someBigNumber; j++ )
		{
			Array[ i ].value = Array[ i ].value + 2.; 		//while applying Fix#1	
//			tmp = tmp + 2.;						//while applying Fix#2
		}
//		Array[ i ].value = tmp;						//while applying Fix#2
	}

	//time end
	double time1 = omp_get_wtime();
	
	// print performance here:::
        double performance = 4*someBigNumber/(time1-time0)/1000000.;
	printf("   Performance = %8.2lf MegaPads/sec\n", performance );

}

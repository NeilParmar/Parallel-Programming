#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>


#define NUMTRIES  10


//Array:
float *A = new float[NUM];
float *B = new float[NUM];
float *C = new float[NUM];


//Functions:
void Mult(float *A, float *B, float *C, int num)		//For personal reference: Reference cannot be passed throught the functions.
{

	for( int i=0; i < num; i++)	
	{
		C[i] = A[i] * B[i];
	}

}

double SumMult(float *A, float *B)
{	
	float temp;
	float sum;

	for( int i=0; i < NUM; i++)	
	{
		temp  = A[i] * B[i];
		sum   = sum + temp;
	}

	return sum;

}
	

int
main( )
{

#ifndef _OPENMP
	fprintf( stderr, "OpenMP is not supported here --sorry. \n");
	return 1;
#endif

	printf("NUM: %d\n", NUM);

	float maxmmults = 0.;
	float summults	 = 0.;	
	double time0;
	double time0_0;
	double time1;
	double time1_1;
	

	time0  = omp_get_wtime();

	for(int i=0;i<NUMTRIES;i++)
	{
		
		time0_0  = omp_get_wtime();

		Mult( A, B, C, NUM);

		time1_1  = omp_get_wtime();

		float mmults = (float)NUM/(time1_1-time0_0)/1000000.;
		summults     +=	mmults;
		if(mmults > maxmmults)
		maxmmults = mmults;


	}

	time1  = omp_get_wtime();

	printf(" Peak Performance = %8.2lf MegaMults/sec\n",maxmmults);
	printf(" Average Performance = %8.2lf MegaMults/Sec\n", (float)NUM/(time1-time0)/(float)NUMTRIES/1000000.  );


	time0  = omp_get_wtime();

	for(int i=0;i<NUMTRIES;i++)
	{
		

		time0_0  = omp_get_wtime();
	
		SumMult( A, B );
		
		time1_1  = omp_get_wtime();

		float mmults = (float)NUM/(time1_1-time0_0)/1000000.;
		summults     +=	mmults;
		if(mmults > maxmmults)
		maxmmults = mmults;

	}

	time1  = omp_get_wtime();
	
	printf(" Peak Performance = %8.2lf MegaMults/sec\n",maxmmults);
	printf(" Average Performance = %8.2lf MegaMults/Sec\n", (float)NUM/(time1-time0)/(float)NUMTRIES/1000000.  );

        return 0;


}






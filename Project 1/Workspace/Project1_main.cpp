#include <omp.h>
#include <stdio.h>
#include <math.h>


#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define Z00	0.
#define Z10	1.
#define Z20	0.
#define Z30	0.

#define Z01	1.
#define Z11	6.
#define Z21	1.
#define Z31	0.

#define Z02	0.
#define Z12	1.
#define Z22	0.
#define Z32	4.

#define Z03	3.
#define Z13	2.
#define Z23	3.
#define Z33	3.


#define NUMS 1500	//Number of Subdivisions -- 4,200,500,700,1000,1500 -- Volume 14.05
/*Number of lines subdividing the floor area*/
#define NUMT 7	 	//Number of Threads -- 1,2,3,4,5,6,7

float Height( int , int );		//declaration of function height
static float volume = 0.0;		//declaration of volume
float height;				//declaration of height

int main( int argc, char *argv[] )
{

#ifndef _OPENMP
	fprint(stderr, "OpenMP is not supported here -- sorry. \n");
	return 1
#endif

	double time0 = omp_get_wtime(); 
	//start time

	omp_set_num_threads( NUMT );
	fprintf( stderr, "Using %d threads ", NUMT);
	fprintf( stderr, "and %d NUMS ", NUMS);
	
	#pragma omp parallel for reduction(+:volume),collapse(2)
	for( int iv = 0; iv < NUMS; iv++ )
	{


		for( int iu = 0; iu < NUMS; iu++ )
		{

			if( (iu == 0 && iv == 0) || (iu == 0 && iv == (NUMS-1)) || (iu == (NUMS-1) && iv == 0) || (iu == (NUMS-1) && iv == (NUMS-1)) )
			{			
				//Computing the height of the function:
				height = Height (iu, iv);
				//printf(" height=%8.2f ", height);

				// the area of a single full-sized tile:
				float fullTileArea = ( (XMAX-XMIN)/(float)(NUMS-1) )  *  ( ( YMAX - YMIN )/(float)(NUMS-1) ) ;

				//the volume:
				volume = volume + (0.25 * height * fullTileArea);
				
				//print:
				//printf(" iu = %d ", iu );
				//printf(" and iv = %d", iv);
				//printf(" 1/4 Volume = %8.2lf \n", volume );

			}//area at the corners
			
			else if( (iu == 0) || (iv == 0) || (iu == (NUMS-1)) || (iv == (NUMS-1)))
			{
				//Computing the height of the function:
				height = Height (iu, iv);
				//printf(" height=%8.2f ", height);

				// the area of a single full-sized tile:
				float fullTileArea = ( (XMAX-XMIN)/(float)(NUMS-1) )  *  ( ( YMAX - YMIN )/(float)(NUMS-1) );
				
				//the volume:				
				volume = volume + (0.50 * height * fullTileArea);
			
				//print:			
				//printf(" iu = %d ", iu );
				//printf(" and iv = %d", iv);
				//printf(" 1/2 Volume = %8.2lf \n", volume );

			}//area at the edges

			else
			{
				//Computing the height of the function:
				height = Height (iu, iv);
				//printf(" height=%8.2f ", height);

				// the area of a single full-sized tile:
				float fullTileArea = (  ( (XMAX-XMIN)/(float)(NUMS-1) )  *  ( ( YMAX - YMIN )/(float)(NUMS-1) )  );

				//the volume:
				volume = volume + (height * fullTileArea);

				//print:
				//printf(" iu = %d ", iu );
				//printf(" and iv = %d", iv);
				//printf(" 1 Volume = %8.2lf \n", volume );
			
			}//volume for center and all other areas	

		}//iu for loop
			
	}//iv for loop


	double time1 = omp_get_wtime(); 
	//stop time

	printf("   Total Volume = %8.2lf ", volume );
	printf("   Time = %8.2lf ", ((time1-time0)*1000000.) );				//microseconds measurement
	printf("   Time = %8.2lf ", ((time1-time0)) );						//seconds measurement

	//Performance computation:
        double performance = (double)(NUMS*NUMS)/(time1-time0)/1000000.;
		
	printf("   Performance = %8.2lf MegaHeights/sec\n", performance );

}//main

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMS-1
{
	float u = (float)iu / (float)(NUMS-1);
	float v = (float)iv / (float)(NUMS-1);

	// the basis functions:

	float bu0 = (1.-u) * (1.-u) * (1.-u);
	float bu1 = 3. * u * (1.-u) * (1.-u);
	float bu2 = 3. * u * u * (1.-u);
	float bu3 = u * u * u;

	float bv0 = (1.-v) * (1.-v) * (1.-v);
	float bv1 = 3. * v * (1.-v) * (1.-v);
	float bv2 = 3. * v * v * (1.-v);
	float bv3 = v * v * v;

	// finally, we get to compute something:

	float height = 	  bu0 * ( bv0*Z00 + bv1*Z01 + bv2*Z02 + bv3*Z03 )
			+ bu1 * ( bv0*Z10 + bv1*Z11 + bv2*Z12 + bv3*Z13 )
			+ bu2 * ( bv0*Z20 + bv1*Z21 + bv2*Z22 + bv3*Z23 )
			+ bu3 * ( bv0*Z30 + bv1*Z31 + bv2*Z32 + bv3*Z33 );

	return height;
}


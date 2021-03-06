#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

//Global Variables:
int NowYear;		//2016-2021
int NowMonth;		//0-11
float NowPrecip;	//inches of rain per month
float NowTemp;		//temperature this month
float NowHeight;	//grain height in inches
int NowNumDeer;		//number of deer in the current population
int NowNumLion;		//Variable for MyAgent:

//Temperature and Precipitation Global Variables:
float ang;		
float temp;
float precip;
float tempFactor;
float precipFactor;

//Parameters:
const float GRAIN_GROWS_PER_MONTH	=	8.0;
const float ONE_DEER_EATS_PER_MONTH	=	0.5;
const float ONE_LION_EATS_PER_MONTH	=	1.5;

const float AVG_PRECIP_PER_MONTH	=	6.0;
const float AMP_PRECIP_PER_MONTH	=	6.0;
const float RANDOM_PRECIP		=	2.0;
//inches

const float AVG_TEMP			=	50.0;
const float AMP_TEMP			=	10.0;		//Changed the particular from the original code
const float RANDOM_TEMP			=	10.0;
//Fahrenheit(°F)

//Functions
float Ranf( float, float );
float GrainDeer();
float Grain();
float Watcher();
float Lion();				//MyAgent();

//Output Setup
FILE *f1,*f2,*f3,*f4,*f5,*f6,*f7;


int
main()
{

	#ifndef _OPENMP
		fprintf( stderr, "OpenMP not supported here -- sorry.\n" );
		return 1;
	#endif

	//Starting State:
	NowNumDeer	= 2;
	NowNumLion	= 1;
	NowHeight	= 1.0;

	//Starting Date and Time:
	NowMonth	= 12;
	NowYear		= 2021;
	
	//Temperature and Precipitation (Initial Computation)
	ang		= (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	temp		= AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp		= temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

	precip		= AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip	= precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
	NowPrecip = 0.;
	
	//Temperature Factor and Precipitation Factor (Initial Computation)
	tempFactor	= exp( ( -1 ) * ( pow ( ( ( NowTemp - NowTemp / 2 ) / 10 ), 2 ) ) ); 
	precipFactor	= exp( ( pow ( ( ( NowTemp - NowTemp / 2 ) / 10 ), 2 ) ) );


	//OpenMP Parallel Call
	omp_set_num_threads(4);
	#pragma omp parallel sections
	{

		#pragma omp section
		{

			GrainDeer();

		}//GrainDeer

		#pragma omp section
		{
			Grain();

		}//Grain

		#pragma omp section
		{
			Watcher();

		}//Watcher

		#pragma omp section
		{
			Lion();

		}//MyAgent


	}//#pragma parallel for sections

	//Output Printing Setup
	f1 = fopen("Output.xlsx","a");
	fprintf(f1,"Year");

	f2 = fopen("Output.xlsx","a");
	fprintf(f2,"\tMonth");

	f3 = fopen("Output.xlsx","a");
	fprintf(f3,"\tPrecipitation");

	f4 = fopen("Output.xlsx","a");
	fprintf(f4,"\tTemperature");

	f5 = fopen("Output.xlsx","a");
	fprintf(f5,"\tGrain Height");

	f6 = fopen("Output.xlsx","a");
	fprintf(f6,"\tDeers");

	f7 = fopen("Output.xlsx","a");
	fprintf(f7,"\tLions");


	//Deallocating the file pointing memory before closing the file
	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);
	fclose(f5);
	fclose(f6);
	fclose(f7);

	return 0;

}//main


float
Ranf(float low, float high)
{
	float r		= (float) rand();		//0 - RAND_MAX
	
	return ( low + r*(high-low)/(float)RAND_MAX);
}//Ranf FLOAT

float
Lion()
{
	int tempNumLion = NowNumLion;

	while(NowYear>=2016)
	{
	tempNumLion	= NowNumLion;		//updating the recent value

	//Compute the temporary next-value for this quantity based upon the current state of the simulation:
	if( NowNumDeer <= tempNumLion * ONE_LION_EATS_PER_MONTH )
	{
		tempNumLion	-= 1;

	}
	else if( NowNumDeer > tempNumLion * ONE_LION_EATS_PER_MONTH)		
	{
		tempNumLion	+= 1;	

	}
	else if(( tempNumLion == 0))
	{
		tempNumLion = 1;
	}
		
	//DoneComputing Barrier:
	#pragma omp barrier
	
	NowNumLion	 = tempNumLion;

	
	//Done Assigning Barrier:
	#pragma omp barrier

	//Done Printing Barrier:
	#pragma omp barrier

	}

}//GrainDeer

float
GrainDeer()
{
	int tempNumDeer = NowNumDeer;

	while(NowYear>=2016)
	{
	tempNumDeer	= NowNumDeer;		//updating the recent value
 
	//Compute the temporary next-value for this quantity based upon the current state of the simulation:
	if( ( (tempNumDeer * ONE_DEER_EATS_PER_MONTH ) >= NowHeight ) || (NowNumLion * ONE_LION_EATS_PER_MONTH >= tempNumDeer))		
	{
		tempNumDeer	-= 1;	
	}
	else if( ( (tempNumDeer * ONE_DEER_EATS_PER_MONTH ) < NowHeight ) )
	{
		tempNumDeer	+= 2;
	}

	if( tempNumDeer == 0)
	tempNumDeer	= 1;	
	else if( tempNumDeer > 10 )
	tempNumDeer	= 10;
		
	//DoneComputing Barrier:
	#pragma omp barrier
	
	NowNumDeer	 = tempNumDeer;
	
	//Done Assigning Barrier:
	#pragma omp barrier

	//Done Printing Barrier:
	#pragma omp barrier

	}

}//GrainDeer

float
Grain()
{
	float TempNowHeight = NowHeight;

	while(NowYear>=2016)
	{
	TempNowHeight = NowHeight;	//updating the recent value

	//Compute the temporary next-value for this quantity based upon the current state of the simulation:
	
	if(NowTemp <= 41. && NowPrecip <= 3. )		
	TempNowHeight	-= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
	else if(NowTemp <= 41.)
	TempNowHeight	-= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
	if(NowTemp > 41. && NowPrecip <= 3. )		
	TempNowHeight	-= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
	else if(NowTemp > 41. && NowPrecip > 3. )					//Grain Growth only possible if the temperature is high and precipitation is high
	TempNowHeight	+= tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;

	//Claming the height against 0
	if (TempNowHeight<0)
	TempNowHeight=0; 
	else if(TempNowHeight>10)
	TempNowHeight=10; 	

	//DoneComputing Barrier:
	#pragma omp barrier
	
	NowHeight	= TempNowHeight;
	
	//Done Assigning Barrier:
	#pragma omp barrier

	//Done Printing Barrier:
	#pragma omp barrier
	
	}

}//GrainDeer

float 
Watcher()
{
	while(NowYear>=2016)
	{
	//Compute the temporary next-value for this quantity based upon the current state of the simulation:

	
	//DoneComputing Barrier:
	#pragma omp barrier

	//Done Assigning Barrier:
	#pragma omp barrier

	//Printing:
	printf(" Year:%d ", NowYear);
	printf("\tMonth:%d ", NowMonth);
	printf("\tPrecipitation:%8.2lf ", NowPrecip);
	printf("\tTemperature:%8.2lf°C ", ( (5./ 9.) * (NowTemp -32) ) );
	printf("\tGrain Height:%8.2lfinches ", ( NowHeight * 2.54 ));
	printf("\tDeers:%d ", NowNumDeer);
	printf("\tLions:%d\n", NowNumLion);

	//Printing/Store the Number of Lion
	f7 = fopen("Output.xlsx","a");
	fprintf(f7,"\t%d", NowNumLion );

	//Printing/Store the Num of Deer
	f6 = fopen("Output.xlsx","a");
	fprintf(f6,"\t%d", NowNumDeer );

	//Printing/Store the grain height
	f5 = fopen("Output.xlsx","a");
	fprintf(f5,"\t%8.2lf", ( NowHeight * 2.54) );

	//Printing/Store the temperature
	f4 = fopen("Output.xlsx","a");
	fprintf(f4,"\t%8.2lf", ( (5./ 9.) * (NowTemp -32) ) );
	
	//Printing/Store the precipitaton 
	f3 = fopen("Output.xlsx","a");
	fprintf(f3,"\t%8.2lf",NowPrecip);

	//Printing/Store the month 
	f2 = fopen("Output.xlsx","a");
	fprintf(f2,"\t%d",NowMonth);

	//Store the year 
	f1 = fopen("Output.xlsx","a");
	fprintf(f1,"\n%d",NowYear);

/**Computing the next month's Temperature and Precipitation **/

	//Incrementing and maintaining month and year:
	NowMonth	-= 1;	
	if(NowMonth == -1)
	{	NowMonth = 12;
		NowYear  -= 1;
	}

	//Temperature(Computation):
	ang		= (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
	
	temp		= AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp		= temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );
	if( NowTemp < 32.)
	NowTemp	= 32.;			//Minimum Temperature 0°C
//	printf("NowTemp: %8.2lf°F\n",NowTemp);

	//Precipitation(Computation):
	precip		= AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip	= precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
	NowPrecip = 0.;
	
	//Temperature Factor and Precipitation Factor (Computation)
	tempFactor	= exp( ( -1 ) * ( pow ( ( ( NowTemp - NowTemp / 2 ) / 10 ), 2 ) ) ); 
	precipFactor	= exp( ( pow ( ( ( NowTemp - NowTemp / 2 ) / 10 ), 2 ) ) );

	//Done Printing Barrier:
	#pragma omp barrier
	
	}
}//Watcher



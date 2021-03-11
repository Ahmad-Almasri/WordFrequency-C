/*
	Ahmad Almasri
	CPSC 355 - Assignment1
*/

//----------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//----------------------------

// initialize the table
bool initialize(int *, int, int,char *);
// with random numbers
bool initializeR(int *, int, int);
// with scanned numbers from a file
bool initializeF(int *, int, int, char *); 
// return a random number between 0 - 9
int randomNum(int, int);
// to display the table 
void displayT(int *, int, int);
// to find and sort the frequency for a given word (index)
void topRelevantDocs(int *, int , int , int , float *);
// to save the table & result (assign.log) 
void logToFile(int *, float *, int, int, int, int);

//----------------------------

// to check the scanned command line arguments
bool checkValidity(int, char  *[]); 
// to check the size of rows and cols
bool checkSize(int , int);
// to check the index (word) and the maximum # of diplayed records
bool checkIndexAndMaxNum(int,int,int,int);

//----------------------------

// Sorting algorithm -- recursion function
void quickSort(float* , int , int);
// to divide the array based on a selected index
int partition(float* , int , int);
// to swap the value after a comparision
void swap(float*,float*);
// to display the result
void displayR(float* , int);

//----------------------------

int main(int argc, char *argv[])
{
	// the rows and cols
	int m , n;

	//----------------------------
	if(!checkValidity(argc,argv))
	{
		exit(0);
	}
	else
	{
		m = atoi(argv[1]); n = atoi(argv[2]);
	}
	//----------------------------

	// allocate the 2D-array with size = M * N
	int table[m][n];
	// allocate a sigle array to store the sorted result
	float result[m];

	//----------------------------
	if(initialize(table[0], m, n, argv[3]))
	{
		displayT(table[0],m,n);

		// index is the word that we are gonna search for
		// maxNum is the number of top documents
		// choice --> stop or continue
		int index, maxNum;
		char *choice;

		while(1)
		{
			printf("\nEnter the index of the word you are searching for:  ");
			scanf("%d",&index);
			printf("\nHow many top documents you want to retrieve?  ");
			scanf("%d",&maxNum);

			if(checkIndexAndMaxNum(index,maxNum,m,n))
			{
				topRelevantDocs(table[0], m, n, index, result);

				displayR(result,maxNum);

				logToFile(table[0],result,maxNum,index,m,n);
			}

			printf("PRESS any key to continue || PRESS 0 to exit\n");
			scanf("%s",&choice);

			if(choice - '0' == 0)
				break;
			else
				choice = NULL;
		}
	}
	//----------------------------

	return 0;
}

//----------------------------

bool checkValidity(int argc , char *argv[])
{
	if(argc != 3 && argc != 4)
	{
		printf("The number of arguments is not correct please make sure that you enter 3 or 4 arguments only ...\n");
		return false; // stop the program
	}
	else
	{
		if(!checkSize(atoi(argv[1]),atoi(argv[2])))
		{
			printf("The numbers of rows and cols should be between 5 and 20 ...\n");
			return false; // stop the program
		}	
	}
	return true;
}

//----------------------------

bool checkSize(int m , int n)
{
	return (m >= 5 && m <= 20) && ( n >= 5 && n <= 20); 
}

//----------------------------

bool checkIndexAndMaxNum(int index, int maxNum, int c, int r)
{
	if(index >= c)
	{
		// # of cols starts from 0 to n-1 therefore n is not accepted.
		printf("The index (word) is GREATER than COLS number (%d)\n", c-1);
		return false; // stop
	}
	else if(index < 0)
	{
		printf("The index (word) could not be NEGATIVE\n", c);
		return false; // stop
	}
	else if(maxNum > r)
	{
		printf("The number of requested documents is GREATER than ROWS number (%d)\n", r);
		return false; // stop
	} 
	else if(maxNum <= 0)
	{
		printf("The number of requested documents could not be LESS than ZERO\n");
		return false; // stop
	}

	return true;
}

//----------------------------

bool initialize(int *table , int r, int c , char *fileName)
{
	if(fileName == NULL)
		return initializeR(table,r,c);
	else
		return initializeF(table,r,c,fileName);
}

//----------------------------

bool initializeR(int *table , int r, int c)
{
	// to set the seeds for the random # generator function --> rand()
	time_t t;
	srand((unsigned) time(&t));

	for(int i = 0 ; i < r*c ; i++ , table+=1)
		*table = randomNum(0,9);

	return true;	
}

//----------------------------

bool initializeF(int *table , int r, int c , char *fileName)
{
	FILE *f ;  
	char ch ;
	int number = 0, counter = 0;

	if((f = fopen(fileName,"r")) != NULL)
	{
		while (1) // infinite loop until it breaks 
		{

			ch = fgetc (f) ; // read char by char

			if (ch == EOF) // the stop condition  
				break ;

			number = ch - '0'; // convert the char into int

			if(number >= 0 && number <= 9)
			{
				// if numbers in the file are more than the size of the array
				if(++counter > (r*c))
				{
					printf("The total number is greater than the actual size of the array (%d)\n", r*c );
					return false; // stop the program
				}

				*table = number;
				table+=1;
			}
		}

		fclose (f);

		// if numbers in the file are less than the size of the array
		if(counter < (r*c))
		{
			printf("The total numbers is less than the actual size of the array (%d)\n", r*c);
			return false; // stop the program
		} 
	}
	else
	{
		printf("File is not exist\n");
		return false; // stop the program
	}

	return true;
}

//----------------------------

int randomNum(int lower, int upper)
{
	// rand()%10
	return rand()%(upper-lower+1);
}

//----------------------------

void displayT(int *table , int r , int c)
{
	for(int i = 0 ; i < r ; i++)
	{
		for(int j = 0 ; j < c ; j++)
		{
			//*(table+i*c+j)
			printf("%d  ", *table );
			table+=1;
		}
		printf("\n");
	}
}

//----------------------------

void topRelevantDocs(int *table, int r, int c, int index, float *result)
{
	int sum=0; // sum -> the sum of the whole row

	float temp; // store the value of the index (word)

	for(int i = 0 ; i < r ; i++)
	{
		temp = *(table+i*c+index); // fetch the value of the index

		if(temp != 0) // if == 0 no need to calc the sum 0/n = 0
		{
			sum = 0; // reset the sum to zero

			for(int j = 0 ; j < c ; j++)
			{
				sum+=*(table+i*c+j); // calc the whole row
			}

			temp = temp/sum; // calc the frequency (a float division)
		}	

		result[i] = temp; // store the frequency
	}

	quickSort(result , 0, r-1); // sorting the results in Descending order

	
}

//----------------------------

void quickSort(float *result , int low , int high)
{
	/*  
		    -- partitionIndex is --

	      the returned index (the PIVOT) from partion func.
	      The array is going to be divided into two parts

	        -- Descending Order --

	      LEFT  Part Greater Than PIVOT
	      Right Part Smaller Than PIVOT

	        -- Stop Condition --

	      IFF low >= high

	      low  : first element of the array
	      High : Last  element of the array
	*/
	int partitionIndex;

	if(low<high)
	{
		partitionIndex = partition(result , low , high);

		// divide and conquer technique -- Recursion function
		quickSort(result , low , partitionIndex-1);
		quickSort(result , partitionIndex+1 , high);
	}
}

//----------------------------

int partition(float *result , int low , int high)
{
	/*
		pivot --> last element of the array
		i --> iterating over the values of the array
		j --> the initial position is in the first element
			  and is increased IFF result[i] >= pivot
	*/

	float pivot = result[high];
	int i = low , j = low;

	for(;i<high;i++)
		if(result[i] >= pivot)
			swap(&result[i],&result[j++]);

	// PLACING THE
	swap(&result[i],&result[j]);

	return j;	
}

//----------------------------

void swap(float *a , float *b)
{

	// swapping the value by reference
	float temp = *a; *a = *b; *b = temp;
}

//----------------------------

void displayR(float *result , int maxNum)
{
	printf("\nTHE RESULT: \n\n");

	int i = 0;
	while(i<maxNum)
		printf("%f\n", result[i++]);

	printf("\n");
}

//----------------------------

void logToFile(int *table, float *result, int maxNum, int index, int r, int c)
{
	/* 
	   (0) using the STATIC COUNTER to dertmine WHETHER this is the FIRST time to logToFile() or NOT
	   (1) if it is the first time ... we are gonna use "w" & print the table
	   (2) if it is not the first time then we are gonna use "a" & not printing the table
	*/
	static int counter = 0;
	counter++;

	FILE *f;

	if(counter == 1) // only first time
	{
		f = fopen("assign1.log","w"); // create file.log

		fprintf(f, "%s\n", "The table:" );

		for(int i = 0 ; i < r ; i++)
		{
			for(int j = 0 ; j < c ; j++)
			{
				//*(table+i*c+j)
				fprintf(f, "%d  ", *table ); // print the table
				table+=1;
			}
			fprintf(f, "\n");
		}

		fprintf(f, "%s\n\n", "---------------------------------------" );

		//General info. about the table
		fprintf(f, "%s\n%s%d\n%s%d\n%s%d\n\n\n", "General Info:" ,"The # of rows: ", r, "The # of cols: ", c, "The total # of documents: ", r*c );
	}
	else
	{
		f = fopen("assign1.log","a"); // Append to file.log
	}

	 

	char *a = "The highest frequency for word :"; // the index value

	

	
	fprintf(f, "%s\n", "---------------------------------------" );

	fprintf(f, "%s %d\n\n",a,index);

	// Display the result
	fprintf(f, "%s%d\n\n", "The requested # of documents: ", maxNum);
	int i = 0;
	fprintf(f,"************************\n");
	fprintf(f,"*                      *\n");
	fprintf(f,"*   Record # | Value   *\n");
	fprintf(f,"*                      *\n");
	fprintf(f,"************************\n");
	while(i<maxNum)
		fprintf(f, "   %3d\t   %s %.3f\n",i,"  |",result[i++]);
	fprintf(f, "%s\n\n\n", "---------------------------------------" );

	fclose(f);
}

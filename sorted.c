
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>


const int MAX_INPUT_SIZE = 1000;  //maximum number of elements for storage in the hash table
const int MAX_SEARCH_SIZE = 1000; //maximum number of elements for search
								  
//reads the specified number of values from the specified file into the specified array
//fileName: the path to the input file
//data: pointer to an array of int where the values are stored
//size: the size of the array, no more than this amount of values will be read
//returns: number of values that was actually read
int readData(char* fileName, int* data, int size)
{
	//try to open the input file
	FILE* file;
	file = fopen(fileName, "r");
	if (file == NULL) //unable to open the file
	{
		printf("Error: cannot open file %s!\n", fileName);
		return 0; //nothing was read
	}

	int i; //here we count the number of values read

	//read at most size values
	for (i = 0; i < size; ++i)
	{
		//if we have reached the end of the file, terminate
		if (fscanf(file, "%d", &(data[i])) == EOF)
		{
			break;
		}
	}

	//close the file
	fclose(file);

	//return the number of values read
	return i;
}

//reads the values from both the input files and stores them in their respective arrays using the readData function
//inputFile: path to the file with input values
//inputData: pointer to an array where to store the input values
//inputSize: the number of values that was actually read from the input file is saved here
//searchFile: path to the file with search values
//searchData: pointer to an array where to store the search values
//searchSize: the number of values that was actually read fro the search file is saved here
//returns: 0 if some values were read from both files, -1 otherwise
int readInputs(char* inputFile, int* inputData, int* inputSize, char* searchFile, int* searchData, int* searchSize)
{
	*inputSize = readData(inputFile, inputData, MAX_INPUT_SIZE);
	if (*inputSize == 0)
	{
		printf("Error: No input data loaded from file %s!\n", inputFile);
		return -1; //nothing read from the input file
	}

	*searchSize = readData(searchFile, searchData, MAX_SEARCH_SIZE);
	if (*searchSize == 0)
	{
		printf("Error: No retrieval data loaded from file %s!\n", searchFile);
		return -1; //nothing read from the search file
	}

	return 0;
}

//performs the selection sort algorithm to sort an array in an ascending order
//data: pointer to an array to be sorted
//size: the size of the array
//comparisons: the number of comparisons that the algorithm performs gets saved here
//swaps: the number of swaps that the algorithm performs gets saved here
void selectionSort(int* data, int size, int* comparisons, int* swaps)
{
	//initialize the counters
	*comparisons = 0;
	*swaps = 0;

	//when the first size-1 elements are sorted, the entire array is sorted
	for (int i = 0; i < size - 1; ++i)
	{
		//find the smallest element in the unsorted part of the array
		int smallest = data[i];
		int smallestIndex = i;
		for (int j = i + 1; j < size; ++j)
		{
			if (data[j] < smallest)
			{
				smallest = data[j];
				smallestIndex = j;
			}

			(*comparisons)++; 
		}

		//if the smallest element isn't at the beginning of the unsorted part of the array
		//switch it with the first element of the unsorted part of the array
		if (smallestIndex != i)
		{
			int aux = data[i];
			data[i] = data[smallestIndex];
			data[smallestIndex] = aux;

			(*swaps)++;
		}
	}
}

//performs binary search on a sorted array (in ascending order)
//data: pointer to the sorted array
//size: the size of the sorted array
//value: the value to search for
//returns: the index in the array where the searched value is located, -1 if it is not found
int binarySearch(int* data, int size, int value)
{
	//the boundaries of the currently searched range in the array
	//initially the whole array
	int left = 0;
	int right = size - 1;
	
	while (left <= right)
	{
		//divide the range in half
		int middle = (left + right) / 2;

		//if the searched value is greater than the middle element, continue searching in the right portion of the original range
		if (data[middle] < value)
		{
			left = middle + 1;
		}
		//if the searched value is smaller than the middle element, continue searching in the left portion of the original range
		else if (data[middle] > value)
		{
			right = middle - 1;
		}
		//if the searched value is equal to the middle element, we have found our value
		else
		{
			return middle;
		}
	}

	return -1; //the searched value is not in the array
}

//prints the statistics
//inputFile: the path to the input file
//searchFile: the path to the search file
//inputSize: the size of the input array
//searchData: pointer to an array where the search values are stored
//searchSize: the size of the search array
//positions: pointer to an array of the same size as searchData that contains the indices where the corresponding search elements were found in the input array
//comparisons: the number of comparisons performed by the selection sort algorithm
//swaps: the number of swaps performed by the selection sort algorithm
//storageTime: the time in miliseconds that the storage took
//searchTime: the time in miliseconds that the search took
void printStats(char* inputFile, char* searchFile, int inputSize, int* searchData, int searchSize,
	int* positions, int comparisons, int swaps, float storageTime, float searchTime)
{
	printf("Data storage and retrieval:\n");
	printf("a comparison of hashing and directed search of sorted data\n");
	printf("================================\n\n");

	printf("Input data loaded from file %s\n", inputFile);
	printf("Retrieval data loaded from file %s\n", searchFile);
	printf("Storage Method: Directed search of sorted data\n");
	printf("Number of items stored in the array: %d\n", inputSize);
	printf("Number of items searched: %d\n", searchSize);

	int numFound = 0;
	for (int i = 0; i < searchSize; ++i)
	{
		if (positions[i] != -1)
		{
			numFound++;
		}
	}

	printf("Number of items found: %d\n\n", numFound);

	printf("Storage details:\n\n");

	printf("  <%d> comparisons performed\n", comparisons);
	printf("  <%d> swaps performed\n\n", swaps);

	printf("Retrieval details:\n\n");

	for (int i = 0; i < searchSize; ++i)
	{
		if (positions[i] != -1)
		{
			printf("  Value %d found in the sorted array at position %d\n", searchData[i], positions[i]);
		}
		else
		{
			printf("  Value %d not found in the sorted array\n", searchData[i]);
		}
	}

	printf("\nExecution times:\n\n");

	printf("  Time to store data: %f ms\n", storageTime);
	printf("  Time to retrieve data: %f ms\n\n", searchTime);

	printf("================================\n\n");
}

//copies one array into another array
//source: pointer to the array that is copied
//destination: pointer to the array that is copied into
//size: the number of elements to be copied
void copyArray(int* source, int* destination, int size)
{
	for (int i = 0; i < size; ++i)
	{
		destination[i] = source[i];
	}
}

int main(int argc, char* argv[])
{
	//check the number of arguments
	if (argc != 3)
	{
		//wrong number of arguments
		printf("Error: 2 arguments required!\n");
		return -1;
	}

	//try to allocate memory for the input array
	int* inputData = (int*)malloc(sizeof(int) * MAX_INPUT_SIZE);
	if (inputData == NULL)
	{
		//unable to allocate memory, exit
		printf("Error: couldn't allocate memory!\n");
		return -1;
	}

	//try to allocate memory for the search array
	int* searchData = (int*)malloc(sizeof(int) * MAX_SEARCH_SIZE);
	if (searchData == NULL)
	{
		//unable to allocate memory, free memory and exit
		printf("Error: couldn't allocate memory!\n");
		free(inputData); 
		return -1;
	}

	//try to read the data from the input files
	int inputSize;
	int searchSize;
	if (readInputs(argv[1], inputData, &inputSize, argv[2], searchData, &searchSize) == -1)
	{
		//error while reading, free memory and exit
		free(inputData);
		free(searchData);
		return -1;
	}

	//try to allocate memory for the sorted array
	int* sortedData = (int*)malloc(sizeof(int) * MAX_INPUT_SIZE);
	if (sortedData == NULL)
	{
		//unable to allocate memory, free memory and exit
		printf("Error: couldn't allocate memory!\n");
		free(inputData);
		free(searchData);
		return -1;
	}
	
	int comparisons;
	int swaps;
	clock_t totalTime = 0;
	clock_t lastTime;
	//do the selection sort many times, measure the total time
	for (int i = 0; i < 100000; ++i)
	{
		//replace the sorted data with the original data and repeat the sorting
		copyArray(inputData, sortedData, inputSize);
		lastTime = clock();
		selectionSort(sortedData, inputSize, &comparisons, &swaps);
		totalTime += clock() - lastTime;
	}
	//compute the time of one execution of selection sort
	float storageTime = ((((float)totalTime) / CLOCKS_PER_SEC) * 1000) / (100000);
	
	//try to allocate memory for the array of the indices where the corresponding search elements were found in the input array
	int* positions = (int*)malloc(sizeof(int) * MAX_SEARCH_SIZE);
	if (positions == NULL)
	{
		//unable to allocate memory, free memory and exit
		printf("Error: couldn't allocate memory!\n");
		free(inputData);
		free(sortedData);
		free(searchData);
		return -1;
	}

	//do the binary search many times, measure the total time
	totalTime = 0;
	for (int i = 0; i < 100000; ++i)
	{
		for (int j = 0; j < searchSize; ++j)
		{
			lastTime = clock();
			positions[j] = binarySearch(sortedData, inputSize, searchData[j]);
			totalTime += clock() - lastTime;
		}
	}
	//compute the time of one execution of binary search
	//totalTime = clock() - lastTime;
	float searchTime = ((((float)totalTime) / CLOCKS_PER_SEC) * 1000) / (100000 * searchSize);
	
	//print the stats
	printStats(argv[1], argv[2], inputSize, searchData, searchSize, positions, comparisons, swaps, storageTime, searchTime);

	//free the memory
	free(inputData);
	free(sortedData);
	free(searchData);
	free(positions);

	return 0;
}
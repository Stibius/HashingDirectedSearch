
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>

const int MAX_INPUT_SIZE = 1000; //maximum number of elements for storage in the hash table
const int MAX_SEARCH_SIZE = 1000; //maximum number of elements for search

//entry in the list of elements for storage in the hash table
typedef struct InputEntry InputEntry;
struct InputEntry
{
	int key; //the value of the element
	int* collisions; //array of indices into the hash table where collisions occurred when storig this element
	int numCollisions; //number of collisions which occurred when storig this element in the has table
};

//the list of elements for storage in the hash table
//this list is created when reading the elements from the input file
//it is later used to store the elemets in the hash table and record any collisions that occur
typedef struct InputList InputList;
struct InputList
{
	InputEntry* array; //array of entries
	int size; //size of the array of entries
};

//entry in the list of elements for search
typedef struct SearchEntry SearchEntry;
struct SearchEntry
{
	int key; //the value of the element
	int index; //the index in the hash table where the element was found, -1 if it was not found
};

//the list of elements for search
//this list is created when reading the elements from the search file
//it is later used to search for these elements in the hash table and record which ones were found and where
typedef struct SearchList SearchList;
struct SearchList
{
	SearchEntry* array; //array of entries
	int size; //size of the array of entries
	int numFound; //number of elements which were found in the hash table
};

//two possible states of entries in the hash table
typedef enum EntryStatus EntryStatus;
enum EntryStatus { OCCUPIED, EMPTY };

//entry in the hash table
typedef struct HashEntry HashEntry;
struct HashEntry
{
	int key; //the value of the stored element
	EntryStatus status; //whether this entry is empty or occupied
};

//the hash table
typedef struct HashTable HashTable;
struct HashTable
{
	HashEntry* array; //array of entries
	int size; //the size of the array of entries
	int numOccupied; //the number of entries that are occupied
};

//tries to insert one entry into the hash table, uses quadratic probing
//hashTable: pointer to the hash table 
//entry: pointer to the entry that is to be inserted, information about collisions that occurred, if any, is saved into the entry
//returns: the index in the hash table where the entry was inserted, -1 if it was not possible to insert the entry
int insertHash(HashTable* table, InputEntry* entry)
{
	entry->numCollisions = 0;

	//perform the quadratic probing algorithm
	for (int i = 0; i < table->size; ++i)
	{
		//compute index into the hash table
		int index = (entry->key + (i * i)) % table->size;

		//if the place at the index is empty, update the hash table and exit
		if (table->array[index].status == EMPTY)
		{
			table->array[index].key = entry->key;
			table->array[index].status = OCCUPIED;
			table->numOccupied++;
			return index;
		}
		//if the place at the index isn't empty, record the collision and continue with the next iteration
		else
		{
			entry->collisions[i] = index;
			entry->numCollisions++;
		}
	}

	//the value couldn't be inserted into the hash table
	return -1;
}

//tries to insert all the elements from the input list into the hash table, uses the insertHash function
//hashTable: pointer to the hash table
//inputList: the input list with entries to insert into the hash table
void createHash(HashTable* hashTable, InputList inputList)
{
	hashTable->numOccupied = 0;

	//set all hash table entries to empty
	for (int i = 0; i < hashTable->size; ++i)
	{
		hashTable->array[i].status = EMPTY;
	}

	//go through the input list
	for (int i = 0; i < inputList.size; ++i)
	{
		//and try to insert each input element into the hash table
		insertHash(hashTable, &inputList.array[i]);
	}
}

//searches for the given entry in the hash table
//hashTable: pointer to the hash table 
//entry: pointer to the entry to search for, the index in the entry gets set to the index in the hash table where the entry was found, -1 if it was not found
//returns: the index in the hash table where the entry was found, -1 if it was not found
int searchHash(HashTable* table, SearchEntry* entry)
{
	//perform the quadratic probing algorithm
	for (int i = 0; i < table->size; ++i)
	{
		//compute index into the hash table
		int index = (entry->key + (i * i)) % table->size;

		if (table->array[index].status == EMPTY)
		{
			//the searched value was not found
			entry->index = -1;
			return -1;
		}
		//if the place at the index contains the searched value, save the index and exit
		else if (table->array[index].status == OCCUPIED && table->array[index].key == entry->key)
		{
			entry->index = index;
			return index;
		}
	}

	//the searched value was not found
	entry->index = -1;

	return -1;
}

//frees the dynamically allocated memory associated with the hash table
//hashTable: pointer to the hash table 
void deleteHash(HashTable* hashTable)
{
	free(hashTable->array);
	hashTable->array = NULL;
	hashTable->size = 0;
	hashTable->numOccupied = 0;
}

//frees the dynamically allocated memory associated with the search list
//searchList: pointer to the search list
void deleteSearchList(SearchList* searchList)
{
	free(searchList->array);
	searchList->array = NULL;
	searchList->numFound = 0;
	searchList->size = 0;
}

//reads elements from the given file and creates a search list from them
//fileName: the path to the search file
//returns: a search list with the elements from the search file
SearchList readSearchList(char* fileName)
{
	//create empty search list
	SearchList searchList;
	searchList.array = NULL;
	searchList.numFound = 0;
	searchList.size = 0;

	//try to open the search file, return empty search list if it fails
	FILE* file;
	file = fopen(fileName, "r");
	if (file == NULL)
	{
		printf("Error: cannot open file %s!\n", fileName);
		return searchList;
	}

	//try to allocate memory for the search list, return empty search list if it fails
	searchList.array = (SearchEntry*)malloc(MAX_SEARCH_SIZE * sizeof(SearchEntry));
	if (searchList.array == NULL)
	{
		fclose(file);
		printf("Error: couldn't allocate memory!\n");
		return searchList;
	}

	//read entries from the search file until the end of file or until the maximum allowed number of values is read
	while (searchList.size < MAX_SEARCH_SIZE && fscanf(file, "%d", &searchList.array[searchList.size].key) != EOF)
	{
		searchList.array[searchList.size].index = -1;
		searchList.size++;
	}

	fclose(file);

	//if nothing was read from the search file
	if (searchList.size == 0)
	{
		deleteSearchList(&searchList); //free the memory of the search list
		printf("Error: No retrieval data loaded from file %s!\n", fileName);
	}

	return searchList;
}

//frees the dynamically allocated memory associated with the input list
//searchList: pointer to the input list
void deleteInputList(InputList* inputList)
{
	//free the memory of the arrays of collisions of all the entries
	for (int i = 0; i < inputList->size; ++i)
	{
		free(inputList->array[i].collisions);
		inputList->array[i].collisions = NULL;
	}

	//free the memory of the input list
	free(inputList->array);
	inputList->array = NULL;
	inputList->size = 0;
}

//reads elements from the given file and creates an input list from them
//fileName: the path to the input file
//hashSize: the size of the hash table (this is needed to allocate the space for storing possible collisions)
//returns: an input list with the elements from the input file
InputList readInputList(char* fileName, int hashSize)
{
	//create empty input list
	InputList inputList;
	inputList.array = NULL;
	inputList.size = 0;

	//try to open the input file, return empty input list if it fails
	FILE* file;
	file = fopen(fileName, "r");
	if (file == NULL)
	{
		printf("Error: cannot open file %s!\n", fileName);
		return inputList;
	}

	//try to allocate memory for the input list, return empty input list if it fails
	inputList.array = (InputEntry*)malloc(MAX_INPUT_SIZE * sizeof(InputEntry));
	if (inputList.array == NULL)
	{
		fclose(file);
		printf("Error: couldn't allocate memory!\n");
		return inputList;
	}

	//read entries from the input file until the end of file or until the maximum allowed number of values is read
	while (inputList.size < MAX_INPUT_SIZE && fscanf(file, "%d", &inputList.array[inputList.size].key) != EOF)
	{
		//try to allocate memory for the array of collisions for this entry, return empty input list if it fails
		inputList.array[inputList.size].collisions = (int*)malloc(hashSize * sizeof(int));
		if (inputList.array[inputList.size].collisions == NULL)
		{
			fclose(file);
			deleteInputList(&inputList); //free the memory of the input list
			printf("Error: couldn't allocate memory!\n");
			return inputList;
		}

		inputList.array[inputList.size].numCollisions = 0;
		inputList.size++;
	}

	fclose(file);

	//if nothing was read from the input file
	if (inputList.size == 0)
	{
		deleteInputList(&inputList); //free the memory of the input list
		printf("Error: No input data loaded from file %s!\n", fileName);
	}

	return inputList;
}

//prints the statistics
//inputFile: the path to the input file
//searchFile: the path to the search file
//hashTable: the hashTable
//searchList: the list of entries for searching
//inputList: the list of entries for storage
//storageTime: the time in miliseconds that the storage took
//searchTime: the time in miliseconds that the search took
void printStats(char* inputFile, char* searchFile, HashTable hashTable, SearchList searchList, InputList inputList,
	float storageTime, float searchTime)
{
	printf("Data storage and retrieval:\n");
	printf("a comparison of hashing and directed search of sorted data\n");
	printf("================================\n\n");

	printf("Input data loaded from file %s\n", inputFile);
	printf("Retrieval data loaded from file %s\n", searchFile);
	printf("Storage Method: Hashing\n");
	printf("Number of items stored in the hash table: %d\n", hashTable.numOccupied);
	printf("Number of items searched: %d\n", searchList.size);
	printf("Number of items found: %d\n\n", searchList.numFound);

	printf("Storage details:\n\n");

	printf("  Hash table size: %d\n", hashTable.size);

	for (int i = 0; i < inputList.size; ++i)
	{
		for (int j = 0; j < inputList.array[i].numCollisions; ++j)
		{
			printf("  Collision occurred saving item with value %d at hash table location %d\n", inputList.array[i].key, inputList.array[i].collisions[j]);
		}
	}

	printf("\nRetrieval details:\n\n");
	for (int i = 0; i < searchList.size; ++i)
	{
		if (searchList.array[i].index != -1)
		{
			printf("  Value %d found in the hash table at position %d\n", searchList.array[i].key, searchList.array[i].index);
		}
		else
		{
			printf("  Value %d not found in the hash table\n", searchList.array[i].key);
		}
	}

	printf("\nExecution times:\n\n");

	printf("  Time to store data: %f ms\n", storageTime);
	printf("  Time to retrieve data: %f ms\n\n", searchTime);

	printf("Hash table is %3.0f%% full.\n\n", (hashTable.numOccupied * 100.0) / hashTable.size);

	printf("================================\n\n");
}

int main(int argc, char* argv[])
{
	//check the number of arguments
	if (argc != 4)
	{
		//wrong number of arguments
		printf("Error: 3 arguments required!\n");
		return -1;
	}

	//get the hash table size
	int hashSize = atoi(argv[3]);

	//check if the hash table size is valid
	if (hashSize <= 0)
	{
		//invalid hash table size
		printf("Error: hash table size must be greater than 0!\n");
		return -1;
	}

	//read the input data from the specified file and store it in the input list
	InputList inputList = readInputList(argv[1], hashSize);
	if (inputList.size == 0)
	{
		//nothing was read
		return -1; 
	}

	//read the search data from the specified file and store it in the search list
	SearchList searchList = readSearchList(argv[2]);
	if (searchList.size == 0)
	{
		//nothing was read
		//free dynamically allocated memory
		deleteInputList(&inputList);
		return -1;
	}

	//allocate the hash table
	HashTable hashTable;
	hashTable.array = (HashEntry*)malloc(hashSize * sizeof(HashEntry));

	//if there was an allocation error
	if (hashTable.array == NULL)
	{
		printf("Error: couldn't allocate memory!\n");
		//free dynamically allocated memory
		deleteInputList(&inputList);
		deleteSearchList(&searchList);
		return -1;
	}

	hashTable.size = hashSize;

	//try to insert all of the elements from the input list into the hash table
	//do it many times for purposes of execution time measuring
	clock_t lastTime = clock();
	for (int i = 0; i < 1000000; ++i)
	{
		createHash(&hashTable, inputList);
	}
	//compute the storage time for the hash table
	clock_t totalTime = clock() - lastTime;
	float storageTime = ((((float)totalTime) / CLOCKS_PER_SEC) * 1000) / (1000000);

	//do the search many times for purposes of execution time measuring
	totalTime = 0;
	for (int i = 0; i < 1000000; ++i)
	{
		searchList.numFound = 0;

		//for each item in the search list
		for (int j = 0; j < searchList.size; ++j)
		{
			//try to find it in the hash table
			lastTime = clock();
			int index = searchHash(&hashTable, &searchList.array[j]);

			if (index != -1)
			{
				searchList.numFound++; //it was found
			}
			totalTime += clock() - lastTime;
		}
	}
	//compute the search time for the hash table
	float searchTime = ((((float)totalTime) / CLOCKS_PER_SEC) * 1000) / (1000000 * searchList.size);
	
	//print the statistics
	printStats(argv[1], argv[2], hashTable, searchList, inputList, storageTime, searchTime);

	//free dynamically allocated memory
	deleteInputList(&inputList);
	deleteSearchList(&searchList);
	deleteHash(&hashTable);

	return 0;
}



















# Data storage and retrieval: a comparison of hashing and directed search of sorted data

This assignment involves writing C programs and functions which will store lists of integers in (a) a hash table and (b) a sorted array, and then will find given values within either the hash table or the sorted array. An evaluation and comparison of the run-time and complexity of these algorithms will be performed. The particular algorithms chosen for study are:

- Hashing
- Selection sort
- Binary search

You should produce 2 main programs in files hash.c and sorted.c.  One program (hash.c) will implement the storage and retrieval using a hash table ADT. The other will use selection sort to put the sorted data into an array, and then binary search to find data. Their functionality is defined as follows:

### Input parameters:

Both of your programs should read data from a filename passed into your program on the command line. This file is expected to contain a list of non-negative integers to be stored, with one integer per line, as in the example file input.txt provided. You may assume that the maximum amount of data you are expected to store is 1000 integers. You may also assume that duplicate entries are not permitted, i.e. any integer may not appear twice in the list. Both programs should read another set of integers from a similar file (see example file find.txt provided), again specified on the command line; these integers are values that should be retrieved from the data-store. In the case of hash.c, the program should take an input parameter defining the size of the hash table. Calling your program from the command line will then occur as in the following example:

    $./hash input.txt find.txt 19
    $./sorted input.txt find.txt

where the input filenames are input.txt and find.txt, and hash table size is 19.

### Data storage:

- The program hash.c should initialise a hash table that uses quadratic probing for collision handling. It is up to you whether you decide to use a statically allocated or dynamically allocated  array. It should then store the integers read in from the input file in the hash table using the hash function: f(k) = k mod TableSize and quadratic probing. It is not necessary for you to implement rehashing.
- The program sorted.c should store the integers in a list in the order in which they are read from the file, and then sort the array using the selection sort algorithm. It is up to you whether you store the data in an array or in a linked list pointer-based structure.

### Data retrieval:

- The program hash.c should perform a find operation against the hash table for each item read in from the file containing values to be retrieved. Again, it should use quadratic probing to find items where there might have been a collision. 
- The program sorted.c should use a binary search algorithm against the data in the sorted array for each item read in from the file containing values to be retrieved.

The programs should be structured so that they produce statistics on the run-time of functions for storing and retrieving data. The programs should print out results which consist of a summary header, results and a summary footer. The header should provide details of the input parameters – input filename and retrieval filename, number of items stored in the list, number of items searched for and number found, and which method of storage and retrieval has been used (i.e. hashing or sorting). 

The storage details should then be printed: for hashing, you should output size of hash table, and collisions that have occurred, and for selection sort, you should output the number of comparisons and swaps performed. You should also output retrieval results, i.e. which items were found and which not found. The summary footer should give details of run-time for both time to store the data and time to retrieve the data. In the case of hashing it should specify as a percentage how full the hash table is. 

A typical output from a single run of the hash programme should be similar to the output below:

    Data storage and retrieval: a comparison of hashing and directed search of sorted data
    ================================

    Input data loaded from file input.txt.
    Retrieval data loaded from file find.txt
    Storage Method: Hashing
    Number of items stored in list: 10
    Number of items searched: 5
    Number of items found: 2

    Storage details:

    Hash table size: 19
    Collision occurred saving item with value 121 at hash table location 07
    Collision occurred saving item with value 29 at hash table location 10

    Retrieval details:

    Value 15 not found in table
    Value 72 found in table
    Value 9 found in table
    Value 28 not found in table
    Value 44 not found in table

    Execution times:

    Time to store data:  1 ms
    Time to retrieve data: 2 ms

    Hash table is 53% full.

    ================================

A typical output from a single run of the sorted.c programme should be similar to the output below:

    Data storage and retrieval: a comparison of hashing and directed search of sorted data
    ================================

    Input data loaded from file input.txt.
    Retrieval data loaded from file find.txt
    Storage Method: Directed search of sorted data
    Number of items stored in list: 10
    Number of items searched: 5
    Number of items found: 2

    Storage details:

    <x> comparisons performed
    <y> swaps performed

    Retrieval details:

    Value 15 not found in table
    Value 72 found in table
    Value 9 found in table
    Value 28 not found in table
    Value 44 not found in table

    Execution times:

    Time to store data:  2 ms
    Time to retrieve data: 1 ms

    ================================

The final objective of the exercise is to explore and comment on the relative computational complexity of each algorithm. You should produce a plot that has a curve for each storage method and shows the growth of execution time versus list length for data storage and growth of execution time versus list length for data retrieval. You should do this for three types of data: random, sorted ascending and sorted descending. You should comment on your measurements and make some considered observations about the relative advantages of each method.

### Documentation:

[doc/results.pdf](doc/results.pdf)



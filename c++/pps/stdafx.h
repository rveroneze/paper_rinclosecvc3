// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h> 
#include <math.h>
#include <float.h>
#include <algorithm>
#include <queue>
#include <list>
#include <unordered_map> // it is used only for the file config.txt

#include <sys/resource.h> // it is used only for the experiments about memory RAM usage

using namespace std;

typedef double data_t;
typedef data_t** dataset_t;

typedef unsigned int row_t;
typedef unsigned int col_t;

typedef pair<row_t*, row_t> part_t; // row-set and row-set size

struct concept_t {
	list<part_t> partitions;
	bool *B;
	col_t sizeB;
	col_t col;
};
typedef concept_t *pconcept_t;

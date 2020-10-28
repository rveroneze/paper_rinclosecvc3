#include "stdafx.h"
#include "globalsv.h"

unsigned long MVS = 999999; // value that represents a Missing Value
unsigned short g_output = 1; // 1 - matlab; 2 - python

// ----- Variables for the search of the biclusters -----
unsigned g_cont = 0; // contador de biclusters validos
pair<data_t, row_t> *g_RWp; // vetor para guardar RW
ofstream g_filebics; // pointeiro para o arquivo que vai guardar os bics

// ----- Variable for the experiments about memory RAM usage -----
long int g_ram = 0;
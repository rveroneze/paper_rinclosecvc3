#pragma once

#include "stdafx.h"

extern unsigned long MVS;  // value that represents a Missing Value
extern unsigned short g_output; // 1 - matlab; 2 - python

// ----- Variables for the search of the biclusters -----
extern unsigned g_cont;
extern pair<data_t, row_t> *g_RWp;
extern ofstream g_filebics;

// ----- Variable for the experiments about memory RAM usage -----
extern long int g_ram;
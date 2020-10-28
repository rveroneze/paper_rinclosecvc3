#pragma once

#include "stdafx.h"

#define MAX_BICS 40000000

extern unsigned long MVS;  // value that represents a Missing Value
extern unsigned short g_output; // 1 - matlab; 2 - python

// ----- Variables for the search of the biclusters -----
extern int g_rnew;
extern unsigned g_cont;
extern pair<data_t, row_t> *g_RWp;
extern ofstream g_filebics;

// Biclusters' info:
extern unordered_set<string> g_st;
// ------------------------------------------------------

// ---- Variables for the clique search ----
extern bool **g_Adj;
extern queue<clique_t> g_cliques;
// -----------------------------------------

// ----- Variables for the search of CVC biclusters in OPSM -----
extern row_t *g_RW[2];
extern row_t *g_RM[2];

// ----- Variables for the search using class labels -----
extern unsigned short *g_classes;
extern unsigned short g_maxLabel;
extern double g_minConf;
// --------------------------------------------------------------


// ----- Variable for the experiments about memory RAM usage -----
extern long int g_ram;
// ---------------------------------------------------------------

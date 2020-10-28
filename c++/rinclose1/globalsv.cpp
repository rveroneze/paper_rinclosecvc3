#include "stdafx.h"
#include "globalsv.h"

unsigned long MVS = 999999; // value that represents a Missing Value
unsigned short g_output = 1; // 1 - matlab; 2 - python

// ----- Variables for the search of the biclusters -----
int g_rnew; // contador de biclusters
unsigned g_cont = 0; // contador de biclusters validos
pair<data_t, row_t> *g_RWp; // vetor para guardar RW
ofstream g_filebics; // pointeiro para o arquivo que vai guardar os bics

// Biclusters' info:
unordered_set<string> g_st(MAX_BICS); // guarda os extents que ja foram gerados
// ------------------------------------------------------

// ---- Variables for the clique search ----
bool **g_Adj; // guarda a matriz de adjacencia
queue<clique_t> g_cliques; // guarda os cliques
// -----------------------------------------

// ----- Variables for the search of CVC biclusters in OPSM -----
row_t *g_RW[2];  // vetor para guardar RW
row_t *g_RM[2];  // vetor para guardar RM
// Obs:g_RW[0] eh usado tb no RIn-Close_CVC

// ----- Variables for the search using class labels -----
unsigned short *g_classes; // vetor para guardar a classe de cada objeto
unsigned short g_maxLabel; // maior label existente
double g_minConf = 0; // menor confianca aceita em um bicluster
// --------------------------------------------------------------


// ----- Variable for the experiments about memory RAM usage -----
long int g_ram = 0;
// ---------------------------------------------------------------
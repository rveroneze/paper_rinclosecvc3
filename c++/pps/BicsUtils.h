#pragma once

#include "stdafx.h"
#include "globalsv.h"

void openPrintFile(const string &filename);
void closePrintFile();
void processConcept(const dataset_t &D, const row_t &n, const col_t &m, const col_t &minCol, const data_t &epsilon, const pconcept_t &concept, const bool *ispm);
bool isColMaximal(const dataset_t &D, const row_t &n, const col_t &m, const data_t &epsilon, const part_t &part, const bool *B);
bool isRowMaximal(const dataset_t &D, const row_t &n, const col_t &m, const data_t &epsilon, const part_t &part, const bool *B);
void printBic(const part_t &part, const bool *B, const col_t m);

void printDebugPartitions(list<part_t> partitions);
void printDebugColSet(const bool *B, const col_t &m);
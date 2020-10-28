#pragma once

#include "stdafx.h"
#include "globalsv.h"
#include "BicsUtils.h"

float buildLattice(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const data_t &epsilon);
list<part_t> getPartitions(const dataset_t &D, const row_t &n, const row_t &minRow, const data_t &epsilon, const col_t &j);
void computeConcept(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const data_t &epsilon, const pconcept_t &concept, list<part_t> *partAtr);
bool addColj(const dataset_t &D, const data_t &epsilon, const col_t &j, const pconcept_t &concept);
list<part_t> intersectPartitions(list<part_t> &part1, list<part_t> &part2, const row_t &minRow, bool *ispm);
int isNewPartition(list<part_t> &lista, part_t &cand);
bool isCanonical(const dataset_t &D, const data_t &epsilon, const col_t &y, const bool *B, list<part_t> &nparts);
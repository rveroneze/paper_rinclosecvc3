#pragma once

#include "stdafx.h"
#include "globalsv.h"
#include "BicsUtils.h"

float runRInCloseCVCP(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol);
void RInCloseCVCP(const dataset_t &D, const col_t &m, const row_t &minRow, const col_t &minCol, const pbic_t &bic);
bool RCVCP_IsCanonical(const dataset_t &D, const col_t &y, const row_t &p1, const row_t &p2, const bool *B);

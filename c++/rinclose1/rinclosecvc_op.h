#pragma once

#include "stdafx.h"
#include "globalsv.h"
#include "step3.h"

float runRInCloseCVC_OP(const datasetOP_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol);
void RInCloseCVC_OP(const datasetOP_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const pbic_t &bic);
bool RCVCOP_IsCanonical(const datasetOP_t &D, const col_t &y, const short &crw, const row_t &sizeRW, const pbic_t &bic);
bool RCVCOP_IsMaximal(const datasetOP_t &D, const col_t &m, const col_t &y, const short &crw, const row_t &sizeRW, const pbic_t &bic);
void RCVCOP_computeRM(const row_t &minRow, const pbic_t &bic, const pbic_t &child, const short &crw, const row_t &sizeRM, const row_t &cshared);

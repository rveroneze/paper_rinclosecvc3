#include "stdafx.h"
#include "rinclosecvc_op.h"

float runRInCloseCVC_OP(const datasetOP_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol)
{
	// Allocating memory for the global variables
	g_RWp = new pair<data_t, row_t>[n];
	g_RW[0] = new row_t[n];
	g_RW[1] = new row_t[n];
	g_RM[0] = new row_t[n];
	g_RM[1] = new row_t[n];
	

	clock_t clocks = clock();
	g_rnew = 0;

	// Creating the supremum
	//string sext = "";
	//char buffer[20];
	pbic_t bic = new bic_t;
	bic->A = new row_t[n];
	for (row_t i = 0; i < n; ++i)
	{
		bic->A[i] = i;
		//snprintf(buffer, 20, "%d ", i);
		//sext = sext + buffer;
	}
	bic->sizeA = n;
	bic->B = new bool[m];
	for (col_t i = 0; i < m; ++i)
		bic->B[i] = false;
	bic->sizeB = 0;
	bic->sizeRM = 0;
	bic->col = 0;
	//g_st.insert(sext); // nao preciso guardar o supremum, s� tem 1 com esse extent

	RInCloseCVC_OP(D, n, m, minRow, minCol, bic); // call RIn-Close

	clocks = clock() - clocks;
	return ((float)clocks) / CLOCKS_PER_SEC;
}

void RInCloseCVC_OP(const datasetOP_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const pbic_t &bic)
{
	string sext;
	char buffer[20];

	queue<pbic_t> children;

	// Iterating across the attributes
	for (col_t j = bic->col; j < m; ++j)
	{
		if (m - j + bic->sizeB < minCol)
			break;

		if (!bic->B[j])
		{
			row_t sRW[2] = {0, 0}, sRM[2] = {0, 0}, cshared = 0;
			for (row_t i = 0; i < bic->sizeA; ++i)
			{
				if (D[bic->A[i]][j] == 1)
				{
					g_RW[0][sRW[0]++] = bic->A[i];
					g_RM[1][sRM[1]++] = bic->A[i];
				}
				else if (D[bic->A[i]][j] == 3)
				{
					g_RW[1][sRW[1]++] = bic->A[i];
					g_RM[0][sRM[0]++] = bic->A[i];
				}
				else if (D[bic->A[i]][j] == 2)
				{
					g_RW[0][sRW[0]++] = bic->A[i];
					g_RW[1][sRW[1]++] = bic->A[i];
					++cshared;
				}
			}

			if (sRW[0] == bic->sizeA || sRW[1] == bic->sizeA)
			{
				bic->B[j] = true; // add the attribute j to B[r] (incremental closure)
				++bic->sizeB;
			}
			else
			{
				if (bic->sizeA > minRow) // otherwise, bic r can not generate descendants with at least minRow rows
				{
					for (short crw = 0; crw <= 1; ++crw)
					{
						if (sRW[crw] >= minRow)
						{
							if (RCVCOP_IsCanonical(D, j, crw, sRW[crw], bic) && RCVCOP_IsMaximal(D, m, j, crw, sRW[crw], bic))
							{
								//Creates a string with the extent
								sext = "";
								for (row_t i2 = 0; i2 < sRW[crw]; ++i2)
								{
									snprintf(buffer, 20, "%d ", g_RW[crw][i2]);
									sext = sext + buffer;
								}
								// Tests if the extent is not redundant
								if (g_st.count(sext) == 0)
								{
									pbic_t child = new bic_t;
									g_st.insert(sext);
									child->A = new row_t[bic->sizeRM + sRW[crw] + sRM[crw]];
									for (row_t i2 = 0; i2 < sRW[crw]; ++i2)
										child->A[i2] = g_RW[crw][i2];
									child->sizeA = sRW[crw];
									child->col = j + 1;
									RCVCOP_computeRM(minRow, bic, child, crw, sRM[crw], cshared);
									children.push(child);
								}
							}
						}
					}
				}
			}
		}
	}

	// imprime o bicluster e elimina suas linhas da memoria
	if (bic->sizeB >= minCol)
		S3_step3_OP(D, n, m, minCol, bic);
	delete[] bic->A;

	// fechando os filhos
	while (!children.empty())
	{
		pbic_t child = children.front();
		child->B = new bool[m];
		for (col_t j = 0; j < m; ++j)
			child->B[j] = bic->B[j];
		child->B[child->col - 1] = true;
		child->sizeB = bic->sizeB + 1;
		RInCloseCVC_OP(D, n, m, minRow, minCol, child);
		children.pop();
	}
	delete[] bic->B;
	delete bic;
}

bool RCVCOP_IsCanonical(const datasetOP_t &D, const col_t &y, const short &crw, const row_t &sizeRW, const pbic_t &bic)
{
	row_t i;
	for (col_t j = 0; j < y; ++j)
	{
		if (!bic->B[j])
		{
			if (sizeRW > 1)
			{
				if (D[g_RW[crw][0]][j] != MVS)
				{
					dataOP_t maior = D[g_RW[crw][0]][j], menor = maior;
					for (i = 1; i < sizeRW; ++i)
					{
						if (D[g_RW[crw][i]][j] == MVS)
							break;
						if (D[g_RW[crw][i]][j] > maior)
							maior = D[g_RW[crw][i]][j];
						if (D[g_RW[crw][i]][j] < menor)
							menor = D[g_RW[crw][i]][j];
						if (maior - menor > 1)
							break;
					}
					if (i == sizeRW)
						return false;
				}
			}
			else if (D[g_RW[crw][0]][j] != MVS)
				return false;
		}
	}
	return true;
}

bool RCVCOP_IsMaximal(const datasetOP_t &D, const col_t &m, const col_t &y, const short &crw, const row_t &sizeRW, const pbic_t &bic)
{
	row_t pr;
	col_t k, j;
	dataOP_t maior, menor;
	for (row_t i = 0; i < bic->sizeRM; ++i)
	{
		pr = bic->sizeA + i; // posicao em g_A que esta a linha a ser testada

		// Verificando a coluna y
		if (D[bic->A[pr]][y] == MVS)
			continue;
		maior = D[bic->A[pr]][y];
		menor = maior;
		for (k = 0; k < sizeRW; ++k)
		{
			if (D[g_RW[crw][k]][y] == MVS)
				break;
			if (D[g_RW[crw][k]][y] > maior)
				maior = D[g_RW[crw][k]][y];
			if (D[g_RW[crw][k]][y] < menor)
				menor = D[g_RW[crw][k]][y];
			if (maior - menor > 1)
				break;
		}
		if (k < sizeRW)
			continue;

		// Verificando as demais colunas do bicluster
		for (j = 0; j < m; ++j)
		{
			if (bic->B[j])
			{
				if (D[bic->A[pr]][j] == MVS)
					break;
				maior = D[bic->A[pr]][j];
				menor = maior;
				for (k = 0; k < sizeRW; ++k)
				{
					if (D[g_RW[crw][k]][j] == MVS)
						break;
					if (D[g_RW[crw][k]][j] > maior)
						maior = D[g_RW[crw][k]][j];
					if (D[g_RW[crw][k]][j] < menor)
						menor = D[g_RW[crw][k]][j];
					if (maior - menor > 1)
						break;
				}
				if (k < sizeRW)
					break;
			}
		}

		if (j == m)
			return false;
	}
	return true;
}

void RCVCOP_computeRM(const row_t &minRow, const pbic_t &bic, const pbic_t &child, const short &crw, const row_t &sizeRM, const row_t &cshared)
{
	row_t pRM = child->sizeA;

	// Inheriting the RM
	for (row_t i = 0; i < bic->sizeRM; ++i)
		child->A[pRM++] = bic->A[bic->sizeA + i];

	// Computing the new piece of the RM
	if (cshared >= minRow)
		for (row_t i = 0; i < sizeRM; ++i)
			child->A[pRM++] = g_RM[crw][i];

		child->sizeRM = pRM - child->sizeA;
}

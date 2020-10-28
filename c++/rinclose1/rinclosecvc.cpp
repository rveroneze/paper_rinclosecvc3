#include "stdafx.h"
#include "rinclosecvc.h"

float runRInCloseCVC(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const data_t &epsilon, const bool &s3)
{
	// Allocating memory for the global variables
	g_RWp = new pair<data_t, row_t>[n];
	g_RW[0] = new row_t[n];
	g_RW[1] = new row_t[n];
	g_RM[0] = new row_t[n];
	g_RM[1] = new row_t[n];
	

	clock_t clocks = clock();

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

	RInCloseCVC(D, n, m, minRow, minCol, epsilon, bic, s3); // call RIn-Close

	clocks = clock() - clocks;
	return ((float)clocks) / CLOCKS_PER_SEC;
}

void RInCloseCVC(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const data_t &epsilon, const pbic_t &bic, const bool &s3)
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
			row_t qnmv = 0; // number of non-missing values
			data_t maior = -FLT_MAX, menor = FLT_MAX;
			for (row_t i = 0; i < bic->sizeA; ++i)
			{
				if (D[bic->A[i]][j] != MVS)
				{
					g_RWp[qnmv].first = D[bic->A[i]][j];
					g_RWp[qnmv].second = bic->A[i];
					if (g_RWp[qnmv].first > maior) maior = g_RWp[qnmv].first;
					if (g_RWp[qnmv].first < menor) menor = g_RWp[qnmv].first;
					++qnmv;
				}
			}

			if (qnmv == bic->sizeA  &&  maior - menor <= epsilon)
			{
				bic->B[j] = true; // add the attribute j to B[r] (incremental closure)
				++bic->sizeB;
			}
			else if (bic->sizeA > minRow && qnmv >= minRow) // otherwise, bic r can not generate descendants with at least minRow rows
			{
				sort(g_RWp, g_RWp + qnmv);
				row_t p1 = 0, p2 = 0, last = qnmv + 1;
				while (p1 <= qnmv - minRow && p2 < qnmv - 1)
				{
					if (p2 < p1) p2 = p1;
					while (p2 < qnmv - 1 && g_RWp[p2 + 1].first - g_RWp[p1].first <= epsilon)
						++p2;
					if (p2 != last && p2 - p1 + 1 >= minRow)
					{
						if (RCVC_IsCanonical(D, epsilon, j, p1, p2, bic) && RCVC_IsMaximal(D, m, epsilon, j, p1, p2, bic))
						{
							//Creates a string with the ordered extent
							row_t sRW = 0;
							for (row_t i2 = p1; i2 <= p2; ++i2)
								g_RW[0][sRW++] = g_RWp[i2].second;
							sort(g_RW[0], g_RW[0] + sRW);
							sext = "";
							for (row_t i2 = 0; i2 < sRW; ++i2)
							{
								snprintf(buffer, 20, "%d ", g_RW[0][i2]);
								sext = sext + buffer;
							}
							// Tests if the extent is not redundant
							if (g_st.count(sext) == 0)
							{
								pbic_t child = new bic_t;
								g_st.insert(sext);
								child->A = new row_t[qnmv + bic->sizeRM];
								for (row_t i2 = 0; i2 < sRW; ++i2)
									child->A[i2] = g_RW[0][i2];
								child->sizeA = sRW;
								child->col = j + 1;
								RCVC_computeRM(minRow, epsilon, bic, child, p1, p2, qnmv);
								children.push(child);
							}
						}
					}
					p1 = p1 + 1;
					last = p2;
				}
			}
		}
	}

	// For the experiments about memory RAM usage
	struct rusage r_usage;
    getrusage(RUSAGE_SELF,&r_usage);
	if (r_usage.ru_maxrss > g_ram) g_ram = r_usage.ru_maxrss;

	// imprime o bicluster e elimina suas linhas da memoria
	if (bic->sizeB >= minCol)
	{
		if (s3)
			S3_step3(D, n, m, minCol, epsilon, bic);
		else
			printBic(bic, m);
	}
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
		RInCloseCVC(D, n, m, minRow, minCol, epsilon, child, s3);
		children.pop();
	}
	delete[] bic->B;
	delete bic;
}

bool RCVC_IsCanonical(const dataset_t &D, const data_t &epsilon, const col_t &y, const row_t &p1, const row_t &p2, const pbic_t &bic)
{
	row_t i;
	for (col_t j = 0; j < y; ++j)
	{
		if (!bic->B[j])
		{
			if (p1 != p2)
			{
				if (D[g_RWp[p1].second][j] != MVS)
				{
					data_t maior = D[g_RWp[p1].second][j], menor = maior;
					for (i = p1 + 1; i <= p2; ++i)
					{
						if (D[g_RWp[i].second][j] == MVS)
							break;
						if (D[g_RWp[i].second][j] > maior)
							maior = D[g_RWp[i].second][j];
						if (D[g_RWp[i].second][j] < menor)
							menor = D[g_RWp[i].second][j];
						if (maior - menor > epsilon)
							break;
					}
					if (i > p2)
						return false;
				}
			}
			else if (D[g_RWp[p1].second][j] != MVS)
				return false;
		}
	}
	return true;
}

bool RCVC_IsMaximal(const dataset_t &D, const col_t &m, const data_t &epsilon, const col_t &y, const row_t &p1, const row_t &p2, const pbic_t &bic)
{
	// Pega o menor e maior valor do bicluster filho por coluna
	// Note que por definicao, o filho nao contem dados faltantes (e nem o pai)
	data_t *menorF = new data_t[m];
	data_t *maiorF = new data_t[m];
	for (col_t j = 0; j < m; ++j)
	{
		if (j == y || bic->B[j])
		{
			menorF[j] = D[g_RWp[p1].second][j];
			maiorF[j] = D[g_RWp[p1].second][j];
			for (row_t i = p1 + 1; i <= p2; ++i)
			{
				if (D[g_RWp[i].second][j] < menorF[j]) menorF[j] = D[g_RWp[i].second][j];
				if (D[g_RWp[i].second][j] > maiorF[j]) maiorF[j] = D[g_RWp[i].second][j];
			}
		}
	}

	row_t pr;
	col_t j;
	data_t maior, menor;
	for (row_t i = 0; i < bic->sizeRM; ++i)
	{
		pr = bic->sizeA + i; // posicao em bic->A que esta a linha a ser testada
		for (j = 0; j < m; ++j)
		{
			if (j == y || bic->B[j])
			{
				if (D[bic->A[pr]][j] == MVS) break;
				menor = menorF[j];
				maior = maiorF[j];
				if (D[bic->A[pr]][j] < menor) menor = D[bic->A[pr]][j];
				if (D[bic->A[pr]][j] > maior) maior = D[bic->A[pr]][j];
				if (maior - menor > epsilon) break;
			}
		}
		if (j == m)
		{
			delete [] menorF;
			delete [] maiorF;
			return false;
		}
	}

	delete [] menorF;
	delete [] maiorF;
	return true;
}

void RCVC_computeRM(const row_t &minRow, const data_t &epsilon, const pbic_t &bic, const pbic_t &child, const row_t &p1, const row_t &p2, const row_t qnmv)
{
	row_t pRM = child->sizeA;

	// Inheriting the RM
	for (row_t i = 0; i < bic->sizeRM; ++i)
		child->A[pRM++] = bic->A[bic->sizeA + i];

	// Computing the new piece of the RM
	if (p1 > 0)
	{
		
		row_t posPivo = p1 + minRow - 1, p = p1 - 1;
		while (g_RWp[posPivo].first - g_RWp[p].first <= epsilon)
		{
			child->A[pRM++] = g_RWp[p].second;
			if (p > 0)
				--p;
			else
				break;
		}
	}
	if (p2 < qnmv - 1)
	{
		row_t posPivo = p2 - minRow + 1, p = p2 + 1;
		while (p < qnmv && g_RWp[p].first - g_RWp[posPivo].first <= epsilon)
		{
			child->A[pRM++] = g_RWp[p].second;
			++p;
		}
	}

	child->sizeRM = pRM - child->sizeA;
}



// RInClose_CVC que aceita um epsilon por coluna:

float runRInCloseCVCve(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const data_t *epsilons)
{
	clock_t clocks = clock();

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
	//g_st.insert(sext); // nao preciso guardar o supremum, soh tem 1 com esse extent

	RInCloseCVCve(D, n, m, minRow, minCol, epsilons, bic); // call RIn-Close

	clocks = clock() - clocks;
	return ((float)clocks) / CLOCKS_PER_SEC;
}

void RInCloseCVCve(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const data_t *epsilons, const pbic_t &bic)
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
			row_t qnmv = 0; // number of non-missing values
			data_t maior = -FLT_MAX, menor = FLT_MAX;
			for (row_t i = 0; i < bic->sizeA; ++i)
			{
				if (D[bic->A[i]][j] != MVS)
				{
					g_RWp[qnmv].first = D[bic->A[i]][j];
					g_RWp[qnmv].second = bic->A[i];
					if (g_RWp[qnmv].first > maior) maior = g_RWp[qnmv].first;
					if (g_RWp[qnmv].first < menor) menor = g_RWp[qnmv].first;
					++qnmv;
				}
			}

			if (qnmv == bic->sizeA  &&  maior - menor <= epsilons[j])
			{
				bic->B[j] = true; // add the attribute j to B[r] (incremental closure)
				++bic->sizeB;
			}
			else if (bic->sizeA > minRow && qnmv >= minRow) // otherwise, bic r can not generate descendants with at least minRow rows
			{
				sort(g_RWp, g_RWp + qnmv);
				row_t p1 = 0, p2 = 0, last = qnmv + 1;
				while (p1 <= qnmv - minRow && p2 < qnmv - 1)
				{
					if (p2 < p1) p2 = p1;
					while (p2 < qnmv - 1 && g_RWp[p2 + 1].first - g_RWp[p1].first <= epsilons[j])
						++p2;
					if (p2 != last && p2 - p1 + 1 >= minRow)
					{
						if (RCVC_IsCanonical(D, epsilons, j, p1, p2, bic) && RCVC_IsMaximal(D, m, epsilons, j, p1, p2, bic))
						{
							//Creates a string with the ordered extent
							row_t sRW = 0;
							for (row_t i2 = p1; i2 <= p2; ++i2)
								g_RW[0][sRW++] = g_RWp[i2].second;
							sort(g_RW[0], g_RW[0] + sRW);
							sext = "";
							for (row_t i2 = 0; i2 < sRW; ++i2)
							{
								snprintf(buffer, 20, "%d ", g_RW[0][i2]);
								sext = sext + buffer;
							}
							// Tests if the extent is not redundant
							if (g_st.count(sext) == 0)
							{
								pbic_t child = new bic_t;
								g_st.insert(sext);
								child->A = new row_t[qnmv + bic->sizeRM];
								for (row_t i2 = 0; i2 < sRW; ++i2)
									child->A[i2] = g_RW[0][i2];
								child->sizeA = sRW;
								child->col = j + 1;
								RCVC_computeRM(minRow, epsilons[j], bic, child, p1, p2, qnmv);
								children.push(child);
							}
						}
					}
					p1 = p1 + 1;
					last = p2;
				}
			}
		}
	}

	// imprime o bicluster e elimina suas linhas da memoria
	if (bic->sizeB >= minCol)
		printBic(bic, m);
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
		RInCloseCVCve(D, n, m, minRow, minCol, epsilons, child);
		children.pop();
	}
	delete[] bic->B;
	delete bic;
}

bool RCVC_IsCanonical(const dataset_t &D, const data_t *epsilons, const col_t &y, const row_t &p1, const row_t &p2, const pbic_t &bic)
{
	row_t i;
	for (col_t j = 0; j < y; ++j)
	{
		if (!bic->B[j])
		{
			if (p1 != p2)
			{
				if (D[g_RWp[p1].second][j] != MVS)
				{
					data_t maior = D[g_RWp[p1].second][j], menor = maior;
					for (i = p1 + 1; i <= p2; ++i)
					{
						if (D[g_RWp[i].second][j] == MVS)
							break;
						if (D[g_RWp[i].second][j] > maior)
							maior = D[g_RWp[i].second][j];
						if (D[g_RWp[i].second][j] < menor)
							menor = D[g_RWp[i].second][j];
						if (maior - menor > epsilons[j])
							break;
					}
					if (i > p2)
						return false;
				}
			}
			else if (D[g_RWp[p1].second][j] != MVS)
				return false;
		}
	}
	return true;
}

bool RCVC_IsMaximal(const dataset_t &D, const col_t &m, const data_t *epsilons, const col_t &y, const row_t &p1, const row_t &p2, const pbic_t &bic)
{
	// Pega o menor e maior valor do bicluster filho por coluna
	// Note que por definicao, o filho nao contem dados faltantes (e nem o pai)
	data_t *menorF = new data_t[m];
	data_t *maiorF = new data_t[m];
	for (col_t j = 0; j < m; ++j)
	{
		if (j == y || bic->B[j])
		{
			menorF[j] = D[g_RWp[p1].second][j];
			maiorF[j] = D[g_RWp[p1].second][j];
			for (row_t i = p1 + 1; i <= p2; ++i)
			{
				if (D[g_RWp[i].second][j] < menorF[j]) menorF[j] = D[g_RWp[i].second][j];
				if (D[g_RWp[i].second][j] > maiorF[j]) maiorF[j] = D[g_RWp[i].second][j];
			}
		}
	}

	row_t pr;
	col_t j;
	data_t maior, menor;
	for (row_t i = 0; i < bic->sizeRM; ++i)
	{
		pr = bic->sizeA + i; // posicao em bic->A que esta a linha a ser testada
		for (j = 0; j < m; ++j)
		{
			if (j == y || bic->B[j])
			{
				if (D[bic->A[pr]][j] == MVS) break;
				menor = menorF[j];
				maior = maiorF[j];
				if (D[bic->A[pr]][j] < menor) menor = D[bic->A[pr]][j];
				if (D[bic->A[pr]][j] > maior) maior = D[bic->A[pr]][j];
				if (maior - menor > epsilons[j]) break;
			}
		}
		if (j == m)
		{
			delete [] menorF;
			delete [] maiorF;
			return false;
		}
	}

	delete [] menorF;
	delete [] maiorF;
	return true;
}

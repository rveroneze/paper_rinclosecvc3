#include "stdafx.h"
#include "BicsUtils.h"

void openPrintFile(const string &filename)
{
	g_filebics.open(filename);
	if (g_output == 2) // python
	{
		g_filebics << "#!/usr/bin/env python3" << endl << endl;
		g_filebics << "bics = []" << endl;
	}
}

void closePrintFile()
{
	g_filebics.close();
}

void processConcept(const dataset_t &D, const row_t &n, const col_t &m, const col_t &minCol, const data_t &epsilon, const pconcept_t &concept, const bool *ispm)
{
	if (concept->sizeB >= minCol)
	{
		row_t idx = 0;
		for (list<part_t>::iterator it=concept->partitions.begin(); it != concept->partitions.end(); ++it)
		{
			if (ispm[idx] && isColMaximal(D, n, m, epsilon, (*it), concept->B) && isRowMaximal(D, n, m, epsilon, (*it), concept->B))
			{
				printBic((*it), concept->B, m);
			}
			idx++;
		}
	}
}

bool isColMaximal(const dataset_t &D, const row_t &n, const col_t &m, const data_t &epsilon, const part_t &part, const bool *B)
{
	// By defintion, trere is no missing values in the concepts
	for (col_t j = 0; j < m; ++j)
	{
		if (!B[j])
		{
			data_t maior = -FLT_MAX, menor = FLT_MAX;
			row_t i;
			for (i = 0; i < part.second; ++i)
			{
				if (D[part.first[i]][j] < menor) menor = D[part.first[i]][j];
				if (D[part.first[i]][j] > maior) maior = D[part.first[i]][j];
				if (maior - menor > epsilon) break;
			}
			if (i == part.second) return false;
		}
	}
	return true;
}

bool isRowMaximal(const dataset_t &D, const row_t &n, const col_t &m, const data_t &epsilon, const part_t &part, const bool *B)
{
	// By defintion, trere is no missing values in the concepts
	data_t *menorF = new data_t[m];
	data_t *maiorF = new data_t[m];
	for (col_t j = 0; j < m; ++j)
	{
		if (B[j])
		{
			menorF[j] = D[part.first[0]][j];
			maiorF[j] = D[part.first[0]][j];
			for (row_t i = 1; i < part.second; ++i)
			{
				if (D[part.first[i]][j] < menorF[j]) menorF[j] = D[part.first[i]][j];
				if (D[part.first[i]][j] > maiorF[j]) maiorF[j] = D[part.first[i]][j];
			}
		}
	}

	data_t maior, menor;
	for (row_t i = 0; i < n; ++i)
	{
		row_t *p = find(part.first, part.first+part.second, i);
		if (p == part.first+part.second)  // row i is not in the partition
		{
			col_t j;
			for (j = 0; j < m; ++j)
			{
				if (B[j])
				{
					if (D[i][j] == MVS) break;
					menor = menorF[j];
					maior = maiorF[j];
					if (D[i][j] < menor) menor = D[i][j];
					if (D[i][j] > maior) maior = D[i][j];
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
	}
	delete [] menorF;
	delete [] maiorF;
	return true;
}

void printBic(const part_t &part, const bool *B, const col_t m)
{
	++g_cont;
	if (g_output == 1) // matlab
	{
		g_filebics << "A{" << g_cont << "} = [";
		for (row_t i = 0; i < part.second; ++i)
			g_filebics << part.first[i] + 1 << " ";
		g_filebics << "];\nB{" << g_cont << "} = [";
		for (col_t i = 0; i < m; ++i)
		{
			if (B[i])
				g_filebics << i + 1 << " ";
		}
		g_filebics << "];\n";
	}
	else // python
	{
		g_filebics << "bics.append([[";
		for (row_t i = 0; i < part.second; ++i)
			g_filebics << part.first[i]  << ",";
		g_filebics << "],[";
		for (col_t i = 0; i < m; ++i)
		{
			if (B[i])
				g_filebics << i << ",";
		}
		g_filebics << "]])" << endl;
	}
}

void printDebugPartitions(list<part_t> partitions)
{
	cout << "Particoes:" << endl;
	for (list<part_t>::iterator it=partitions.begin(); it != partitions.end(); ++it)
	{
		for (row_t i = 0; i < it->second; ++i)
			cout << it->first[i] << " ";
		cout << endl;
	}
}

void printDebugColSet(const bool *B, const col_t &m)
{
	cout << "Colunas: ";
	for (col_t i = 0; i < m; ++i)
	{
		if (B[i])
			cout << i << " ";
	}
	cout << endl;
}

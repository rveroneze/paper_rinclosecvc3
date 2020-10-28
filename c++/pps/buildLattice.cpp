#include "stdafx.h"
#include "buildLattice.h"

float buildLattice(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const data_t &epsilon)
{
	// Allocating memory for the global variable
	g_RWp = new pair<data_t, row_t>[n];

	clock_t clocks = clock();

	// Creating the partitions of each attribute
	list<part_t> *partAtr = new list<part_t>[m];
	for (col_t j = 0; j < m; ++j){
		partAtr[j] = getPartitions(D, n, minRow, epsilon, j);
	}

	// Creating the supremum
	part_t part;
	part.second = n;
	part.first = new row_t[n];
	for (row_t i = 0; i < n; ++i) part.first[i] = i;
	pconcept_t concept = new concept_t;
	concept->partitions.push_back(part);
	concept->B = new bool[m];
	for (col_t i = 0; i < m; ++i) concept->B[i] = false;
	concept->sizeB = 0;
	concept->col = 0;

	computeConcept(D, n, m, minRow, minCol, epsilon, concept, partAtr);

	clocks = clock() - clocks;
	return ((float)clocks) / CLOCKS_PER_SEC;
}

list<part_t> getPartitions(const dataset_t &D, const row_t &n, const row_t &minRow, const data_t &epsilon, const col_t &j)
{
	list<part_t> partAtrs;
	row_t qnmv = 0; // number of non-missing values
	for (row_t i = 0; i < n; ++i)
	{
		if (D[i][j] != MVS)
		{
			g_RWp[qnmv].first = D[i][j];
			g_RWp[qnmv].second = i;
			++qnmv;
		}
	}
	if (qnmv >= minRow)
	{
		sort(g_RWp, g_RWp + qnmv);
		row_t p1 = 0, p2 = 0, last = qnmv + 1;
		while (p1 <= qnmv - minRow && p2 < qnmv - 1)
		{
			if (p2 < p1)
				p2 = p1;
			while (p2 < qnmv - 1 && g_RWp[p2 + 1].first - g_RWp[p1].first <= epsilon)
				++p2;
			if (p2 != last && p2 - p1 + 1 >= minRow)
			{
				part_t part;
				part.second = p2 - p1 + 1; // partition size
				part.first = new row_t[part.second];
				for (row_t i2 = p1; i2 <= p2; ++i2)
					part.first[i2 - p1] = g_RWp[i2].second;
				sort(part.first, part.first + part.second);
				partAtrs.push_back(part);
			}
			p1 = p1 + 1;
			last = p2;
		}
	}
	return partAtrs;
}

void computeConcept(const dataset_t &D, const row_t &n, const col_t &m, const row_t &minRow, const col_t &minCol, const data_t &epsilon, const pconcept_t &concept, list<part_t> *partAtr)
{
	/*char debug;
	cout << "Concept being closed " << endl;
	printDebugPartitions(concept->partitions);
	printDebugColSet(concept->B, m);
	cin >> debug;*/

	queue<pconcept_t> children;

	// It is used to break the lattice and avoid mining many non-maximal biclusters - BL-ANM
	bool *ispm = new bool[concept->partitions.size()];
	for (row_t i = 0; i < concept->partitions.size(); ++i)  ispm[i] = true;
	// -----------------------------------------------------------------------

	// Iterating across the attributes
	for (col_t j = concept->col; j < m; ++j)
	{
		if (m - j + concept->sizeB < minCol)
			break; // this bic and its next descendants would not meet minCol

		if (!concept->B[j])
		{
			//cout << "Iteracao coluna " << j << endl;
			if (addColj(D, epsilon, j, concept))
			{
				//cout << "add col " << j << endl;
				concept->B[j] = true; // add the attribute j to B (incremental closure)
				++concept->sizeB;
			}
			else
			{
				list<part_t> nparts = intersectPartitions(concept->partitions, partAtr[j], minRow, ispm);
				//cout << "Particao candidata " << endl;
				//printDebugPartitions(nparts);
				if (nparts.size() > 0)
				{
					if (isCanonical(D, epsilon, j, concept->B, nparts))
					{
						//cout << "Eh canonica " << endl;
						pconcept_t child = new concept_t;
						child->partitions = nparts;
						child->col = j + 1;
						children.push(child);
					}
					else
					{
						for (list<part_t>::iterator it=nparts.begin(); it != nparts.end(); ++it)
						{
							delete [] it->first;
						}
					}
				}
			}
			//cin >> debug;
		}
	}

	// For the experiments about memory RAM usage
	struct rusage r_usage;
    getrusage(RUSAGE_SELF,&r_usage);
	if (r_usage.ru_maxrss > g_ram) g_ram = r_usage.ru_maxrss;
	
	processConcept(D, n, m, minCol, epsilon, concept, ispm);

	// Deallocating memory:
	delete [] ispm;  // BL-ANM
	for (list<part_t>::iterator it=concept->partitions.begin(); it != concept->partitions.end(); ++it)
	{
		delete [] it->first;
	}
	concept->partitions.clear();

	// closing the children
	while (!children.empty())
	{
		pconcept_t child = children.front();
		child->B = new bool[m];
		for (col_t j = 0; j < m; ++j)
			child->B[j] = concept->B[j];
		child->B[child->col - 1] = true;
		child->sizeB = concept->sizeB + 1;
		computeConcept(D, n, m, minRow, minCol, epsilon, child, partAtr);
		children.pop();
	}
	// Deallocating memory:	
	delete [] concept->B;
	delete concept;
}

bool addColj(const dataset_t &D, const data_t &epsilon, const col_t &j, const pconcept_t &concept)
{
	for (list<part_t>::iterator it=concept->partitions.begin(); it != concept->partitions.end(); ++it)
	{
		data_t maior = -FLT_MAX, menor = FLT_MAX;
		for (row_t i = 0; i < it->second; ++i)
		{
			row_t row = it->first[i];
			if (D[row][j] == MVS) return false;
			if (D[row][j] < menor) menor = D[row][j];
			if (D[row][j] > maior) maior = D[row][j];
			if (maior - menor > epsilon) return false;
		}
	}
	return true;
}

list<part_t> intersectPartitions(list<part_t> &part1, list<part_t> &part2, const row_t &minRow, bool *ispm)
{
	list<part_t> nparts;
	row_t idx = 0;
	for (list<part_t>::iterator it1=part1.begin(); it1 != part1.end(); ++it1)
	{
		if (ispm[idx]) // BL-ANM
		{
			for (list<part_t>::iterator it2=part2.begin(); it2 != part2.end(); ++it2)
			{
				part_t aux;
				aux.first = new row_t[it1->second+it2->second];
				row_t *it;
				it = (row_t*) set_intersection (it1->first, it1->first+it1->second, it2->first, it2->first+it2->second, aux.first);
				aux.second = it - aux.first;
				if (aux.second >= minRow)
				{
					int res = isNewPartition(nparts, aux);
					if (res == 0) delete [] aux.first;
					else
					{
						if (res == 2) nparts.push_back(aux);
						
						// BL-ANM ----------------------
						if (aux.second == it1->second)
						{
							ispm[idx] = false;
							break;
						}
						// -----------------------------
					}
				}
				else delete [] aux.first;
			}
		}
		++idx;
	}
	return nparts;
}

int isNewPartition(list<part_t> &lista, part_t &cand)
{
	for (list<part_t>::iterator it=lista.begin(); it != lista.end(); ++it)
	{
		if (cand.second <= it->second)
		{
			if (includes(it->first, it->first+it->second, cand.first, cand.first+cand.second))
			{
				return 0;
			}
		}
		else
		{
			if (includes(cand.first, cand.first+cand.second, it->first, it->first+it->second))
			{
				delete [] it->first;
				it->first = cand.first;
				it->second = cand.second;
				return 1;
			}
		}
	}
	return 2;
}

bool isCanonical(const dataset_t &D, const data_t &epsilon, const col_t &y, const bool *B, list<part_t> &nparts)
{
	for (col_t j = 0; j < y; ++j)
	{
		if (!B[j])
		{
			bool canAddj =  true;
			for (list<part_t>::iterator it=nparts.begin(); it != nparts.end(); ++it)
			{
				data_t maior = -FLT_MAX, menor = FLT_MAX;
				for (row_t i = 0; i < it->second; ++i)
				{
					if (D[it->first[i]][j] == MVS)
					{
						canAddj =  false;
						break;
					}
					else
					{
						if (D[it->first[i]][j] < menor) menor = D[it->first[i]][j];
						if (D[it->first[i]][j] > maior) maior = D[it->first[i]][j];
						if (maior - menor > epsilon)
						{
							canAddj =  false;
							break;
						}
					}
				}
				if (!canAddj) break;
			}
			if (canAddj) return false;
		}
	}
	return true;
}

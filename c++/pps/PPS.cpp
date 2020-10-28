#include "stdafx.h"
#include "globalsv.h"
#include "BicsUtils.h"
#include "buildLattice.h"
#include "string.h"

bool readDataset(const string &dataSetName, dataset_t &matrix, row_t &n, col_t &m);
void printData(const dataset_t &matrix, const row_t &n, const col_t &m);
bool readConfigFile();

int main(int argc, char* argv[])
{
	if (argc < 5)
	{
		cout << "\n!!! Wrong Arguments !!!" << endl << endl;
		cout << "List of the arguments:" << endl;
		cout << "1 - Dataset's filename;" << endl;
		cout << "2 - minRow;" << endl;
		cout << "3 - minCol;" << endl;
		cout << "4 - Epsilon;" << endl;
		cout << "5 - Output filename for the list of biclusters;" << endl;
		exit(1);
	}

	if (!readConfigFile())
	{
		cout << "\nConfiguration file was not loaded.\n";
		cout << "Default configuration:" << endl;
		cout << "Not implemented - Value that represents a Missing Value: " << MVS << endl;
		cout << "Output format (1 - matlab; 2 - python): " << g_output << endl;
	}

	row_t minRow = atoi(argv[2]);
	col_t minCol = atoi(argv[3]);
	data_t epsilon = atof(argv[4]);

	// List the user parameters
	cout << "\nArguments: " << endl;
	cout << "Dataset's filename: " << argv[1] << endl;
	cout << "minRow: " << minRow << endl;
	cout << "minCol: " << minCol << endl;
	cout << "Epsilon: " << epsilon << endl;
	cout << "File with the list of bicluster: " << argv[5] << endl;

	dataset_t matrix; // pointer to the dataset
	row_t n; // number of dataset's rows
	col_t m; // number of dataset's columns
	if (!readDataset(argv[1], matrix, n, m))
	{
		cout << "\nDataset was not loaded!";
		exit(1);
	}
	printf("\nDataset loaded: %dx%d\n\n", n, m);
	//printData(matrix, n, m);
	
	float tempo;
	openPrintFile(argv[5]);
	cout << "\nRunning..." << endl;
	tempo = buildLattice(matrix, n, m, minRow, minCol, epsilon);
	closePrintFile();

	cout << "\n\nRuntime(s): " << tempo << endl;
	cout << "Number of biclusters = " << g_cont << endl;
	cout << "Memory RAM usage = " << g_ram << endl;

	// Guarda qtd de bics e runtime num txt
	// Tambem guarda ram maxima usada (em kbytes) para fins experimentais
	ofstream myfile;
	myfile.open("summary.txt", ofstream::app);
	myfile << g_cont << '\t';
	myfile << tempo << '\t';
	myfile << g_ram << endl;
	myfile.close();
	
	return 0;
}

bool readDataset(const string &dataSetName, dataset_t &matrix, row_t &n, col_t &m)
{
	ifstream myStream;
	myStream.open(dataSetName, ifstream::in);

	if (!myStream.is_open())
		return false;

	//Discovering the number of rows
	n = count(istreambuf_iterator<char>(myStream), istreambuf_iterator<char>(), '\n');

	//Discovering the number of columns
	data_t dbltmp;
	string line;
	m = 0;
	myStream.seekg(0);
	getline(myStream, line);
	stringstream stream(line);
	while (stream.good())
	{
		stream >> dbltmp;
		++m;
	}

	//Allocating memory
	matrix = new data_t*[n];
	for (row_t i = 0; i < n; ++i)
		matrix[i] = new data_t[m];

	//Storing the data
	myStream.seekg(0);
	for (row_t i = 0; i < n; ++i)
	{
		for (col_t j = 0; j < m; ++j)
			myStream >> matrix[i][j];
	}

	myStream.close();
	return true;
}

void printData(const dataset_t &matrix, const row_t &n, const col_t &m)
{
	for (row_t i = 0; i < n; ++i)
	{
		for (col_t j = 0; j < m; ++j)
			cout << matrix[i][j] << '\t';
		cout << endl;
	}
}

bool readConfigFile()
{
	unordered_map<string,string> params;
	ifstream myStream;
	string line;

	myStream.open("config.txt", ifstream::in);

	if (!myStream.is_open())
		return false;

	myStream.seekg(0);
	while (myStream.good())
	{
		getline(myStream, line);
		size_t found = line.find("=");
		if (found!=string::npos)
		{
			params[line.substr(0,found)] = line.substr(found+1);
		}
	}

	cout << "myparams contains:" << endl;
	for (auto& x: params)
		cout << x.first << ": " << x.second << endl;
	
	// Setando as variaveis com os parametros
	MVS = stoi(params["MVS"]);
	if (params["OUT"].compare("matlab") == 0)
		g_output = 1;
	else
		g_output = 2;
	
	myStream.close();
	return true;
}

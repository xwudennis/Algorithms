#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

void readFile(char * fileName, int ** nArray, int * nArraySize)
{
	ifstream infile(fileName);
	string line;
	vector<int> nVector;
	int i = 0;
	while (getline(infile, line))
	{
		istringstream iss(line);
		int n;
		iss >> n;
		nVector.push_back(n);
		i++;
	}
	*nArray = new int [nVector.size()];
	*nArraySize = nVector.size();
	copy(nVector.begin(), nVector.end(), *nArray);
}

// Using the first (left) element as the pivot element for quick sort
void partionLP(int * A, int l, int r, unsigned long long * comparisonLP)
{
	// A is the input array. l is the left boundary; r is the right boundary.
	if(r - l >= 1)
	{
		int p = A[l]; // Let p be the first element in the array
		int i = l + 1;
		for(int j = l + 1; j <= r; j++)
		{
			if (A[j] < p)
			{
				int a = A[j];
				A[j] = A[i];
				A[i] = a;
				i++;
			}
		}
		int b = A[l];
		A[l] = A[i-1];
		A[i-1] = b;
		*comparisonLP += r - l;

		partionLP(A, l, i-2, comparisonLP);
		partionLP(A, i, r, comparisonLP);
	}
}

// Using the last (right) element as the pivot element for quick sort
void partionRP(int * A, int l, int r, unsigned long long * comparisonRP)
{
	// A is the input array. l is the left boundary; r is the right boundary.
	if(r - l >= 1)
	{
		int p = A[r]; // Let p be the last element in the array. Exchange A[r] and A[l]
		A[r] = A[l];
		A[l] = p;
		int i = l + 1;
		for(int j = l + 1; j <= r; j++)
		{
			if (A[j] < p)
			{
				int a = A[j];
				A[j] = A[i];
				A[i] = a;
				i++;
			}
		}
		int b = A[l];
		A[l] = A[i-1];
		A[i-1] = b;
		*comparisonRP += r - l;

		partionRP(A, l, i-2, comparisonRP);
		partionRP(A, i, r, comparisonRP);
	}
}

void moveMedianToFirst(int * A, int l, int r)
{
	int *first = A + l;
	int *last = A + r;
	int *middle = A + (l + r)/2;
	int median;
	
	if(r - l >=2)
	{
		if((*middle > *first && *middle < *last) || (*middle > *last & *middle < *first))
		{
			// middle is the median, exchange middle with first
			median = *middle;
			*middle = *first;
			*first = median;
		}
		else if((*first > *middle && *first < *last) || (*first > *last & *first < *middle))
		{
			// first is the median
			// do nothing
		}
		else if ((*last > *first && *last < *middle) || (*last > *middle && *last < *first))
		{
			// last is the median, exchange last with first
			median = *last;
			*last = *first;
			*first = median;
		}
	}
	else if(r - l == 1)
	{
		// Only two numbers in the array
		// Do nothing
	}
}

// Using the middle element as the pivot element for quick sort
void partionMP(int * A, int l, int r, unsigned long long * comparisonMP)
{
	// A is the input array. l is the left boundary; r is the right boundary.
	if(r - l >= 1)
	{
		moveMedianToFirst(A, l, r); // Exhange the median to the first
		int p = A[l]; // Let p be the first element in the array
		int i = l + 1;
		for(int j = l + 1; j <= r; j++)
		{
			if (A[j] < p)
			{
				int a = A[j];
				A[j] = A[i];
				A[i] = a;
				i++;
			}
		}
		int b = A[l];
		A[l] = A[i-1];
		A[i-1] = b;
		*comparisonMP += r - l;

		partionMP(A, l, i-2, comparisonMP);
		partionMP(A, i, r, comparisonMP);
	}
}

void outputArray(int * nArray, int nArraySize, unsigned long long comparisonLP, 
	unsigned long long comparisonRP, unsigned long long comparisonMP)
{
	ofstream fout("output.txt");
	if(!fout || !fout.good())
	{
		cout<<"ERROR! Unable to open .txt file for writing!\n";
	}
	else
	{
		fout << "comparisonLP " << comparisonLP << endl << endl;
		fout << "comparisonRP " << comparisonRP << endl << endl;
		fout << "comparisonMP " << comparisonMP << endl << endl;
		for(int i = 0; i < nArraySize; i++)
		{
			fout << nArray[i] << endl;
		}
	}
}

int main()
{
    int * nArray;
    int nArraySize;
	clock_t t;
	unsigned long long comparisonLP = 0;
	unsigned long long comparisonRP = 0;
	unsigned long long comparisonMP = 0;

    // 1. Read the file
    readFile("QuickSort.txt", &nArray, &nArraySize);

	// 2. Quick Sort
	// Uncomment only one of partionLP, partionRP and partionMP to do quick sort and count the comparisons made for quick sort.
	//partionLP(nArray, 0, nArraySize - 1, &comparisonLP);
	partionRP(nArray, 0, nArraySize - 1, &comparisonRP);
	//partionMP(nArray, 0, nArraySize - 1, &comparisonMP);

	// 3. Output Comparisons
	outputArray(nArray, nArraySize, comparisonLP, comparisonRP, comparisonMP);

    return 0;
}

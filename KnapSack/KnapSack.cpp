#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>

using namespace std;

class Knapsack
{
public:
	int W;				// W is the size of knapsack
	int n;				// n is the number of items
	double * values;
	int * weights;
	double ** A;
public:
	Knapsack() {};
	~Knapsack() {};

	void readFile(char * fileName)
	{
		// Initialize knapsackSize, numberOfItems, values, and weights
		ifstream infile(fileName);
		string line;
		getline(infile, line);
		istringstream iss(line);
		iss >> W >> n;
		values = new double[n+1];
		weights = new int[n+1];
		
		// Read items
		values[0] = 0;
		weights[0] = 0;
		for(int i = 1; i < n+1; i++)
		{
			getline(infile, line);
			istringstream iss1(line);
			iss1 >> values[i] >> weights[i];
		}

		// Initialize A
		A = new double * [n+1];
		for(int i = 0; i < n+1; i++)
		{
			double * B = new double [W+1];
			A[i] = B;
		}
		for(int x = 0; x < W+1; x++)
		{
			A[0][x] = 0;
		}
	}

	double getAix(int i, int x)
	{
		double a = A[i-1][x];
		double b;
		if(x - weights[i] >= 0)
		{
			b = A[i-1][x-weights[i]] + values[i];
		}
		else
		{
			b = 0;
		}
		if(a > b)
		{
			return a;
		}
		// Else
		return b;
	}

	double solve()
	{
		for(int i = 1; i < n+1; i++)
		{
			for(int x = 0; x < W+1; x++)
			{
				A[i][x] = getAix(i, x);
			}
		}
		return A[n][W];
	}
};

int main()
{
	Knapsack knapsack = Knapsack();
	knapsack.readFile("knapsack1.txt");
	double result = knapsack.solve();
	cout.precision(15);
	cout << result << endl;
}

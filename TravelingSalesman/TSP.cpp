#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <bitset> 

#define DIGIT 24
#define INF 999999;

using namespace std;

class TSP
{
public:
	int NofCities;
	int NofS;
	double ** Coordinates;
	double ** Distances;
	float ** A;
	
private:
	int getTheLastS()
	{
		bitset<DIGIT> S;
		for(int i = 0; i < DIGIT; i++)
		{
			S[i] = 1;
		}
		return S.to_ulong();
	}

	void MovePi(int * p, int i, int m)
	{
		if(m == 1)
		{
			// when m == 1, i can only be 0
			if(p[0] < DIGIT - m)
			{
				p[0]++;
			}
		}
		else if(m == 2)
		{
			// when m == 2, i can be either 0 or 1
			if(i == 0)
			{
				if(p[1] < DIGIT - 1)
				{
					// Do nothing
				}
				else
				{
					p[i]++;
				}
			}
			else
			{
				if(p[i] < DIGIT - 1)
				{
					p[i]++;
				}
				else
				{
					p[i] = p[i-1] + 1;
				}
			}
		}
		else
		{
			// when m is strictly greater than 2
			if(i == 0)
			{
				if(p[1] < DIGIT - m + 1)
				{
					// Do nothing
				}
				else
				{
					p[i]++;
				}
			}
			else if(i == m - 1)
			{
				if(p[i] < DIGIT - 1)
				{
					p[i]++;
				}
				else
				{
					p[i] = p[i-1] + 1;
				}
			}
			else
			{
				if(p[i + 1] < DIGIT - m + i + 1)
				{
					// Do nothing
				}
				else
				{
					if(p[i] < DIGIT - m + i)
					{
						p[i]++;
					}
					else
					{
						p[i] = p[i-1] + 1;
					}
				}
			}
		}
	}

	void processS(int * p, int m)
	{
		bitset<DIGIT> S;
		for(int i = 0; i < m; i++)
		{
			S[p[i]] = 1;
		}
		int SI = S.to_ulong();
		
		for(int i = 0; i < m; i++)
		{
			int j = p[i] + 2;
			// Create a new bitset SmJ
			// Copy S to SmJ
			// Set SmJ[p[i]] = 0
			bitset<DIGIT> SmJ;
			for(int l = 0; l < DIGIT; l++)
			{
				SmJ[l] = S[l];
			}
			SmJ[p[i]] = 0;
			int SmJI = SmJ.to_ulong();

			A[SI][j] = INF;

			if(m == 1)
			{
				float d = A[SmJI][1] + Distances[1][j];
				if(d < A[SI][j])
				{
					A[SI][j] = d;
				}
			}
			else if(m > 1)
			{
				for(int i1 = 0; i1 < m; i1++)
				{
					int k = p[i1] + 2;
					if(k != j)
					{
						float d = A[SmJI][k] + Distances[k][j];
						if(d < A[SI][j])
						{
							A[SI][j] = d;
						}
					}
				}
			}
		}
	}

	void processM(int m)
	{
		if(m == 0)
		{
			// Do nothing
		}
		else if(m <= DIGIT)
		{
			// Initialize p, offsets to bitset
			int * p = new int[m];
			for(int i = 0; i < m; i++)
			{
				p[i] = i;
			}

			processS(p, m);

			while(p[0] < DIGIT - m)
			{
				for(int i = 0; i < m; i++)
				{
					MovePi(p, i, m);
				}
				processS(p, m);
			}
		}
	}

public:
	TSP() {}
	~TSP() {}
	
	void readFile(char * fileName)
	{
		// Get NofCities and NofS. Initialize coordinates.
		ifstream infile(fileName);
		string line;
		getline(infile, line);
		istringstream iss(line);
		iss >> NofCities;
		NofS = int(pow(2.0, NofCities - 1.0) + 0.5);
		// The total number of S is 2^(NofCities - 1). As the city 1 is always
		// in S. Round to the nearest int.

		Coordinates = new double * [NofCities + 1];
		// Set a virtual city C1. The x and y coordinates of c1 are both INF.
		Coordinates[0] = new double[2];
		Coordinates[0][0] = INF;
		Coordinates[0][1] = INF;
		for(int i = 1; i <= NofCities; i++)
		{
			if(getline(infile, line))
			{
				istringstream iss1(line);
				Coordinates[i] = new double[2];
				iss1 >> Coordinates[i][0] >> Coordinates[i][1];
			}
		}
	}

	void getDistances()
	{
		// Initialize Distances
		Distances = new double * [NofCities + 1];
		for(int i = 0; i <= NofCities; i++)
		{
			Distances[i] = new double [NofCities + 1];
		}

		for(int i = 0; i <= NofCities; i++)
		{
			for(int j = i; j <= NofCities; j++)
			{
				double deltaX = Coordinates[i][0] - Coordinates[j][0];
				double deltaY = Coordinates[i][1] - Coordinates[j][1];
				double distance = sqrt(pow(deltaX, 2.0) + pow(deltaY, 2.0));
				Distances[i][j] = distance;
				Distances[j][i] = distance;
			}
		}
	}

	void getA()
	{
		A = new float * [NofS];
		for(int i = 0; i < NofS; i++)
		{
			A[i] = new float [NofCities + 1];
			A[i][0] = INF;
			A[i][1] = INF;
		}
		A[0][1] = 0; // Only City 1 in S

		for(int m = 1; m <= NofCities - 1; m++)
		{
			processM(m);
		}
	}

	float returnMin()
	{
		float MinDistance = INF;
		int SI = getTheLastS();
		for(int j = 2; j <= NofCities; j++)
		{
			float d = A[SI][j] + Distances[j][1];
			if(d < MinDistance)
			{
				MinDistance = d;
			}
		}
		return MinDistance;
	}
};

void process(char * fileName)
{
	// Generate a new TSP problem
	TSP tsp;
	tsp.readFile(fileName);
	tsp.getDistances();
	tsp.getA();
	cout << tsp.returnMin() << endl;
}

int main()
{
	process("tsp.txt");
	cout << "Press RETURN to continue...";
	cin.get();
	return 0;
}

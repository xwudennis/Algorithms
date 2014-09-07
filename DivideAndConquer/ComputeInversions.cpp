#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

using namespace std;

void readFile(char * fileName, int ** inputA, int * inputASize)
{
	ifstream infile(fileName);
	string line;
	vector<int> nVector;
	int i = 0;
	while (getline(infile, line)
	{
        istringstream iss(line);
        int n;
        iss >> n;
        nVector.push_back(n);
		i++;
	}
    *inputA = new int [nVector.size()];
    *inputASize = nVector.size();
    copy(nVector.begin(), nVector.end(), *inputA);
}

void output(char * fileName, unsigned long long inverseCount, int * sortedA, int sortedASize)
{
	ofstream fout(fileName);
	if(!fout || !fout.good())
	{
		cout<<"ERROR! Unable to open .txt file for writing!\n";
	}
	else
	{
		fout << "Inverse:" << endl << inverseCount << endl << endl;
		for(int i = 0; i < sortedASize; i++)
		{
			fout << sortedA[i] << endl;
		}
	}
}

void merge(int * leftA, int leftASize, int * rightA, int rightASize, unsigned long long * inverseCount)
{
	// Left Array and Right Array are continous
    int mergedASize = leftASize + rightASize;
    int * mergedA = (int*)malloc(mergedASize*sizeof(int));
    int j = 0;
    int k = 0;
    for(int i = 0; i < mergedASize; i++)
    {
		if(j < leftASize && k < rightASize)
		{
			if(leftA[j] <= rightA[k])
			{
				mergedA[i] = leftA[j];
				j++;
			}
			else
			{
				mergedA[i] = rightA[k];
				k++;
				*inverseCount += leftASize - j;
			}
		}
		else if(j == leftASize)
		{
			mergedA[i] = rightA[k];
			k++;
			//*inverseCount += leftASize - j; is not required as leftASize == j
		}
		else if(k == rightASize)
		{
			mergedA[i] = leftA[j];
			j++;
		}
    }

	// Copy mergedA to the original inputA (same pointer as leftA)
	copy(mergedA, mergedA + mergedASize, leftA);
	free(mergedA);
}

void dvdNCq (int * inputA, int inputASize, unsigned long long * inverseCount)
{
    if(inputASize > 1)
    {
	    int * leftA = inputA;
        int leftASize = inputASize - inputASize/2;
		int * rightA = inputA + leftASize;
        int rightASize = inputASize/2;
        dvdNCq(leftA, leftASize, inverseCount);
        dvdNCq(rightA, rightASize, inverseCount);
        merge(leftA, leftASize, rightA, rightASize, inverseCount);
    }
    else
    {
		// Do nothing
    }
}

int main()
{
    int * inputA;
	int inputASize = 0;
	unsigned long long inverseCount = 0;

    // 1. Read the file
	readFile("IntegerArray.txt", &inputA, &inputASize);
	cout << "All numbers inputted" << endl;

    // 2. Divide and Conquer
	clock_t t = clock();
    dvdNCq (inputA, inputASize, &inverseCount);
	t = clock() - t;
	cout << "It took me " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;
	cout << "All numbers sorted" << endl;
	
	// 3. Output sorted Array
	output("output.txt", inverseCount, inputA, inputASize);

    return 0;
}

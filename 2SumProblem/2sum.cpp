#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

void readFile(char *fileName, vector<long long> * integerList)
{
	ifstream infile(fileName);
	string line;
	while(getline(infile, line))
	{
		istringstream iss(line);
		long long n;
		iss >> n;
		integerList->push_back(n);
	}
}

void myBinarySearch(long long * arrayBegin, long long * arrayEnd, long long val, long long ** pre, long long ** suc)
{
	if(*arrayBegin <= val && *arrayEnd >= val)
	{
		int length = arrayEnd - arrayBegin;
		if(length <= 1)
		{
			if(*arrayBegin == val)
			{
				*pre = arrayBegin;
				*suc = arrayBegin;
			}
			else if(*arrayEnd == val)
			{
				*pre = arrayEnd;
				*suc = arrayEnd;
			}
			else
			{
				*pre = arrayBegin;
				*suc = arrayEnd;
			}
		}
		else
		{
			if(arrayBegin[length/2]==val)
			{
				*pre = &arrayBegin[length/2];
				*suc = &arrayBegin[length/2];
			}
			else if(arrayBegin[length/2] > val)
			{
				myBinarySearch(arrayBegin, &arrayBegin[length/2], val, pre, suc);
			}
			else
			{
				myBinarySearch(&arrayBegin[length/2], arrayEnd, val, pre, suc);
			}
		}
	}
	else
	{
		cout << "Error!!! Binary Search Out of Range " << val << endl;
	}
}

void statisticLoop(long long * arrayBegin, long long * arrayEnd, int lowerBond, int upperBond, vector<int> * outputList)
{
	int length = arrayEnd - arrayBegin;
	for(int i = 0; i <= length; i++)
	{
		long long x = arrayBegin[i];
		long long lB = max(lowerBond - x, *arrayBegin);
		long long uB = min(upperBond - x, *arrayEnd);
		long long * lBPre;
		long long * lBSuc;
		long long * uBPre;
		long long * uBSuc;

		if(uB >= lB)
		{
			myBinarySearch(arrayBegin, arrayEnd, lB, &lBPre, &lBSuc);
			myBinarySearch(arrayBegin, arrayEnd, uB, &uBPre, &uBSuc);
			if(*lBSuc <= uB)
			{
				int l = uBPre - lBSuc;
				for(int i = 0; i <= l; i++)
				{
					long long y = lBSuc[i];
					int t = x + y;
					if(x!=y)
					{
						outputList->push_back(t);
					}
				}
				cout << "There are matched ys for " << i << endl;
			}
			else
			{
				//cout << "No matched y for: " << i << endl;
			}
		}
		else
		{
			//cout << "No matched y for: " << i << endl;
		}
	}
}

int main()
{
	long long integerArray[1000000];
	vector<long long> integerList;
	vector<int> outputList;
	int lowerBond = -10000;
	int upperBond = 10000;

	// 1. Read File
	readFile("2sum.txt", &integerList);
	
	// 2. Sort
	sort(integerList.begin(), integerList.end());
	copy(integerList.begin(), integerList.end(), integerArray);
	cout << "Sorted" << endl;

	// 3. Statistic Loop
	statisticLoop(&integerArray[0], &integerArray[999999], lowerBond, upperBond, &outputList);
	cout << "Statistic Loop Completed" << endl;

	// 4. Generate Results
	sort(outputList.begin(), outputList.end());
	vector<int> outputList_distinct;
	outputList_distinct.push_back(outputList[0]);
	for(int i = 1; i < outputList.size(); i++)
	{
		if(outputList[i] > outputList[i-1])
		{
			outputList_distinct.push_back(outputList[i]);
		}
	}

	cout << "Results:" << endl << outputList_distinct.size() << endl;
}

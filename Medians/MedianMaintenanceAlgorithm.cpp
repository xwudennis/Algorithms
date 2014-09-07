#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#define MAXLIMIT 999999
#define MINLIMIT -999999

void readFile(char *fileName, vector<int> * inputArray)
{
	ifstream infile(fileName);
	string line;
	while(getline(infile, line))
	{
		istringstream iss(line);
		int n;
		iss >> n;
		inputArray->push_back(n);
	}
}

class HLow
{
	// HLow. Maintain Max at the top
public:
	int * HeapPointer;
	int CurrentSize;
public:
	HLow() {}
	void Insert(int N);
	void BubbleUp(int childI);
	int ExtractMax();
	void BubbleDown(int parentI);
	~HLow() {}
};

void HLow::Insert(int N)
{
	HeapPointer[CurrentSize] = N;
	BubbleUp(CurrentSize);
	CurrentSize++;
}

void HLow::BubbleUp(int childI)
{
	if(childI == 0)
	{
	}
	else
	{
		int parentI = (childI + 1)/2 - 1;
		if(HeapPointer[parentI] < HeapPointer[childI])
		{
			int temp = HeapPointer[childI];
			HeapPointer[childI] = HeapPointer[parentI];
			HeapPointer[parentI] = temp;
			BubbleUp(parentI);
		}
	}
}

int HLow::ExtractMax()
{
	if(CurrentSize == 0)
	{
		cout << "Error!! Empty Heap" << endl;
		return 0;
	}
	else
	{
		int extracted = HeapPointer[0];
		HeapPointer[0] = HeapPointer[CurrentSize - 1];
		HeapPointer[CurrentSize - 1] = 0;
		CurrentSize--;
		BubbleDown(0);
		return extracted;
	}
}

void HLow::BubbleDown(int parentI)
{
	int child1I = 2 * parentI + 1;
	int child2I = 2 * parentI + 2;

	if(child2I <= CurrentSize - 1)
	{
		if(HeapPointer[child1I] >= HeapPointer[child2I])
		{
			if(HeapPointer[parentI] < HeapPointer[child1I])
			{
				int temp = HeapPointer[parentI];
				HeapPointer[parentI] = HeapPointer[child1I];
				HeapPointer[child1I] = temp;
				BubbleDown(child1I);
			}
		}
		else
		{
			if(HeapPointer[parentI] < HeapPointer[child2I])
			{
				int temp = HeapPointer[parentI];
				HeapPointer[parentI] = HeapPointer[child2I];
				HeapPointer[child2I] = temp;
				BubbleDown(child2I);
			}
		}
	}
	else if(child1I = CurrentSize - 1)
	{
		if(HeapPointer[parentI] < HeapPointer[child1I])
		{
			int temp = HeapPointer[parentI];
			HeapPointer[parentI] = HeapPointer[child1I];
			HeapPointer[child1I] = temp;
			// Reach the bottom. No more recursive calls.
		}
	}
	else
	{
		// Reach the bottom. No more recursive calls.
	}
}

class HHigh
{
	// HHigh. Maintain Min at the top
public:
	int * HeapPointer;
	int CurrentSize;
public:
	HHigh() {}
	void Insert(int N);
	void BubbleUp(int childI);
	int ExtractMin();
	void BubbleDown(int parentI);
	~HHigh() {}
};

void HHigh::Insert(int N)
{
	HeapPointer[CurrentSize] = N;
	BubbleUp(CurrentSize);
	CurrentSize++;
}

void HHigh::BubbleUp(int childI)
{
	if(childI == 0)
	{
	}
	else
	{
		int parentI = (childI + 1)/2 - 1;
		if(HeapPointer[parentI] > HeapPointer[childI])
		{
			int temp = HeapPointer[childI];
			HeapPointer[childI] = HeapPointer[parentI];
			HeapPointer[parentI] = temp;
			BubbleUp(parentI);
		}
	}
}

int HHigh::ExtractMin()
{
	if(CurrentSize == 0)
	{
		cout << "Error!! Empty Heap" << endl;
		return 0;
	}
	else
	{
		int extracted = HeapPointer[0];
		HeapPointer[0] = HeapPointer[CurrentSize - 1];
		HeapPointer[CurrentSize - 1] = 0;
		CurrentSize--;
		BubbleDown(0);
		return extracted;
	}
}

void HHigh::BubbleDown(int parentI)
{
	int child1I = 2 * parentI + 1;
	int child2I = 2 * parentI + 2;

	if(child2I <= CurrentSize - 1)
	{
		if(HeapPointer[child1I] <= HeapPointer[child2I])
		{
			if(HeapPointer[parentI] > HeapPointer[child1I])
			{
				int temp = HeapPointer[parentI];
				HeapPointer[parentI] = HeapPointer[child1I];
				HeapPointer[child1I] = temp;
				BubbleDown(child1I);
			}
		}
		else
		{
			if(HeapPointer[parentI] > HeapPointer[child2I])
			{
				int temp = HeapPointer[parentI];
				HeapPointer[parentI] = HeapPointer[child2I];
				HeapPointer[child2I] = temp;
				BubbleDown(child2I);
			}
		}
	}
	else if(child1I = CurrentSize - 1)
	{
		if(HeapPointer[parentI] > HeapPointer[child1I])
		{
			int temp = HeapPointer[parentI];
			HeapPointer[parentI] = HeapPointer[child1I];
			HeapPointer[child1I] = temp;
			// Reach the bottom. No more recursive calls.
		}
	}
	else
	{
		// Reach the bottom. No more recursive calls.
	}
}

void checkHeapsBalance(HLow * hLow, HHigh * hHigh)
{
	if(hLow->CurrentSize >= hHigh->CurrentSize + 2)
	{
		int n = hLow->ExtractMax();
		hHigh->Insert(n);
	}
	else if(hHigh->CurrentSize >= hLow->CurrentSize + 1)
	{
		int n = hHigh->ExtractMin();
		hLow->Insert(n);
	}
}

long long feedLoop(HLow * hLow, HHigh * hHigh, vector<int> inputArray)
{
	// Initialize the roots of hLow and hHigh
	hLow->HeapPointer[0] = MINLIMIT;
	hHigh->HeapPointer[0] = MAXLIMIT;
	long long sum = 0;

	for(int i = 0; i < inputArray.size(); i++)
	{
		int hLowMax = hLow->HeapPointer[0];
		int hHighMin = hHigh->HeapPointer[0];
		int newNumber = inputArray[i];
		if(newNumber < hLowMax)
		{
			hLow->Insert(newNumber);
		}
		else if(newNumber > hHighMin)
		{
			hHigh->Insert(newNumber);
		}
		else
		{
			if(hLow->CurrentSize <= hHigh->CurrentSize)
			{
				hLow->Insert(newNumber);
			}
			else
			{
				hHigh->Insert(newNumber);
			}
		}
		checkHeapsBalance(hLow, hHigh);
		sum += hLow->HeapPointer[0];
		cout << i << " added to the heaps" << endl;
	}
	return sum;
}

int main()
{
	vector<int> inputArray;
	int inputSize;
	HLow hLow;
	hLow.CurrentSize = 0;
	HHigh hHigh;
	hHigh.CurrentSize = 0;

	// 1. Read File and Initialize
	readFile("median.txt", &inputArray);
	inputSize = inputArray.size();
	hLow.HeapPointer = (int*)malloc((inputSize/2 + 1)*sizeof(int));
	hHigh.HeapPointer = (int*)malloc((inputSize/2 + 1)*sizeof(int));

	// 2. Feed InputArray to Heaps
	long long sum = feedLoop(&hLow, &hHigh, inputArray);

	// 3. Output
	cout << "Sum of medians " << sum << endl;
}

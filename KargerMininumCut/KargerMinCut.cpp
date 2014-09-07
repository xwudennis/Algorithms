#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>

#define VERSEMIN 999999;

using namespace std;

class Node
{
public:
	int nodeNumber;
	int nodeIndex;
	vector<int> connectedEdges;
public:
	Node (int NodeNumber, int NodeIndex)
	{
		nodeNumber = NodeNumber;
		nodeIndex = NodeIndex;
	}
	~Node (){}
};

class Edge
{
public:
	int edgeIndex;
	int n1N;	// Node1 Number
	int n1I;	// Node1 Index
	int n2N;	// Node2 Number
	int n2I;	// Node2 Index
public:
	Edge (int EdgeIndex, int Node1Number, int Node1Index, int Node2Number, int Node2Index)
	{
		edgeIndex = EdgeIndex;
		n1N = Node1Number;
		n1I = Node1Index;
		n2N = Node2Number;
		n2I = Node2Index;
	}
	~Edge (){}
};

class Graph
{
public:
	vector<Node> nodeList;
	vector<Edge> edgeList;
	vector<int> remainingEdges;
public:
	Graph() {}
	~Graph() {}

	void readFile(char * fileName)
	{
		int nodeIndex = 0;
		int edgeIndex = 0;

		// Initialize nodeList, edgeList and NofEdges
		ifstream infile(fileName);
		string line;
		while(getline(infile, line))
		{
			istringstream iss(line);
			int nodeNumber;
			iss >> nodeNumber;
			Node newNode(nodeNumber, nodeIndex);
			nodeList.push_back(newNode);

			int nodeNumber2;
			while(iss >> nodeNumber2)
			{
				if(nodeNumber2 > nodeNumber)
				// Force nodeNumber2 to be grater than nodeNumber. Avoid repeatition.
				{
					Edge newEdge(edgeIndex, nodeNumber, nodeIndex, nodeNumber2, nodeNumber2 - 1);
					// Node2Index equals to nodeNumber2 - 1 in the file kargerMinCut
					edgeList.push_back(newEdge);
					remainingEdges.push_back(edgeIndex);
					edgeIndex++;
				}
			}
			nodeIndex++;
		}

		// Generate connectedEdges for each node
		for(unsigned int i = 0; i < edgeList.size(); i++)
		{
			int n1I = edgeList[i].n1I;
			int n2I = edgeList[i].n2I;
			nodeList[n1I].connectedEdges.push_back(i);
			nodeList[n2I].connectedEdges.push_back(i);
		}
	}

	void outputList()
	{
		ofstream fout("output.txt");
		if(!fout || !fout.good())
		{
			cout<<"ERROR! Unable to open .txt file for writing!\n";
		}
		for(unsigned int i = 0; i < nodeList.size(); i++)
		{
			fout << nodeList[i].nodeNumber << "\t";
			for(unsigned int j = 0; j < nodeList[i].connectedEdges.size(); j++)
			{
				int edgeI = nodeList[i].connectedEdges[j];
				if(edgeList[edgeI].n1I == i)
				{
					fout << edgeList[edgeI].n2N << "\t";
				}
				else if(edgeList[edgeI].n2I == i)
				{
					fout << edgeList[edgeI].n1N << "\t";
				}
				else
				{
					fout << "Error!! Check the program" << "\t";
				}
			}
			fout << endl;
		}
	}

	void mergeEdge(int mergedEdge)
	{
		// It actually doesn't matter whether node 1 is merged to node 2, or 
		// node 2 is merged to node 1. Here, we simply assume that node 2 is 
		// merged to node 1.
		int rmnNode = edgeList[mergedEdge].n1I;
		int mgdNode = edgeList[mergedEdge].n2I;
		for(unsigned int i = 0; i < nodeList[mgdNode].connectedEdges.size(); i++)
		{
			// Replace mgdNode with rmnNode
			int edgei = nodeList[mgdNode].connectedEdges[i];
			if (edgeList[edgei].n1I == mgdNode)
			{
				edgeList[edgei].n1I = rmnNode;
				edgeList[edgei].n1N = rmnNode + 1;
			}
			else if (edgeList[edgei].n2I == mgdNode)
			{
				edgeList[edgei].n2I = rmnNode;
				edgeList[edgei].n2N = rmnNode + 1;
			}
			else
			{
				cout << "Error! Check program" << endl;
			}

			// Add all connectedEdges of mgdNode to rmnNode
			nodeList[rmnNode].connectedEdges.push_back(nodeList[mgdNode].connectedEdges[i]);
		}
		removeSelfLoop(rmnNode);
		clearRemainingEdges();
		nodeList[mgdNode].nodeIndex = -1;
		nodeList[mgdNode].nodeNumber = -1;
		nodeList[mgdNode].connectedEdges.clear();
	}

	void removeSelfLoop(int rmnNode)
	{
		// Repeated entry is also a selfloop
		vector<int> removed;
		for(unsigned int i = 0; i < nodeList[rmnNode].connectedEdges.size(); i++)
		{
			int edgei = nodeList[rmnNode].connectedEdges[i];
			if(edgeList[edgei].n1I == edgeList[edgei].n2I)
			{
				removed.push_back(i);
			}
		}
		//sort(removed.begin(), removed.end());
		for(int i = removed.size() - 1; i >=0; i--)
		{
			nodeList[rmnNode].connectedEdges.erase(nodeList[rmnNode].connectedEdges.begin() + removed[i]);
		}
	}

	void clearRemainingEdges()
	{
		vector<int> removed;
		for(unsigned int i = 0; i < remainingEdges.size(); i++)
		{
			int edgei = remainingEdges[i];
			if(edgeList[edgei].n1I == edgeList[edgei].n2I)
			{
				removed.push_back(i);
			}
		}
		for(int i = removed.size() - 1; i >= 0; i--)
		{
			remainingEdges.erase(remainingEdges.begin() + removed[i]);
		}
	}

	int kargerMinCut()
	{
		int NofEdges = remainingEdges.size();
		for(int j = 0; j < nodeList.size() - 2; j++)
		{
			int i = rand() % remainingEdges.size();
			int mergedEdge = remainingEdges[i];
			mergeEdge(mergedEdge);	
		}
		NofEdges = remainingEdges.size();
		return NofEdges;
	}

	void copyFromAnExistingGraph(Graph exstGraph)
	{
		nodeList = exstGraph.nodeList;
		edgeList = exstGraph.edgeList;
		remainingEdges = exstGraph.remainingEdges;
	}
};

void statisticLoop(Graph backupGraph)
{
	int min = VERSEMIN;
	for(int i = 0; i < 100; i++)
	{
		Graph newGraph;
		newGraph.copyFromAnExistingGraph(backupGraph);
		int NofEdges = newGraph.kargerMinCut();
		if(NofEdges < min)
		{
			min = NofEdges;
		}
		cout << "Current Min: " << min << endl;
	}
}

int main()
{
	// Set the seed
	srand (3549841);
	// Build a new graph
	Graph backupGraph;
	backupGraph.readFile("kargerMinCut.txt");
	// Output newGraph for debugging
	backupGraph.outputList();
	// Main Loop of karger Minimum Cut
	statisticLoop(backupGraph);
    return 0;
}

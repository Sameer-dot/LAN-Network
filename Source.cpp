#include<iostream>
#include<string>
#include<list>
#include<vector>
#include<fstream>

using namespace std;

//-------------------Edge Class-------------------

class Edge {
	int destination;
	int Eweight;
public:
	friend class Graph;
	Edge() {
		destination = 0;
		Eweight = 0;
	}

	Edge(int d, int Ew) {
		destination = d;
		Eweight = Ew;
	}

	Edge(const Edge& obj) {
		destination = obj.destination;
		Eweight = obj.Eweight;
	}

	~Edge() {
		destination = 0;
		Eweight = 0;
	}
};


//----------------GraphNode Class-----------------

class GraphNode {
	int iD;
	string type;
	int weight;
	list<Edge> edgelist;
public:
	friend class Graph;
	template<class type>friend class Heap;
	GraphNode() {
		iD = 0;
		type = "";
		weight = INT_MAX;
	}

	GraphNode(int i, string t, int w = INT_MAX) {
		iD = i;
		type = t;
		weight = w;
	}

	GraphNode(const GraphNode& obj) {
		iD = obj.iD;
		type = obj.type;
		weight = obj.weight;
		edgelist = obj.edgelist;
	}

	~GraphNode() {
		iD = 0;
		type = "";
		weight = 0;
		edgelist.clear();
	}
};


//-------------------Heap Class-------------------

template<class type>
class Heap {
private:
	int currentSize;
	int capacity;
	vector<type> data;
	int* position;

public:
	friend class Graph;

	//-----Default Constructor-----
	Heap() {
		currentSize = 0;
	}

	//-----Parametrized Constructor-----
	Heap(int c) {
		capacity = c;
		position = new int[c - 1];
	}

	//-----Insert Function-----
	void insert(type d) {
		data.push_back(d);
		currentSize++;
	}

	//Function to Check if the heap
	//is empty or not
	bool isEmpty()const {
		if (currentSize == 0) {
			return true;
		}
		else return false;
	}

	//--------------Min Heapifying--------------- 
	void heapify(vector<type>& data, int n, int i) {
		int smallest = i;
		int l = 2 * i + 1;
		int r = 2 * i + 2;

		if (l < n && data[l].weight < data[smallest].weight) {
			smallest = l;
		}

		if (r < n && data[r].weight < data[smallest].weight) {
			smallest = r;
		}

		if (smallest != i) {

			//the two vertices that are going
			//to swap updating their locations
			//in positions array too...
			position[data[smallest].iD] = i;
			position[data[i].iD] = smallest;

			swap(data[i], data[smallest]);
			heapify(data, n, smallest);
		}
	}

	//---Build Heap Function---
	void BuildHeap() {
		int start = (capacity / 2) - 1;

		for (int i = start; i >= 0; i--) {
			heapify(data, capacity, i);
		}
	}

	//Function to Check if the
	//Vertex is in heap or not
	bool isInHeap(int u)
	{
		//Using position array to Check
		//vertex exists in heap or not
		if (position[u] < currentSize)
			return true;
		return false;
	}


	//Returns the Min Vertex and removing
	//it from the heap  
	type ExtractMin() {
		type first = data[0];
		type last = data[currentSize - 1];
		data[0] = last;

		//Updating the location of first and last vertex 
		//present in heap in position array
		position[first.iD] = currentSize - 1;
		position[last.iD] = 0;

		this->currentSize--;

		//min heapifying again
		heapify(data, capacity, 0);
		return first;
	}


	void DecreaseKey(int u, int w) {
		//Taking the location where the vertex exists in 
		//heap from position array and then by going at that
		//index in heap updating its weight....
		int index = position[u];
		data[index].weight = w;


		//By updating weight if relation of that vertex is disturbed with it's
		//parent then heapifying up till the index becomes zero and the weight of 
		//child is less than it's parent....

		while (index && data[index].weight < data[(index - 1) / 2].weight) {
			//Updating locations of swapping
			//data in position array too...
			position[data[index].iD] = (index - 1) / 2;
			position[data[(index - 1) / 2].iD] = index;
			swap(data[index], data[(index - 1) / 2]);

			index = (index - 1) / 2;
		}
	}

	//Destructor
	~Heap() {
		if (!position) {
			delete[]position;
			position = 0;
		}
		currentSize = 0;
		capacity = 0;
		data.clear();
	}
};


//------------------Graph Class-------------------

class Graph
{
	int nodes;
	int size;
	vector<GraphNode> adj;
public:

	//----Constructor----
	Graph() {
		nodes = 0;
		size = 0;
	}

	Graph(const Graph& obj) {
		size = obj.size;
		nodes = obj.nodes;
		adj = obj.adj;
	}

	//Creating new Node of Edge data
	Edge newNodes(int dest, int weight) {
		Edge newNode(dest, weight);
		return newNode;
	}

	//Adding edge information in the stl list 
	//which is present at the given index
	//of vector...
	void addEdge(int src, int dest, int weight) {

		Edge newNode = newNodes(dest, weight);
		adj.at(src).edgelist.push_back(newNode);

		newNode = newNodes(src, weight);
		adj.at(dest).edgelist.push_back(newNode);

		nodes++;
	}

	//----------Reading File Data----------
	void TakeInput(string file) {

		adj.clear();
		ifstream fin;
		fin.open(file);
		int totalVertices;
		int iD;
		string type;
		if (fin) {
			fin >> totalVertices;

			//Adding GraphNodes in vector 
			for (int i = 0; i <= totalVertices; i++) {

				//Handling 0th index
				if (i == 0) {
					GraphNode V(0, "");
					adj.push_back(V);
				}
				else {
					fin >> iD;
					fin.ignore(256, ',');
					fin >> type;
					GraphNode V(iD, type);
					adj.push_back(V);
				}
			}
			this->size = ++totalVertices;
			int totalEdges;
			int starting;
			int ending;
			int weight;
			fin >> totalEdges;

			//Making list of edges on each vertex
			for (int i = 0; i < totalEdges; i++) {
				fin >> starting;
				fin.ignore(256, ',');
				fin >> ending;
				fin >> weight;
				addEdge(starting, ending, weight);
			}
			fin.close();
		}
		else {
			cout << "Unable to Open File\n";
		}
	}

	//---------Saving Adjacency List in File--------
	void SaveData(string file) {
		ofstream fout;
		fout.open(file);

		int t = size;
		fout << --t << endl;

		//Writing data of each Vertex in File
		vector<GraphNode>::iterator itr = adj.begin();
		while (itr != adj.end()) {
			if ((*itr).iD != 0) {
				fout << (*itr).iD << ", " << (*itr).type << endl;
			}
			itr++;
		}
		fout << nodes << endl;

		//Writing data of Edges in file and as
		//Graph is undirected So, here using
		//visited array for not storing the 
		//same edge twice...

		bool* visited = new bool[size];
		for (int i = 0; i < size; i++) {
			visited[i] = false;
		}
		itr = adj.begin();
		int i = 0;
		for (; itr != adj.end(); itr++) {
			if ((*itr).iD != 0) {
				list<Edge>::iterator it = (*itr).edgelist.begin();
				for (; it != (*itr).edgelist.end(); it++) {
					if (!visited[(*it).destination]) {
						fout << i << "," << (*it).destination << " " << (*it).Eweight << endl;
					}
				}
				visited[i] = true;
			}
			i++;
		}
		fout.close();
		delete[]visited;
	}


	//Adding new Vertex and connecting
	//it with Graph so disconnected graph will
	//not form
	void addNewVertex() {
		int iD;
		iD = size;
		string type;
		int EndingV, EW;
		//Giving next available iD to the
		//Vertex and taking it's name....
		cout << "\nID given to new Vertex is:  " << iD << endl;
		cout << "type of Vertex: ";
		cin >> type;
		GraphNode G(iD, type);
		adj.push_back(G);   //adding at the end of vector Array
		this->size++;

		cout << "\n\t\tAdd Edge to connect newly made vertex with other Vertices\n\n";

		cout << "Enter Vertex iD you want to Connect with inbetween 1 to " << iD - 1 << ":  ";
		cin >> EndingV;
		if (EndingV < 1 || EndingV > iD - 1) {
			do {
				cout << "\t\t\tID does not exist:(\n";
				cout << "Enter Again: ";
				cin >> EndingV;
			} while (EndingV < 1 || EndingV > iD - 1);
		}
		cout << "Enter Edge Weight:  ";
		cin >> EW;
		addEdge(iD, EndingV, EW); //adding edge Node  
	}

	//Adding Edge in pre-existing Vertices
	//or replacing edge if it already exists
	bool addUserEdge(int s, int d, int w) {
		if (s < 1 || s > size - 1 || d<1 || d>size - 1) {
			cout << "\t\t\tSorce or Destination out of Bound\n";
			cout << "\t\t\t\tRange is 1 to " << size - 1 << endl;
			return false;
		}
		else {
			bool flag = false;
			list<Edge>::iterator itr = adj[s].edgelist.begin();
			list<Edge>::iterator itr1 = adj[d].edgelist.begin();
			while (itr != adj[s].edgelist.end()) {
				if ((*itr).destination == d) {
					(*itr).Eweight = w;
					flag = true;
				}
				itr++;
			}
			if (flag) {
				while (itr1 != adj[d].edgelist.end()) {
					if ((*itr1).destination == s) {
						(*itr1).Eweight = w;
						cout << "\t\t\tConnection already exist\n";
						cout << "\t\tNew Connection is replaced with the previous one\n";
						return true;
					}
					itr1++;
				}
			}
		}
		addEdge(s, d, w);
		cout << "\t\t\tNew Connection made Successfully:}\n";
		return true;
	}

	//Algorithm to find the minimum Path
	void SpanningTree() {

		vector<GraphNode> Clone;
		Heap<GraphNode> H(size - 1);

		//Creating Copy of original adj List 
		//So, it remains unchanged
		Clone = adj;
		Clone[1].weight = 0;  //Setting weight of vertex at index 1 to zero

		//Inserting Vertices in Heap and
		//setting there location in heap
		//in position aray....
		for (int i = 0; i < Clone.size(); i++) {
			H.insert(Clone[i]);
			H.position[i] = i;
		}
		H.BuildHeap();  //Building heap of inserted data

		//Array to store the set of edges 
		//that covers all the vertices...
		int* store = new int[size];
		for (int i = 0; i < size; i++) {
			store[i] = -1;
		}
		GraphNode vertex;  //to store the extracted vertex


		//looping till the heap is not empty
		while (!H.isEmpty()) {
			vertex = H.ExtractMin();  //extracting min vertex from heap
			int v = vertex.iD;

			//Making Iterator to traverse the list of
			//extracted vertex....
			list<Edge>::iterator itr = adj[v].edgelist.begin();

			//Iterating till the end of list
			for (; itr != adj[v].edgelist.end(); itr++) {
				int u = (*itr).destination;    //iD of the adjacent Vertex

				//Checking if the vertes still in heap or visited 
				//before and checking if the weight of vertex is
				//greater than it's edge (if both are true then going in)
				if (H.isInHeap(u) && Clone[u].weight > (*itr).Eweight) {

					//updating weight of u with edge weight
					Clone[u].weight = (*itr).Eweight;
					store[u] = v; //storing edge iD at the index of recent neigbour's iD

					H.DecreaseKey(u, Clone[u].weight); //Decreasing Key
				}
			}
		}

		cout << "\n\t\t--------------Connections with Minimum Wire-------------\n\n";
		for (int i = 0; i < size; i++) {
			if (store[i] != -1) {
				cout << "Vertex: [" << i << "] ---------- Vertex: [" << store[i] << "]";
				cout << "\t|| Wire Used: " << Clone[i].weight << endl;
				cout << "      [" << Clone[i].type << "]  ------ " << " [" << Clone[store[i]].type << "]";
				cout << "\n\n\n";
			}
		}
		delete[] store;
	}


	//Printing the Adjacency List
	void print() {
		vector<GraphNode>::iterator itr = adj.begin();
		for (int i = 0; i < size; i++) {
			if ((*itr).iD != 0) {
				list<Edge>::iterator it = (*itr).edgelist.begin();
				cout << "ID: " << (*itr).iD << " " << "type: " << (*itr).type << "\t" << "|| List->  ";
				while (it != (*itr).edgelist.end()) {
					cout << i << "--" << (*it).destination << "  EdgeW(" << (*it).Eweight << ")" << " -->  ";
					it++;
				}
				cout << "0" << endl;
			}
			itr++;
		}
		cout << endl;
	}

	//--Destructor--
	~Graph() {
		nodes = 0;
		size = 0;
		adj.clear();
	}
};

//----------------------Menu----------------------

void Menu() {
	cout << "\t\t---------------------------------------------------------\n";
	cout << "\t\t\t\t\tMenu\n";
	cout << "\t\t---------------------------------------------------------\n";
	cout << "\t\t\tPress 1 to Input a graph from a file\n";
	cout << "\t\t\tPress 2 to Save a graph in a file\n";
	cout << "\t\t\tPress 3 to Add a vertex in existing graph G\n";
	cout << "\t\t\tPress 4 to Add an edge in existing graph G\n";
	cout << "\t\t\tPress 5 to Print the set of edges that covers all\n";
	cout << "\t\t\t        the vertices\n";
	cout << "\t\t\tPress 6 to Print Graph in Adjacency List form\n";
	cout << "\t\t\tPress 0 to exit\n";
	cout << "\t\t---------------------------------------------------------\n\n";
}


//----------------------Main----------------------

int main()
{
	Graph G;
	int num;
	Menu();
	bool flag = false;
	do {
		cout << "\nEnter Number: ";
		cin >> num;
		switch (num) {
		case 1:
			G.TakeInput("data.txt");
			cout << "\t\t\t\tFile Read Successfully\n";
			flag = true;
			break;
		case 2:
			if (flag) {
				G.SaveData("output.txt");
				cout << "\t\t\tGraph Data Saved in File Successfully\n";
			}
			else {
				cout << "\t\t\t\tPress 1 to Read File First\n";
			}
			break;
		case 3:
			if (flag) {
				cout << "\t\t\t\tAdd new Vertex in Graph\n";
				G.addNewVertex();
				cout << "\n\t\tNew vertex Added to Graph Successfully!\n";
			}
			else {
				cout << "\t\t\t\tPress 1 to Read File First\n";
			}
			break;
		case 4:
			if (flag) {
				int SV, EV, W;
				bool Check = false;
				cout << "\t\t\t\tAdd New Edge\n\n";
				do {
					cout << "Starting Vertex iD: ";
					cin >> SV;
					cout << "Ending Vertex iD: ";
					cin >> EV;
					cout << "Weight: ";
					cin >> W;
					Check = G.addUserEdge(SV, EV, W);
					if (!Check) {
						cout << "\t\t\tAgain add data in the range of Graph!\n\n";
					}
				} while (!Check);
			}
			else {
				cout << "\t\t\t\tPress 1 to Read File First\n";
			}
			break;
		case 5:
			if (flag) {
				G.SpanningTree();
			}
			else
				cout << "\t\t\t\tPress 1 to Read File First\n";
			break;
		case 6:
			if (flag) {
				cout << endl;
				G.print();
			}
			else {
				cout << "\t\t\t\tPress 1 to Read File First\n";
			}
			break;
		default:
			if (num < 0 || num>6) {
				cout << "\t\t\t\tPress the Correct Number:)\n";
			}
			break;
		}
	} while (num != 0);
	return 0;
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <queue>
#include <limits>
#include <algorithm>
#include <set>
#include <stack>
using namespace std;
const int TABLE_SIZE = 25;

struct Airport
{
    int originAirportID;
    int destinationAirportID;
    string originCity;
    string destinationCity;
    double distance;
    Airport *left = nullptr;
    Airport *right = nullptr;

    Airport(int originID, int destID, const string &origCity, const string &destCity, double dist)
        : originAirportID(originID), destinationAirportID(destID), originCity(origCity),
          destinationCity(destCity), distance(dist) {}
};

vector<Airport *> hashTable[TABLE_SIZE];

Airport *head = NULL;
Airport *tail = NULL;
class Queue
{
public:
    Airport *front;
    Airport *rear;
    Queue() : front(NULL), rear(NULL) {}
    void enqueue(Airport *newNode)
    {
        if (front == NULL)
        {
            front = newNode;
            rear = newNode;
        }
        else if (newNode->distance < front->distance)
        {
            newNode->right = front;
            front->left = newNode;
            front = newNode;
        }
        else if (newNode->distance > rear->distance)
        {
            rear->right = newNode;
            newNode->left = rear;
            rear = newNode;
        }
        else
        {
            Airport *temp = front;
            while (temp->right != NULL && temp->right->distance <= newNode->distance)
            {
                // Skip nodes with the same distance
                if (temp->right->distance == newNode->distance)
                {
                    delete newNode;
                    return; // Already in the queue
                }
                temp = temp->right;
            }
            newNode->right = temp->right;
            newNode->left = temp;
            if (temp->right != NULL)
            {
                temp->right->left = newNode;
            }
            temp->right = newNode;
        }
    }

    Airport *dequeue()
    {
        Airport *temp = front;
        if (front->right != NULL)
        {
            front->right->left = NULL;
            front = front->right;
        }
        else
        {
            front = NULL;
            rear = NULL;
        }
        return temp;
    }
};
int getHashIndex(int airportID)
{
    return airportID % TABLE_SIZE;
}

void displayHashTable()
{
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        cout << "Hash Index " << i << ":" << endl;

        if (hashTable[i].empty())
        {
            cout << "   Empty" << endl;
        }
        else
        {
            Airport *current = hashTable[i][0];
            while (current != nullptr)
            {
                cout << "   Origin Airport ID: " << current->originAirportID << endl;
                cout << "   Origin City: " << current->originCity << endl;
                cout << "   Destination Airport ID: " << current->destinationAirportID << endl;
                cout << "   Destination City: " << current->destinationCity << endl;
                cout << "   Distance: " << fixed << setprecision(2) << current->distance << " miles" << endl;
                cout << "   ------------------------" << endl;

                current = current->left;
            }
        }
    }
}

void insertHashNode(int originAirportID, int destinationAirportID,
                    const string &originCity, const string &destinationCity, double distance)
{
    int index = getHashIndex(originAirportID);
    Airport *newNode = new Airport(originAirportID, destinationAirportID, originCity, destinationCity, distance);

    if (hashTable[index].empty())
    {
        hashTable[index].push_back(newNode);
    }
    else
    {
        Airport *current = hashTable[index][0];
        while (current->left != nullptr)
        {
            current = current->left;
        }
        current->left = newNode;
        newNode->right = current;
    }
}

void displayFlightsByAirportID(int airportID)
{
    int index = getHashIndex(airportID);

    if (hashTable[index].empty())
    {
        cout << "No flights found for Airport ID " << airportID << endl;
    }
    else
    {
        Airport *current = hashTable[index][0];
        while (current != nullptr)
        {
            if (current->originAirportID == airportID)
            {
                cout << "   Origin Airport ID: " << current->originAirportID << endl;
                cout << "   Origin City: " << current->originCity << endl;
                cout << "   Destination Airport ID: " << current->destinationAirportID << endl;
                cout << "   Destination City: " << current->destinationCity << endl;
                cout << "   Distance: " << fixed << setprecision(2) << current->distance << " miles" << endl;
                cout << "   ------------------------" << endl;
            }
            current = current->left;
        }
    }
}
void addNode(int originAirportID, int destinationAirportID, string originCity, string destinationCity, double distance)
{
    Airport *newNode = new Airport(originAirportID, destinationAirportID, originCity, destinationCity, distance);
    if (head == NULL)
    {
        head = newNode;
        tail = newNode;
        return;
    }
    newNode->left = tail;
    tail->right = newNode;
    tail = newNode;
}

void displayList()
{
    Airport *current = head;
    while (current != NULL)
    {
        cout << "Origin Airport ID: " << current->originAirportID << endl;
        cout << "Origin City: " << current->originCity << endl;
        cout << "Destination Airport ID: " << current->destinationAirportID << endl;
        cout << "Destination City: " << current->destinationCity << endl;
        cout << "Distance: " << fixed << setprecision(2) << current->distance << " miles" << endl;
        cout << "------------------------" << endl;
        current = current->right;
    }
}
double calculateCost(double distance, double costPerMile)
{
    return distance * costPerMile;
}
void AllFlightCost(int start, const double distances[][25])
{
    const int numAirports = 25;
    vector<double> distance(numAirports, numeric_limits<double>::infinity());
    vector<bool> visited(numAirports, false);
    vector<int> parent(numAirports, -1);
    distance[start] = 0;

    for (int count = 0; count < numAirports - 1; ++count)
    {
        int u = -1;
        for (int v = 0; v < numAirports; ++v)
        {
            if (!visited[v] && (u == -1 || distance[v] < distance[u]))
            {
                u = v;
            }
        }

        visited[u] = true;

        for (int v = 0; v < numAirports; ++v)
        {
            if (!visited[v] && distances[u][v] != 0 && distance[u] != numeric_limits<double>::infinity() &&
                distance[u] + distances[u][v] < distance[v])
            {
                distance[v] = distance[u] + distances[u][v];
                parent[v] = u;
            }
        }
    }

    cout << "Minimum FLight Cost for All Airports: " << start << ":" << endl;
    for (int i = 0; i < numAirports; ++i)
    {
        if (i != start)
        {
            if (distance[i] != numeric_limits<double>::infinity())
            {
                cout << "To Airport " << i << ": " << distance[i] << " miles, Cost: $" << calculateCost(distance[i], 5.0) << endl;
                cout << "Path: ";
                int j = i;
                stack<int> path;
                while (j != start)
                {
                    path.push(j);
                    j = parent[j];
                }
                cout << start;
                while (!path.empty())
                {
                    cout << " -> " << path.top();
                    path.pop();
                }
                cout << endl;
            }
            else
            {
                cout << "To Airport " << i << ": Not reachable" << endl;
            }
        }
    }
}
// Function to apply Dijkstra's algorithm
void shortestPath(int start, const double distances[][25])
{
    const int numAirports = 25;
    vector<double> distance(numAirports, numeric_limits<double>::infinity());
    vector<bool> visited(numAirports, false);
    distance[start] = 0;

    for (int count = 0; count < numAirports - 1; ++count)
    {
        int u = -1;
        for (int v = 0; v < numAirports; ++v)
        {
            if (!visited[v] && (u == -1 || distance[v] < distance[u]))
            {
                u = v;
            }
        }

        visited[u] = true;

        for (int v = 0; v < numAirports; ++v)
        {
            if (!visited[v] && distances[u][v] != 0 && distance[u] != numeric_limits<double>::infinity() &&
                distance[u] + distances[u][v] < distance[v])
            {
                distance[v] = distance[u] + distances[u][v];
            }
        }
    }

    cout << "Dijkstra's Shortest Distances from Airport " << start << ":" << endl;
    for (int i = 0; i < numAirports; ++i)
    {
        cout << "To Airport " << i << ": " << distance[i] << " miles" << endl;
    }
}


pair<vector<int>, double> FlightCost(int start, int destination, const double distances[][25])
{
    const int numAirports = 25;
    vector<double> distance(numAirports, numeric_limits<double>::infinity());
    vector<bool> visited(numAirports, false);
    vector<int> parent(numAirports, -1);
    distance[start] = 0;

    for (int count = 0; count < numAirports - 1; ++count)
    {
        int u = -1;
        for (int v = 0; v < numAirports; ++v)
        {
            if (!visited[v] && (u == -1 || distance[v] < distance[u]))
            {
                u = v;
            }
        }

        visited[u] = true;

        for (int v = 0; v < numAirports; ++v)
        {
            if (!visited[v] && distances[u][v] != 0 && distance[u] != numeric_limits<double>::infinity() &&
                distance[u] + distances[u][v] < distance[v])
            {
                distance[v] = distance[u] + distances[u][v];
                parent[v] = u;
            }
        }
    }

    vector<int> path;
    int dest = destination;
    while (dest != -1)
    {
        path.push_back(dest);
        dest = parent[dest];
    }
    reverse(path.begin(), path.end());

    return {path, distance[destination]};
}

void displayFlightCost(int originID, int destID, const double distances[][25])
{
    auto result = FlightCost(originID, destID, distances);
    const vector<int> &path = result.first;
    double cost = result.second;

    cout << "Shortest Path from Airport " << originID << " to Airport " << destID << ":" << endl;
    for (int i = 0; i < path.size(); ++i)
    {
        cout << path[i];
        if (i < path.size() - 1)
            cout << " -> ";
    }
    cout << "\nCost: $" << calculateCost(cost, 5.0) << endl;
}
// Function to apply Prim's algorithm
void prim(const double distances[][25])
{
    const int numAirports = 25;
    vector<int> parent(numAirports, -1);
    vector<double> key(numAirports, numeric_limits<double>::infinity());
    vector<bool> mstSet(numAirports, false);

    key[0] = 0;

    for (int count = 0; count < numAirports - 1; ++count)
    {
        int u = -1;
        for (int v = 0; v < numAirports; ++v)
        {
            if (!mstSet[v] && (u == -1 || key[v] < key[u]))
            {
                u = v;
            }
        }

        mstSet[u] = true;

        for (int v = 0; v < numAirports; ++v)
        {
            if (distances[u][v] != 0 && !mstSet[v] && distances[u][v] < key[v])
            {
                parent[v] = u;
                key[v] = distances[u][v];
            }
        }
    }

    cout << "Prim's Minimum Spanning Tree:" << endl;
    for (int i = 1; i < numAirports; ++i)
    {
        cout << "Edge: " << parent[i] << " - " << i << ", Distance: " << distances[i][parent[i]] << " miles" << endl;
    }
}

// Function to apply Kruskal's algorithm
class Edge
{
public:
    int src, dest;
    double weight;
};

class Graph
{
public:
    vector<Edge> edges;
    int numVertices;

    Graph(int numV) : numVertices(numV) {}

    void addEdge(int src, int dest, double weight)
    {
        Edge edge;
        edge.src = src;
        edge.dest = dest;
        edge.weight = weight;
        edges.push_back(edge);
    }
};
bool compareEdges(const Edge &e1, const Edge &e2)
{
    return e1.weight < e2.weight;
}
int find(vector<int> &parent, int i)
{
    if (parent[i] == -1)
        return i;
    return find(parent, parent[i]);
}

void unionSets(vector<int> &parent, int x, int y)
{
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    parent[xroot] = yroot;
}

void kruskal(const double distances[][25])
{
    const int numAirports = 25;
    vector<Edge> result;
    vector<int> parent(numAirports, -1);
    vector<Edge> edges;

    for (int i = 0; i < numAirports; ++i)
    {
        for (int j = i + 1; j < numAirports; ++j)
        {
            if (distances[i][j] != 0)
            {
                Edge edge;
                edge.src = i;
                edge.dest = j;
                edge.weight = distances[i][j];
                edges.push_back(edge);
            }
        }
    }

    sort(edges.begin(), edges.end(), compareEdges);

    for (int i = 0; i < edges.size(); ++i)
    {
        int x = find(parent, edges[i].src);
        int y = find(parent, edges[i].dest);

        if (x != y)
        {
            result.push_back(edges[i]);
            unionSets(parent, x, y);
        }
    }

    cout << "Kruskal's Minimum Spanning Tree:" << endl;
    for (int i = 0; i < result.size(); ++i)
    {
        cout << "Edge: " << result[i].src << " - " << result[i].dest << ", Distance: " << result[i].weight << " miles" << endl;
    }
}

// Function to apply BFS
void bfs(int start, const double distances[][25])
{
    const int numAirports = 25;
    vector<bool> visited(numAirports, false);
    queue<int> q;
    q.push(start);
    visited[start] = true;

    cout << "BFS starting from Airport " << start << ":" << endl;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        cout << u << " ";

        for (int v = 0; v < numAirports; ++v)
        {
            if (!visited[v] && distances[u][v] != 0)
            {
                q.push(v);
                visited[v] = true;
            }
        }
    }

    cout << endl;
}

// Function to apply DFS
void dfs(int start, const double distances[][25])
{
    const int numAirports = 25;
    vector<bool> visited(numAirports, false);
    stack<int> s;
    s.push(start);

    cout << "DFS starting from Airport " << start << ":" << endl;

    while (!s.empty())
    {
        int u = s.top();
        s.pop();

        if (!visited[u])
        {
            cout << u << " ";
            visited[u] = true;
        }

        for (int v = numAirports - 1; v >= 0; --v)
        {
            if (!visited[v] && distances[u][v] != 0)
            {
                s.push(v);
            }
        }
    }

    cout << endl;
}

int main()
{
    ifstream fin("file.csv");
    if (!fin.is_open())
    {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    const int numAirports = 25;
    double distances[numAirports][numAirports] = {0.0};
    string line, word;
    vector<string> lineData;
    int lines = 0;
    while (getline(fin, line))
    {
        stringstream s(line);
        while (getline(s, word, ','))
        {
            lineData.push_back(word);
        }
        stringstream s2(lineData[0]);
        int originAirportID;
        s2 >> originAirportID;
        stringstream s3(lineData[11]);
        int destinationAirportID;
        s3 >> destinationAirportID;
        stringstream s4(lineData[15]);
        double distance;
        s4 >> distance;

        if (lines != 0)
        {
            addNode(originAirportID, destinationAirportID, lineData[7], lineData[2], distance);
            distances[originAirportID][destinationAirportID] = distance;
            distances[destinationAirportID][originAirportID] = distance;
            insertHashNode(originAirportID, destinationAirportID, lineData[7], lineData[2], distance);
        }
        lineData.clear();
        lines++;
    }
    fin.close();
    int choice;
    do
    {
        cout << "\nFlight Management System\n";
        cout << "1.  Display Flight Data for All Airports\n";
        cout << "2.  Find the Shortest Distance from the Source Airport\n";
        cout << "3.  Find the Minimum Spanning Tree of All Airports\n";
        cout << "4.  Explore Kruskal's Minimum Spanning Tree\n";
        cout << "5.  Perform BFS Traversal\n";
        cout << "6.  Perform DFS Traversal\n";
        cout << "7.  Display Sorted Flight Data\n";
        cout << "8.  Display Flight Data for a Specific Airport\n";
        cout << "9.  Calculate Minimum Cost of a Specific Flight\n";
        cout << "10. Calculate Minimum Cost for All Flights\n";
        cout << "0.  Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            displayHashTable();
            break;

        case 2:
            int start;
            cout << "Enter the starting airport ID (0-24): ";
            cin >> start;
            if (start < 0 || start >= 25)
            {
                cerr << "Invalid starting airport ID. Please try again.\n";
                continue;
            }
            shortestPath(start, distances);
            break;

        case 3:
            prim(distances);
            break;

        case 4:
            kruskal(distances);
            break;

        case 5:
            int bfsStart;
            cout << "Enter the starting airport ID (0-24) for BFS: ";
            cin >> bfsStart;
            if (bfsStart < 0 || bfsStart >= 25)
            {
                cerr << "Invalid starting airport ID. Please try again.\n";
                continue;
            }
            bfs(bfsStart, distances);
            break;

        case 6:
            int dfsStart;
            cout << "Enter the starting airport ID (0-24) for DFS: ";
            cin >> dfsStart;
            if (dfsStart < 0 || dfsStart >= 25)
            {
                cerr << "Invalid starting airport ID. Please try again.\n";
                continue;
            }
            dfs(dfsStart, distances);
            break;

        case 7:
        {
            Queue q;
            Airport *current;
            while (head != NULL)
            {
                current = head;
                head = head->right;
                if (head)
                {
                    head->left = NULL;
                    current->right = NULL;
                    q.enqueue(current);
                }
            }
            while (q.front != NULL)
            {
                Airport *nextFlight = q.front;
                q.dequeue();

                cout << "Next Flight: " << nextFlight->originCity << " to " << nextFlight->destinationCity
                     << ", Distance: " << nextFlight->distance << " miles" << endl;
            }

            cout << "Completed" << endl;
            break;
        }

        case 8:
        {
            int airportIDToFind;
            cout << "Enter the Airport ID to fetch all Flights for that Airport :";
            cin >> airportIDToFind;
            cout << "Flights for Airport ID " << airportIDToFind << ":" << endl;
            displayFlightsByAirportID(airportIDToFind);
            break;
        }

        case 9:
            int originID, destID;
            cout << "Enter the origin airport ID (0-24): ";
            cin >> originID;
            cout << "Enter the destination airport ID (0-24): ";
            cin >> destID;

            if (originID < 0 || originID >= 25 || destID < 0 || destID >= 25)
            {
                cerr << "Invalid airport IDs. Please try again.\n";
                continue;
            }

            displayFlightCost(originID, destID, distances);
            break;

        case 10:
            int originID1;
            cout << "Enter the source :";
            cin >> originID1;
            if (originID1 < 0 || originID1 >= 25)
            {
                cerr << "Invalid airport IDs. Please try again.\n";
                continue;
            }
            AllFlightCost(originID1, distances);
        case 0:
            cout << "Exiting the program.\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}

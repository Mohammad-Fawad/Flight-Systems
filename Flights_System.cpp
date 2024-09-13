#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits.h>
#include <queue>
#include <stack>

using namespace std;

struct FlightNode
{
    int year;
    int month;
    int day;
    int deptTime;
    int deptdelay;
    int arrTime;
    int arrdelay;
    string carrier;
    string tailnum;
    int flightNo;
    string origin;
    string destination;
    int airTime;
    int distance;
    int hours;
    int minutes;
    FlightNode *next = NULL;
    FlightNode *prev = NULL;
};

FlightNode *first = NULL;
FlightNode *last = NULL;

class Graph
{
public:
    int vertices;
    int **adjacencyMatrix;
    string *vertexNames;
    int *vertexIndices;

    Graph()
    {
        vertices = 1000;
        adjacencyMatrix = new int *[vertices];
        for (int i = 0; i < vertices; i++)
        {
            adjacencyMatrix[i] = new int[vertices];
        }

        vertexNames = new string[vertices];
        vertexIndices = new int[vertices];

        for (int i = 0; i < vertices; i++)
        {
            vertexIndices[i] = -1; // Initialize indices to -1 (invalid index)
            for (int j = 0; j < vertices; j++)
            {
                adjacencyMatrix[i][j] = 0;
            }
        }
    }

    int HashFunctiom(string key)
    {
        int sum = 0;
        for (int i = 0; i < key.length(); i++)
        {
            char ch = key[i];
            sum += ch;
        }
        return sum % vertices;
    }

    int getVertexIndex(string vertex)
    {
        int hash = HashFunctiom(vertex);
        int index = hash;

        while (vertexIndices[index] != -1)
        {
            // If Found
            if (vertexNames[index] == vertex)
            {
                return index;
            }
            index = (index + 1) % vertices;
        }

        // If Not found
        return -1;
    }

    void addVertex(string vertex)
    {
        int check = getVertexIndex(vertex);
        if (check == -1)
        {
            int hash = HashFunctiom(vertex);
            int index = hash;

            // If Collision Occurs
            while (vertexIndices[index] != -1)
            {
                index = (index + 1) % vertices;
            }

            vertexNames[index] = vertex;
            vertexIndices[index] = index;
        }
    }

    void addEdge(string start, string end, int cost)
    {
        int startIndex = getVertexIndex(start);
        int endIndex = getVertexIndex(end);

        if (startIndex != -1 && endIndex != -1)
        {
            adjacencyMatrix[startIndex][endIndex] = cost;
            adjacencyMatrix[endIndex][startIndex] = cost;
        }
    }

    // DFS Tarversal Of Graph

    void DFS(string given_start)
    {
        int startIndex = getVertexIndex(given_start);
        if (startIndex == -1)
        {
            cout << "Invalid starting vertex.Enter Valid Starting City" << endl;
            return;
        }

        stack<int> q;
        int visited[vertices] = {0};
        int index;

        visited[startIndex] = 1;
        cout << endl
             << "BFS Traversal: ";
        q.push(startIndex);

        while (!q.empty())
        {
            index = q.top();
            cout << vertexNames[index] << " ->";
            q.pop();
            for (int j = 0; j < vertices; j++)
            {
                if (adjacencyMatrix[index][j] != 0 && visited[j] == 0)
                {
                    q.push(j);
                    visited[j] = 1;
                }
            }
        }
        cout << endl;
    }

    // BFS Traversal OF Graph

    void BFS(string given_start)
    {
        int startIndex = getVertexIndex(given_start);
        if (startIndex == -1)
        {
            cout << "Invalid starting vertex.Enter Valid Starting City" << endl;
            return;
        }

        queue<int> q;
        int visited[vertices] = {0};
        int index;

        visited[startIndex] = 1;
        cout << endl
             << "BFS Traversal: ";
        q.push(startIndex);

        while (!q.empty())
        {
            index = q.front();
            cout << vertexNames[index] << " -> ";
            q.pop();
            for (int j = 0; j < vertices; j++)
            {
                if (adjacencyMatrix[index][j] != 0 && visited[j] == 0)
                {
                    q.push(j);
                    visited[j] = 1;
                }
            }
        }
        cout << endl;
    }

    // Supporting Function for Dijstra's Algorithm

    int minDistance(int dist[], bool visited[])
    {
        int min = INT_MAX;
        int min_index = -1;

        for (int i = 0; i < vertices; i++)
        {
            if (!visited[i] && dist[i] <= min)
            {
                min = dist[i];
                min_index = i;
            }
        }

        return min_index;
    }

    // Print Solution for Dijsktra Algorithm

    void printSolution(int dist[], string start)
    {
        cout << "Cities \t\t  Distance from " << start << endl;
        for (int i = 0; i < vertices; i++)
        {

            if (vertexIndices[i] != -1)
            {
                cout << vertexNames[i] << " \t\t\t" << dist[i] << endl;
            }
        }
        cout << endl;
    }

    // Dijkstra Algorithm

    void dijkstra(string given_start)
    {
        // First Check Whater City Exist or Not
        int startIndex = getVertexIndex(given_start);
        if (startIndex == -1)
        {
            cout << "Invalid starting vertex.Enter Valid Starting City" << endl;
            return;
        }

        int dist[vertices];
        bool visited[vertices];

        for (int i = 0; i < vertices; i++)
        {
            dist[i] = INT_MAX;
            visited[i] = false;
        }

        dist[startIndex] = 0;

        for (int count = 0; count < vertices - 1; count++)
        {
            int u = minDistance(dist, visited);
            if (u == -1)
                break;

            visited[u] = true;

            for (int v = 0; v < vertices; v++)
            {
                if (!visited[v] && adjacencyMatrix[u][v] != 0 &&
                    dist[u] != INT_MAX && dist[u] + adjacencyMatrix[u][v] < dist[v])
                {
                    dist[v] = dist[u] + adjacencyMatrix[u][v];
                }
            }
        }

        printSolution(dist, given_start);
    }

    // Supporting Function for Prim's Algorithm

    void printSolution1(int parent[], int start)
    {
        cout << "Cities \t\t\tDistnace\n";
        for (int i = 0; i < vertices; i++)
        {
            if (i == start || parent[i] == 0) // Exclude edges where parent is 0 or start
            {
                continue;
            }
            cout << vertexNames[parent[i]] << " - " << vertexNames[i] << " \t\t"
                 << adjacencyMatrix[i][parent[i]] << " \n";
        }
        cout << endl;
    }

    // Now Prims Algorithm

    void Prims(string given_start)
    {
        int parent[vertices] = {0};
        int dist[vertices];
        bool visted[vertices];

        for (int i = 0; i < vertices; i++)
        {
            dist[i] = INT_MAX;
            visted[i] = false;
        }

        int startIndex = getVertexIndex(given_start);
        if (startIndex == -1)
        {
            cout << "Invalid starting vertex.Enter Valid Starting City" << endl;
            return;
        }

        dist[startIndex] = 0;

        for (int count = 0; count < vertices - 1; count++)
        {
            int u = minDistance(dist, visted);
            visted[u] = true;
            for (int j = 0; j < vertices; j++)
            {
                if (adjacencyMatrix[u][j] != 0 && visted[j] == false && adjacencyMatrix[u][j] < dist[j])
                {

                    parent[j] = u;
                    dist[j] = adjacencyMatrix[u][j];
                }
            }
        }

        printSolution1(parent, startIndex);
    }
};

void addFlightNode(Graph g, int yeaar, int mnth, int dy, int dptTme, int deptdelay, int arrivtme, int arrivedly, string carier,
                   string tailnum, int flight_no, string orgin, string dest, int airtim, int dstanc, int hrs, int mntues)
{
    FlightNode *crnt = new FlightNode;
    crnt->year = yeaar;
    crnt->month = mnth;
    crnt->day = dy;
    crnt->deptTime = dptTme;
    crnt->deptdelay = deptdelay;
    crnt->arrTime = arrivtme;
    crnt->arrdelay = arrivedly;
    crnt->carrier = carier;
    crnt->tailnum = tailnum;
    crnt->flightNo = flight_no;
    crnt->origin = orgin;
    crnt->destination = dest;
    crnt->airTime = airtim;
    crnt->distance = dstanc;
    crnt->hours = hrs;
    crnt->minutes = mntues;

    if (first == NULL)
    {
        first = crnt;
        last = crnt;
        crnt->next = NULL;
    }
    else
    {
        // Insert at End
        crnt->prev = last;
        last->next = crnt;
        last = crnt;
    }
    g.addVertex(crnt->origin);
    g.addVertex(crnt->destination);
    g.addEdge(crnt->origin, crnt->destination, crnt->distance);
}

void addFlightNode(int yeaar, int mnth, int dy, int dptTme, int deptdelay, int arrivtme, int arrivedly, string carier,
                   string tailnum, int flight_no, string orgin, string dest, int airtim, int dstanc, int hrs, int mntues)
{
    FlightNode *crnt = new FlightNode;
    crnt->year = yeaar;
    crnt->month = mnth;
    crnt->day = dy;
    crnt->deptTime = dptTme;
    crnt->deptdelay = deptdelay;
    crnt->arrTime = arrivtme;
    crnt->arrdelay = arrivedly;
    crnt->carrier = carier;
    crnt->tailnum = tailnum;
    crnt->flightNo = flight_no;
    crnt->origin = orgin;
    crnt->destination = dest;
    crnt->airTime = airtim;
    crnt->distance = dstanc;
    crnt->hours = hrs;
    crnt->minutes = mntues;

    if (first == NULL)
    {
        first = crnt;
        last = crnt;
        crnt->next = NULL;
    }
    else
    {
        // Insert at End
        crnt->prev = last;
        last->next = crnt;
        last = crnt;
    }
}

// Delete A Single Flight by Its Carrier,tialnum and FlightNum
void deleteFlightByDetails(string carrier, string tailNumber, int flightNumber)
{
    FlightNode *current = first;

    while (current != NULL)
    {
        if (current->carrier == carrier && current->tailnum == tailNumber && current->flightNo == flightNumber)
        {
            if (first == last)
            {
                first = last = NULL;
            }
            else if (current == first)
            {
                first = first->next;
                first->prev = NULL;
            }
            else if (current == last)
            {
                last = last->prev;
                last->next = NULL;
            }
            else
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            delete current;
            cout << "Flight with Carrier " << carrier << ", Tail Number " << tailNumber
                 << ", and Flight Number " << flightNumber << " deleted successfully." << endl;
            return;
        }

        current = current->next;
    }

    cout << "Flight with Carrier " << carrier << ", Tail Number " << tailNumber
         << ", and Flight Number " << flightNumber << " not found." << endl;
}

// printing information about all flights
void printAllFlights()
{
    FlightNode *p = first;
    cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;
    if (first != NULL)
    {
        while (p != NULL)
        {
            cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                 << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                 << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            p = p->next;
        }
    }
    else
    {
        cout << "Theere is no data present to display" << endl;
    }
}

// All Flights of Same Year
void pirntFlightsByYear(int givenyear)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "Theere is no Elemts Prent in the liist";
    }
    else
    {
        cout << "\n All Flights With Given Year are: \n";
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;
        while (p != NULL)
        {
            if (p->year == givenyear)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

// All Flights of Same Year and Same Moneth
void pirntFlightsOfYearMon(int givenyear, int givenMont)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "Theere is no Elemts Prent in the liist";
    }
    else
    {
        cout << "\n All Flights of Given Year and month are: \n";
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;

        while (p != NULL)
        {
            if (p->year == givenyear && p->month == givenMont)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

// All Flights of Same Year and Same Moneth and Same Day
void pirntFlightsOfYearMontDay(int givenyear, int givenMont, int givenday)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "Theere is no Elemts Prent in the liist";
    }
    else
    {
        cout << "\n All Flights of Given same Year and month and Day are: \n";
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;

        while (p != NULL)
        {
            if (p->year == givenyear && p->month == givenMont && p->day == givenday)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

// Print All Flights of An Carrier
void printFlightsByCarrier(string givenCarrier)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There are no elements present in the list";
    }
    else
    {
        cout << "\n All Flights of Given Carrier are: \n";
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;

        while (p != NULL)
        {
            if (p->carrier == givenCarrier)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

// Function to print ALL flights with a same Tail numbers
void printFlightsByTailNum(string giventailnum)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There is Nothing To Display" << endl;
    }
    else
    {
        cout << "All Flight Information for Given Tail Number " << giventailnum << ":" << endl;
        cout << "Dept Time , Arr Time , Sched Arrival , Depart Arrival , Carrier , Flight No , Origin , Dest , Air Time , Distance , Hours , Minutes" << endl;
        while (p != NULL)
        {
            if (p->tailnum == giventailnum)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

// Function to print ALL flights with a same Tail numbers Plus Same FlightnO
void printFlightsByTailFlightNum(string giventailnum, int givenflightno)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There is Nothing To Display" << endl;
    }
    else
    {
        cout << "All Flight Information for Given Tail Number: " << giventailnum << " And with flight no " << givenflightno << ":" << endl;
        cout << "Dept Time , Arr Time , Sched Arrival , Depart Arrival , Carrier , Flight No , Origin , Dest , Air Time , Distance , Hours , Minutes" << endl;
        while (p != NULL)
        {
            if (p->tailnum == giventailnum && p->flightNo == givenflightno)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printFlightsByOrigin(string givenOrigin)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There are no elements present in the list";
    }
    else
    {
        cout << "\n All Flights of Given Origin are: \n";
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;

        while (p != NULL)
        {
            if (p->origin == givenOrigin)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printFlightsByDestination(string givenDestination)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There are no elements present in the list";
    }
    else
    {
        cout << "\n All Flights to Given Destination are: \n";
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;

        while (p != NULL)
        {
            if (p->destination == givenDestination)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printShortDurationFlights(int given_maxduration)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There is Nothing To Display" << endl;
    }
    else
    {
        cout << "Flights with Duration Less Than " << given_maxduration << " minutes:" << endl;
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;
        while (p != NULL)
        {
            int totalMinutes = p->hours * 60 + p->minutes;
            if (totalMinutes < given_maxduration)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printLongDurationFlights(int given_minduration)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There is Nothing To Display" << endl;
    }
    else
    {
        cout << "Flights with Duration Greater Than " << given_minduration << " minutes:" << endl;
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;
        while (p != NULL)
        {
            int totalMinutes = p->hours * 60 + p->minutes;
            if (totalMinutes > given_minduration)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printFlightbyDistance(int given_dist)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There are no elements present in the list";
    }
    else
    {
        cout << "\n All Flights of Given distance" << given_dist << " : \n";
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;

        while (p != NULL)
        {
            if (p->distance == given_dist)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

// Print all flights with distnce less than given distnace
void printLessDistanceFlights(int given_disance)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There is Nothing To Display" << endl;
    }
    else
    {
        cout << "Flights with Distance Shorter Than " << given_disance << " disatnce:" << endl;
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;
        while (p != NULL)
        {
            if (p->distance < given_disance)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printMoreDistanceFlights(int given_disance)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There is Nothing To Display" << endl;
    }
    else
    {
        cout << "Flights with Distance Graeter Than " << given_disance << " disatnce:" << endl;
        cout << "Year , Month , Day, Dept Time ,Dept Delay, Arrival Time ,Arrival Delay , Carrier ,TailNum, Flight No , Origin , Destination , Air Time , Distance, Hours, Minutes" << endl;
        while (p != NULL)
        {
            if (p->distance > given_disance)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printFlightsByOriginAndDepartureTime(string give_origin, int give_deptTime)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There is Nothing To Display" << endl;
    }
    else
    {
        cout << "Flights from " << give_origin << " with Departure Time at " << give_deptTime << ":" << endl;
        cout << "Dept Time , Arr Time , Sched Arrival , Depart Arrival , Carrier , Flight No , Origin , Dest , Air Time , Distance , Hours , Minutes" << endl;
        while (p != NULL)
        {
            if (p->origin == give_origin && p->deptTime == give_deptTime)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printFlightsByDestinationAndAirTime(string given_destination, int airtme)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There is Nothing To Display" << endl;
    }
    else
    {
        cout << "Flights to " << given_destination << " with Air Time " << airtme << " minutes:" << endl;
        cout << "Dept Time , Arr Time , Sched Arrival , Depart Arrival , Carrier , Flight No , Origin , Dest , Air Time , Distance , Hours , Minutes" << endl;
        while (p != NULL)
        {
            if (p->destination == given_destination && p->airTime == airtme)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printFlightsByDestinationAndArrivalTime(string destination, int arrTime)
{
    FlightNode *p = first;
    if (p == NULL)
    {
        cout << "There is Nothing To Display" << endl;
    }
    else
    {
        cout << "Flights to " << destination << " with Arrival Time at " << arrTime << ":" << endl;
        cout << "Dept Time , Arr Time , Sched Arrival , Depart Arrival , Carrier , Flight No , Origin , Dest , Air Time , Distance , Hours , Minutes" << endl;
        while (p != NULL)
        {
            if (p->destination == destination && p->arrTime == arrTime)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printHighestDeptDealyFlight()
{
    if (first == NULL)
    {
        cout << "There is No Elemnts in List";
    }
    else
    {
        FlightNode *crnt = first;

        FlightNode *p = first;
        int highest = first->deptdelay;

        while (crnt != NULL)
        {
            if (crnt->deptdelay > highest)
            {
                highest = crnt->deptdelay;
                p = crnt;
            }
            crnt = crnt->next;
        }

        cout << "Dept Time , Arr Time , Sched Arrival , Depart Arrival , Carrier , Flight No , Origin , Dest , Air Time , Distance , Hours , Minutes" << endl;
        cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
             << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
             << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
    }
}

void printHighestArrivalDealyFlight()
{
    if (first == NULL)
    {
        cout << "There is No Elemnts in List";
    }
    else
    {
        FlightNode *crnt = first;

        FlightNode *p = first;
        int highest = first->arrdelay;

        while (crnt != NULL)
        {
            if (crnt->arrdelay > highest)
            {
                highest = crnt->arrdelay;
                p = crnt;
            }
            crnt = crnt->next;
        }

        cout << "The Flight with highest arrival delay is: " << endl;
        cout
            << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
            << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
            << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
    }
}

void printFlightbyOriginDeptTime(string given_origin, int time)
{
    if (first == NULL)
    {
        cout << "There is nothing to display";
    }
    else
    {
        FlightNode *p = first;
        int departuretime;

        cout << "All Flights of Your Given Time and Origin are: ";
        cout << "Dept Time , Arr Time , Sched Arrival , Depart Arrival , Carrier , Flight No , Origin , Dest , Air Time , Distance , Hours , Minutes" << endl;

        while (p != NULL)
        {
            departuretime = p->deptTime + p->deptdelay;
            if (departuretime == time && p->origin == given_origin)
            {
                cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
                     << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
                     << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
            }
            p = p->next;
        }
    }
}

void printLongestDistanceFlight()
{
    if (first == NULL)
    {
        cout << "There is No Elemnts in List";
    }
    else
    {
        FlightNode *crnt = first;

        FlightNode *p = first;
        int highest = first->distance;

        while (crnt != NULL)
        {
            if (crnt->distance > highest)
            {
                highest = crnt->distance;
                p = crnt;
            }
            crnt = crnt->next;
        }

        cout << "Dept Time , Arr Time , Sched Arrival , Depart Arrival , Carrier , Flight No , Origin , Dest , Air Time , Distance , Hours , Minutes" << endl;
        cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
             << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
             << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
    }
}

void printShortestDistanceFlight()
{
    if (first == NULL)
    {
        cout << "There is No Elemnts in List";
    }
    else
    {
        FlightNode *crnt = first;

        FlightNode *p = first;
        int highest = first->distance;

        while (crnt != NULL)
        {
            if (crnt->distance < highest)
            {
                highest = crnt->distance;
                p = crnt;
            }
            crnt = crnt->next;
        }

        cout << "The Shoretst Distance Flight Info is: " << endl;
        cout << "Dept Time , Arr Time , Sched Arrival , Depart Arrival , Carrier , Flight No , Origin , Dest , Air Time , Distance , Hours , Minutes" << endl;
        cout << p->year << " , " << p->month << " , " << p->day << " , " << p->deptTime << " , " << p->deptdelay << " , " << p->arrTime << " , " << p->arrdelay << " , "
             << p->carrier << " , " << p->tailnum << " , " << p->flightNo << " , " << p->origin << " , " << p->destination << " , "
             << p->airTime << " , " << p->distance << " , " << p->hours << " , " << p->minutes << endl;
    }
}

void deleteFlightsByCarrier(string carrier)
{
    FlightNode *current = first;
    while (current != NULL)
    {
        if (current->carrier == carrier)
        {
            if (first == last)
            {
                first = last = NULL;
            }
            else if (current == first)
            {
                first = first->next;
                first->prev = NULL;
            }
            else if (current == last)
            {
                last = last->prev;
                last->next = NULL;
            }
            else
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
        }
        current = current->next;
    }
    cout << "All Flights with given carrier Name has been deleted";
}

void deleteFlightsByOrigin(string origin)
{
    FlightNode *current = first;
    while (current != NULL)
    {
        if (current->origin == origin)
        {
            if (first == last)
            {
                first = last = NULL;
            }
            else if (current == first)
            {
                first = first->next;
                first->prev = NULL;
            }
            else if (current == last)
            {
                last = last->prev;
                last->next = NULL;
            }
            else
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
        }
        current = current->next;
    }
    cout << "All Flights with given Origin Airport has been deleted";
}

void deleteFlightsByDestination(string given_destination)
{
    FlightNode *current = first;
    while (current != NULL)
    {
        if (current->destination == given_destination)
        {
            if (first == last)
            {
                first = last = NULL;
            }
            else if (current == first)
            {
                first = first->next;
                first->prev = NULL;
            }
            else if (current == last)
            {
                last = last->prev;
                last->next = NULL;
            }
            else
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
        }
        current = current->next;
    }
    cout << "All Flights with given Same Destination Airport has been deleted";
}

void deleteFlightsByTailNumber(string given_tailNumber)
{
    FlightNode *current = first;
    while (current != NULL)
    {
        if (current->tailnum == given_tailNumber)
        {
            if (first == last)
            {
                first = last = NULL;
            }
            else if (current == first)
            {
                first = first->next;
                first->prev = NULL;
            }
            else if (current == last)
            {
                last = last->prev;
                last->next = NULL;
            }
            else
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
        }
        current = current->next;
    }
    cout << "All Flights with given Tailnumber has been deleted";
}

void printAverageFlightDistance()
{
    if (first == NULL)
    {
        cout << "ther is nothing to display";
    }
    else
    {

        int totalDistance = 0;
        int numberOfFlights = 0;

        FlightNode *current = first;
        while (current != NULL)
        {
            totalDistance += current->distance;
            numberOfFlights++;
            current = current->next;
        }

        double averageDistance = static_cast<double>(totalDistance) / numberOfFlights;
        cout << "Average Flight Distance: " << averageDistance << " miles" << endl;
    }
}

void printFastestAverageSpeedFlight()
{
    if (first == NULL)
    {
        cout << "ther is nothing to display";
    }
    else
    {

        FlightNode *current = first;
        FlightNode *fastestFlight = first;

        double highestSpeed = 0.0;
        double cuurentspped;

        while (current != NULL)
        {
            cuurentspped = static_cast<double>(current->distance) / current->airTime;

            if (cuurentspped > highestSpeed)
            {
                highestSpeed = cuurentspped;
                fastestFlight = current;
            }

            current = current->next;
        }

        cout << "Flight with the fastest average speed: " << fastestFlight->carrier << " "
             << fastestFlight->flightNo << " from " << fastestFlight->origin << " to "
             << fastestFlight->destination << " with an average speed of " << highestSpeed << " miles per minute." << endl;
    }
}

void printLowestAverageSpeedFlight()
{
    if (first == NULL)
    {
        cout << "ther is nothing to display";
    }
    else
    {
        FlightNode *current = first;
        FlightNode *fastestFlight = first;

        double lowestSpeed = 0.0;
        double cuurentspped;

        while (current != NULL)
        {
            cuurentspped = static_cast<double>(current->distance) / current->airTime;

            if (cuurentspped < lowestSpeed)
            {
                lowestSpeed = cuurentspped;
                fastestFlight = current;
            }

            current = current->next;
        }

        cout << "Flight with the Lowest average speed: " << fastestFlight->carrier << " "
             << fastestFlight->flightNo << " from " << fastestFlight->origin << " to "
             << fastestFlight->destination << " with an average speed of " << lowestSpeed << " miles per minute." << endl;
    }
}

void displaymenu()
{
    cout << "\n1. Insert A New Flight in List" << endl;
    cout << "2. Delete a flight from the list with its Carrier,TailNumber and FlightNo" << endl;
    cout << "3. Print information about all flights" << endl;
    cout << "4. Print Information About Flights of A Year" << endl;
    cout << "5. Print Information About Flights of A Year with its month" << endl;
    cout << "6. Print Information about flights of same year, month, and day" << endl;
    cout << "7. Print Flights by Carrier" << endl;
    cout << "8. Print Flights Of Same Origin" << endl;
    cout << "9. Print Flights Of Same Destination" << endl;
    cout << "10. Print Flights by Tail Number" << endl;
    cout << "11. Print Short Time Flights with flight time less than given time" << endl;
    cout << "12. Print Short Time Flights with flight time greater than given time" << endl;
    cout << "13. Print all Flights Information of given distance" << endl;
    cout << "14. Print all Flights Information with distance lesser than given range" << endl;
    cout << "15. Print all Flights Information with distance greater than given range" << endl;
    cout << "16. Print flights by origin and departure time" << endl;
    cout << "17. Print flights by destination and air time" << endl;
    cout << "18. Print flights by Specific destination and arrival time" << endl;
    cout << "19. Print Highest Depture Dealyed Flight" << endl;
    cout << "20. Print Highest Arrival Dealyed Flight" << endl;
    cout << "21. Print All Flights with your given origin and dept Flying time" << endl;
    cout << "22. Print The Most Longest Distance Flight" << endl;
    cout << "23. Print The Most Shortest Distance Flight" << endl;
    cout << "24. Delete All The Flights of Given Carrier" << endl;
    cout << "25. Delete All The Flights of Given TailNum" << endl;
    cout << "26. Delete All The Flights of Given Origin" << endl;
    cout << "27. Delete All The Flights of Given Destination" << endl;
    cout << "28. Print Average of All Flights Distance " << endl;
    cout << "29.Print The Fastest Speed Flight Infomration" << endl;
    cout << "30.Print The Slowest Speed Flight Infomration" << endl;
    cout << "31.Breadth First Traversal of Graph" << endl;
    cout << "32.Depth First Traversal of Graph" << endl;
    cout << "33.Apply Dijkstra Thoeorm" << endl;
    cout << "34.Apply Prims's Thoeorm" << endl;
    cout << "0. Exit" << endl;
}

int main()
{
    Graph flightgraph;
    ifstream fin;
    fin.open("FlightsDataset.csv");
    string line, word;
    vector<string> lineData;
    int lines = 0;

    getline(fin, line);

    while (!fin.eof())
    {
        getline(fin, line);
        stringstream s(line);
        while (getline(s, word, ','))
        {
            lineData.push_back(word);
        }

        stringstream s0(lineData[0]);
        int year;
        s0 >> year;

        stringstream s1(lineData[1]);
        int month;
        s1 >> month;

        stringstream s2(lineData[2]);
        int day;
        s2 >> day;

        stringstream s3(lineData[3]);
        int deptTime;
        s3 >> deptTime;

        stringstream s4(lineData[4]);
        int deptdelay;
        s4 >> deptdelay;

        stringstream s5(lineData[5]);
        int arrTime;
        s5 >> arrTime;

        stringstream s6(lineData[6]);
        int arrdelay;
        s6 >> arrdelay;

        stringstream s9(lineData[9]);
        int flightNo;
        s9 >> flightNo;

        stringstream s12(lineData[12]);
        int airTime;
        s12 >> airTime;

        stringstream s13(lineData[13]);
        int distance;
        s13 >> distance;

        stringstream s14(lineData[14]);
        int hours;
        s14 >> hours;

        stringstream s15(lineData[15]);
        int minutes;
        s15 >> minutes;

        // adding data to linked list
        addFlightNode(flightgraph, year, month, day, deptTime, deptdelay, arrTime, arrdelay, lineData[7], lineData[8], flightNo, lineData[10], lineData[11], airTime, distance, hours, minutes);

        lineData.clear();
        lines++;
    }

    int choice = 0;
    do
    {
        displaymenu();

        cout << "Select an option: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int year, month, day, deptTime, deptdelay, arrTime, arrdelay, flightNo, airTime, distance, hours, minutes;
            string carrier, tailnum, origin, destination;

            cout << "Enter Year: ";
            cin >> year;
            cout << "Enter Month: ";
            cin >> month;
            cout << "Enter Day: ";
            cin >> day;
            cout << "Enter Departure Time: ";
            cin >> deptTime;
            cout << "Enter Departure Delay: ";
            cin >> deptdelay;
            cout << "Enter Arrival Time: ";
            cin >> arrTime;
            cout << "Enter Arrival Delay: ";
            cin >> arrdelay;
            cout << "Enter Carrier: ";
            cin >> carrier;
            cout << "Enter Tail Number: ";
            cin >> tailnum;
            cout << "Enter Flight Number: ";
            cin >> flightNo;
            cout << "Enter Origin: ";
            cin >> origin;
            cout << "Enter Destination: ";
            cin >> destination;
            cout << "Enter Air Time: ";
            cin >> airTime;
            cout << "Enter Distance: ";
            cin >> distance;
            cout << "Enter Hours: ";
            cin >> hours;
            cout << "Enter Minutes: ";
            cin >> minutes;

            addFlightNode(year, month, day, deptTime, deptdelay, arrTime, arrdelay, carrier, tailnum, flightNo, origin, destination, airTime, distance, hours, minutes);

            cout << "Flight added successfully!" << endl;
            break;
        }
        case 2:
        {
            string a;
            cout << "Enter Carrier Name";
            cin >> a;

            string b;
            cout << "Enter TailNumber";
            cin >> b;

            int c;
            cout << "Enter FlightNo";
            cin >> c;

            deleteFlightByDetails(a, b, c);
            break;
        }
        case 3:
            printAllFlights();
            break;
        case 4:
        {
            int year;
            cout << "Enter Year: ";
            cin >> year;
            pirntFlightsByYear(year);
            break;
        }
        case 5:
        {
            int year, month;
            cout << "Enter Year: ";
            cin >> year;
            cout << "Enter Month: ";
            cin >> month;
            pirntFlightsOfYearMon(year, month);
            break;
        }
        case 6:
        {
            int year, month, day;
            cout << "Enter Year: ";
            cin >> year;
            cout << "Enter Month: ";
            cin >> month;
            cout << "Enter Day: ";
            cin >> day;
            pirntFlightsOfYearMontDay(year, month, day);
            break;
        }
        case 7:
        {
            string carrier;
            cout << "Enter Carrier: ";
            cin >> carrier;
            printFlightsByCarrier(carrier);
            break;
        }
        case 8:
        {
            string origin;
            cout << "Enter Origin: ";
            cin >> origin;
            printFlightsByOrigin(origin);
            break;
        }
        case 9:
        {
            string destination;
            cout << "Enter Destination: ";
            cin >> destination;
            printFlightsByDestination(destination);
            break;
        }
        case 10:
        {
            string tailNum;
            cout << "Enter Tail Number: ";
            cin >> tailNum;
            printFlightsByTailNum(tailNum);
            break;
        }
        case 11:
        {
            int maxDuration;
            cout << "Enter Maximum Duration (in minutes): ";
            cin >> maxDuration;
            printShortDurationFlights(maxDuration);
            break;
        }
        case 12:
        {
            int minDuration;
            cout << "Enter Minimum Duration (in minutes): ";
            cin >> minDuration;
            printLongDurationFlights(minDuration);
            break;
        }
        case 13:
        {
            int distance;
            cout << "Enter Distance: ";
            cin >> distance;
            printFlightbyDistance(distance);
            break;
        }
        case 14:
        {
            int minDistance;
            cout << "Enter Minimum Distance: ";
            cin >> minDistance;
            printLessDistanceFlights(minDistance);
            break;
        }
        case 15:
        {
            int maxDistance;
            cout << "Enter Maximum Distance: ";
            cin >> maxDistance;
            printMoreDistanceFlights(maxDistance);
            break;
        }
        case 16:
        {
            string origin;
            int deptTime;
            cout << "Enter origin: ";
            cin >> origin;
            cout << "Enter departure time: ";
            cin >> deptTime;
            printFlightsByOriginAndDepartureTime(origin, deptTime);
            break;
        }
        case 17:
        {
            string destination;
            int airTime;
            cout << "Enter destination: ";
            cin >> destination;
            cout << "Enter air time: ";
            cin >> airTime;
            printFlightsByDestinationAndAirTime(destination, airTime);
            break;
        }
        case 18:
        {
            string destination;
            int arrTime;
            cout << "Enter destination: ";
            cin >> destination;
            cout << "Enter arrival time: ";
            cin >> arrTime;
            printFlightsByDestinationAndArrivalTime(destination, arrTime);
            break;
        }
        case 19:
        {
            printHighestDeptDealyFlight();
            break;
        }
        case 20:
        {
            printHighestArrivalDealyFlight();
            break;
        }
        case 21:
        {
            int time;
            cout << "Enter the Dept Flying Time in this fomat 940 for 9:40: ";
            cin >> time;
            string origin;
            cout << "Enter the Flying Airport/Oroign: ";
            cin >> origin;
            printFlightbyOriginDeptTime(origin, time);
        }
        case 22:
        {
            printLongestDistanceFlight();
            break;
        }
        case 23:
        {
            printShortestDistanceFlight();
            break;
        }
        case 24:
        {
            string given;
            cout << "Enter The Carrier You Want To Delete: ";
            cin >> given;
            deleteFlightsByCarrier(given);
            cout << endl;
            break;
        }
        case 25:
        {
            string given;
            cout << "Enter The TailNum You Want To Delete: ";
            cin >> given;
            deleteFlightsByTailNumber(given);
            cout << endl;
            break;
        }

        case 26:
        {
            string given;
            cout << "Enter The oRIGIN Airport You Want To Delete: ";
            cin >> given;
            deleteFlightsByOrigin(given);
            cout << endl;
            break;
        }

        case 27:
        {
            string given;
            cout << "Enter The Destination Airports You Want To Delete: ";
            cin >> given;
            deleteFlightsByDestination(given);
            cout << endl;
            break;
        }

        case 28:
        {
            printAverageFlightDistance();
            break;
        }
        case 29:
        {
            printFastestAverageSpeedFlight();
            break;
        }
        case 30:
        {
            printLowestAverageSpeedFlight();
            break;
        }
        case 31:
        {
            string input;
            cout << "Enter the Source City to Start Tarversal with: ";
            cin >> input;
            flightgraph.BFS(input);
            break;
        }
        case 32:
        {
            string input;
            cout << "Enter the Source City to Start Tarversal with: ";
            cin >> input;
            flightgraph.DFS(input);
            break;
        }
        case 33:
        {
            string input;
            cout << "Enter the Source City to Start with: ";
            cin >> input;
            flightgraph.dijkstra(input);
            break;
        }
        case 34:
        {
            string input;
            cout << "Enter the Source City to Start with: ";
            cin >> input;
            flightgraph.Prims(input);
            break;
        }
        case 0:
            cout << "Exiting the program." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 0);

    return 0;
}

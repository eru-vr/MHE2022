// MHE2022_GraphColoring.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <map>
#include <string>
#include <iostream>

#define vertCount 7
#define edgeCount 12
#define colorCount 3
#define randSeed 16462

using namespace std;

class Vert 
{
    public:
        int idx, color;
        vector<int> adjacentIndices;

        void SetAdjacent(int index) {
            adjacentIndices.push_back(index);
        }

        Vert(int index, int Color) {
            idx = index;
            color = Color;
        }
};

vector<Vert> GenVertices()
{
    vector<Vert> vertices;
    int randColor;
    for (int i = 0; i < vertCount; i++) {
        randColor = rand() % colorCount + 1;
        Vert currentVertex(i, randColor);
        vertices.push_back(currentVertex); //add to the end of list
    }
    return vertices;
};

void GenRandomGraph() {

    /*
        - Ka�dy wierzcho�ek musi mie� conajmniej jedn� kraw�d� = obs�uga wyizolowanych wierzcho�k�w
        - Nie mo�e by� duplikat�w kraw�dzi.

        Wsparcie: https://www.tutorialspoint.com/cplusplus-program-to-create-a-random-graph-using-random-edge-generation
    */

    int edges[edgeCount][2]; //warto�� to wierzcho�ek, pierwszy wymiar to index edga, drugi to pierwszy lub drugi vert w edgu (bo edge ma dwa wierzcho�ki)
    int i = 0, count = 0;

    while(i < edgeCount){
    //for (int i = 0; i < edgeCount; i++) { // chcia�em zrobi� wersj� z forem zamiast while, ale z jakiego� powodu nie dzia�a tak samo... no idea why.

        edges[i][0] = rand() % vertCount;
        edges[i][1] = rand() % vertCount;

        if (edges[i][0] == edges[i][1]){ // je�li oba ko�ce edga nie s� tym samym wierzcho�kiem
            //cout << "[" << i << "] " << " - (" << edges[i][0] << ") --- (" << edges[i][1] << ")\n"; //bad edges
            continue;
        }
        else
        {
            for (int j = 0; j < i; j++) {
                if ((edges[i][0] == edges[j][0] && edges[i][1] == edges[j][1])
                  ||(edges[i][0] == edges[j][1] && edges[i][1] == edges[j][0]))
                    i--;
            }
        }
        i++;
    }

    cout << "### Adjacent vertices ### " << endl;
    for (int i = 0; i < vertCount; i++) {
        count = 0;
        cout << i << " adj to = { ";

        for (int j = 0; j < edgeCount; j++) {
            if (edges[j][0] == i) //je�li pierwszy vert edga, to wypisz drugi vert edga
            {
                cout << edges[j][1] << " ";
                count++;
            }else if (edges[j][1] == i) //a je�li drugi vert edga, to wypisz pierwszy vert edga
            {
                cout << edges[j][0] << " ";
                count++;
            }
            else if (j == edgeCount-1 && count == 0) { // wypisz na ko�cu p�tli, gdy nie wszed� w �adnego ifa
                cout << "Isolated "; //Print �Isolated vertex� for the vertex having no degree.
            }
        }
        cout << "}\n";
    }
}

int main(int argc, char** argv)
{
    srand(randSeed); //seeding random number generator
    vector<Vert> vertices = GenVertices();
    
    cout << "vertices.size() = " << vertices.size() << endl << endl;

    for (int i = 0; i < vertices.size(); i++) {
        cout << "Vert (" << vertices[i].idx << ", " << vertices[i].color << ")\n";
    }
    cout << endl << endl;

    GenRandomGraph();

    return 0;
}
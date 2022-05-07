/*
    Projekt na zaj�cia MHE 2022 PJATK
    Temat: Graph Coloring
    Autor: Jaromir Daruk 
    Index: s16462

*/

/*
    Parametry na kt�rych b�dzie oparty graf i generowanie rozwi�za� dla niego.
    TODO: Przerobienie na wprowadzanie tych zmiennych poprzez argumenty z linii polece�.

    #define vertCount 5
    #define edgeCount 6
    #define colorCount 3
    #define randSeed 16462
    https://csacademy.com/app/graph_editor/
 */

#include <vector>
#include <map>
#include <string>
#include <iostream>

#define vertCount 5
#define edgeCount 6
#define colorCount 3
#define randSeed 16462

using namespace std;

class Vert {

    private:
        int idx, cr;
        vector<int> adjacentIndices;

    public:
        int GetIndex() { return idx; }

        int GetColor() { return cr; }
        void SetColor(int color) { cr = color; }

        vector<int> GetAdjacent() { return adjacentIndices; }
               void SetAdjacent(int color) { adjacentIndices.push_back(color); }

        void PrintAdjacent() {
            for (auto adj : adjacentIndices) {
                cout << adj << " ";
            }
        }

        Vert(int index, int color) {
            idx = index;
            cr = color;
        }
};

vector<Vert> GenVertices(){
    vector<Vert> vertices;
    int randColor;
    cout << "Generating vertex list...\n";
    for (int i = 0; i < vertCount; i++) {
        randColor = rand() % colorCount + 1;
        Vert currentVertex(i, randColor);
        vertices.push_back(currentVertex); //add to the end of list
    }
    return vertices;
};

void GenRandomGraph(vector<Vert> &vertices) {
    /*
        - Ka�dy wierzcho�ek musi mie� conajmniej jedn� kraw�d� = obs�uga wyizolowanych wierzcho�k�w
        - Nie mo�e by� duplikat�w kraw�dzi.

        Wsparcie: https://www.tutorialspoint.com/cplusplus-program-to-create-a-random-graph-using-random-edge-generation
    */

    int edges[edgeCount][2]; //warto�� to wierzcho�ek, pierwszy wymiar to index edga, drugi to pierwszy lub drugi vert w edgu (bo edge ma dwa wierzcho�ki)
    int i = 0, count = 0;

    cout << "Generating graph...\n";
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

    for (int i = 0; i < vertCount; i++) {
        
        count = 0;
        for (int j = 0; j < edgeCount; j++) {
            if (edges[j][0] == i){ //je�li pierwszy vert edga, to wypisz drugi vert edga
                vertices[i].SetAdjacent(edges[j][1]);
                count++;
            }else if (edges[j][1] == i){ //a je�li drugi vert edga, to wypisz pierwszy vert edga
                vertices[i].SetAdjacent(edges[j][0]);
                count++;
            }
            else if (j == edgeCount-1 && count == 0) { // wypisz na ko�cu p�tli, gdy nie wszed� w �adnego ifa
                cout << "Isolated "; //Print �Isolated vertex� for the vertex having no degree.
            }
        }
    }
}

void GraphColoring(vector<Vert> &vertices) {
    vertices[0].SetColor(1);
    for (int i = 1; i < vertices.size(); i++) {
        for (auto adj : vertices[i].GetAdjacent()) {
            vertices[adj].GetColor();
        }
        vertices[i].SetColor(1 % colorCount);
    }
}

void PrintVertexInfo(vector<Vert> &vertices) {
    cout << "Vertex count = " << vertices.size() << "\n";
    cout << "Vertex list: \n";
    for (int i = 0; i < vertices.size(); i++) {
        cout << "(" << vertices[i].GetIndex() << ") | col = (" << vertices[i].GetColor() << ") | adjacent = { ";
        vertices[i].PrintAdjacent();
        cout << "}\n";
    }
}

int main(int argc, char** argv){
    srand(randSeed); //seeding random number generator

    vector<Vert> vertices = GenVertices();

    GenRandomGraph(vertices);
    PrintVertexInfo(vertices);

    return 0;
}
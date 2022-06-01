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
#include <algorithm>
#include <iterator>
#include <sstream>
#include <tuple>

#define vertCount 6
#define edgeCount 7
#define colorCount 4
#define randSeed 16462

using namespace std;

class Vert {

private:
    int idx, cr;
    string stringColor;
    vector<int> adjacentIndices;

public:
    int GetIndex() { return idx; }

    int GetColor() { return cr; }
    void SetColor(int color) { cr = color; }

    string GetStringColor() { return stringColor; }
    void SetStringColor(string strColor) { stringColor = strColor; }

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

vector<Vert> GenVertices() {
    vector<Vert> vertices;
    cout << "Generating vertex list...\n";
    for (int i = 0; i < vertCount; i++) {
        Vert currentVertex(i, 0);
        vertices.push_back(currentVertex); //add to the end of list
    }
    return vertices;
};

void GenRandomGraph(vector<Vert>& vertices) {
    /*
        - Ka�dy wierzcho�ek musi mie� conajmniej jedn� kraw�d� = obs�uga wyizolowanych wierzcho�k�w
        - Nie mo�e by� duplikat�w kraw�dzi.

        Wsparcie: https://www.tutorialspoint.com/cplusplus-program-to-create-a-random-graph-using-random-edge-generation
    */

    int edges[edgeCount][2]; //warto�� to wierzcho�ek, pierwszy wymiar to index edga, drugi to pierwszy lub drugi vert w edgu (bo edge ma dwa wierzcho�ki)
    int i = 0, count = 0;

    cout << "Generating graph...\n";
    while (i < edgeCount) {
        //for (int i = 0; i < edgeCount; i++) { // chcia�em zrobi� wersj� z forem zamiast while, ale z jakiego� powodu nie dzia�a tak samo... no idea why.

        edges[i][0] = rand() % vertCount;
        edges[i][1] = rand() % vertCount;

        if (edges[i][0] == edges[i][1]) { // je�li oba ko�ce edga nie s� tym samym wierzcho�kiem
            //cout << "[" << i << "] " << " - (" << edges[i][0] << ") --- (" << edges[i][1] << ")\n"; //bad edges
            continue;
        }
        else
        {
            for (int j = 0; j < i; j++) {
                if ((edges[i][0] == edges[j][0] && edges[i][1] == edges[j][1])
                    || (edges[i][0] == edges[j][1] && edges[i][1] == edges[j][0]))
                    i--;
            }
        }
        i++;
    }

    for (int i = 0; i < vertCount; i++) {

        count = 0;
        for (int j = 0; j < edgeCount; j++) {
            if (edges[j][0] == i) { //je�li pierwszy vert edga, to wypisz drugi vert edga
                vertices[i].SetAdjacent(edges[j][1]);
                count++;
            }
            else if (edges[j][1] == i) { //a je�li drugi vert edga, to wypisz pierwszy vert edga
                vertices[i].SetAdjacent(edges[j][0]);
                count++;
            }
            else if (j == edgeCount - 1 && count == 0) { // wypisz na ko�cu p�tli, gdy nie wszed� w �adnego ifa
                cout << "Isolated "; //Print �Isolated vertex� for the vertex having no degree.
            }
        }
    }
}

void GraphColoring(vector<Vert>& vertices, bool showInfo) {
    cout << "Coloring graph...\n";

    vertices[0].SetColor(1); //first vert is always color 1
    for (int i = 1; i < vertices.size(); i++) {
        if (showInfo) {
            cout << "\n### GRAPH COLORING ###\n";
            cout << "\nColored Vert [" << i << "]" << endl;
            cout << "\nAvailable colors: " << endl;
        }

        vector<int> notAllowedColors;

        for (auto adj : vertices[i].GetAdjacent()) {
            int adjColor = vertices[adj].GetColor();
            if (adjColor != 0) {
                notAllowedColors.push_back(adjColor);
                if (showInfo) { cout << "Adj (" << vertices[adj].GetIndex() << ") - Not allowed color = " << adjColor << endl; }
            }
        }

        vector<int> allowedColors(colorCount);
        for (int i = 0; i < colorCount; i++) {
            if (find(notAllowedColors.begin(), notAllowedColors.end(), i + 1) == notAllowedColors.end()) {
                allowedColors[i] = i + 1;
                if (showInfo) { cout << "Allowed color = " << allowedColors[i] << endl; }
            }
        }
        allowedColors.erase(remove(allowedColors.begin(), allowedColors.end(), 0), allowedColors.end());
        int minColor = *min_element(allowedColors.begin(), allowedColors.end());
        vertices[i].SetColor(minColor);
        if (showInfo) { cout << "Color set to  = " << minColor << endl; }
    }
}

void PrintVertexInfo(vector<Vert>& vertices) {
    cout << "\n### VERTEX INFO ###\n";
    cout << "Vertex count = " << vertices.size() << endl;
    cout << "Available colors = " << colorCount << endl;
    cout << "Vertex list: \n";

    for (int i = 0; i < vertices.size(); i++) {
        cout << "(" << vertices[i].GetIndex() << ") | col = (" << vertices[i].GetColor() << ") | adjacent = { ";
        vertices[i].PrintAdjacent();
        cout << "}\n";
    }
}

string vis(vector<Vert> vertices) {
    using namespace std;
    /*
        graph G {
    0 [style=filled fillcolor=red]
    1 [style=filled fillcolor=red]
    2 [style=filled fillcolor=blue]
    3 [style=filled fillcolor=green]
    4 [style=filled fillcolor=blue]
    5 [style=filled fillcolor=red]

    0--5

    1--4
    1--3
    1--2

    2--3
    2--1

    3--2
    3--5
    3--1

    4--5
    4--1

    5--4
    5--3
    5--0
    }
    */
    stringstream out;
    out << "graph G {" << endl;
    int num = 0;
    for (auto vert : vertices) {
        out << vert.GetIndex() << " [ style = filled fillcolor = " << vert.GetColor() << "] " << endl;
        num++;
    }

    for (auto vert : vertices) {
        for (auto adj : vert.GetAdjacent()) {
            out << vert.GetIndex() << " -- " << adj << endl;
        }
    }

    out << "}" << endl;
    return out.str();
}

tuple<int, string> ConvertVertIdColorToString(int id) {
    vector<string> stringColors;
    if (id == 1) return make_tuple(1, "red");
    if (id == 2) return make_tuple(2, "green");
    if (id == 3) return make_tuple(3, "blue");
    if (id == 4) return make_tuple(4, "yellow");
    if (id == 5) return make_tuple(5, "cyan");
    //throw std::invalid_argument("id");
}

int main(int argc, char** argv) {
    srand(randSeed); //seeding random number generator

    vector<Vert> vertices = GenVertices();

    GenRandomGraph(vertices);
    GraphColoring(vertices, false);
    PrintVertexInfo(vertices);

    //cout<<vis(vertices);

    return 0;
}
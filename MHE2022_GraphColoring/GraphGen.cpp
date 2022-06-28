#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <random>

using namespace std;

class Graph {
public:
    class Vert {
    public:
        int index;
        vector<int> adjacentIndices;

        void SetAdjacent(int index) { adjacentIndices.push_back(index); }
        void SetAdjacentVector(vector<int> adjacentVector) { adjacentIndices = adjacentVector; }

        vector<int> GetAdjacent() { return adjacentIndices; }

        Vert(int idx) {
            index = idx;
        }
    };

    vector<Vert> vertices;
    int vertCount, edgeCount, isolatedCount, badEdgesCount;

    void GenerateGraph() {
        // problem
        /*
            - Kazdy wierzcholek musi miec conajmniej jedna krawedz = obsluga wyizolowanych wierzcholkow
            - Nie moze byc duplikatow krawedzi.

            Wsparcie: https://www.tutorialspoint.com/cplusplus-program-to-create-a-random-graph-using-random-edge-generation
        */
        vector<vector<int>> edges(edgeCount, vector<int>(2));
        int i = 0, count = 0;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis1(0, vertCount);
        uniform_int_distribution<> dis2(0, vertCount);

        cout << "Generating graph...\n";
        while (i < edgeCount) {
            //for (int i = 0; i < edgeCount; i++) { // chcialem zrobic wersje z forem zamiast while, ale z jakiegos powodu nie dziala tak samo... no idea why.

            edges.at(i).at(0) = dis1(gen);
            edges.at(i).at(1) = dis2(gen);


            if (edges.at(i).at(0) == edges.at(i).at(1)) { // jesli oba konce edga nie sa tym samym wierzcholku
                badEdgesCount++;
                continue;
            }
            else
            {
                for (int j = 0; j < i; j++) {
                    if ((edges.at(i).at(0) == edges.at(j).at(0) && edges.at(i).at(1) == edges.at(j).at(1))
                        || (edges.at(i).at(0) == edges.at(j).at(1) && edges.at(i).at(1) == edges.at(j).at(0)))
                        i--;
                }
            }
            i++;
        }

        for (int i = 0; i < vertCount; i++) {
            count = 0;
            for (int j = 0; j < edgeCount; j++) {
                if (edges.at(j).at(0) == vertCount || edges.at(j).at(1) == vertCount) {
                    /*z jakiegoœ powodu znajduje s¹siada z indexem o ca³kowiej liczby vertów, gdzie indexy s¹ w zakresie o jeden mniej
                    przez co siê wysypuje, st¹d obs³uga takiego przypadku  */
                    continue;
                }
                if (edges.at(j).at(0) == i) { //jezli pierwszy vert edga, to wypisz drugi vert edga

                    vertices[i].SetAdjacent(edges.at(j).at(1));
                    count++;
                }
                else if (edges.at(j).at(1) == i) { //a jezli drugi vert edga, to wypisz pierwszy vert edga

                    vertices[i].SetAdjacent(edges.at(j).at(0));
                    count++;
                }
                else if (j == edgeCount - 1 && count == 0) { // wypisz na koncu petli, gdy nie wszedl w zadnego ifa
                    isolatedCount++;
                }
            }
        }
    }


    Graph(int vertcount, int edgecount, vector<Vert> vertVector) {
        vertCount = vertcount;
        edgeCount = edgecount;
        vertices = vertVector;
    }
    Graph() {}
};

vector<Graph::Vert> GenerateVertices(int vertCount) {
    cout << "Generating vertex list...\n";
    vector<Graph::Vert> vertices;
    for (int i = 0; i < vertCount; i++) {
        Graph::Vert currentVertex(i);
        vertices.push_back(currentVertex); //add to the end of list
    }
    return vertices;
};

void SaveGraphToFile(Graph graph) {
    cout << "Saving graph to file...\n";

    stringstream out;
    for (auto vert : graph.vertices) {
        for (auto adj : vert.GetAdjacent()) {
            out << adj << " ";
        }
        out << endl;
    }

    ofstream myfile;
    myfile.open("myGraph.txt");
    myfile << out.str();
    myfile.close();
    return;
}
int main(int argc, char** argv) {
    vector<int> GraphArgs(4);

    if (argc > 1) {
        //cout << "argv[1] = " << argv[1] << endl;
    }
    else {
        cout << "No file name entered.\n\n";
        return -1;
    }
    ifstream infile(argv[1]); //otwiera plik

    if (infile.is_open() && infile.good()) { //check czy otwarty
        string line = "";
        int i = 0;
        while (getline(infile, line)) {
            GraphArgs.at(i) = stoi(line);
            i++;
        }
    }
    else {
        cout << "Failed to open file..\n\n";
        return -2;
    }
    int vertCount = GraphArgs.at(0),
        edgeCount = GraphArgs.at(1),
        iterations = GraphArgs.at(2),
        maxColorCount = GraphArgs.at(3);

    cout << "Arguments from file: " << endl;
    cout << "VertCount = " << vertCount << endl;
    cout << "EdgeCount = " << edgeCount << endl;
    cout << "Iterations = " << iterations << endl;
    cout << "Max color count = " << maxColorCount << endl;

    cout << endl;

    Graph graph(vertCount, edgeCount, GenerateVertices(vertCount));
    SaveGraphToFile(graph);
    return 0;
}

/*
    Projekt na zajecia MHE 2022 PJATK
    Temat: Graph Coloring
    Autor: Jaromir Daruk
    Index: s16462

*/

/*
    Parametry na ktorych bedzie oparty graf i generowanie rozwiazan dla niego.
    TODO: Przerobienie na wprowadzanie tych zmiennych poprzez argumenty z linii polecen.

    https://dreampuf.github.io/GraphvizOnline/
 */

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <random>

using namespace std;

class Graph {
    private:
        class Vert {
        private:
            int index, colorIndex;
            string hexColor;
            vector<int> adjacentIndices;

        public:
            int GetIndex() { return index; }
            int GetColorIndex() { return colorIndex; }
            void SetColor(int color) {
                colorIndex = color; 
                srand(color+1);
                char hex_char[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
                int i;

                hexColor = "";
                if (color == 0) {
                    hexColor = "ffffff";
                    return;
                }
                else {
                    for (i = 0; i < 6; i++)
                    {
                        hexColor += hex_char[rand() % 16];
                    }
                    return;
                }
            }

            string GetHexColor() { return hexColor; }
            void SetAdjacent(int index) { adjacentIndices.push_back(index); }
            vector<int> GetAdjacent() { return adjacentIndices; }

            void PrintAdjacent() {
                for (auto adj : adjacentIndices) {
                    cout << adj << " ";
                }
            }

            Vert(int idx) {
                index = idx;
                SetColor(0);
            }
        };
     
    public:
        vector<Vert> vertices;
        int vertCount, edgeCount, colorCount, isolatedCount, coloredCount, badEdgesCount, score;

        void PrintStatistics() {
            cout << "Graph has " << vertCount << " vertices.\n";
            cout << "Graph has " << edgeCount << " edges.\n";
            cout << "Graph has " << isolatedCount << " isolated vertices.\n";
            for (int i = 0; i < vertices.size(); i++) {
                if (vertices[i].GetColorIndex() != 0) {
                    coloredCount++;
                }
            }
            cout << "Graph has " << coloredCount << " colored vertices.\n";
            cout << "Graph has " << badEdgesCount << " edges that are connected to single vertex.\n";
        }

        vector<Vert> GenerateVertices() {
            vector<Vert> vertices;
            cout << "Generating vertex list...\n";
            for (int i = 0; i < vertCount; i++) {
                Vert currentVertex(i);
                vertices.push_back(currentVertex); //add to the end of list
            }
            return vertices;
        };


        void GenerateGraph() {
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
                        /*z jakiegoś powodu znajduje sąsiada z indexem o całkowiej liczby vertów, gdzie indexy są w zakresie o jeden mniej
                        przez co się wysypuje, stąd obsługa takiego przypadku  */
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
        void GraphColoring(bool showInfo) {
            cout << "Coloring graph...\n";
            
            vertices[0].SetColor(1); //pierwszy vert jest zawsze pierwszym kolorem
            for (int i = 1; i < vertices.size(); i++) {
                if (showInfo) {
                    cout << "\nColored Vert [" << i << "]" << endl;
                    cout << "\nAvailable colors: " << endl;
                }

                vector<int> notAllowedColors;

                for (auto adj : vertices[i].GetAdjacent()) {
                    int adjColor = vertices[adj].GetColorIndex();
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
                bool canColor = false;
                int notAllowedColorsCount = 0;
                for (int i = 0; i < allowedColors.size(); i++) {
                    //cout << "(" << i << ") = " << allowedColors.at(i) << endl;
                    if (allowedColors.at(i) == 0) {
                        notAllowedColorsCount++;
                    }
                }
                if (notAllowedColorsCount == allowedColors.size()) {
                    vertices[i].SetColor(0);
                    canColor = false;
                    coloredCount++;
                    if (showInfo) { cout << "Can't color, not enough colors" << endl; }
                }
                else {
                    canColor = true;
                }

                if(canColor == true){
                    allowedColors.erase(remove(allowedColors.begin(), allowedColors.end(), 0), allowedColors.end());
                    int minColor = *min_element(allowedColors.begin(), allowedColors.end());
                    vertices[i].SetColor(minColor);
                    if (showInfo) { cout << "Color set to  = " << minColor << endl; }
                }
            }
        }
        void GraphVizToFile() {
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
                out << vert.GetIndex() << " [ style = filled fillcolor = \"#" << vert.GetHexColor() << "\"] " << endl;
                num++;
            }

            for (auto vert : vertices) {
                for (auto adj : vert.GetAdjacent()) {
                        out << vert.GetIndex() << " -- " << adj << endl;
                }
            }

            out << "}" << endl;
            ofstream myfile;
            myfile.open("GraphViz.txt");
            myfile << out.str();
            myfile.close();
            return;
        }

        int EvauluateGraph() {
            score = 0;
            score = score + coloredCount*5 + badEdgesCount*2;
            cout << "Graph score = " << score <<endl;
            return score;
        }

        void PrintVertList() {
            for (int i = 0; i < vertices.size(); i++) {
                cout << "(" << vertices[i].GetIndex() << ") | col = (" << vertices[i].GetColorIndex() << ") | adjacent = { ";
                vertices[i].PrintAdjacent();
                cout << "}\n";
            }
        }

        Graph(int vertcount, int edgecount) {
            vertCount = vertcount;
            edgeCount = edgecount;
            vertices = GenerateVertices();
            GenerateGraph();
        }
};

class Colorizer {
    public:
        int colorCount;

        void Colorize(Graph &graph, bool showInfo) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(1, 2);
            cout << "Coloring...\n";
            for (int i = 0; i < graph.vertices.size(); i++) {
                int color = dis(gen);
                graph.vertices[i].SetColor(color);
                if (showInfo) {
                    cout << "Vert [" << i << "] = " << color << endl;
                }
            }
        }

        Colorizer(Graph& graph) {
            colorCount = graph.vertices.size();
        }

        int Evaluate(Graph& graph) {
            int score = 0;
            for (int i = 0; i < graph.vertices.size(); i++) {
               //cout << "\nvert = " << graph.vertices[i].GetIndex() << endl;
                //graph.vertices[i].PrintAdjacent();
                for (auto adj : graph.vertices[i].GetAdjacent()) {
                    //cout << "adj idx = " << adj << endl;
                    int adjColor = graph.vertices[adj].GetColorIndex();
                    if (graph.vertices[i].GetColorIndex() == adjColor)
                    {
                        score++;
                    }

                }
            }
            score = score + colorCount;
            
            cout << "\n## EVALUATION ##" << endl;
            cout << "colorCount = " << colorCount << endl;
            cout << "adjacent color error = " << score - colorCount << endl;
            cout << "Score = " << score << endl;
            return score;
        }
};


int main(int argc, char** argv) {
    srand(16462);
    vector<int> GraphArgs(3);

    if (argc > 1) {
        cout << "argv[1] = " << argv[1] << endl;
    }
    else {
        cout << "No file name entered.";
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
        cout << "Failed to open file..";
        return -2;
    }

    cout << "Arguments from file: " << endl;
    cout << "VertCount = " << GraphArgs.at(0) << endl;
    cout << "EdgeCount = " << GraphArgs.at(1) << endl;

    Graph graph(GraphArgs.at(0), GraphArgs.at(1));
    Colorizer colorizer(graph);

    colorizer.Colorize(graph, false);
    //graph.PrintVertList();
    //graph.PrintStatistics();
    colorizer.Evaluate(graph);

    graph.GraphVizToFile();

    return 0;
}
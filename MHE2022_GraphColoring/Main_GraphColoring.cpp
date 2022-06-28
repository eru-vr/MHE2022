﻿/*
    Projekt na zajecia MHE 2022 PJATK
    Temat: Graph Coloring
    Autor: Jaromir Daruk
    Index: s16462
*/

/*
    Graph vizualizer 
    https://dreampuf.github.io/GraphvizOnline/
 */

/*
    TODO: Algorytymy:
            - Wspinaczkowy
            - Tabu
            - Genetyczny
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
#include <array>
#include <chrono>

using namespace std;


class Graph {
    public:
        class Vert {
        public:
            int index, colorIndex;
            string hexColor;
            vector<int> adjacentIndices;
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
            void SetAdjacentVector(vector<int> adjacentVector) { adjacentIndices = adjacentVector; }

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
     
        vector<Vert> vertices;
        int vertCount, edgeCount, isolatedCount, coloredVertCount, badEdgesCount, colorCount;

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
      
        void PrintVertList() {
            cout << "## Vertex list ##\n";
            for (int i = 0; i < vertices.size(); i++) {
                cout << "(" << vertices[i].GetIndex() << ") | col = (" << vertices[i].GetColorIndex() << ") | adjacent = { ";
                vertices[i].PrintAdjacent();
                cout << "}\n";
            }
            cout << endl;
        }
        void PrintStatistics() {
            cout << "## Graph Stats ##\n";
            cout << "Graph has " << vertCount << " vertices.\n";
            cout << "Graph has " << edgeCount << " edges.\n";
            cout << "Graph has " << isolatedCount << " isolated vertices.\n";
            for (int i = 0; i < vertices.size(); i++) {
                if (vertices[i].GetColorIndex() != 0) {
                    coloredVertCount++;
                }
            }
            cout << "Graph has " << coloredVertCount << " colored vertices.\n";
            cout << "Graph has " << badEdgesCount << " edges that are connected to single vertex.\n";
            cout << endl;
        }

        Graph(int vertcount, int edgecount, vector<Vert> vertVector) {
            vertCount = vertcount;
            edgeCount = edgecount;
            vertices = vertVector;
        }
        Graph() {}

};


class Colorizer {
    // Solution
public:
    Graph RandomColorize(Graph graph, int colorCount, bool showInfo) {
        if (colorCount < 2) { cout << "colorCount has to be more than 2!\n"; 
            return graph; }

        Graph coloredGraph = graph;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, colorCount);
        coloredGraph.colorCount = colorCount;
        for (int i = 0; i < coloredGraph.vertices.size(); i++) {
            int color = dis(gen);
            coloredGraph.vertices[i].SetColor(color);
            if (showInfo) cout << "Vert [" << i << "] = " << color << endl;
        }
        return coloredGraph;
    }
    Colorizer() {}
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
vector<Graph::Vert> LoadGraphFromFile(string fname, bool showInfo) {
    cout << "Loading graph from file...\n";

    vector<Graph::Vert> vertices;

    ifstream infile(fname); //otwiera plik
    if (infile.is_open() && infile.good()) { //check czy otwarty
        string line = "";
        int i = 0;
        while (getline(infile, line)) {
            Graph::Vert currentVertex(i);
            stringstream stringadj(line);
            int num;
            vector<int> adj;
            while (stringadj >> num)
                adj.push_back(num);
            if (showInfo) cout << "[" << currentVertex.GetIndex() << "] - " << line << endl;

            currentVertex.SetAdjacentVector(adj);
            //GraphArgs.at(i) = stoi(line);
            vertices.push_back(currentVertex); //add to the end of list
            i++;
        }
        return vertices;
    }
    else {
        cout << "Failed to open Graph file..\n\n";
        return vertices;
    }
}
void GraphVizToFile(Graph graph, string fname) {
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
    for (auto vert : graph.vertices) {
        out << vert.GetIndex() << " [ style = filled fillcolor = \"#" << vert.GetHexColor() << "\"] " << endl;
        num++;
    }

    for (auto vert : graph.vertices) {
        for (auto adj : vert.GetAdjacent()) {
            out << vert.GetIndex() << " -- " << adj << endl;
        }
    }

    out << "}" << endl;
    ofstream myfile;
    myfile.open(fname);
    myfile << out.str();
    myfile.close();
    return;
}

vector<Graph> GetNeighbors(Graph coloredGraph, bool showInfo) {
    vector<Graph> neighbourGraphs;
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> dis(1, coloredGraph.colorCount);
    uniform_int_distribution<> dis2(0, coloredGraph.vertCount-1);
    if (showInfo) { cout << "### Neighborhood ###\n"; }
    for (int i = 0; i < coloredGraph.vertCount/5; i++) {
        Graph neighbour = coloredGraph;
        int newColor = dis(gen);
        int vertIndex = dis2(gen);
        neighbour.vertices[vertIndex].SetColor(newColor);
        neighbourGraphs.push_back(neighbour);

        if (showInfo) { cout << "vert (" << vertIndex << ") - col (" << newColor << ")\n"; }
    }
    return neighbourGraphs;
}

int Evaluate(Graph coloredGraph, bool showInfo) {
    int errors = 0;
    for (int i = 0; i < coloredGraph.vertices.size(); i++) {
        for (auto adj : coloredGraph.vertices[i].GetAdjacent()) {
            int adjColor = coloredGraph.vertices[adj].GetColorIndex();
            if (coloredGraph.vertices[i].GetColorIndex() == adjColor){
                errors++;
            }
        }
    }
    if (showInfo) cout << "Errors (score) = " << errors << endl;
    return errors;
}

Graph GetBestNeighbor(Graph solution) {
    int bestNeighborScore, currentNeighborScore;
    Graph bestNeighbor = solution;
    auto neighbors = GetNeighbors(solution, false);

    // Finding best neighbor
    for (int k = 0; k < neighbors.size(); k++) {
        currentNeighborScore = Evaluate(neighbors[k], false);
        bestNeighborScore = Evaluate(bestNeighbor, false);

        //cout << "currentNeighborScore = " << currentNeighborScore <<" < " <<" bestNeighborScore  = " << bestNeighborScore << endl;
        if (currentNeighborScore < bestNeighborScore) {
            bestNeighbor = neighbors[k];
        }
    }
    return bestNeighbor;
}

Graph Algoritm_HillClimbing(Graph graph, Colorizer colorizer, int colorCount, int iterations, bool showInfo) {
    cout << "HillClimbing started...\n";
    Graph bestNeighbor;
    int solutionScore, bestNeighborScore, iteration_counter = 0;
    Graph solution = colorizer.RandomColorize(graph, colorCount, false);
    for(int i = 0; i < iterations; i++){
        bestNeighbor = GetBestNeighbor(solution);
        solutionScore = Evaluate(solution, false);
        bestNeighborScore = Evaluate(bestNeighbor, false);

        if (bestNeighborScore < solutionScore) {
            solution = bestNeighbor;
            if (showInfo) cout << "## " << bestNeighborScore << " < " << solutionScore << endl;
        }
        else {
            return solution;
        }
    }
}

Graph Algoritm_HillClimbingRandom(Graph graph, Colorizer colorizer, int colorCount, int iterations, bool showInfo) {
    cout << "Random HillClimbing started...\n";
    Graph bestSolution;
    int solScore, bestScore;
    Graph solution = colorizer.RandomColorize(graph, colorCount, false);
    for(int i = 0; i < iterations; i++){
        if (i == 0) bestSolution = solution;

        random_device rd;
        mt19937 gen(rd());
        auto neighbors = GetNeighbors(solution, false);

        uniform_int_distribution<> dis(0, neighbors.size() - 1);
        int neighborIndex = dis(gen);
        Graph neighbor = neighbors[neighborIndex];

        solScore = Evaluate(solution, false);
        bestScore = Evaluate(bestSolution, false);
        if (solScore < bestScore) {
            bestSolution = solution;
            if (showInfo) cout << "## ";
        }
        if (showInfo) cout << solScore << " < " << bestScore << endl;
        solution = neighbor;
    }
    return bestSolution;
}

int main(int argc, char** argv) {
    srand(16462);
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

    //TODO aby pokazywało ilość wierzchołków z pliku. z Edgami będzie problem. Info że po prostu ładuje z pliku

    cout << "Arguments from file: " << endl;
    cout << "VertCount = " << vertCount << endl;
    cout << "EdgeCount = " << edgeCount << endl;
    cout << "Iterations = " << iterations << endl;
    cout << "Max color count = " << maxColorCount << endl;

    cout << endl;

    //Graph graph(vertCount, edgeCount, GenerateVertices(vertCount));
    //SaveGraphToFile(graph);

    Graph graph(vertCount, edgeCount, LoadGraphFromFile("myGraph.txt", false));
    //TODO aby pokazywało ilość wierzchołków z pliku. z Edgami będzie problem. Info że po prostu ładuje z pliku
    //TODO VertCount i edge count tu jest zbyteczny jak ładuje z pliku
    //TODO jeśli failed to load graph life to return

    //graph.PrintVertList();
    //cout << endl;
    //graph.PrintStatistics(); // doesn't count isolated vertices and bad edges if vertices loaded from file

    Colorizer colorizer;

    auto calculation_start = chrono::steady_clock::now();
    Graph HillClimbSolution = Algoritm_HillClimbing(graph, colorizer, maxColorCount, iterations, false);
    auto calculation_end = chrono::steady_clock::now();
    chrono::duration<double> calculation_duration = calculation_end - calculation_start;
    cout << "Calculation_time: " << calculation_duration.count() << endl;

    Evaluate(HillClimbSolution, true);
    GraphVizToFile(HillClimbSolution, "VIZ_HillClimb.txt");
    cout << endl;

    calculation_start = chrono::steady_clock::now();
    Graph HillClimbRandomSolution = Algoritm_HillClimbingRandom(graph, colorizer, maxColorCount, iterations, false);
    calculation_end = chrono::steady_clock::now();
    calculation_duration = calculation_end - calculation_start;
    cout << "Calculation_time: " << calculation_duration.count() << endl;

    Evaluate(HillClimbRandomSolution, true);
    GraphVizToFile(HillClimbRandomSolution, "VIZ_HillClimbRandom.txt");
    cout << endl;



    cout << "\n\n";
    return 0;
}
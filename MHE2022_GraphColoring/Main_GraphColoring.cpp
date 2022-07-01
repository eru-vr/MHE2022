/*
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
#include <array>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <random>
#include <chrono>
#include <cmath>

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

        Graph(vector<Vert> vertVector, int edgecount) { // Graph Generating
            vertices = vertVector;
            edgeCount = edgecount;
            vertCount = vertVector.size();
        }
        Graph(vector<Vert> vertVector) { // Graph from file
            vertices = vertVector;
            vertCount = vertVector.size();
            edgeCount = 0;
            isolatedCount = 0;
            coloredVertCount = 0;
            badEdgesCount = 0;
            colorCount = 0;
        }
        Graph() {}

};

bool operator==(Graph a, Graph b)
{
    for (int i = 0; i < a.vertices.size();i++) {
        if (a.vertices[i].GetColorIndex() != b.vertices[i].GetColorIndex()) {
            return false;
        }
    }
    return true;
}

class Colorizer {
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
        vertices.push_back(currentVertex);
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
        cout << endl;
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
    out << "// colorCount = " << graph.colorCount << endl;
    ofstream myfile;
    myfile.open(fname);
    myfile << out.str();
    myfile.close();
    return;
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

vector<Graph> GetNeighbors(Graph coloredGraph, bool showInfo) {
    vector<Graph> neighbourGraphs;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, coloredGraph.colorCount);
    uniform_int_distribution<> dis2(0, coloredGraph.vertCount - 1);
    int newColor, vertIndex;
    if (showInfo) { cout << "### Neighborhood ###\n"; }
    for (int i = 0; i < coloredGraph.vertCount / 5; i++) {
        Graph neighbour = coloredGraph;
        newColor = dis(gen);
        vertIndex = dis2(gen);
        neighbour.vertices[vertIndex].SetColor(newColor);
        neighbourGraphs.push_back(neighbour);

        if (showInfo) { cout << "vert (" << vertIndex << ") - col (" << newColor << ")\n"; }
    }
    return neighbourGraphs;
}

Graph GetBestNeighborOf(Graph solution) {
    int bestNeighborScore, currentNeighborScore;
    Graph bestNeighbor = solution;
    vector<Graph> neighbors = GetNeighbors(solution, false);

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

Graph GetBestNeighborFrom(vector<Graph> neighbors) {
    int currentNeighborScore, bestNeighborScore;
    Graph currentNeighbor, bestNeighbor;

    for (int k = 0; k < neighbors.size(); k++) {
        currentNeighborScore = Evaluate(neighbors[k], false);
        if (k == 0) bestNeighborScore = currentNeighborScore;

        //cout << "currentNeighborScore = " << currentNeighborScore <<" < " <<" bestNeighborScore  = " << bestNeighborScore << endl;
        if (currentNeighborScore <= bestNeighborScore) {
            bestNeighbor = neighbors[k];
        }
    }
    //cout << Evaluate(bestNeighbor, false) <<endl;

    return bestNeighbor;
}


Graph Algoritm_HillClimbing(Graph graph, 
                            Colorizer colorizer, 
                            int colorCount, 
                            int iterations, 
                            bool showInfo) {
    cout << "HillClimbing started...\n";
    int solutionScore, bestNeighborScore;
    Graph bestNeighbor, solution = colorizer.RandomColorize(graph, colorCount, false);

    for(int i = 0; i < iterations; i++){
        bestNeighbor = GetBestNeighborOf(solution);
        solutionScore = Evaluate(solution, false);
        bestNeighborScore = Evaluate(bestNeighbor, false);

        if (bestNeighborScore < solutionScore) {
            solution = bestNeighbor;
            if (showInfo) cout << "## " << bestNeighborScore << " < " << solutionScore << " [" << i << "]" << endl;
        }
    }
    return solution;
}

Graph Algoritm_HillClimbingRandom(Graph graph, 
                                  Colorizer colorizer, 
                                  int colorCount, 
                                  int iterations, 
                                  bool showInfo) {
    cout << "Random HillClimbing started...\n";
    int solutionScore, randomNeighborScore, neighborIndex;
    Graph randomNeighbor, solution = colorizer.RandomColorize(graph, colorCount, false);
    random_device rd;

    for(int i = 0; i < iterations; i++){
        vector<Graph> neighbors = GetNeighbors(solution, false);
        uniform_int_distribution<> dis(0, neighbors.size() - 1);
        mt19937 gen(rd());
        neighborIndex = dis(gen);
        randomNeighbor = neighbors[neighborIndex];

        solutionScore = Evaluate(solution, false);
        randomNeighborScore = Evaluate(randomNeighbor, false);
        if (randomNeighborScore < solutionScore) {
            solution = randomNeighbor;
            if (showInfo) cout << "## " << randomNeighborScore << " < " << solutionScore << " ["<<i<<"]" <<endl;
        }
    }
    return solution;
} 

Graph Algoritm_Tabu(Graph graph, 
                    Colorizer colorizer, 
                    int colorCount, 
                    int iterations, 
                    int maxTabuSize, 
                    bool showInfo) {
    cout << "Tabu started...\n";
    int solutionScore, bestNeighborScore;
    Graph bestNonTabuNeighbor, bestsolution, solution = colorizer.RandomColorize(graph, colorCount, false);
    list<Graph> tabu;
    for (int i = 0; i < iterations; i++) {
        if (i == 0) bestsolution = solution;
        //weź sąsiadów
        //sprawdź czy są jacyś w tabu
        //weź z listy sąsiadów najlepszego który nie jest w tabu, nieważne czy lepszy od aktualnego score.
        //ale zapisuj najlepszy score
        vector<Graph> nonTabu, neighbors = GetNeighbors(solution, false);
        for (auto neighbor : neighbors) {
            if (find(tabu.begin(), tabu.end(), neighbor) == tabu.end()) {
                nonTabu.push_back(neighbor);
            }
        }
        solution = GetBestNeighborFrom(nonTabu);
        
        tabu.push_back(solution);
        if (maxTabuSize < tabu.size()) {
            tabu.pop_front();
        }

        if (showInfo) cout << "## " << Evaluate(solution, false) << " [" << i << "]" << "[size:" << tabu.size()<<"]" << endl;
        if (Evaluate(solution, false) < Evaluate(bestsolution,false)) {
            bestsolution = solution;
        }
    }
    return bestsolution;
}

Graph Algoritm_SimulatedAnnealing(Graph graph,
                                Colorizer colorizer,
                                int colorCount,
                                int iterations,
                                int temperature,
                                bool showInfo) {
    cout << "Simulated Annealing started...\n";
    int solutionScore, randomNeighborScore, neighborIndex;
    Graph randomNeighbor, solution = colorizer.RandomColorize(graph, colorCount, false);
    Graph bestSolution = solution;
    random_device rd;
    mt19937 gen(rd());

    auto temp = [&](double t) {
        return temperature / t;
    };

    for (int i = 0; i < iterations; i++) {
        vector<Graph> neighbors = GetNeighbors(solution, false);
        uniform_int_distribution<> dis(0, neighbors.size() - 1);
        neighborIndex = dis(gen);
        randomNeighbor = neighbors[neighborIndex];

        solutionScore = Evaluate(solution, false);
        randomNeighborScore = Evaluate(randomNeighbor, false);

        if (randomNeighborScore < solutionScore) {
            solution = randomNeighbor;
            if (showInfo) cout << "## " << randomNeighborScore << " < " << solutionScore << " [" << i << "] [t:" << temp(i) << "]" << endl;
        }
        else {
            uniform_real_distribution<double> distr(0, 1);
            auto u = distr(gen);
            if (u < exp(-abs(Evaluate(solution, false) - Evaluate(randomNeighbor, false)) / temp(i))) {
                solution = randomNeighbor;
                //if (showInfo) cout << "### " << Evaluate(randomNeighbor, false) << " < " << Evaluate(solution, false) << " [" << i << "] [" << temp(i + 1) << "]" << endl;
                //if (showInfo) cout << u << " < " << exp(-abs(Evaluate(solution, false) - Evaluate(randomNeighbor, false) / temp(i))) << endl;

            }
        }
        if (solutionScore < Evaluate(bestSolution, false)) {
            bestSolution = solution;
        }
    }
    return bestSolution;
}

void StartExperiment(string experimentName, 
                     Graph graph, 
                     Colorizer colorizer, 
                     int maxColorCount, 
                     int iterations,
                     int maxTabuSize,
                     int temperature,
                     bool showInfo) {

    Graph experiment;
    string fname;
    auto calculation_start = chrono::steady_clock::now();

    if (experimentName == "HillClimbing") {
        experiment = Algoritm_HillClimbing(graph, colorizer, maxColorCount, iterations, showInfo);
    } else if (experimentName == "HillClimbingRandom") {
        experiment = Algoritm_HillClimbingRandom(graph, colorizer, maxColorCount, iterations, showInfo);
    }else if (experimentName == "Tabu") {
        experiment = Algoritm_Tabu(graph, colorizer, maxColorCount, iterations, maxTabuSize, showInfo);
    }else if (experimentName == "SimulatedAnnealing") {
        experiment = Algoritm_SimulatedAnnealing(graph, colorizer, maxColorCount, iterations, temperature, showInfo);
    }
    auto calculation_end = chrono::steady_clock::now();
    chrono::duration<double> calculation_duration = calculation_end - calculation_start;
    cout << "Calculation_time: " << calculation_duration.count() << endl;

    Evaluate(experiment, true);
    fname = "VIZ_" + experimentName + ".txt";
    GraphVizToFile(experiment, fname);
    cout << endl;
}

int main(int argc, char** argv) {
    vector<int> GraphArgs(8);

    ifstream infile("args.txt"); //otwiera plik
    if (infile.is_open() && infile.good()) { //check czy otwarty
        string line = "";
        int i = 0;
        while (getline(infile, line) && i < GraphArgs.size()) {
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
        maxColorCount = GraphArgs.at(3),
        maxTabuSize = GraphArgs.at(4),
        temperature = GraphArgs.at(5),
        showInfo = GraphArgs.at(6),
        loadGraphFromFile = GraphArgs.at(7);

    cout << "File arguments: " << endl;
    cout << "VertCount = " << vertCount << endl;
    cout << "EdgeCount = " << edgeCount << endl;
    cout << "Iterations = " << iterations << endl;
    cout << "Max color count = " << maxColorCount << endl;
    cout << "Max Tabu Size = " << maxTabuSize << endl;
    cout << "Temperature = " << temperature << endl;
    cout << "Show info = " << showInfo << endl;
    cout << "Load graph from file = " << loadGraphFromFile << endl;

    cout << endl;

    Graph graph;
    Colorizer colorizer;

    if (loadGraphFromFile) {
        graph = Graph(LoadGraphFromFile("myGraph.txt", false));
    }
    else {
        graph = Graph(GenerateVertices(vertCount), edgeCount);
        graph.GenerateGraph();
        SaveGraphToFile(graph);
    }
    if(showInfo){
        graph.PrintVertList();
        cout << endl;
        graph.PrintStatistics(); // doesn't count isolated vertices and bad edges if vertices loaded from 
    }

    StartExperiment("HillClimbing", graph, colorizer, maxColorCount, iterations, maxTabuSize, temperature, showInfo);
    StartExperiment("HillClimbingRandom", graph, colorizer, maxColorCount, iterations, maxTabuSize, temperature, showInfo);
    StartExperiment("Tabu", graph, colorizer, maxColorCount, iterations, maxTabuSize, temperature, showInfo);
    StartExperiment("SimulatedAnnealing", graph, colorizer, maxColorCount, iterations, maxTabuSize, temperature, showInfo);

    cout << "\n";
    return 0;
}
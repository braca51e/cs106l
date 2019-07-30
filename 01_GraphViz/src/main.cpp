#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include "SimpleGraph.h"

using std::cout;	using std::endl;

const double kPi = 3.14159265358979323;
const double k_rep = 0.001;
const double k_attrac = 0.001;

void Welcome();
SimpleGraph preInit();
void forceDirect(SimpleGraph& graph);

// Main method
int main() {
    //Welcome();
    SimpleGraph myGraph = preInit();
    InitGraphVisualizer(myGraph);
    DrawGraph(myGraph);

    //do force direct
    double simTime;
    std::string ctrlInput;
    bool loop = true;
    cout << "Enter time for each simulation step: " << endl;
    std::cin >> simTime;
    time_t startTime = time(NULL);
    while(loop) {
        forceDirect(myGraph);
        double elapsedTime = difftime(time(NULL), startTime);
        if(elapsedTime >= simTime){
            DrawGraph(myGraph);
            cout << "Simulation step completed!" << endl;
            cout << "Press 1 to continue another step or 2 to terminated simulation!!" << endl;
            std::cin >> ctrlInput;
            if(ctrlInput.find("2") != std::string::npos){
                loop = false;
                cout << "Simulation terminated!" << endl;
            }
            //restart timer
            startTime = time(NULL);
        }
    }

    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
SimpleGraph preInit(){
    std::string diskGraphLoc;
    cout << "Enter file location in disk: " << endl;
    std::cin >> diskGraphLoc;
    std::ifstream graphFile(diskGraphLoc);
    //create graph from file
    SimpleGraph myGraph;
    std::string lineStr;
    bool firstLine = true;
    while(std::getline(graphFile, lineStr)){
        //read first line number of nodes
        if(firstLine){
            //Init according to assigment notes
            firstLine = false;
            int n = stoi(lineStr);
            for(auto k=0; k < n; ++k){
                Node tmp;
                tmp.x = cos((2*kPi*k)/(n));
                tmp.y = sin((2*kPi*k)/(n));
                myGraph.nodes.push_back(tmp);
            }
        }
        //it's an edge
        else if (lineStr.size() > 0){
            //find first int
            size_t space_pos = lineStr.find(" ");
            Edge tmp;
            unsigned long e_start = stoi(lineStr.substr(0, space_pos));
            unsigned long e_end = stoi(lineStr.substr(space_pos+1, lineStr.size()));
            tmp.start = e_start;
            tmp.end = e_end;
            myGraph.edges.push_back(tmp);
        }
    }

    return myGraph;
}

void forceDirect(SimpleGraph& graph){
    //compute repulsive forces
    //Init variables
    std::vector<Node> nodes = graph.nodes;
    std::vector<Edge> edges = graph.edges;
    double x0 = 0;
    double x1 = 0;
    double y0 = 0;
    double y1 = 0;
    double theta = 0;
    //compute for each pair of nodes
    for (unsigned long i=0; i < nodes.size(); ++i) {
        for (auto j=i+1; j < nodes.size(); ++j) {
            x0 = nodes[i].x;
            x1 = nodes[j].x;
            y0 = nodes[i].y;
            y1 = nodes[j].y;
            double delta_x0 = 0;
            double delta_x1 = 0;
            double delta_y0 = 0;
            double delta_y1 = 0;
            double F_rep = 0;

            F_rep = k_rep/(sqrt(pow(y1-y0,2)+pow(x1-x0, 2)));
            theta = atan2(y1-y0, x1-x0);

            delta_x0 -=  F_rep*cos(theta);
            delta_y0 -=  F_rep*sin(theta);
            delta_x1 +=  F_rep*cos(theta);
            delta_y1 +=  F_rep*sin(theta);
            //Adjust Node
            nodes[i].x += delta_x0;
            nodes[j].x += delta_x1;
            nodes[i].y += delta_y0;
            nodes[j].y += delta_y1;

            graph.nodes = nodes;
        }
    }
    //compute attracting forces
    for (unsigned long i=0; i < edges.size(); ++i) {
        size_t node_s = edges[i].start;
        size_t node_e = edges[i].end;
        Node node_1 = nodes[node_s];
        Node node_2 = nodes[node_e];
        x0 = node_1.x;
        x1 = node_2.x;
        y0 = node_1.y;
        y1 = node_2.y;
        double delta_x0 = 0;
        double delta_x1 = 0;
        double delta_y0 = 0;
        double delta_y1 = 0;
        double F_attra = 0;

        F_attra = k_attrac*(pow(x1-x0,2)+pow(y1-y0,2));
        theta = atan2(y1-y0, x1-x0);
        delta_x0 +=  F_attra*cos(theta);
        delta_y0 +=  F_attra*sin(theta);
        delta_x1 -=  F_attra*cos(theta);
        delta_y1 -=  F_attra*sin(theta);

        //readjust
        node_1.x += delta_x0;
        node_1.y += delta_y0;
        node_2.x += delta_x1;
        node_2.y += delta_y1;
        nodes[node_s] = node_1;
        nodes[node_e] = node_2;

        graph.nodes = nodes;
    }
}

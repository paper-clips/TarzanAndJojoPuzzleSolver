#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

int main()
{
    const string INFILE("input.txt");       //Change input file name
    const string OUTFILE("output.txt");
    ifstream input(INFILE);
    ofstream output(OUTFILE);
    istringstream instream;
    int r, c;
    int s_r, s_c;
    int j_r, j_c = -1;      // Jojo's location
    string line;
    string direction;

    // Check the input and output files
    if (!input.is_open()) {
        cout << "Error opening file: " << INFILE << endl;
        return 1;
    }
    if (!output.is_open()) {
        cout << "Error opening file: " << OUTFILE << endl;
        return 1;
    }

    cout << "Reading input..." << endl;

    // Read first line of input file
    getline(input, line);
    instream.str(line);
    instream >> r;
    instream >> c;
    instream.clear();

    // Read second line of input file
    getline(input, line);
    instream.str(line);
    instream >> s_r;
    instream >> s_c;
    instream.clear();

    vector<vector<string>> maze;                                            // Stores directions as they are, ex. S, N, NE, etc.
    vector<vector<int>> vertice(r+1, vector<int>(c+1, 0));                  // Stores every direction in maze as unique number, ex. S=0, N=1, NE=2, S=3, etc.
    typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;  // Store edge weights
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, EdgeWeightProperty> mazeGraph;    //Adjacency list graph
    mazeGraph g;

    maze.resize(r+1);   // Set maze row size
    int count = 0;
    vector<pair<int, pair<int, int>>> mem;      // Stores every vertex, each with location i (row) and j (column) in the maze

    // Store directional information for each vine in the maze
    for (int i = 1; i <= r; i++)
    {
        maze[i].resize(c+1);        // Set maze column size for each row
        getline(input, line);
        istringstream instream(line);

        for (int j = 1; j <= c; j++)
        {
            instream >> direction;

            maze[i][j] = direction;     // Save direction, as it is
            vertice[i][j] = count;      // Save direction as unique number
            count++;

            mem.push_back(make_pair(vertice[i][j], make_pair(i, j)));       // Save vertices and location

            // Store location of Jojo
            if (direction == "J")
            {
                j_r = i;    // Row location
                j_c = j;    // Column location
            }
        }
    }
    instream.clear();

    // If Jojo's location is not found, ends program
    if (j_r == -1 && j_c == -1)
    {
        cout << "Jojo not found" << endl;
        return 1;
    }
 
    //Add edges based on location that is 3 or 4 locations away from current position
    //The if statements make sure the next move is not X and is within the maze size constraints
    for (int i = 1; i <= r; i++)
    {
        for (int j = 1; j <= c; j++)
        {
            if ((i+3 <= r) && (maze[i][j] == "S") && (maze[i+3][j] != "X"))                             // South, 3 positions
                add_edge(vertice[i][j], vertice[i+3][j], EdgeWeightProperty(3), g);
            if ((i+4 <= r) && (maze[i][j] == "S") && (maze[i+4][j] != "X"))                             // South, 4 positions
                add_edge(vertice[i][j], vertice[i+4][j], EdgeWeightProperty(4), g);

            else if ((j+3 <= c) && (maze[i][j] == "E") && (maze[i][j+3] != "X"))                        // East, 3 positions
                add_edge(vertice[i][j], vertice[i][j+3], EdgeWeightProperty(3), g);
            if ((j+4 <= c) && (maze[i][j] == "E") && (maze[i][j+4] != "X"))                             // East, 4 positions
                add_edge(vertice[i][j], vertice[i][j+4], EdgeWeightProperty(4), g);

            else if ((j-3 >= 1) && (maze[i][j] == "W") && (maze[i][j-3] != "X"))                        // West, 3 positions
                add_edge(vertice[i][j], vertice[i][j-3], EdgeWeightProperty(3), g);
            if ((j-4 >= 1) && (maze[i][j] == "W") && (maze[i][j-4] != "X"))                             // West, 4 positions
                add_edge(vertice[i][j], vertice[i][j-4], EdgeWeightProperty(4), g);

            else if ((i-3 >= 1) && (maze[i][j] == "N") && (maze[i-3][j] != "X"))                        // North, 3 positions
                add_edge(vertice[i][j], vertice[i-3][j], EdgeWeightProperty(3), g);
            if ((i-4 >= 1) && (maze[i][j] == "N") && (maze[i-4][j] != "X"))                             // North, 4 positions
                add_edge(vertice[i][j], vertice[i-4][j], EdgeWeightProperty(4), g);

            else if ((i-3 >= 1) && (j+3 <= c) && (maze[i][j] == "NE") && (maze[i-3][j+3] != "X"))       // North East, 3 positions
                add_edge(vertice[i][j], vertice[i-3][j+3], EdgeWeightProperty(3), g);
            if ((i-4 >= 1) && (j+4 <= c) && (maze[i][j] == "NE") && (maze[i-4][j+4] != "X"))            // North East, 4 positions
                add_edge(vertice[i][j], vertice[i-4][j+4], EdgeWeightProperty(4), g);

            else if ((i+3 <= r) && (j+3 <= c) && (maze[i][j] == "SE") && (maze[i+3][j+3] != "X"))       // South East, 3 positions
                add_edge(vertice[i][j], vertice[i+3][j+3], EdgeWeightProperty(3), g);
            if ((i+4 <= r) && (j+4 <= c) && (maze[i][j] == "SE") && (maze[i+4][j+4] != "X"))            // South East, 4 positions
                add_edge(vertice[i][j], vertice[i+4][j+4], EdgeWeightProperty(4), g);

            else if ((i+3 <= r) && (j-3 >= 1) && (maze[i][j] == "SW") && (maze[i+3][j-3] != "X"))       // South West, 3 positions
                add_edge(vertice[i][j], vertice[i+3][j-3], EdgeWeightProperty(3), g);
            if ((i+4 <= r) && (j-4 >= 1) && (maze[i][j] == "SW") && (maze[i+4][j-4] != "X"))            // South West, 4 positions
                add_edge(vertice[i][j], vertice[i+4][j-4], EdgeWeightProperty(4), g);

            else if ((i-3 >= 1) && (j-3 >= 1) && (maze[i][j] == "NW") && (maze[i-3][j-3] != "X"))       // North West, 3 positions
                add_edge(vertice[i][j], vertice[i-3][j-3], EdgeWeightProperty(3), g);
            if ((i-4 >= 1) && (j-4 >= 1) && (maze[i][j] == "NW") && (maze[i-4][j-4] != "X"))            // North West, 4 positions
                add_edge(vertice[i][j], vertice[i-4][j-4], EdgeWeightProperty(4), g);
        }
    }

    typedef boost::graph_traits < mazeGraph >::vertex_descriptor vertex_descriptor;
    vector<vertex_descriptor> p(num_vertices(g));                                                   // To store parents
    vector<int> d(num_vertices(g));                                                                 // To store distances
    vertex_descriptor s = vertex(vertice[s_r][s_c], g);                                             // Starting location, Tarzan's location
    boost::dijkstra_shortest_paths(g, s, boost::predecessor_map(&p[0]).distance_map(&d[0]));        // Dijkstra's algorithm finds shortest path, updates parents and distances

    int Jojo = vertice[j_r][j_c];       // Jojo's unique number
    int Tarzan = vertice[s_r][s_c];     // Tarzan's unique number

    int temp = p[Jojo];     // Parent of Jojo
    int t_r;                // Temporary row location
    int t_c;                // Temporary column location
    vector<string> path;    // The path from Tarzan's location to Jojo's location
    vector<int> dist;       // The distance between each location
    int newTemp = Jojo;     // Used to calculate distance between each move
    int distance;

    while (temp != Tarzan)
    {
        t_r = mem[temp].second.first;       // Row location of temp, parent[parent...[Jojo]]
        t_c = mem[temp].second.second;      // Column location of temp, parent[parent...[Jojo]]
        distance = d[newTemp] - d[temp];    // Distance between two vertices
        path.push_back(maze[t_r][t_c]);     // Store path from Jojo to Tarzan
        dist.push_back(distance);           // Store distance from each vertex
        temp = p[temp];
        newTemp = p[newTemp];
    }
    path.push_back(maze[s_r][s_c]);     // Store Tarzan's location
    distance = d[newTemp] - d[temp];    // Distance from Tarzan's parent to Tarzan's location
    dist.push_back(distance);           // Store distance from Tarzan's parent to Tarzan's location

    // Print path and distance onto output file
    // Reverses path found
    for (int i = path.size()-1; i >= 0; i--)
        output << path.at(i) << "-" << dist.at(i) << " ";

    //Close file
    output.close();
    return 0;
}

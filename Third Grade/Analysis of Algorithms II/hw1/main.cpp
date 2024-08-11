/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 1
 * Student Name: Kaan Karataş
 * Student Number: 150200081
 */

/* * * * * * * * * * * * * * * * * *
 * DO NOT MODIFY UNLESS REQUESTED  *
 * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
#include <chrono> // For time measurement

/* INCLUDE ANY OTHER NECESSARY LIBRARIES HERE */
/* START YOUR CODE HERE */

#include <climits> // For INT_MIN

/* END YOUR CODE HERE */

using namespace std;
using namespace std::chrono;

/* USE HERE IF YOU NEED TO DEFINE EXTRA FUNCTIONS */
/* START YOUR CODE HERE */

const vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; // Directions for traversal, N-S-E-W

// To Check if the given row and column indices are valid
bool isCellValid(int r, int c, int rows, int cols)
{
    return (r >= 0 && r < rows && c >= 0 && c < cols);
}

/* END YOUR CODE HERE */

/**
 * Perform Depth-First Search (DFS) traversal on a map.
 *
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the DFS traversal.
 * @param col The starting column index for the DFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the DFS traversal.
 */
int dfs(vector<vector<int>> &map, int row, int col, int resource)
{

    stack<pair<int, int>> stack; // Stack for DFS traversal

    stack.push(make_pair(row, col)); // Starting cell pushed to the stack

    int size = 0; // Size of the colony initialized to 0

    while (!stack.empty())
    {
        pair<int, int> cell = stack.top(); // Get the top cell

        int r = cell.first; // Get the row index
        int c = cell.second;    // Get the column index

        stack.pop(); // Pop the top cell

        if (map[r][c] != resource)
        {
            continue; // Skip the cell if not equal to resource
        }

        size++;              // Increment the size of the colony
        map[r][c] = INT_MIN; // Mark the cell as visited

        // Traverse the neighbors
        for (const auto &dir : directions)
        {
            int new_r = (r + dir.first + map.size()) % map.size();      // Wrap around vertically
            int new_c = (c + dir.second + map[0].size()) % map[0].size();   // Wrap around horizontally

            if (isCellValid(new_r, new_c, map.size(), map[0].size()) && map[new_r][new_c] == resource)
            {
                stack.push(make_pair(new_r, new_c)); // Push the neighbor to the stack
            }
        }
    }
    return size; // Return the size of the colony
}

/**
 * Perform Breadth-First Search (BFS) traversal on a map.
 *
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the BFS traversal.
 * @param col The starting column index for the BFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the BFS traversal.
 */
int bfs(vector<vector<int>> &map, int row, int col, int resource)
{

    queue<pair<int, int>> queue; // Queue for BFS traversal

    queue.push(make_pair(row, col)); // Starting cell pushed to the queue

    int size = 0; // Size of the colony

    while (!queue.empty())
    {
        pair<int, int> cell = queue.front(); // Get the front cell

        int r = cell.first; // Get the row index
        int c = cell.second;    // Get the column index

        queue.pop(); // Pop the front cell

        if (map[r][c] != resource)
        {
            continue; // Skip the cell if not equal to resource
        }

        size++;              // Increment the size of the colony
        map[r][c] = INT_MIN; // Mark the cell as visited

        // Traverse the neighbors
        for (const auto &dir : directions)
        {
            int new_r = (r + dir.first + map.size()) % map.size();        // Wrap around vertically
            int new_c = (c + dir.second + map[0].size()) % map[0].size(); // Wrap around horizontally

            if (isCellValid(new_r, new_c, map.size(), map[0].size()) && map[new_r][new_c] == resource)
            {
                queue.push(make_pair(new_r, new_c)); // Push the neighbor to the queue
            }
        }
    }
    return size; // Return the size of the colony
}

/**
 * Finds the top-k largest colonies in a given map.
 *
 * @param map The map represented as a 2D vector of integers.
 * @param useDFS A boolean flag indicating whether to use Depth-First Search (DFS) or Breadth-First Search (BFS) algorithm.
 * @param k The number of top colonies to find.
 * @return A vector of pairs representing the size and resource type of the top-k largest colonies.
 */
vector<pair<int, int>> top_k_largest_colonies(vector<vector<int>> &map, bool useDFS, unsigned int k)
{

    auto start = high_resolution_clock::now(); // Start measuring time

    vector<pair<int, int>> colonies; // Vector to store the size and resource type of colonies

    // Perform BFS or DFS traversal for each cell in the map
    for (unsigned int i = 0; i < map.size(); i++)
    {
        for (unsigned int j = 0; j < map[0].size(); j++)
        {
            int resource = map[i][j];   // Get resource type
            if (resource != INT_MIN)    // Skip if already visited
            {
                int size;
                if (useDFS)
                {
                    size = dfs(map, i, j, resource);    // Perform DFS traversal
                }
                else
                {   
                    size = bfs(map, i, j, resource);    // Perform BFS traversal
                }
                colonies.push_back(make_pair(size, resource));  // Store size and resource type
            }
        }
    }

    if (colonies.size() != 0)   // Sort the colonies if not empty
    {
        for (unsigned int i = 0; i < colonies.size() - 1; i++)
        {
            for (unsigned int j = 0; j < colonies.size() - i - 1; j++)
            {
                if (colonies[j].first < colonies[j + 1].first || (colonies[j].first == colonies[j + 1].first && colonies[j].second > colonies[j + 1].second))
                {
                    swap(colonies[j], colonies[j + 1]); // Sort the colonies
                }
            }
        }
    }

    // Get the top-k largest colonies
    vector<pair<int, int>> topColonies(colonies.begin(), colonies.begin() + std::min(k, static_cast<unsigned int>(colonies.size())));

    auto stop = high_resolution_clock::now();                 // Stop measuring time
    auto duration = duration_cast<nanoseconds>(stop - start); // Calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    return topColonies;
}

// Main function
int main(int argc, char *argv[])
{

    // Check the command line arguments
    if (argc != 4)
    {

        // It should be: <1 or 0> <k> <filename.txt>
        cerr << "Usage: " << argv[0] << " <1 or 0> <k> <filename.txt>" << endl;
        return 1;
    }

    // Get the arguments
    bool useDFS = (atoi(argv[1]) == 1);
    int k = atoi(argv[2]);

    // Read the map from the text file
    string filename = argv[3];
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    vector<vector<int>> sector_map;
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        vector<int> row;
        int num;
        while (iss >> num)
        {
            row.push_back(num);
        }
        sector_map.push_back(row);
    }
    file.close();

    // Find the largest colony
    vector<pair<int, int>> result = top_k_largest_colonies(sector_map, useDFS, k);

    // Print the result
    cout << "Algorithm: " << (useDFS ? "DFS" : "BFS") << endl;
    cout << "Map: " << filename << endl;

    // If no colonies are found
    if (result.empty())
    {
        cout << "No colonies found." << endl;
        return 0;
    }

    // Print the top-k colonies
    for (size_t i = 0; i < result.size(); ++i)
    {
        cout << "Colony " << i + 1 << ": Size = " << result[i].first << ", Resource Type = " << result[i].second << endl;
    }

    return 0;
}
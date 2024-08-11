/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 2
 * Kaan Karataş
 * 150200081
*/

#include <iostream>   // For input/output
#include <cmath>      // For distance calculations
#include <vector>     // For vector operations
#include <algorithm>  // For sorting operations
#include <limits>     // For min(), max() functions
#include <fstream>    // To read/write files
#include <chrono>     // For time measurement

//(These libraries are sufficient for this assignment.)

using namespace std;

// Structure to represent a point in 2D space
struct Point {
    double x, y;

    // Equality operator overloading to compare two points
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Helper function to calculate distance between two points
double distance(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)); // Euclidean distance
}

// Helper function to compare points based on x-coordinate, can use this for sorting.
bool compareX(Point p1, Point p2) {
    return p1.x < p2.x; // Sort based on x-coordinate
}

// Helper function to compare points based on y-coordinate, can use this for sorting.
bool compareY(Point p1, Point p2) {
    return p1.y < p2.y; // Sort based on y-coordinate
}

// Function to find the closest pair of points using brute force.
pair<Point, Point> bruteForceClosestPair(vector<Point>& points, int start, int end) {
    pair<Point, Point> closestPair = {points[start], points[start + 1]};
    double minDistance = distance(points[start], points[start + 1]);

    // For loop iteration over all pairs of points and calculating the distance
    for (int i = start; i < end; i++) {
        for (int j = i + 1; j < end; j++) {
            double dist = distance(points[i], points[j]);
            if (dist < minDistance) {
                minDistance = dist; // Update the minimum distance
                closestPair = {points[i], points[j]};   // Update the closest pair
            }
        }
    }

    return closestPair;
}

// The main objective of the assignment. This is the function to find the closest pair of points recursively using divide and conquer.
pair<Point, Point> closestPair(vector<Point>& points, int start, int end) {
    // If there are 3 or fewer points, use brute force
    if (end - start <= 3) {
        return bruteForceClosestPair(points, start, end);
    }

    int mid = (start + end) / 2;    // Find the middle point
    Point midPoint = points[mid];

    pair<Point, Point> leftPair = closestPair(points, start, mid);  // Recursively find the closest pair on the left side
    pair<Point, Point> rightPair = closestPair(points, mid, end);   // Recursively find the closest pair on the right side

    double leftDistance = distance(leftPair.first, leftPair.second);    // Calculate the distance between the left pair
    double rightDistance = distance(rightPair.first, rightPair.second); // Calculate the distance between the right pair
    double minDistance = min(leftDistance, rightDistance);  // Find the minimum distance

    pair<Point, Point> closestPair = leftDistance < rightDistance ? leftPair : rightPair;   // Initialize the closest pair

    // Create a vector to store the points within the strip
    vector<Point> strip;
    
    // Iterate over the points and add the ones that are within the minimum distance from the middle point to the strip vector
    for (int i = start; i < end; i++) {
        if (abs(points[i].x - midPoint.x) <= minDistance) {
            strip.push_back(points[i]); // Add the point to the strip vector
        }
    }

    sort(strip.begin(), strip.end(), compareY); // Sort the strip vector based on y-coordinate

    // Iterate over the points in the strip vector
    for (unsigned int i = 0; i < strip.size(); i++) {
        // Iterate over the remaining points in the strip vector that are within the minimum distance from the current point
        for (unsigned int j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < minDistance; j++) {
            // Calculate the distance between the current pair of points
            double dist = distance(strip[i], strip[j]);
            // Update the closest pair if the distance is less than the minimum distance
            if (dist < minDistance) {
                minDistance = dist; // Update the minimum distance
                closestPair = {strip[i], strip[j]};
            }
        }
    }

    return closestPair;
}

// Function to remove a pair of points from the point vector. Returns the newly formed vector.
vector<Point> removePairFromVector(vector<Point>& point_vector, pair<Point,Point> point_pair) {
    // Find the pair of points in the vector and remove them
    point_vector.erase(remove(point_vector.begin(), point_vector.end(), point_pair.first), point_vector.end());
    point_vector.erase(remove(point_vector.begin(), point_vector.end(), point_pair.second), point_vector.end());

    return point_vector;    // Return the updated vector
}

    // This function should wrap up the entire algorithm. It should:
    //    1- Find the closest pair
    //    2- Add the pair to the "pairs" vector
    //    3- Remove the paired result from the map
    // at each iteration.
    // Then it should set "unconnected" variable as the final, unconnected city (if it exists).
void findClosestPairOrder(vector<Point> points) {
    vector<pair<Point,Point>> pairs; //add your pairs to this vector
    Point unconnected = {-1,-1}; //set this as the unconnected point if it exists, leave as is otherwise.

    // Sort the points based on x-coordinate
    sort(points.begin(), points.end(), compareX);

    while (points.size() > 1) {

        // Uncomment the algorithm you want to execute

        // pair<Point, Point> closest = bruteForceClosestPair(points, 0, points.size());   // Find the closest pair of points using brute force
        pair<Point, Point> closest = closestPair(points, 0, points.size()); // Find the closest pair of points using divide and conquer
        
        pairs.push_back(closest);   // Add the closest pair to the vector
        
        removePairFromVector(points, closest);  // Remove the paired points from the vector
    }

    // If there's one point left, it remains unconnected
    if (!points.empty()) {
        unconnected = points[0];
    }


    //before printing, please make sure that within each pair, the city with the smaller y coordinate is printed first...
    //...if the y coordinates are the same, print the city with the smaller x coordinate first.

    // Sorting the pairs based on the rule: smaller y coordinate first, then smaller x coordinate
    for (auto& p : pairs) {
        if (p.first.y > p.second.y || (p.first.y == p.second.y && p.first.x > p.second.x)) {
            swap(p.first, p.second);
        }
    }

    //This part prints the output, don't modify.    
    for(size_t i = 0; i < pairs.size(); i++){
        cout << "Pair " << i+1 << ": " << pairs[i].first.x << ", " << pairs[i].first.y << " - " << pairs[i].second.x << ", " << pairs[i].second.y << endl;
    }
    if (unconnected.x != -1){
        cout << "Unconnected " << unconnected.x << ", " << unconnected.y;
    }
}

//Read the coordinates from the file and convert them to a vector. Return the vector as a "vector<Point>"
vector<Point> readCoordinatesFromFile(const string& filename) {
    vector<Point> points;   // Vector to store the Points
    ifstream file(filename);    // Open file
    if (!file.is_open()) {
        return points;  // Return an empty vector if the file cannot be opened
    }

    string line;
    while (getline(file, line)) {   // Read each line from the file
        Point p;    // Create a new Point
        sscanf(line.c_str(), "%lf %lf", &p.x, &p.y);   // Parse the line to extract the x and y coordinates
        points.push_back(p);    // Add the Point to the vector
    }
    file.close();   // Close file

    return points;  // Return vector of Points
}

// Main function. Ideally, you don't need to modify this.
int main(int argc, char* argv[]) {
    vector<Point> points = readCoordinatesFromFile(argv[1]);

    auto start = chrono::high_resolution_clock::now();  // Start the timer

    findClosestPairOrder(points);

    auto end = chrono::high_resolution_clock::now();    // End the timer
    chrono::duration<double, milli> duration = end - start; // Calculate the duration in milliseconds
    cout << "Duration: " << duration.count() << " milliseconds" << endl;

    return 0;
}

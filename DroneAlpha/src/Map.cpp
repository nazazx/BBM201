#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited

    for (int i=0;i<MAX_SIZE;i++) {
        visited[i]=false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix

    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    string line;
    int r=0;
    while(getline(file,line)&&r<MAX_SIZE) {
        stringstream ss(line);
        string number;
        int c=0;
        while (getline(ss,number,',')&&c<MAX_SIZE) {
            int distance=stoi(number);

            distanceMatrix[r][c]=distance;

            c++;
        }
        r++;
    }
    file.close();

}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here

    if(distanceMatrix[provinceA][provinceB]<=maxDistance) {
        return true;
    }
    return false;

}

// Marks a province as visited
void Map::markAsVisited(int province) {
    visited[province]=true;
    // TODO: Your code here
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    if(visited[province]) {
        return true;
    }
    return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (int i=0;i<MAX_SIZE;i++) {
        visited[i]=false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int c=0;
    for (int i=0;i<MAX_SIZE;i++) {
        if(visited[i]) {
            c++;
        }
    }
    return c;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here

    return distanceMatrix[provinceA][provinceB];

}
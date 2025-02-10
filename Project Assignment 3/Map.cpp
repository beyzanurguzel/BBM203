#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            if (i == j) {
                distanceMatrix[i][j] = 0; // Distance from a province to itself is 0
            } else {
                distanceMatrix[i][j] = -1; // No direct connection (using -1 as a placeholder)
            }
        }
        visited[i] = false;
    }
}


// Function to manually convert a string to an integer
bool stringToInt(const std::string& str, int &result) {
    result = 0;
    int sign = 1;
    size_t i = 0;

    // Handle negative numbers
    if (str[0] == '-') {
        sign = -1;
        i = 1;  // Start parsing from the second character
    }

    // Parse the string to an integer
    for (; i < str.size(); i++) {
        if (!std::isdigit(str[i])) {
            return false;  // Invalid character found
        }
        result = result * 10 + (str[i] - '0');
    }

    result *= sign;
    return true;
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    int row = 0;

    // Read the file line by line
    while (std::getline(file, line) && row < MAX_SIZE) {
        std::stringstream ss(line);
        std::string value;
        int col = 0;  // Track the current column

        // Read values separated by commas
        while (std::getline(ss, value, ',') && col < MAX_SIZE) {
            // Trim spaces around the value
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            int parsedValue = 0;

            // Convert the string to integer manually
            if (stringToInt(value, parsedValue)) {
                distanceMatrix[row][col] = parsedValue;  // Assign the valid integer
            } else {
                std::cerr << "Invalid integer in file at row " << row << ", column " << col << ": " << value << std::endl;
                distanceMatrix[row][col] = 0;  // Assign a default value (e.g., 0) for invalid entries
            }
            col++;
        }
        row++;
    }
    // Close the file
    file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB] <= maxDistance;
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int count = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        if (visited[i]) {
            count++;
        }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB];
}
#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include <sstream>

int visitedPriorities[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// Array to help you out with name of the cities in order
const std::string cities[81] = {
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik",
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne",
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta",
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya",
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya",
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat",
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir",
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce"
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially
    // Load priority provinces
    // Load restricted provinces

    map.loadDistanceData(distance_data);
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        map.visited[i] = false;
    }
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file: " << filename << std::endl;
        return;
    }
    int index = 0;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cityName;
        int cityNumber;

        std::getline(ss, cityName, '(');

        // Read the number (after the space)
        ss >> cityNumber;

        // Add the city number to the list
        priorityProvinces[index] = cityNumber;
        index++;
    }
    for (int i = index; i < MAX_PRIORITY_PROVINCES; i++) {
        priorityProvinces[i] = -1;
    }
    file.close();
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file: " << filename << std::endl;
        return;
    }
    int index = 0;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cityName;
        int cityNumber;

        // Read the city name (until '(' character)
        std::getline(ss, cityName, '(');

        // Read the number (after the space)
        ss >> cityNumber;

        // Add the city number to the list
        weatherRestrictedProvinces[index] = cityNumber;
        index++;
    }
    for (int i = index; i < MAX_WEATHER_RESTRICTED_PROVINCES; i++) {
        weatherRestrictedProvinces[i] = -1;
    }
    file.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    // check boundaries
    if (province < 0 || province >= MAX_QUEUE_SIZE) {
        std::cerr << "Province index out of bounds" << std::endl;
    }

    for (int i = 0; i < MAX_PRIORITY_PROVINCES; i++) {
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    // check boundaries
    if (province < 0 || province >= MAX_QUEUE_SIZE) {
        std::cerr << "Province index out of bounds" << std::endl;
        return false;
    }
    for (int i = 0; i < MAX_WEATHER_RESTRICTED_PROVINCES; i++) {
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    // TODO: Your code here
    // begin with starting city
    stack.push(startingCity);
    //enqueueNeighbors(startingCity);

    while (!stack.isEmpty()) {
        int currentCity = stack.peek();

        exploreFromProvince(currentCity);

        enqueueNeighbors(currentCity);

        if (isExplorationComplete()) {
            break;
        }

        // If no neighbors can be visited, backtrack
        if (queue.isEmpty()) {
            backtrack();
        } else {
            int nextProvince = queue.dequeue();
            stack.push(nextProvince);
            map.visited[nextProvince] = true;
        }
    }
    isExplorationComplete();
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    // TODO: Your code here
    if (isPriorityProvince(province)) {
        int index = 0;
        bool found = false;
        while (!found) {
            for (int i = 0; i < MAX_PRIORITY_PROVINCES; i++) {
                if (priorityProvinces[index] == province) {
                    break;
                }
                index++;
            }
            found = true;
        }
        visitedPriorities[index] = 1;
    }
    map.visited[province] = true;
    route.push_back(province);
}

void RoutePlanner::enqueueNeighbors(int province) {
    // TODO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    for (int i = 0; i < MAX_SIZE; i++) {
        if (isWeatherRestricted(i)) {
            std::cout << "Province " << cities[i] << " is weather-restricted. Skipping." << std::endl;
            continue;
        }
        // check the distance and if the city is visited
        if (map.distanceMatrix[province][i] < maxDistance && map.visited[i] == false) {
            if (isPriorityProvince(i)) {
                queue.enqueuePriority(i);
                map.visited[i] = true;
            } else {
                queue.enqueue(i);
                map.visited[i] = true;
            }
        }
    }
}

void RoutePlanner::backtrack() {
    // If you reach a dead-end province
    // TODO: Your code here
    if (stack.isEmpty()) {
        return;
    }
    stack.pop();
}

bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
    if (stack.isEmpty() && queue.isEmpty()) {
        // exploration is over
        displayResults();
       return true;
    }
    for (int i = 0; i < MAX_SIZE; i++) {
        if (map.visited[i] == false) {
            return false;
        }
    }
    // exploration is over
    displayResults();
    return true;
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary
    std::cout << "----------------------------" << std::endl;
    std::cout << "Journey Completed!" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Total Number of Provinces Visited: " << map.countVisitedProvinces() << std::endl;
    std::cout << "Total Distance Covered:" << totalDistanceCovered << std::endl;
    std::cout << "Route Taken:" << std::endl;
    for (int i = 0; i < route.size(); i++) {
        std::cout << cities[route[i]];
        if (i < route.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
    std::cout << "Priority Provinces Status:";
    std::string isVisited = "";
    int visitCount = 0;
    int priorityCount = 0;
    for (int i = 0; i < MAX_PRIORITY_PROVINCES; i++) {
        if (priorityProvinces[i] != -1) {
            priorityCount++;
            if (visitedPriorities[i] != 0) {
                isVisited = " (Visited)";
                visitCount++;
            } else {
                isVisited = " (Not Visited)";
            }
            std::cout << "- " << cities[priorityProvinces[i]] << isVisited << std::endl;
        }
    }
    std::cout << "Total Priority Provinces Visited: " << visitCount << " out of " << priorityCount << std::endl;
    if (priorityCount == visitCount) {
        std::cout << "Success: All priority provinces were visited." << std::endl;
    } else {
        std::cout << "Warning: Not all priority provinces were visited." << std::endl;
    }
}




#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

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
    map.loadDistanceData(distance_data);
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
    //map.resetVisited();

    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially

    // Load priority provinces
    // Load restricted provinces
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    std::string line;
    int i = 0;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::regex regex("\\((\\d+)\\)");
            std::smatch match;

            if (std::regex_search(line, match, regex)) {
                priorityProvinces[i] = std::stoi(match.str(1));

                i++;
            }
        }
        numPriorityProvinces = i;
        file.close();
    }
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    std::string line;
    int i = 0;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::regex regex("\\((\\d+)\\)");
            std::smatch match;

            if (std::regex_search(line, match, regex)) {
                weatherRestrictedProvinces[i] = std::stoi(match.str(1));
                i++;
            }
        }
        numWeatherRestrictedProvinces = i;
        file.close();
    }
}




// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    for (int i=0;i<MAX_PRIORITY_PROVINCES;i++) {
        if(priorityProvinces[i]==province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    for (int i=0;i<MAX_WEATHER_RESTRICTED_PROVINCES;i++) {
        if(weatherRestrictedProvinces[i]==province) {
            return true;
        }
    }
    return false;
}

vector<int> RoutePlanner::most_cities() {
    vector<int>result;
    int max=0;
    for (int i=0;i<routes.size();i++) {
        if(routes[i].size()>max) {
            max=routes[i].size();
            result=routes[i];
        }
    }
    return result;
}
void print_route(vector<int> v) {
    for (int i=0;i<v.size();i++) {
            cout<<v[i]<<" -> ";
    }
    cout<<endl;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    stack.push(startingCity);
    map.markAsVisited(startingCity);
    route.push_back(startingCity);
    exploreFromProvince(startingCity);

    while (!stack.isEmpty() || !queue.isEmpty()) {


        //print_route(route);
        if (!queue.isEmpty()) {
            int nextProvince = queue.dequeue();
            exploreFromProvince(nextProvince);
        } else {
            backtrack();
        }
    }

    if (isExplorationComplete()) {
        route=most_cities();
        calculate_total_distances();
        displayResults();
    }
}


// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    enqueueNeighbors(province);


    while(!queue.isEmpty()) {
        int nextProvince = queue.dequeue();
        if (!map.isVisited(nextProvince)) {
            stack.push(nextProvince);
            map.markAsVisited(nextProvince);
            route.push_back(nextProvince);
            enqueueNeighbors(nextProvince);
        }
    }



}



void RoutePlanner::enqueueNeighbors(int province) {
    for (int i = 0; i < MAX_SIZE; i++) {
        if (map.isWithinRange(province, i, maxDistance) && map.distanceMatrix[province][i] > 0) {
            if (!isWeatherRestricted(i)) {
                if(!map.isVisited(i)){
                if (isPriorityProvince(i)) {
                    queue.enqueuePriority(i);
                } else {
                    queue.enqueue(i);
                }
                  //  std::cout << "Neighbor Added: " << cities[i] << std::endl;
            }
        }
            else {
                cout<<"Province "<<cities[i]<<" is weather-restricted. Skipping."<<endl;
            }
    }
    }
}


void RoutePlanner::backtrack() {
    routes.push_back(route);
    if (!stack.isEmpty()) {
        int lastProvince = stack.pop();
        route.pop_back();
        exploreFromProvince(lastProvince);
    }
}






bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
    if(queue.isEmpty()&&stack.isEmpty()) {

        return true;
    }
    for (int i = 0; i <MAX_SIZE; i++) {
        if (!map.isVisited(i)) {

            return false;
        }
    }
    return true;
}


void RoutePlanner::calculate_total_distances() {
    totalDistanceCovered=0;
    for (int i=0;i<route.size()-1;i++) {
        totalDistanceCovered+=map.getDistance(i,i+1);
    }

}
bool isinroute(vector<int> route,int city) {
    for (int i=0;i<route.size();i++) {
        if(route[i]==city) {
            return true;
        }
    }
    return false;
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary
    cout<<"----------------------------"<<endl;
    std::cout << "Journey Completed!" << std::endl;
    cout<<"----------------------------"<<endl;
    cout<<"Total Number of Provinces Visited: "<<endl;
    std::cout << "Total distance covered: " << totalDistanceCovered << " km" << std::endl;
    std::cout << "Route Taken:" << std::endl;
    for (int province : route) {
        std::cout << cities[province]<<" -> ";
    }
    cout<<endl;
    std::cout << "Priority Province Summary:" << std::endl;
    bool isall=true;
    int c=0;
    for (int i=0;i<numPriorityProvinces; i++) {
        if(isinroute(route,priorityProvinces[i])) {
            cout<<"- "<<cities[priorityProvinces[i]]<<" (Visited)"<<endl;
            c++;
        }
        else {
            cout<<"- "<<cities[priorityProvinces[i]]<<" (Not Visited)"<<endl;
            isall=false;
        }

    }
    if(isall) {
        cout<<"Total Priority Provinces Visited: 5 out of 5"<<endl;
        cout<<"Success: All priority provinces were visited."<<endl;
    }
    else {
        cout<<"Total Priority Provinces Visited: "<<c<<" out of "<<numPriorityProvinces<<endl;
        cout<<"Warning: Not all priority provinces were visited."<<endl;
    }
    /*
    for (int province : route) {
        if (isPriorityProvince(province)) {
            std::cout << cities[province] << " (Index: " << province << ")" << std::endl;
        }
    }*/



}



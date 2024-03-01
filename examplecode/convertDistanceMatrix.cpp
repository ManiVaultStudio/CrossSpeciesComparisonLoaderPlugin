#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <limits.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
struct ClusterInput {
    std::string value1;
    std::string value2;
    std::string cluster;
};

// Function to generate a distance matrix
std::vector<std::vector<int>> generateDistanceMatrix(const std::vector<ClusterInput>& clusterOrder, std::vector<std::string> species) {
    // Sort species in alphabetical order
    std::sort(species.begin(), species.end());

    int numSpecies = species.size();

    // Initialize distance matrix with maximum possible value
    std::vector<std::vector<int>> distanceMatrix(numSpecies, std::vector<int>(numSpecies, INT_MAX));

    // Create a map to store the desired order of clustering
    std::map<std::string, int> orderMap;
    int order = 0;
    for (const auto& input : clusterOrder) {
        if (orderMap.count(input.value1) == 0) {
            orderMap[input.value1] = order++;
        }
        if (orderMap.count(input.value2) == 0) {
            orderMap[input.value2] = order++;
        }
    }

    // Set distances based on desired clustering
    for (int i = 0; i < numSpecies; ++i) {
        for (int j = i + 1; j < numSpecies; ++j) {
            // Set distance based on the order in which clusters should be merged
            int order_i = (orderMap.count(species[i]) > 0) ? orderMap[species[i]] : INT_MAX;
            int order_j = (orderMap.count(species[j]) > 0) ? orderMap[species[j]] : INT_MAX;
            distanceMatrix[i][j] = distanceMatrix[j][i] = std::max(order_i, order_j);
        }
    }

    return distanceMatrix;
}

int main() {
    std::vector<std::string> species = {"Chimpanzee", "Gorilla", "Human", "Marmoset", "Rhesus"};
    std::vector<ClusterInput> order = {{"Human", "Chimpanzee", "Cluster 1"}, {"Gorilla", "Cluster 1", "Cluster 2"}, {"Rhesus", "Cluster 2", "Cluster 3"}, {"Marmoset", "Cluster 3", "Cluster 4"}};

    // Generate distance matrix
    std::vector<std::vector<int>> distanceMatrix = generateDistanceMatrix(order, species);
    
       // Create an output string stream
    std::ostringstream oss;

    // Iterate over the vector and concatenate elements with commas
    for (size_t i = 0; i < species.size(); ++i) {
        if (i != 0) {
            oss << ",";
        }
        oss << species[i];
    }

    // Get the resulting string
    std::string result = oss.str();

    // Print the resulting string
    std::cout << result << std::endl;
    
    int speciesCounter=0;
    // Print distance matrix
    for (const auto &row : distanceMatrix) {
        for (int dist : row) {
            std::cout << dist << ',';
        }
        std::cout<<species[speciesCounter];
        speciesCounter=speciesCounter+1;
        std::cout << '\n';
    }

    return 0;
}

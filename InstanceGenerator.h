#ifndef INSTANCEGENERATOR_H
#define INSTANCEGENERATOR_H

#include <vector>
#include <random>

class InstanceGenerator {
public:
    static std::vector<std::vector<int>> generateGraph(int numVertices, int densityLevel);
};

std::vector<std::vector<int>> InstanceGenerator::generateGraph(int numVertices, int densityLevel) {
    std::vector<std::vector<int>> adjacencyMatrix(numVertices, std::vector<int>(numVertices, 0));
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> probability(0.0, 1.0);
    
    // Define a probabilidade de haver uma aresta com base na densidade
    double density;
    switch (densityLevel) {
        case 1: density = 0.1; break;  // Poucas arestas
        case 2: density = 0.3; break;
        case 3: density = 0.5; break;
        case 4: density = 0.7; break;
        case 5: density = 0.9; break;  // Quase completo
        default: density = 0.5; break; // Valor padrão
    }
    
    // Preenchendo a matriz de adjacência
    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            if (probability(rng) < density) {
                adjacencyMatrix[i][j] = 1;
                adjacencyMatrix[j][i] = 1; // Grafo não direcionado
            }
        }
    }
    
    return adjacencyMatrix;
}

#endif // INSTANCEGENERATOR_H

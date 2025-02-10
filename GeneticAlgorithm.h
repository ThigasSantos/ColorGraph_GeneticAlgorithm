#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_set>

class GeneticAlgorithm {
public:
    GeneticAlgorithm(const std::vector<std::vector<int>>& adjacencyMatrix, int populationSize, int generations, double mutationRate);
    std::vector<int> solve();
    int getChromaticNumber(const std::vector<int>& solution);

private:
    const std::vector<std::vector<int>>& graph;
    int populationSize;
    int generations;
    double mutationRate;
    int numVertices;
    std::vector<std::vector<int>> population;
    std::mt19937 rng;

    void initializePopulation();
    int fitness(const std::vector<int>& solution);
    std::vector<int> tournamentSelection();
    std::vector<int> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2);
    void mutate(std::vector<int>& individual);
    int countColors(const std::vector<int>& solution);
};

GeneticAlgorithm::GeneticAlgorithm(const std::vector<std::vector<int>>& adjacencyMatrix, int populationSize, int generations, double mutationRate)
    : graph(adjacencyMatrix), populationSize(populationSize), generations(generations), mutationRate(mutationRate), rng(std::random_device{}()) {
    numVertices = graph.size();
    initializePopulation();
}

void GeneticAlgorithm::initializePopulation() {
    population.resize(populationSize, std::vector<int>(numVertices));
    for (auto& individual : population) {
        std::vector<int> availableColors(numVertices, -1);
        for (int i = 0; i < numVertices; ++i) {
            std::unordered_set<int> usedColors;
            for (int j = 0; j < numVertices; ++j) {
                if (graph[i][j] == 1 && availableColors[j] != -1) {
                    usedColors.insert(availableColors[j]);
                }
            }
            for (int c = 0; c < numVertices; ++c) {
                if (usedColors.find(c) == usedColors.end()) {
                    availableColors[i] = c;
                    break;
                }
            }
        }
        individual = availableColors;
    }
}

int GeneticAlgorithm::fitness(const std::vector<int>& solution) {
    int conflicts = 0;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            if (graph[i][j] == 1 && solution[i] == solution[j]) {
                conflicts++;
            }
        }
    }
    return conflicts + countColors(solution);
}

int GeneticAlgorithm::countColors(const std::vector<int>& solution) {
    return std::unordered_set<int>(solution.begin(), solution.end()).size();
}

std::vector<int> GeneticAlgorithm::tournamentSelection() {
    std::uniform_int_distribution<int> dist(0, populationSize - 1);
    auto& candidate1 = population[dist(rng)];
    auto& candidate2 = population[dist(rng)];
    return (fitness(candidate1) < fitness(candidate2)) ? candidate1 : candidate2;
}

std::vector<int> GeneticAlgorithm::crossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    std::uniform_int_distribution<int> dist(0, numVertices - 1);
    int crossoverPoint = dist(rng);
    std::vector<int> child(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        child[i] = (i < crossoverPoint) ? parent1[i] : parent2[i];
    }
    return child;
}

void GeneticAlgorithm::mutate(std::vector<int>& individual) {
    std::uniform_real_distribution<double> mutationChance(0.0, 1.0);
    std::uniform_int_distribution<int> colorDist(0, numVertices - 1);
    for (int i = 0; i < numVertices; ++i) {
        if (mutationChance(rng) < mutationRate) {
            std::unordered_set<int> usedColors;
            for (int j = 0; j < numVertices; ++j) {
                if (graph[i][j] == 1) {
                    usedColors.insert(individual[j]);
                }
            }
            for (int c = 0; c < numVertices; ++c) {
                if (usedColors.find(c) == usedColors.end()) {
                    individual[i] = c;
                    break;
                }
            }
        }
    }
}

std::vector<int> GeneticAlgorithm::solve() {
    int bestFitness = fitness(population[0]);
    int generationsWithoutImprovement = 0;
    auto fitnessComp = [this](const std::vector<int>& a, const std::vector<int>& b) {
        return fitness(a) < fitness(b);
    };
    for (int gen = 0; gen < generations; ++gen) {
        std::vector<std::vector<int>> newPopulation;
        while (newPopulation.size() < static_cast<size_t>(populationSize)) {
            auto parent1 = tournamentSelection();
            auto parent2 = tournamentSelection();
            auto child = crossover(parent1, parent2);
            mutate(child);
            newPopulation.push_back(child);
        }
        population = newPopulation;
        int currentBestFitness = fitness(*std::min_element(population.begin(), population.end(), fitnessComp));
        if (currentBestFitness >= bestFitness) {
            generationsWithoutImprovement++;
            if (generationsWithoutImprovement > 50) break;
        } else {
            bestFitness = currentBestFitness;
            generationsWithoutImprovement = 0;
        }
    }
    return *std::min_element(population.begin(), population.end(), fitnessComp);
}

int GeneticAlgorithm::getChromaticNumber(const std::vector<int>& solution) {
    return countColors(solution);
}

#endif

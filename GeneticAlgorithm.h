#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_set>
#include <numeric>

class GeneticAlgorithm
{
public:
    GeneticAlgorithm(const std::vector<std::vector<int>> &adjacencyMatrix, int populationSize, int generations, double mutationRate);
    std::vector<int> solve();
    int getChromaticNumber(const std::vector<int> &solution);

private:
    const std::vector<std::vector<int>> &graph;
    std::vector<std::vector<int>> adjList;
    std::vector<int> degrees;
    int populationSize;
    int generations;
    double mutationRate;
    int numVertices;
    std::vector<std::vector<int>> population;
    std::mt19937 rng;

    void constructiveHeuristic(std::vector<int> &individual);
    void initializePopulation();
    int fitness(const std::vector<int> &solution);
    std::vector<int> tournamentSelection();
    std::vector<int> crossover(const std::vector<int> &parent1, const std::vector<int> &parent2);
    void mutate(std::vector<int> &individual);
    int countColors(const std::vector<int> &solution);
};

GeneticAlgorithm::GeneticAlgorithm(const std::vector<std::vector<int>> &adjacencyMatrix, int populationSize, int generations, double mutationRate)
    : graph(adjacencyMatrix), populationSize(populationSize), generations(generations), mutationRate(mutationRate), rng(std::random_device{}())
{

    numVertices = graph.size();
    adjList.resize(numVertices);
    degrees.resize(numVertices, 0);

    // Construir lista de adjacência e calcular graus
    for (int i = 0; i < numVertices; ++i)
    {
        for (int j = 0; j < numVertices; ++j)
        {
            if (graph[i][j] == 1)
            {
                adjList[i].push_back(j);
                degrees[i]++;
            }
        }
    }

    initializePopulation();
}

void GeneticAlgorithm::constructiveHeuristic(std::vector<int> &individual)
{
    individual.assign(numVertices, -1);
    std::vector<int> vertices(numVertices);
    std::iota(vertices.begin(), vertices.end(), 0);

    // Ordenar por grau decrescente com desempate aleatório
    std::vector<double> randomWeights(numVertices);
    std::uniform_real_distribution<double> weightDist(0.0, 1.0);
    for (auto &w : randomWeights)
        w = weightDist(rng);

    std::sort(vertices.begin(), vertices.end(), [&](int a, int b)
              {
        if (degrees[a] != degrees[b]) return degrees[a] > degrees[b];
        return randomWeights[a] > randomWeights[b]; });

    for (int v : vertices)
    {
        std::unordered_set<int> usedColors;
        for (int u : adjList[v])
        {
            if (individual[u] != -1)
            {
                usedColors.insert(individual[u]);
            }
        }

        int color = 0;
        while (usedColors.count(color))
            ++color;
        individual[v] = color;
    }
}

void GeneticAlgorithm::initializePopulation()
{
    population.resize(populationSize, std::vector<int>(numVertices));
    
    // Primeiro indivíduo usando a heurística construtiva
    constructiveHeuristic(population[0]);

    // Os demais indivíduos são gerados aleatoriamente
    std::uniform_int_distribution<int> colorDist(0, numVertices - 1);
    for (size_t i = 1; i < population.size(); ++i)
    {
        for (int j = 0; j < numVertices; ++j)
        {
            population[i][j] = colorDist(rng);
        }
    }
}

int GeneticAlgorithm::fitness(const std::vector<int> &solution)
{
    int conflicts = 0;
    for (int i = 0; i < numVertices; ++i)
    {
        for (int j : adjList[i])
        {
            if (j > i && solution[i] == solution[j])
                conflicts++;
        }
    }
    return conflicts * 1000 + countColors(solution);
}

std::vector<int> GeneticAlgorithm::tournamentSelection()
{
    std::uniform_int_distribution<int> dist(0, populationSize - 1);
    auto &a = population[dist(rng)];
    auto &b = population[dist(rng)];
    return fitness(a) < fitness(b) ? a : b;
}

std::vector<int> GeneticAlgorithm::crossover(const std::vector<int> &parent1, const std::vector<int> &parent2)
{
    std::vector<int> child(numVertices, -1);
    const int mid = numVertices / 2;

    // Herdar a região central (25% ao redor do meio)
    const int start = mid - numVertices / 4;
    const int end = mid + numVertices / 4;
    std::uniform_int_distribution<int> choice(0, 1);

    for (int i = start; i < end; ++i)
    {
        child[i] = choice(rng) ? parent1[i] : parent2[i];
    }

    // Coletar vértices não coloridos
    std::vector<int> remaining;
    for (int i = 0; i < numVertices; ++i)
    {
        if (child[i] == -1)
            remaining.push_back(i);
    }

    // Ordenar vértices restantes por grau
    std::sort(remaining.begin(), remaining.end(), [&](int a, int b)
              { return degrees[a] > degrees[b]; });

    // Preenchimento guloso
    for (int v : remaining)
    {
        std::unordered_set<int> usedColors;
        for (int u : adjList[v])
        {
            if (child[u] != -1)
                usedColors.insert(child[u]);
        }

        int color = 0;
        while (usedColors.count(color))
            ++color;
        child[v] = color;
    }

    return child;
}

void GeneticAlgorithm::mutate(std::vector<int> &individual)
{
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<int> nodeDist(0, numVertices - 1);

    for (int i = 0; i < numVertices; ++i)
    {
        if (prob(rng) < mutationRate)
        {
            std::unordered_set<int> usedColors;
            for (int u : adjList[i])
            {
                usedColors.insert(individual[u]);
            }

            std::vector<int> available;
            for (int c = 0; c < numVertices; ++c)
            {
                if (!usedColors.count(c))
                    available.push_back(c);
            }

            if (!available.empty())
            {
                std::uniform_int_distribution<int> colorDist(0, available.size() - 1);
                individual[i] = available[colorDist(rng)];
            }
        }
    }
}

int GeneticAlgorithm::countColors(const std::vector<int> &solution)
{
    return std::unordered_set<int>(solution.begin(), solution.end()).size();
}

std::vector<int> GeneticAlgorithm::solve()
{
    const int eliteSize = 2;
    // Correção: Especificar tipos explicitamente
    auto fitnessComp = [this](const std::vector<int> &a, const std::vector<int> &b)
    {
        return fitness(a) < fitness(b);
    };

    for (int gen = 0; gen < generations; ++gen)
    {
        std::vector<std::vector<int>> newPopulation;

        // Elitismo
        std::sort(population.begin(), population.end(), fitnessComp);
        for (int i = 0; i < eliteSize; ++i)
        {
            newPopulation.push_back(population[i]);
        }

        // Preencher o restante da população
        while (newPopulation.size() < static_cast<size_t>(populationSize))
        {
            auto parent1 = tournamentSelection();
            auto parent2 = tournamentSelection();
            auto child = crossover(parent1, parent2);
            mutate(child);
            newPopulation.push_back(child);
        }

        population = newPopulation;
    }

    return *std::min_element(population.begin(), population.end(), fitnessComp);
}

int GeneticAlgorithm::getChromaticNumber(const std::vector<int> &solution)
{
    return countColors(solution);
}

#endif
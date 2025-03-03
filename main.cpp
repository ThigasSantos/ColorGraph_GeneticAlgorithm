#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "InstanceReader.h"
#include "InstanceGenerator.h"
#include "GeneticAlgorithm.h"

int main() {
    // std::vector<std::string> inputFiles;
    // std::vector<std::string> outputFiles;

    // for (int i = 1; i <= 12; i++) {
    //     inputFiles.push_back("../Instances/Instance" + std::to_string(i) + "_450_LEI.txt");
    //     outputFiles.push_back("../Results/output" + std::to_string(i) + "_450_LEI.txt");
    // }
    // for (int i = 1; i <= 5; i++) {
    //     inputFiles.push_back("../Instances/Instance" + std::to_string(i) + "_MYC.txt");
    //     outputFiles.push_back("../Results/output" + std::to_string(i) + "_MYC.txt");
    // }
    // for (int i = 1; i <= 14; i++) {
    //     inputFiles.push_back("../Instances/Instance" + std::to_string(i) + "_REG.col");
    //     outputFiles.push_back("../Results/output" + std::to_string(i) + "_REG.txt");
    // }
    // for (int i = 1; i <= 10; i++) {
    //     inputFiles.push_back("../Instances/Instance" + std::to_string(i) + "_SGB.txt");
    //     outputFiles.push_back("../Results/output" + std::to_string(i) + "_SGB.txt");
    // }

    // for (size_t i = 0; i < inputFiles.size(); ++i) {
    //     const std::string &inputFilename = inputFiles[i];
    //     const std::string &outputFilename = outputFiles[i];

    //     InstanceReader reader(inputFilename);
    //     auto adjacencyMatrix = reader.getAdjacencyMatrix();

    //     GeneticAlgorithm ga(adjacencyMatrix, 20, 10000, 0.05);
    //     std::vector<int> solution = ga.solve();

    //     std::cout << "\n=== Processando " << inputFilename << " ===\n";
    //     int chromaticNumber = ga.getChromaticNumber(solution);
    //     std::cout << "Número cromático: " << chromaticNumber << "\n";

    //     std::ofstream outFile(outputFilename);
    //     if (outFile) {
    //         outFile << "Número cromático: " << chromaticNumber << "\n";
    //         outFile << "Coloração:\n";
    //         for (int color : solution) {
    //             outFile << color << " ";
    //         }
    //         outFile.close();
    //         std::cout << "Resultados salvos em " << outputFilename << "\n";
    //     }
    // }



    const int NUM_VERTICES = 250;       // Número fixo de vértices
    const int INSTANCES_PER_DENSITY = 1; // 5 instâncias por densidade
    const std::string RESULTS_DIR = "../Results/"; 

    for (int density = 1; density <= 5; ++density) {
        for (int instance = 1; instance <= INSTANCES_PER_DENSITY; ++instance) {
            // Gerar grafo
            auto adjacencyMatrix = InstanceGenerator::generateGraph(NUM_VERTICES, density);
            
            // Executar algoritmo genético
            GeneticAlgorithm ga(adjacencyMatrix, 20, 10000, 0.05);
            std::vector<int> solution = ga.solve();
            int chromaticNumber = ga.getChromaticNumber(solution);

            // Nome do arquivo: output_numVertices_density_identifier
            std::string outputFilename = 
                "output_" + std::to_string(NUM_VERTICES) + 
                "_" + std::to_string(density) + 
                "_" + std::to_string(instance) + ".txt";
            
            std::string outputPath = RESULTS_DIR + outputFilename;
            std::ofstream outFile(outputPath);
            
            if (outFile) {
                outFile << "Número cromático: " << chromaticNumber << "\n";
                outFile << "Coloração:\n";
                for (int color : solution) {
                    outFile << color << " ";
                }
                outFile.close();
                std::cout << "Arquivo salvo: " << outputPath << "\n";
            } else {
                std::cerr << "Erro ao salvar: " << outputPath << "\n";
            }
        }
    }

    return 0;
}

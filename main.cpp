#include <iostream>
#include <fstream>
#include "InstanceReader.h"
#include "GeneticAlgorithm.h"

int main() {
    std::vector<std::string> inputFiles;
    std::vector<std::string> outputFiles;

    for (int i = 1; i <= 12; i++) {
        inputFiles.push_back("../Instances/Instance" + std::to_string(i) + "_450_LEI.txt");
        outputFiles.push_back("../Results/output" + std::to_string(i) + "_450_LEI.txt");
    }
    for (int i = 1; i <= 5; i++) {
        inputFiles.push_back("../Instances/Instance" + std::to_string(i) + "_MYC.txt");
        outputFiles.push_back("../Results/output" + std::to_string(i) + "_MYC.txt");
    }
    for (int i = 1; i <= 14; i++) {
        inputFiles.push_back("../Instances/Instance" + std::to_string(i) + "_REG.col");
        outputFiles.push_back("../Results/output" + std::to_string(i) + "_REG.txt");
    }
    for (int i = 1; i <= 10; i++) {
        inputFiles.push_back("../Instances/Instance" + std::to_string(i) + "_SGB.txt");
        outputFiles.push_back("../Results/output" + std::to_string(i) + "_SGB.txt");
    }

    for (size_t i = 0; i < inputFiles.size(); ++i) {
        const std::string &inputFilename = inputFiles[i];
        const std::string &outputFilename = outputFiles[i];

        InstanceReader reader(inputFilename);
        auto adjacencyMatrix = reader.getAdjacencyMatrix();

        GeneticAlgorithm ga(adjacencyMatrix, 500, 10000, 0.2);
        std::vector<int> solution = ga.solve();

        std::cout << "\n=== Processando " << inputFilename << " ===\n";
        int chromaticNumber = ga.getChromaticNumber(solution);
        std::cout << "Número cromático: " << chromaticNumber << "\n";

        std::ofstream outFile(outputFilename);
        if (outFile) {
            outFile << "Número cromático: " << chromaticNumber << "\n";
            outFile << "Coloração:\n";
            for (int color : solution) {
                outFile << color << " ";
            }
            outFile.close();
            std::cout << "Resultados salvos em " << outputFilename << "\n";
        }
    }
    return 0;
}

#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream inputFile("avlDB.txt");  // Input file
    std::ofstream outputFile("output.txt");  // Output file

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find("cout") == std::string::npos) {
            outputFile << line << std::endl;
        }
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Processing complete. Check 'output.txt' for results." << std::endl;

    return 0;
}


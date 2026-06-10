#include "dataset_generator.h"
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>

// Custom minimal hash set for uniqueness checking (Bypassing banned STL containers)
class UniqueIdFilter {
private:
    size_t capacity;
    unsigned long long* table;
    
    size_t hashFunction(unsigned long long id) const {
        return id % capacity;
    }

public:
    UniqueIdFilter(size_t expectedSize) {
        capacity = expectedSize * 2; 
        table = new unsigned long long[capacity](); 
    }

    ~UniqueIdFilter() {
        delete[] table;
    }

    bool insert(unsigned long long id) {
        size_t index = hashFunction(id);
        while (table[index] != 0) {
            if (table[index] == id) {
                return false; 
            }
            index = (index + 1) % capacity;
        }
        table[index] = id;
        return true;
    }
};

// Generates the CSV file based on your Student ID seed
void generateDataset(int datasetSize, const std::string& outputFilename, unsigned long long explicitSeed) {
    std::mt19937_64 generator(explicitSeed);
    std::uniform_int_distribution<unsigned long long> idDist(1000000000ULL, 9999999999ULL);
    std::uniform_int_distribution<int> letterDist(0, 25);

    std::ofstream csvFile(outputFilename);
    if (!csvFile.is_open()) {
        std::cerr << "Error: Could not open or create output CSV file." << std::endl;
        return;
    }

    UniqueIdFilter duplicateFilter(datasetSize);
    std::cout << "[Generator] Writing " << datasetSize << " unique records to " << outputFilename << "...\n";
    
    int generatedCount = 0;
    while (generatedCount < datasetSize) {
        unsigned long long randomId = idDist(generator);
        if (duplicateFilter.insert(randomId)) {
            std::string randomString = "";
            for (int i = 0; i < 5; ++i) {
                randomString += static_cast<char>('a' + letterDist(generator));
            }
            csvFile << randomId << "," << randomString << "\n";
            generatedCount++;
        }
    }
    csvFile.close();
    std::cout << "[Generator] Dataset generation complete.\n";
}

// Highly optimized CSV file loader
std::vector<DataRecord> loadDataset(const std::string& inputFilename) {
    std::vector<DataRecord> records;
    std::ifstream csvFile(inputFilename);
    
    if (!csvFile.is_open()) {
        std::cerr << "Error: Could not open dataset file for loading: " << inputFilename << std::endl;
        return records;
    }

    std::string line;
    // Pre-allocate memory estimate if possible, to avoid continuous resizing slowdowns
    records.reserve(100000); 

    while (std::getline(csvFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idPart;
        std::string textPart;

        if (std::getline(ss, idPart, ',') && std::getline(ss, textPart)) {
            DataRecord record;
            record.id = std::stoull(idPart); // String to unsigned long long
            record.text = textPart;
            records.push_back(record);
        }
    }
    csvFile.close();
    std::cout << "[Loader] Successfully parsed and loaded " << records.size() << " elements into RAM.\n";
    return records;
}
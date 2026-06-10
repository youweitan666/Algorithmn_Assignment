#ifndef DATASET_GENERATOR_H
#define DATASET_GENERATOR_H

#include <string>
#include <vector>

// Core data structure shared across all sorting and searching algorithms
struct DataRecord {
    unsigned long long id; // Securely stores 10-digit integers up to 9,999,999,999
    std::string text;      // Stores the 5-letter lowercase string
};

// Member 1 Functions exposed to the team
void generateDataset(int datasetSize, const std::string& outputFilename, unsigned long long explicitSeed);
std::vector<DataRecord> loadDataset(const std::string& inputFilename);

#endif // DATASET_GENERATOR_H
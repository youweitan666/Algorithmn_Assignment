#include <iostream>
#include <chrono>
#include "dataset_generator.h"

int main() {
    // 你的学号：243UC246VY -> 转换后的 64 位种子
    unsigned long long groupLeaderSeed = 2432132462225ULL;
    
    // 先测试小规模的数据（10万条），确保代码跑通
    int testSize = 100000; 
    std::string filename = "dataset_" + std::to_string(testSize) + ".csv";

    std::cout << "--- Starting Phase 1 Architecture Verification ---\n\n";

    // 调用你写的生成器
    generateDataset(testSize, filename, groupLeaderSeed);

    std::cout << "\n------------------------------------------------\n";

    // 测试读取器
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::vector<DataRecord> loadedMemoryArray = loadDataset(filename);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> loadDuration = endTime - startTime;

    std::cout << "Data reading process performance: " << loadDuration.count() << " seconds.\n";

    // 检查第一条数据是否正确解析出来
    if (!loadedMemoryArray.empty()) {
        std::cout << "\n[Sanity Check] First element in memory array:\n";
        std::cout << "ID: " << loadedMemoryArray[0].id << " | String: " << loadedMemoryArray[0].text << "\n";
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <future>
#include <mutex>

std::vector<int> generateRandomNumbers(size_t amount, int minRange, int maxRange) {
    std::vector<int> randomNumbers;
    
    if (minRange > maxRange) {
        throw std::invalid_argument("minRange must be less than or equal to maxRange");
    }
    
    randomNumbers.reserve(amount);
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(minRange, maxRange);
    
    for (size_t i = 0; i < amount; ++i) {
        randomNumbers.emplace_back(distribution(gen));
    }
    
    return randomNumbers;
}

bool randomizeVectorOrder(std::vector<int>* vecPtr) {
    // Check if the pointer is not null and the vector is not empty
    if (vecPtr && !vecPtr->empty()) {
        static std::random_device rd;
        static std::mt19937 g(rd());

        std::shuffle(vecPtr->begin(), vecPtr->end(), g);
        return true;
    }
    return false;
}

void BogoSort(std::vector<int>* numbers) {
    if (!std::is_sorted(numbers->begin(), numbers->end()))
    {
        while(!std::is_sorted(numbers->begin(), numbers->end())) {
            randomizeVectorOrder(numbers);
        }
    }
}

int main() {
    std::ofstream outputFile("results_output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening results_output.txt for writing." << std::endl;
        return 1;
    }

    int numbersOfTest = std::thread::hardware_concurrency();
    if (numbersOfTest == 0) {
        std::cerr << "Unable to determine the number of hardware threads." << std::endl;
        return 1;
    }
    
    int minRange = std::numeric_limits<int>::min();
    int maxRange = std::numeric_limits<int>::max();

    int amountMaxRange = 10;

    std::cout << "Provide max length of list for BogoSort: ";
    if (!(std::cin >> amountMaxRange)) {
        std::cerr << "Invalid input. Please provide a valid number." << std::endl;
        return 1;
    }

    if (amountMaxRange <= 2) {
        std::cerr << "Provided number should be more than 2" << std::endl;
        return 1;
    }
    
    std::vector<std::chrono::microseconds> totalTimeTaken(amountMaxRange);

    std::mutex outputFileMutex;

    for (int amount = 2; amount <= amountMaxRange; amount++) {
        std::vector<std::future<void>> futures;
        outputFile << "> Results (numbers of test: " << numbersOfTest << "): " << std::endl;

        for (int i = 0; i < numbersOfTest; i++) {
            futures.emplace_back(std::async(std::launch::async, [amount, minRange, maxRange, i, &outputFile, &totalTimeTaken, &outputFileMutex]() {
                std::vector<int> randomNumbers = generateRandomNumbers(amount, minRange, maxRange);
                
                std::string testLiteration = std::to_string(i + 1);
                {
                    std::lock_guard<std::mutex> lock(outputFileMutex);
                    outputFile << "Starting array (" + testLiteration + "): ";
                    for (int j = 0; j < amount; j++) {
                        outputFile << randomNumbers[j] << " ";
                    }
                    outputFile << "\n";
                }

                auto start_time = std::chrono::steady_clock::now();
                BogoSort(&randomNumbers);
                auto end_time = std::chrono::steady_clock::now();

                {
                    std::lock_guard<std::mutex> lock(outputFileMutex);
                    outputFile << "Sorted array (" + testLiteration + "): ";
                    for (int j = 0; j < amount; j++) {
                        outputFile << randomNumbers[j] << " ";
                    }
                    outputFile << "\n";
                }

                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
                std::chrono::microseconds elapsedDuration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
                totalTimeTaken[amount - 1] += elapsedDuration;
            }));
        }

        for (auto& future : futures) {
            future.wait();
        }

        totalTimeTaken[amount - 1] /= numbersOfTest;
        outputFile << "> With amounts of " << amount << " numbers BogoSort taken average " << totalTimeTaken[amount - 1].count() << " microseconds to sort \n\n";
    }

    outputFile.close();

    return 0;
}

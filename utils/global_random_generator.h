// In this file, we provide the functionality of generating random number.
#ifndef GLOBAL_RANDOM_GENERATOR_H
#define GLOBAL_RANDOM_GENERATOR_H

#include <random>
#include <qDebug>

class GlobalRandomGenerator {
public:
    static std::mt19937 &getInstance() {
        static std::mt19937 generator(getSeedFromEnvironment());
        return generator;
    }

    static void setSeed(const unsigned int seed) {
        getInstance().seed(seed);
    }

private:
    static unsigned int getSeedFromEnvironment() {
        const char *seedStr = std::getenv("CUSTOM_SEED");
        if (seedStr != nullptr) {
            try {
                return static_cast<unsigned int>(std::stoi(seedStr));
            }
            catch (const std::exception &e) {
                qDebug() << "Failed to parse CUSTOM_SEED: " << e.what() << "\n";
                qDebug() << "Using std::random_device{}() instead.\n";
            }
        }
        return std::random_device{}();
    }
};

#endif //GLOBAL_RANDOM_GENERATOR_H

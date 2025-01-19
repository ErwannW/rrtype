#include "Random.hpp"

MGE::Random::Random(int seed)
{
    engine.seed(seed);
}

std::string UuidGenerator() {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, 20);

    std::ostringstream uuid;
    uuid << std::hex << std::setfill('0');

    for (int i = 0; i < 8; i++) {
        uuid << distribution(generator);
    }
    uuid << "-";
    for (int i = 0; i < 4; i++) {
        uuid << distribution(generator);
    }
    uuid << "-4";
    for (int i = 0; i < 3; i++) {
        uuid << distribution(generator);
    }
    uuid << "-";
    uuid << distribution(generator) % 4 + 8;
    for (int i = 0; i < 3; i++) {
        uuid << distribution(generator);
    }
    uuid << "-";
    for (int i = 0; i < 12; i++) {
        uuid << distribution(generator);
    }
    return uuid.str();
}
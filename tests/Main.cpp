#include "gtest/gtest.h"

std::string TEST_DATA_DIR;

int main(int argc, char** argv) 
{
    if (argc > 1)
        TEST_DATA_DIR = argv[1];

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
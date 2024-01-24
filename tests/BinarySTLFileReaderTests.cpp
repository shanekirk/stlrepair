#include "BinarySTLFileReader.h"

#include "gtest/gtest.h"

extern std::string TEST_DATA_DIR; // Yeah, I don't feel great about it. But it is what it is for now.

class BinarySTLFileReaderTests : public testing::Test
{

};

TEST_F(BinarySTLFileReaderTests, testInstantiationWithEmptyPath)
{
    try
    {
        BinarySTLFileReader reader("");
        FAIL() << "BinarySTLFileReader should have thrown.";
    }
    catch (const std::runtime_error&)
    {
        return;
    }
}

TEST_F(BinarySTLFileReaderTests, testInstantiationWithBadPath)
{
    try
    {
        BinarySTLFileReader reader("x:\\file\\that\\shouldnt\\exist.stl");
        FAIL() << "BinarySTLFileReader should have thrown.";
    }
    catch (const std::runtime_error&)
    {
        return;
    }
}

TEST_F(BinarySTLFileReaderTests, testOpeningGoodPath)
{
    BinarySTLFileReader reader(TEST_DATA_DIR + "binary.stl");
}
 
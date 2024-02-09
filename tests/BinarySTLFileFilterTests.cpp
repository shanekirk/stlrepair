#include "BinarySTLFileFilter.h"
#include "BinarySTLFileReader.h"
#include "FileUtils.h"
#include "CallGuard.h"

#include "gtest/gtest.h"

extern std::string TEST_DATA_DIR; // Yeah, I don't feel great about it. But it is what it is for now.

class BinarySTLFileFilterTests : public testing::Test
{

};

TEST_F(BinarySTLFileFilterTests, testInstantiationWithEmptyPath)
{
    try
    {
        BinarySTLFileFilter filter("");
        FAIL() << "BinarySTLFileFilter should have thrown.";
    }
    catch (const std::runtime_error&)
    {
        return;
    }
}

TEST_F(BinarySTLFileFilterTests, testNoOptionsSet)
{
    const std::string INPUT_FILE = TEST_DATA_DIR + "binary_5mm_sphere.stl";
    const std::string OUTPUT_FILE = FileUtils::generateUniqueFilePath(INPUT_FILE);
    auto fileGuard = makeCallGuard([&]() { _unlink(OUTPUT_FILE.c_str()); });

    BinarySTLFileFilter filter(OUTPUT_FILE);
    BinarySTLFileReader reader(INPUT_FILE);
    reader.readFile(filter);

    EXPECT_EQ(FileUtils::getFileSize(INPUT_FILE), FileUtils::getFileSize(OUTPUT_FILE));
    EXPECT_EQ(FileUtils::areFilesEqual(INPUT_FILE, OUTPUT_FILE), true);
}

TEST_F(BinarySTLFileFilterTests, testClearHeader)
{
    const std::string INPUT_FILE = TEST_DATA_DIR + "binary_5mm_sphere.stl";
    const std::string OUTPUT_FILE = FileUtils::generateUniqueFilePath(INPUT_FILE);
    auto fileGuard = makeCallGuard([&]() { _unlink(OUTPUT_FILE.c_str()); });

    BinarySTLFileFilter filter(OUTPUT_FILE);
    filter.m_zeroOutHeader = true;

    BinarySTLFileReader reader(INPUT_FILE);
    reader.readFile(filter);

    EXPECT_EQ(FileUtils::getFileSize(INPUT_FILE), FileUtils::getFileSize(OUTPUT_FILE));
    EXPECT_EQ(FileUtils::areFilesEqual(INPUT_FILE, OUTPUT_FILE, 
        BINARY_STL_HEADER_SIZE_IN_BYTES, 
        FileUtils::getFileSize(INPUT_FILE) - BINARY_STL_HEADER_SIZE_IN_BYTES),
        true);
    EXPECT_EQ(FileUtils::areFilesEqual(INPUT_FILE, OUTPUT_FILE,
        0,
        BINARY_STL_HEADER_SIZE_IN_BYTES),
        false);
}

TEST_F(BinarySTLFileFilterTests, testTruncateFile)
{
    const std::string INPUT_FILE = TEST_DATA_DIR + "binary_5mm_sphere_weird_data_on_end.stl";
    const std::string OUTPUT_FILE = FileUtils::generateUniqueFilePath(INPUT_FILE);
    auto fileGuard = makeCallGuard([&]() { _unlink(OUTPUT_FILE.c_str()); });

    BinarySTLFileFilter filter(OUTPUT_FILE);
    filter.m_truncateFileToTriangleCount = true;

    BinarySTLFileReader reader(INPUT_FILE);
    reader.readFile(filter);

    EXPECT_NE(FileUtils::getFileSize(INPUT_FILE), FileUtils::getFileSize(OUTPUT_FILE));

    auto smallerFileSize = FileUtils::getFileSize(OUTPUT_FILE);
    EXPECT_EQ(FileUtils::areFilesEqual(INPUT_FILE, OUTPUT_FILE, 0, smallerFileSize), true);
}

TEST_F(BinarySTLFileFilterTests, testUpdateTriangleCount)
{
    const std::string INPUT_FILE = TEST_DATA_DIR + "binary_5mm_sphere_with_wrong_triangle_count.stl";
    const std::string OUTPUT_FILE = FileUtils::generateUniqueFilePath(INPUT_FILE);
    auto fileGuard = makeCallGuard([&]() { _unlink(OUTPUT_FILE.c_str()); });

    BinarySTLFileFilter filter(OUTPUT_FILE);
    filter.m_updateTriangleCount = true;

    BinarySTLFileReader reader(INPUT_FILE);
    reader.readFile(filter);

    EXPECT_EQ(FileUtils::getFileSize(INPUT_FILE), FileUtils::getFileSize(OUTPUT_FILE));

    // These two files should be identical after the repair.
    EXPECT_EQ(FileUtils::areFilesEqual(TEST_DATA_DIR + "binary_5mm_sphere.stl", OUTPUT_FILE), true);
}

TEST_F(BinarySTLFileFilterTests, testZeroAttributeByteCounts)
{
    const std::string INPUT_FILE = TEST_DATA_DIR + "binary_5mm_sphere_with_abcs.stl";
    const std::string OUTPUT_FILE = FileUtils::generateUniqueFilePath(INPUT_FILE);
    auto fileGuard = makeCallGuard([&]() { _unlink(OUTPUT_FILE.c_str()); });

    BinarySTLFileFilter filter(OUTPUT_FILE);
    filter.m_zeroAttributeByteCounts = true;

    BinarySTLFileReader reader(INPUT_FILE);
    reader.readFile(filter);

    EXPECT_EQ(FileUtils::getFileSize(INPUT_FILE), FileUtils::getFileSize(OUTPUT_FILE));
    EXPECT_EQ(FileUtils::areFilesEqual(TEST_DATA_DIR + "binary_5mm_sphere.stl", OUTPUT_FILE), true);
}
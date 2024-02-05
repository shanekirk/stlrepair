#include "BinarySTLFileWriter.h"
#include "BinarySTLFileReader.h"
#include "FileUtils.h"
#include "CallGuard.h"

#include "gtest/gtest.h"

extern std::string TEST_DATA_DIR; // Yeah, I don't feel great about it. But it is what it is for now.

class BinarySTLFileWriterTests : public testing::Test
{

};

TEST_F(BinarySTLFileWriterTests, testInstantiationWithEmptyPath)
{
    try
    {
        STLBinaryHeader header;
        BinarySTLFileWriter writer("", header, 0);
        FAIL() << "BinarySTLFileWriter should have thrown.";
    }
    catch (const std::runtime_error&)
    {
        return;
    }
}

TEST_F(BinarySTLFileWriterTests, testInstantiationWithBadPath)
{
    try
    {
        STLBinaryHeader header;
        BinarySTLFileWriter writer("x:\\path\\that\\shouldnt\\exist.stl", header, 0);
        FAIL() << "BinarySTLFileWriter should have thrown.";
    }
    catch (const std::runtime_error&)
    {
        return;
    }
}

TEST_F(BinarySTLFileWriterTests, testWritingHeader)
{
    const std::string testFile(TEST_DATA_DIR + "newfile.stl");

    _unlink(testFile.c_str());
    EXPECT_FALSE(FileUtils::fileExists(testFile));
    STLBinaryHeader header;
    BinarySTLFileWriter writer(testFile, header, 0);
    writer.finalize();

    auto fileGuard = makeCallGuard([&]() { _unlink(testFile.c_str()); });
    EXPECT_TRUE(FileUtils::fileExists(testFile));
    EXPECT_EQ(FileUtils::getFileSize(testFile), BINARY_STL_HEADER_SIZE_IN_BYTES + BINARY_STL_TRIANGLE_COUNT_IN_BYTES);
}

TEST_F(BinarySTLFileWriterTests, testWritingTriangle)
{
    const std::string testFile(TEST_DATA_DIR + "newfile.stl");

    _unlink(testFile.c_str());
    EXPECT_FALSE(FileUtils::fileExists(testFile));
    STLBinaryHeader header;
    BinarySTLFileWriter writer(testFile, header, 0);
    STLBinaryTriangleData triangle;
    writer.writeTriangleData(triangle, 0);
    writer.finalize();

    auto fileGuard = makeCallGuard([&]() { _unlink(testFile.c_str()); });
    EXPECT_TRUE(FileUtils::fileExists(testFile));
    EXPECT_EQ(FileUtils::getFileSize(testFile), 
        BINARY_STL_HEADER_SIZE_IN_BYTES + 
        BINARY_STL_TRIANGLE_COUNT_IN_BYTES + 
        BINARY_STL_TRIANGLE_SIZE_IN_BYTES + 
        BINARY_STL_TRIANGLE_ATTRIBUTE_BYTE_COUNT_IN_BYTES);
}

TEST_F(BinarySTLFileWriterTests, testWritingExtraData)
{
    const std::string testFile(TEST_DATA_DIR + "newfile.stl");

    _unlink(testFile.c_str());
    EXPECT_FALSE(FileUtils::fileExists(testFile));
    STLBinaryHeader header;
    BinarySTLFileWriter writer(testFile, header, 0);
    const char data[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    writer.finalize(&data[0], sizeof(data));

    auto fileGuard = makeCallGuard([&]() { _unlink(testFile.c_str()); });
    EXPECT_TRUE(FileUtils::fileExists(testFile));
    EXPECT_EQ(FileUtils::getFileSize(testFile),
        BINARY_STL_HEADER_SIZE_IN_BYTES +
        BINARY_STL_TRIANGLE_COUNT_IN_BYTES + sizeof(data));
}

TEST_F(BinarySTLFileWriterTests, testWritingFullFileOfData)
{
    const std::string testFile(TEST_DATA_DIR + "newfile.stl");

    _unlink(testFile.c_str());
    EXPECT_FALSE(FileUtils::fileExists(testFile));
    auto fileGuard = makeCallGuard([&]() { _unlink(testFile.c_str()); });

    STLBinaryHeader header
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F
    };

    BinarySTLFileWriter writer(testFile, header, 2);

    STLBinaryTriangleData triangle1
    {
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F, 0xFE, 0xEE,
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F, 0xFE, 0xEE,
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F, 0xFE, 0xEE,
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F, 0xFE, 0xEE,
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F
    };
    writer.writeTriangleData(triangle1, 0xBB);

    STLBinaryTriangleData triangle2
    {
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F, 0xFE, 0xEE,
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F, 0xFE, 0xEE,
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F, 0xFE, 0xEE,
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F, 0xFE, 0xEE,
        0xBA, 0xDC, 0x0F, 0xFE, 0xEE, 0xBA, 0xDC, 0x0F
    };
    writer.writeTriangleData(triangle2, 0xBC);

    const char* xtraData = "SKIRK";
    writer.finalize(xtraData, strlen(xtraData));

    EXPECT_TRUE(FileUtils::fileExists(testFile));
    EXPECT_EQ(FileUtils::getFileSize(testFile), MINIMUM_BINARY_STL_SIZE_IN_BYTES + BINARY_STL_TRIANGLE_SIZE_IN_BYTES + BINARY_STL_TRIANGLE_ATTRIBUTE_BYTE_COUNT_IN_BYTES + strlen(xtraData));
}

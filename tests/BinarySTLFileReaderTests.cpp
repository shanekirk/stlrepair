#include "BinarySTLFileReader.h"

#include "gtest/gtest.h"

extern std::string TEST_DATA_DIR; // Yeah, I don't feel great about it. But it is what it is for now.

class TestBinarySTLFileReaderListener : public BinarySTLFileReaderListener
{
public:
    TestBinarySTLFileReaderListener() :
        m_readBeginCalledCount(0),
        m_readEndCalledCount(0),
        m_readFileHeaderCount(0),
        m_readTriangleCount(0),
        m_triangleCount(0)
    {
    }

    //! Called whenever parsing begins.
    void onReadBegin() override
    {
        ++m_readBeginCalledCount;
    }

    //! Called whenever parsing ends. Guaranteed to be called even in the event of errors.
    void onReadEnd() override
    {
        ++m_readEndCalledCount;
    }

    //! Called whenever the file header is parsed.
    void onReadFileHeader(uint8_t* pBytes, size_t byteCount) override
    {
        m_headerBuffer.clear();
        m_headerBuffer.reserve(byteCount);
        m_headerBuffer.assign(pBytes, pBytes + byteCount);
        ++m_readFileHeaderCount;
    }

    //! Called whenever the total triangle count has been parsed.
    void onReadTriangleCount(uint32_t triangleCount) override
    {
        m_triangleCount = triangleCount;
        ++m_readTriangleCount;
    }

    int m_readBeginCalledCount;
    int m_readEndCalledCount;
    int m_readFileHeaderCount;
    int m_readTriangleCount;
    std::vector<char> m_headerBuffer;
    uint32_t m_triangleCount;
};

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

TEST_F(BinarySTLFileReaderTests, testOpeningGoodPathButSmallFile)
{
    try
    {
        BinarySTLFileReader reader(TEST_DATA_DIR + "binarytoosmall.stl");
        FAIL() << "BinarySTLFileReader should have thrown.";
    }
    catch (const std::runtime_error&)
    {
        return;
    }
}

TEST_F(BinarySTLFileReaderTests, testOpeningGoodPath)
{
    const char DATA[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F };

    BinarySTLFileReader reader(TEST_DATA_DIR + "binary_simple.stl");
    TestBinarySTLFileReaderListener listener;
    reader.readFile(listener);

    EXPECT_EQ(listener.m_readBeginCalledCount, 1);
    EXPECT_EQ(listener.m_readEndCalledCount, 1);
    EXPECT_EQ(listener.m_headerBuffer.size(), BINARY_STL_HEADER_SIZE_IN_BYTES);
    EXPECT_EQ(memcmp(listener.m_headerBuffer.data(), &DATA[0], sizeof(DATA)), 0);
    EXPECT_EQ(listener.m_readFileHeaderCount, 1);
    EXPECT_EQ(listener.m_readTriangleCount, 1);
    EXPECT_EQ(listener.m_triangleCount, 1397903696);
}

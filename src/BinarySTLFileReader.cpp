#include "BinarySTLFileReader.h"
#include "FileUtils.h"
#include "Contracts.h"
#include "CallGuard.h"

#include <stdexcept>

/**
 * @since 2024 Jan 21
 */
BinarySTLFileReader::BinarySTLFileReader(const std::string& filepath) :
    m_pFile(nullptr),
    m_totalTriangleCount(0),
    m_currTriangleIndex(0)
{
    if (filepath.empty())
        throw std::runtime_error("STL path cannot be empty.");

    if (!FileUtils::fileExists(filepath))
        throw std::runtime_error("Specified STL file does not exist - " + filepath);

    if (FileUtils::getFileSize(filepath) < MINIMUM_BINARY_STL_SIZE_IN_BYTES)
        throw std::runtime_error("Specified file too small to be a binary STL - " + filepath);

    m_pFile = fopen(filepath.c_str(), "rb");
    if (!m_pFile)
        throw std::runtime_error("Unknown error when opening " + filepath);
}

/**
 * @since 2024 Jan 21
 */
BinarySTLFileReader::~BinarySTLFileReader()
{
    if (m_pFile)
        fclose(m_pFile);
}

/**
 * @since 2024 Jan 24
 */
void BinarySTLFileReader::readFile(BinarySTLFileReaderListener& listener)
{
    invariant_throw(m_pFile != nullptr, "File not opened for reading!");

    fseek(m_pFile, 0, SEEK_SET);

    bool cont = listener.onReadBegin();
    auto parseEndGuard = makeCallGuard([&]() { listener.onReadEnd(); });
    if (!cont)
        return;

    if (!readFileHeader(listener))
        return;

    if (!readTriangleCount(listener))
        return;

    while (readTriangle(listener));
}

/**
 * @since 2024 Jan 24
 */
bool BinarySTLFileReader::readFileHeader(BinarySTLFileReaderListener& listener)
{
    std::array<uint8_t, BINARY_STL_HEADER_SIZE_IN_BYTES> headerData;
    memset(headerData.data(), 0, headerData.size());

    auto bytesRead = fread(headerData.data(), 1, headerData.size(), m_pFile);
    if (bytesRead != headerData.size())
        throw std::runtime_error("Could not read file header.");

    return listener.onReadFileHeader(headerData);
}

/**
 * @since 2024 Jan 24
 */
bool BinarySTLFileReader::readTriangleCount(BinarySTLFileReaderListener& listener)
{
    auto bytesRead = fread(&m_totalTriangleCount, 1, sizeof(m_totalTriangleCount), m_pFile);
    if (bytesRead != sizeof(m_totalTriangleCount))
        throw std::runtime_error("Could not read triangle count.");

    return listener.onReadTriangleCount(m_totalTriangleCount);
}

/**
 * @since 2024 Jan 28
 */
bool BinarySTLFileReader::readTriangle(BinarySTLFileReaderListener& listener)
{
    STLBinaryTriangleData triangle;
    uint16_t attributeCount = 0;

    unsigned char buffer[BINARY_STL_TRIANGLE_SIZE_IN_BYTES + 
        BINARY_STL_TRIANGLE_ATTRIBUTE_BYTE_COUNT_IN_BYTES];

    auto bytesRead = fread(&buffer[0], 1, sizeof(buffer), m_pFile);
    if (bytesRead <= 0)
        return false; // End of file.

    if (bytesRead != sizeof(buffer))
        return listener.onReadUnknownData(&buffer[0], bytesRead);

    if (m_currTriangleIndex >= m_totalTriangleCount)
        return listener.onReadUnknownData(&buffer[0], bytesRead);

    std::copy(&buffer[0], &buffer[0] + BINARY_STL_TRIANGLE_SIZE_IN_BYTES, triangle.data());
    attributeCount = *(reinterpret_cast<uint16_t*>(&buffer[0] + BINARY_STL_TRIANGLE_SIZE_IN_BYTES));
    ++m_currTriangleIndex;
    return listener.onReadTriangle(triangle, attributeCount);
}

/**
 * @since 2024 Feb 11
 */
uint32_t readTriangleCount(const std::string& pathToFile)
{
    // Helper class
    class TriangleCountListener : public BinarySTLFileReaderListener
    {
    public:
        TriangleCountListener() : m_triangleCount(0) {}
        bool onReadTriangleCount(const uint32_t triangleCount) { m_triangleCount = triangleCount; return false; }
        uint32_t m_triangleCount;
    };

    TriangleCountListener listener;
    BinarySTLFileReader reader(pathToFile);
    reader.readFile(listener);
    return listener.m_triangleCount;
}

/**
 * @since 2024 Feb 11
 */
uint32_t calculateTriangleCount(const std::string& pathToFile)
{
    auto fileSize = FileUtils::getFileSize(pathToFile);
    fileSize -= BINARY_STL_HEADER_SIZE_IN_BYTES;
    fileSize -= BINARY_STL_TRIANGLE_COUNT_IN_BYTES;

    return static_cast<uint32_t>(fileSize / (BINARY_STL_TRIANGLE_SIZE_IN_BYTES + BINARY_STL_TRIANGLE_ATTRIBUTE_BYTE_COUNT_IN_BYTES));
}

/**
 * @since 2024 Feb 11
 */
uint32_t hasExtraData(const std::string& pathToFile)
{    
    const uint32_t TRIANGLE_BLOB_SIZE = BINARY_STL_TRIANGLE_SIZE_IN_BYTES + BINARY_STL_TRIANGLE_ATTRIBUTE_BYTE_COUNT_IN_BYTES;
    uint32_t triangleCount = readTriangleCount(pathToFile);
    uint32_t expectedFileSize = BINARY_STL_HEADER_SIZE_IN_BYTES + BINARY_STL_TRIANGLE_COUNT_IN_BYTES + (triangleCount * TRIANGLE_BLOB_SIZE);
    return FileUtils::getFileSize(pathToFile) > expectedFileSize;
}


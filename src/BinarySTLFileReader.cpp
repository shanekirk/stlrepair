#include "BinarySTLFileReader.h"
#include "FileUtils.h"
#include "Contracts.h"
#include "CallGuard.h"

#include <stdexcept>

/**
 * @since 2024 Jan 21
 */
BinarySTLFileReader::BinarySTLFileReader(const std::string& filepath) :
    m_pFile(nullptr)
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
    invariant_throw(m_pFile != nullptr, "File not opened!");

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
    uint32_t triangleCount = 0;

    auto bytesRead = fread(&triangleCount, 1, sizeof(triangleCount), m_pFile);
    if (bytesRead != sizeof(triangleCount))
        throw std::runtime_error("Could not read triangle count.");

    return listener.onReadTriangleCount(triangleCount);
}

/**
 * @since 2024 Jan 28
 */
bool BinarySTLFileReader::readTriangle(BinarySTLFileReaderListener& listener)
{
    uint8_t data[50];

    auto bytesRead = fread(&data[0], 1, sizeof(data), m_pFile);
    if (bytesRead <= 0)
        return false; // End of file.

    if (bytesRead != sizeof(data))
        return listener.onReadUnknownData(&data[0], bytesRead);

    uint16_t* pAttributeCount = reinterpret_cast<uint16_t*>(&data[48]);
    return listener.onReadTriangle(&data[0], sizeof(data) - 2, *pAttributeCount);
}
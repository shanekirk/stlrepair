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

    listener.onReadBegin();
    auto parseEndGuard = makeCallGuard([&]() { listener.onReadEnd(); });

    readFileHeader(listener);
    readTriangleCount(listener);
}

/**
 * @since 2024 Jan 24
 */
void BinarySTLFileReader::readFileHeader(BinarySTLFileReaderListener& listener)
{
    unsigned char headerData[BINARY_STL_HEADER_SIZE_IN_BYTES];
    memset(&headerData[0], 0, sizeof(headerData));

    auto bytesRead = fread(&headerData, 1, sizeof(headerData), m_pFile);
    if (bytesRead != sizeof(headerData))
        throw std::runtime_error("Could not read file header.");

    listener.onReadFileHeader(headerData, sizeof(headerData));
}

/**
 * @since 2024 Jan 24
 */
void BinarySTLFileReader::readTriangleCount(BinarySTLFileReaderListener& listener)
{
    uint32_t triangleCount = 0;

    auto bytesRead = fread(&triangleCount, 1, sizeof(triangleCount), m_pFile);
    if (bytesRead != sizeof(triangleCount))
        throw std::runtime_error("Could not read triangle count.");

    listener.onReadTriangleCount(triangleCount);
}
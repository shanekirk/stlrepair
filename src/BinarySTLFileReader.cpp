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

    // TODO: Validate file size to ensure it's even possible that it's a binary STL file.

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
}

/**
 * @since 2024 Jan 24
 */
void BinarySTLFileReader::readFileHeader(BinarySTLFileReaderListener& listener)
{
    unsigned char headerData[80];
    memset(&headerData[0], 0, sizeof(headerData));

    auto bytesRead = fread(&headerData, 1, sizeof(headerData), m_pFile);
    if (bytesRead != sizeof(headerData))
        throw std::runtime_error("Could not read file header.");

    listener.onReadFileHeader(headerData, sizeof(headerData));
}
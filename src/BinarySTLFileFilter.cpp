#include "BinarySTLFileFilter.h"
#include "Contracts.h"

#include <stdexcept>

/**
 * @since 2024 Feb 04
 */
BinarySTLFileFilter::BinarySTLFileFilter(const std::string& outputFilePath) :
    m_outputFilePath(outputFilePath),
    m_zeroOutHeader(false),
    m_truncateFileToTriangleCount(false),
    m_updateTriangleCount(false),
    m_zeroAttributeByteCounts(false),
    m_readTriangleCount(0),
    m_actualTriangleCount(0)
{
    precondition_throw(!outputFilePath.empty(), std::runtime_error("Output filename cannot be empty."));

    // Zero out the header. If m_zeroOutHeader == true, we'll
    // just write this out as is.
    memset(m_header.data(), 0, m_header.size());
}

/**
 * @since 2024 Feb 04
 */
void BinarySTLFileFilter::onReadEnd()
{
    precondition_throw(m_spWriter != nullptr,
        std::runtime_error("No output file opened for writing."));

    m_spWriter->finalize();

    if ((m_updateTriangleCount) && (m_actualTriangleCount != m_readTriangleCount))
    {
        FILE* pFile = fopen(m_outputFilePath.c_str(), "rb+");
        if (!pFile)
            throw std::runtime_error("Could not update output file triangle count.");

        fseek(pFile, BINARY_STL_HEADER_SIZE_IN_BYTES, SEEK_SET);
        fwrite(&m_actualTriangleCount, 1, sizeof(m_actualTriangleCount), pFile);
        fclose(pFile);
    }
}

/**
 * @since 2024 Feb 04
 */
bool BinarySTLFileFilter::onReadFileHeader(const STLBinaryHeader& header)
{
    if (!m_zeroOutHeader)
        m_header = header;

    return true;
}

/**
 * @since 2024 Feb 04
 */
bool BinarySTLFileFilter::onReadTriangleCount(const uint32_t triangleCount)
{
    m_spWriter = std::make_unique<BinarySTLFileWriter>(
        m_outputFilePath, m_header, triangleCount);

    m_readTriangleCount = triangleCount;

    return true;
}

/**
 * @since 2024 Feb 04
 */
bool BinarySTLFileFilter::onReadTriangle(const STLBinaryTriangleData& triangleData, 
    uint16_t attributeByteCount)
{
    precondition_throw(m_spWriter != nullptr, 
        std::runtime_error("No output file opened for writing."));

    if ((m_truncateFileToTriangleCount) && (m_actualTriangleCount >= m_readTriangleCount))
        return true;

    if (m_zeroAttributeByteCounts) 
        m_spWriter->writeTriangleData(triangleData, 0);
    else
        m_spWriter->writeTriangleData(triangleData, attributeByteCount);

    ++m_actualTriangleCount;

    return true;
}

/**
 * @since 2024 Feb 04
 */
bool BinarySTLFileFilter::onReadUnknownData(const uint8_t* const pData, const size_t dataSize)
{
    precondition_throw(m_spWriter != nullptr,
        std::runtime_error("No output file opened for writing."));

    if ((m_truncateFileToTriangleCount) && (m_actualTriangleCount >= m_readTriangleCount))
        return true;

    m_spWriter->finalize(reinterpret_cast<const char *>(pData), dataSize);

    return true;
}

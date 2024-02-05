#include "BinarySTLFileFilter.h"
#include "Contracts.h"

#include <stdexcept>

/**
 * @since 2024 Feb 04
 */
void BinarySTLFileFilter::onReadEnd()
{
    precondition_throw(m_spWriter != nullptr,
        std::runtime_error("No output file opened for writing."));

    m_spWriter->finalize();
}

/**
 * @since 2024 Feb 04
 */
bool BinarySTLFileFilter::onReadFileHeader(const STLBinaryHeader& header)
{
    m_lastReadHeader = header;
    return true;
}

/**
 * @since 2024 Feb 04
 */
bool BinarySTLFileFilter::onReadTriangleCount(const uint32_t triangleCount)
{
    m_spWriter = std::make_unique<BinarySTLFileWriter>(
        m_outputFilePath, m_lastReadHeader, triangleCount);
    return true;
}

/**
 * @since 2024 Feb 04
 */
bool BinarySTLFileFilter::onReadTriangle(const STLBinaryTriangleData& triangleData, 
    const uint16_t attributeByteCount)
{
    precondition_throw(m_spWriter != nullptr, 
        std::runtime_error("No output file opened for writing."));

    m_spWriter->writeTriangleData(triangleData, attributeByteCount);
    return true;
}

/**
 * @since 2024 Feb 04
 */
bool BinarySTLFileFilter::onReadUnknownData(const uint8_t* const pData, const size_t dataSize)
{
    precondition_throw(m_spWriter != nullptr,
        std::runtime_error("No output file opened for writing."));

    m_spWriter->finalize(reinterpret_cast<const char *>(pData), dataSize);
    return true;
}
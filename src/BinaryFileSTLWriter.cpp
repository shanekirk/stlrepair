#include "BinarySTLFileWriter.h"
#include "CallGuard.h"
#include "Contracts.h"

#include <stdexcept>

/**
 * @since 2024 Feb 01
 */
BinarySTLFileWriter::BinarySTLFileWriter(const std::string& filepath,
    const STLBinaryHeader &header, uint32_t triangleCount) :
    m_pFile(nullptr)
{
    if (filepath.empty())
        throw std::runtime_error("STL output path cannot be empty.");

    m_pFile = fopen(filepath.c_str(), "wb");
    if (!m_pFile)
        throw std::runtime_error("Unknown error when opening " + filepath);

    // Destructors aren't called when exceptions are thrown from 
    // constructors, which is why we're setting up a guard here.
    // We'll dismiss it later if all is well.
    auto exceptionGuard = makeCallGuard([&]() { fclose(m_pFile); });

    size_t bytesWritten = fwrite(header.data(), 1, header.size(), m_pFile);
    if (bytesWritten != header.size())
        throw std::runtime_error("Could not write header to STL file.");

    bytesWritten = fwrite(&triangleCount, 1, sizeof(triangleCount), m_pFile);
    if (bytesWritten != sizeof(triangleCount))
        throw std::runtime_error("Could not write triangle count to STL file.");

    exceptionGuard.dismiss();
}

/**
 * @since 2024 Jan 21
 */
BinarySTLFileWriter::~BinarySTLFileWriter()
{
    finalize();
}

/**
 * @since 2024 Feb 02
 */
void BinarySTLFileWriter::writeTriangleData(const STLBinaryTriangleData& triangle, uint16_t attributeByteCount)
{
    invariant_throw(m_pFile != nullptr, std::runtime_error("File not opened!"));

    auto bytesWritten = fwrite(&triangle, 1, triangle.size(), m_pFile);
    if (bytesWritten != triangle.size())
        throw std::runtime_error("Error writing triangle data to STL file.");

    bytesWritten = fwrite(&attributeByteCount, 1, sizeof(attributeByteCount), m_pFile);
    if (bytesWritten != sizeof(attributeByteCount))
        throw std::runtime_error("Error writing attribute byte count to STL file.");
}

/**
 * @since 2024 Feb 02
 */
void BinarySTLFileWriter::finalize()
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
}

/**
 * @since 2024 Feb 02
 */
void BinarySTLFileWriter::finalize(const char* pBuffer, size_t bufferSize)
{
    invariant_throw(m_pFile != nullptr, std::runtime_error("File not opened!"));

    if ((pBuffer != nullptr) && (bufferSize > 0))
        fwrite(pBuffer, 1, bufferSize, m_pFile);

    finalize();
}

#ifndef STLREPAIR_BINARYSTLFILEWRITER__H_
#define STLREPAIR_BINARYSTLFILEWRITER__H_

#include "STLFileTypes.h"

#include <string>
#include <array>
#include <cstdint>

/**
 * Basic binary STL file writer.
 */
class BinarySTLFileWriter
{
public:

    /**
     * Constructor.
     * 
     * Note that the provided triangle count isn't strictly enforced. This 
     * allows us to append data to the end of the file that isn't truly 
     * triangle data.
     *
     * @param filepath Path to the STL file to create.
     * @param header File header data.
     * @param triangleCount The number of triangles to be written to the STL file.
     * 
     * @throws std::runtime_error
     */
    BinarySTLFileWriter(const std::string& filepath, const STLBinaryHeader &header, const uint32_t triangleCount);

    /**
     * Destructor.
     */
    ~BinarySTLFileWriter();

    /**
     * Writes triangle data to the STL file.
     * 
     * @throws std::runtime_error
     */
    void writeTriangleData(const STLBinaryTriangleData& triangle, uint16_t attributeByteCount);

    /**
     * Calling this function effectively flushes data buffers and closes the 
     * file for writing. Note that this function will be invoked automatically 
     * by the destructor, so calling it explicitly is entirely optional.
     * 
     * Once this function is called, no more data can be written to the STL file.
     */
    void finalize();

    /**
     * Just like to finalize(), with the addition of writing a blob of data of
     * an arbitrary size to the end of the file just before closing the file 
     * out. Some exporters tack extra data on to the end of the file. This is 
     * the mechanism to use for that if we need to do such things.
     */
    void finalize(const char* pBuffer, size_t bufferSize);

private:

    FILE* m_pFile;
};

#endif

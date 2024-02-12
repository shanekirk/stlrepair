#ifndef STLREPAIR_BINARYSTLFILEREADER__H_
#define STLREPAIR_BINARYSTLFILEREADER__H_

#include "STLFileTypes.h"

#include <string>
#include <cstdio>
#include <array>

/**
 * Callback interface for anything wishing to consume parsed data from the
 * BinarySTLFileReader.
 *
 * Returning false from any of these functions will cause the parser to
 * stop immediately.
 *
 * Implementers should NOT throw.
 */
class BinarySTLFileReaderListener
{
public:

    //! Destructor.
    virtual ~BinarySTLFileReaderListener() {}

    //! Called whenever parsing begins.
    virtual bool onReadBegin() { return true; }

    //! Called whenever parsing ends. Guaranteed to be called even in the event of errors.
    virtual void onReadEnd() {}

    //! Called whenever the file header is parsed.
    virtual bool onReadFileHeader(const STLBinaryHeader & /*header*/) { return true; }

    //! Called whenever the total triangle count has been parsed.
    virtual bool onReadTriangleCount(const uint32_t /*triangleCount*/) { return true; }

    /**
     * Called whenever a triangle has been read.
     * Note that we don't currently need to tease apart the triangle data for
     * this app. We only need to treat it as a binary blob. This may change
     * in the future.
     */
    virtual bool onReadTriangle(const STLBinaryTriangleData &triangleData,
        const uint16_t attributeByteCount) { return true; }

    /**
     * Called whenever a blob of unknown data is encountered. This usually
     * happens near the end of a file which has truncated triangle data or
     * an exporter has tacked something else on to the end.
     *
     * Regardless of why, it basically indicates the data isn't the same size as
     * triangle data.
     */
    virtual bool onReadUnknownData(const uint8_t* const pData, const size_t dataSize) { return true; }
};

/**
 * Basic binary STL file reader.
 */
class BinarySTLFileReader
{
public:

    /**
     * Constructor.
     *
     * @throws std::runtime_error
     */
    BinarySTLFileReader(const std::string &filepath);

    /**
     * Destructor.
     */
    ~BinarySTLFileReader();

    /**
     * Attempts to parse the file. Parsed data will be provided to the
     * given listener.
     *
     * @throws std::runtime_error
     */
    void readFile(BinarySTLFileReaderListener &listener);

private:

    bool readFileHeader(BinarySTLFileReaderListener &listener);
    bool readTriangleCount(BinarySTLFileReaderListener &listener);
    bool readTriangle(BinarySTLFileReaderListener& listener);

    FILE *m_pFile;
    uint32_t m_totalTriangleCount;
    uint32_t m_currTriangleIndex;
};

/**
 * Utility function for quickly reading the number of triangles reported by the
 * binary STL file.
 */
uint32_t readTriangleCount(const std::string& pathToFile);

/**
 * Utility function for calculating the number of expected triangles based entirely
 * on the size of the file.
 */
uint32_t calculateTriangleCount(const std::string& pathToFile);

/**
 * Utility function for determining if the specified file has junk data.
 */
uint32_t hasExtraData(const std::string& pathToFile);

#endif

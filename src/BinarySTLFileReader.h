#ifndef STLREPAIR_BINARYSTLFILEREADER__H_
#define STLREPAIR_BINARYSTLFILEREADER__H_

#include <string>
#include <cstdio>

constexpr const int BINARY_STL_HEADER_SIZE_IN_BYTES = 80;
constexpr const int BINARY_STL_TRIANGLE_COUNT_IN_BYTES = 4;
constexpr const int BINARY_STL_TRIANGLE_SIZE_IN_BYTES = 50;

constexpr const int MINIMUM_BINARY_STL_SIZE_IN_BYTES =
    BINARY_STL_HEADER_SIZE_IN_BYTES +
    BINARY_STL_TRIANGLE_COUNT_IN_BYTES +
    BINARY_STL_TRIANGLE_SIZE_IN_BYTES;

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
    virtual bool onReadFileHeader(uint8_t* /*pBytes*/, size_t /*byteCount*/) { return true; }

    //! Called whenever the total triangle count has been parsed.
    virtual bool onReadTriangleCount(uint32_t /*triangleCount*/) { return true; }
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
    BinarySTLFileReader(const std::string& filepath);

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
    void readFile(BinarySTLFileReaderListener& listener);

private:

    bool readFileHeader(BinarySTLFileReaderListener& listener);
    bool readTriangleCount(BinarySTLFileReaderListener& listener);

    FILE *m_pFile;
};

#endif

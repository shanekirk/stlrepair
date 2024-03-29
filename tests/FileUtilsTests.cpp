#include "FileUtils.h"
#include "CallGuard.h"

#include "gtest/gtest.h"

#include <fstream>

extern std::string TEST_DATA_DIR; // Yeah, I don't feel great about it. But it is what it is for now.

class FileUtilsTests : public testing::Test
{

};

TEST_F(FileUtilsTests, testGetFileSizeWithEmptyPath)
{
    EXPECT_EQ(FileUtils::getFileSize(""), 0);
}

TEST_F(FileUtilsTests, testGetFileSizeWithFileThatShouldntExist)
{
    EXPECT_EQ(FileUtils::getFileSize("z:\\this\\file\\shouldnt\\be\\here"), 0);
}

TEST_F(FileUtilsTests, testGetFileSizeWithGoodEmptyFile)
{
    std::ofstream out("test.file");
    out.close();
    auto fileGuard = makeCallGuard([]() { _unlink("test.file"); });
    EXPECT_EQ(FileUtils::getFileSize("test.file"), 0);
}

TEST_F(FileUtilsTests, testGetFileSizeWithGoodFileWithContent)
{
    std::ofstream out("test.file");
    out << 1 << 2 << 3;
    out.close();
    auto fileGuard = makeCallGuard([]() { _unlink("test.file"); });
    EXPECT_EQ(FileUtils::getFileSize("test.file"), 3);
}

TEST_F(FileUtilsTests, testFileExistsWithEmptyPath)
{
    EXPECT_FALSE(FileUtils::fileExists(""));
}

TEST_F(FileUtilsTests, testFileExistsWithFileThatShouldntExist)
{
    EXPECT_FALSE(FileUtils::fileExists("z:\\this\\file\\shouldnt\\be\\here"));
}

TEST_F(FileUtilsTests, testFileExistsWithFileThatShouldExist)
{
    std::ofstream out("test.file");
    out.close();
    auto fileGuard = makeCallGuard([]() { _unlink("test.file"); });
    EXPECT_TRUE(FileUtils::fileExists("test.file"));
}

TEST_F(FileUtilsTests, testSplitPathWithEmptyString)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("", dir, basename, extension);
    EXPECT_TRUE(dir.empty());
    EXPECT_TRUE(basename.empty());
    EXPECT_TRUE(extension.empty());
}

TEST_F(FileUtilsTests, testSplitPathWithDirectoryOnlyVersion1)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:", dir, basename, extension);
    EXPECT_EQ(dir, "c:");
    EXPECT_TRUE(basename.empty());
    EXPECT_TRUE(extension.empty());
}

TEST_F(FileUtilsTests, testSplitPathWithDirectoryOnlyVersion2)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:\\", dir, basename, extension);
    EXPECT_EQ(dir, "c:\\");
    EXPECT_TRUE(basename.empty());
    EXPECT_TRUE(extension.empty());
}

TEST_F(FileUtilsTests, testSplitPathWithDirectoryOnlyVersion3)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:/", dir, basename, extension);
    EXPECT_EQ(dir, "c:/");
    EXPECT_TRUE(basename.empty());
    EXPECT_TRUE(extension.empty());
}

TEST_F(FileUtilsTests, testSplitPathWithFilenameOnlyNoExtension)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("file", dir, basename, extension);
    EXPECT_TRUE(dir.empty());
    EXPECT_EQ(basename, "file");
    EXPECT_TRUE(extension.empty());
}

TEST_F(FileUtilsTests, testSplitPathWithDirectoryAndFilenameWithNoExtension)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:/file", dir, basename, extension);
    EXPECT_EQ(dir, "c:/");
    EXPECT_EQ(basename, "file");
    EXPECT_TRUE(extension.empty());
}

TEST_F(FileUtilsTests, testSplitPathWithDirectoryAndFilenameWithNoExtensionVersion2)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:\\file", dir, basename, extension);
    EXPECT_EQ(dir, "c:\\");
    EXPECT_EQ(basename, "file");
    EXPECT_TRUE(extension.empty());
}

TEST_F(FileUtilsTests, testSplitPathWithDirectoryAndFilenameWithNoExtensionVersion3)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:file", dir, basename, extension);
    EXPECT_EQ(dir, "c:");
    EXPECT_EQ(basename, "file");
    EXPECT_TRUE(extension.empty());
}

TEST_F(FileUtilsTests, testSplitPathFilenameOnlyWithExtension)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("file.stl", dir, basename, extension);
    EXPECT_TRUE(dir.empty());
    EXPECT_EQ(basename, "file");
    EXPECT_EQ(extension, "stl");
}

TEST_F(FileUtilsTests, testSplitPathWithDirectoryAndFilenameWithExtension)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:/file.stl", dir, basename, extension);
    EXPECT_EQ(dir, "c:/");
    EXPECT_EQ(basename, "file");
    EXPECT_EQ(extension, "stl");
}

TEST_F(FileUtilsTests, testSplitPathWithDirectoryAndFilenameWithExtensionVersion2)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:\\file.stl", dir, basename, extension);
    EXPECT_EQ(dir, "c:\\");
    EXPECT_EQ(basename, "file");
    EXPECT_EQ(extension, "stl");
}

TEST_F(FileUtilsTests, testSplitPathWithDirectoryAndFilenameWithExtensionVersion3)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:file.stl", dir, basename, extension);
    EXPECT_EQ(dir, "c:");
    EXPECT_EQ(basename, "file");
    EXPECT_EQ(extension, "stl");
}

TEST_F(FileUtilsTests, testSplitPathFileHavingMultipleDots)
{
    std::string dir, basename, extension;
    FileUtils::splitPath("c:file.test.stl", dir, basename, extension);
    EXPECT_EQ(dir, "c:");
    EXPECT_EQ(basename, "file.test");
    EXPECT_EQ(extension, "stl");
}

TEST_F(FileUtilsTests, testGenerateUniqueFilePathWithEmptyString)
{
    std::string newPath = FileUtils::generateUniqueFilePath("");
    EXPECT_TRUE(newPath.empty());
}

TEST_F(FileUtilsTests, testGenerateUniqueFilePathWithFileThatShouldntAlreadyExist)
{
    std::string newPath = FileUtils::generateUniqueFilePath("z:\\this\\file\\shouldnt\\exist.txt");
    EXPECT_EQ(newPath, "z:\\this\\file\\shouldnt\\exist.txt");
}

TEST_F(FileUtilsTests, testGenerateUniqueFilePathWithFileThatAlreadyExists)
{
    std::string newPath = FileUtils::generateUniqueFilePath(TEST_DATA_DIR + "binary_5mm_sphere.stl");
    EXPECT_EQ(newPath, TEST_DATA_DIR + "binary_5mm_sphere(1).stl");
}

TEST_F(FileUtilsTests, testGenerateUniqueFilePathWithTwoFilesThatAlreadyExists)
{
    const std::string TEST_FILE_1(TEST_DATA_DIR + "dummyfile.stl");
    const std::string TEST_FILE_2(TEST_DATA_DIR + "dummyfile(1).stl");
    const std::string EXPECTED_TEST_FILE(TEST_DATA_DIR + "dummyfile(2).stl");

    // Creating test files
    {
        std::ofstream o1(TEST_FILE_1);
        std::ofstream o2(TEST_FILE_2);
    }
    auto dummyGuard = makeCallGuard([&]() { _unlink(TEST_FILE_1.c_str()); });
    auto dummyGuard2 = makeCallGuard([&]() { _unlink(TEST_FILE_2.c_str()); });

    std::string newPath = FileUtils::generateUniqueFilePath(TEST_FILE_1);
    EXPECT_EQ(newPath, EXPECTED_TEST_FILE);
}
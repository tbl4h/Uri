#include <gtest/gtest.h>
//#include "Uri/include/Uri/Uri.hpp"
#include "../Uri.hpp"
//#include <Uri/Uri.hpp>

TEST(UriTests, ParseFromString) {
    Uri::Uri uri;    
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/foo/bar"));
    ASSERT_EQ("http",uri.GetScheme());
    ASSERT_EQ("www.example.com", uri.GetHost());
    ASSERT_EQ(
        (std::vector<std::string>{
        "",
        "foo",
        "bar",        
    }),
    uri.GetPath());
};

TEST(UriTests, ParseFromString2) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/"));
    ASSERT_EQ("http",uri.GetScheme());
    ASSERT_EQ("www.example.com", uri.GetHost());
};


TEST(UriTests, ParseFromString3) {
    Uri::Uri uri;
    uri.setPathDelimiter(":");
    ASSERT_TRUE(uri.ParseFromString("urn:book:fantasy:Hobbit"));
    ASSERT_EQ("urn",uri.GetScheme());
    ASSERT_EQ("", uri.GetHost());
    ASSERT_EQ(
        (std::vector<std::string>{
        "book",
        "fantasy",
        "Hobbit",

    }),    
     uri.GetPath());
};
TEST(UriTests, ParseFromStringPathCornerCases) {
    struct TestVector {
        std::string pathIn;
        std::vector<std::string> pathOut;
    };
    const std::vector<TestVector> testVectors {
        {"",{}},
        {"/",{""}},
        {"/foo",{"","foo"}},
        {"foo/",{"foo",""}},
    };
    int iter = 0;
    for (const auto& testVector : testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(testVector.pathIn)) << iter;
        ASSERT_EQ(testVector.pathOut,uri.GetPath()) ;
        iter++;
    }
};
TEST(UriTests, ParseFromStringHasAPortNumber){
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:8080/foo/bar"));
    ASSERT_EQ("http",uri.GetScheme());
    ASSERT_EQ("www.example.com",uri.GetHost());
    ASSERT_TRUE(uri.HasPort());
    ASSERT_EQ(8080,uri.GetPort());
};
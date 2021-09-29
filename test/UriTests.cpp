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

TEST(UriTests, ParseFromStringHasNotAPortNumber){
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:spam/foo/bar"));
    ASSERT_EQ("http",uri.GetScheme());
    ASSERT_EQ("www.example.com",uri.GetHost());
    ASSERT_FALSE(uri.HasPort());
    ASSERT_EQ(80,uri.GetPort());
};
TEST(UriTests, ParseFromStringHasNotAPortNegative){
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:-123/foo/bar"));
    ASSERT_EQ("http",uri.GetScheme());
    ASSERT_EQ("www.example.com",uri.GetHost());
    ASSERT_FALSE(uri.HasPort());    
};
TEST(UriTests, ParseFromStringHasNotAPortSpamAndValid){
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:spam123/foo/bar"));
    ASSERT_EQ("http",uri.GetScheme());
    ASSERT_EQ("www.example.com",uri.GetHost());
    ASSERT_FALSE(uri.HasPort());
    ASSERT_EQ(80,uri.GetPort());
};
TEST(UriTests, ParseFromStringIsRelativeVsNonRelativePaths) {
    struct TestVector {
        std::string uriString;
        bool isRelative;
    };
    const std::vector<TestVector> testVectors {
        {"http://www.ecample.com/",false},
        {"http://www.example.com", false},
        {"/",true},
        {"foo",true},
    };
    int iter = 0;
    for (const auto& testVector : testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(testVector.uriString)) << iter;
        ASSERT_EQ(testVector.isRelative,uri.IsRelativeReference());
        iter++;
    }
};

TEST(UriTests, ParseFromStringEndAfterAuthority) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/"));
    
};
TEST(UriTests, ParseFromStringContainsRelativePath) {
    struct TestVector {
        std::string uriString;
        bool containRelative;
    };
    const std::vector<TestVector> testVectors {
        {"http://www.example.com/",false},
        {"http://www.example.com", true},
        {"/",false},
        {"foo",true},
        /*
         *This is only valid test vector if we underastand
         *correctly that an empty string is a valid 
         *relative reference uri with an empty path.
         */
        {"",true},
    };
    int iter = 0;
    for (const auto& testVector : testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(testVector.uriString)) << iter;
        ASSERT_EQ(testVector.containRelative,uri.ContainRelativePath());
        iter++;
    }
};
TEST(UriTests, ParseFromStringFragment) {
    struct TestVector {
        std::string uriString;
        std::string fragment;
    };
    const std::vector<TestVector> testVectors {
        {"http://www.ecample.com/",""},
        {"http://www.example.com?foo", ""},
        {"http://www.example.com#foo", "foo"},
        {"http://www.example.com?foo#bar", "bar"},
    };
    int iter = 0;
    for (const auto& testVector : testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(testVector.uriString)) << iter;
        ASSERT_EQ(testVector.fragment,uri.GetFragment());
        iter++;
    }
};
TEST(UriTests, ParseFromStringQuery) {
    struct TestVector {
        std::string uriString;
        std::string query;
    };
    const std::vector<TestVector> testVectors {
        {"http://www.ecample.com/",""},
        {"http://www.example.com?foo", "foo"},
        {"http://www.example.com#foo", ""},
        {"http://www.example.com?foo#bar", "foo"},
        {"http://www.example.com?foo?earth#bar", "foo?earth"},
        {"http://www.example.com/?", ""},
    };
    int iter = 0;
    for (const auto& testVector : testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(testVector.uriString)) << iter;
        ASSERT_EQ(testVector.query,uri.GetQuery());
        iter++;
    }
};
TEST(UriTests, ParseFromStringUserInfo) {
    struct TestVector {
        std::string uriString;
        std::string userInfo;
    };
    const std::vector<TestVector> testVectors {
        {"http://www.ecample.com/",""},
        {"http://joe@www.example.com", "joe"},
        {"/",""},
        {"foo",""},

    };
    int iter = 0;
    for (const auto& testVector : testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.ParseFromString(testVector.uriString)) << iter;
        ASSERT_EQ(testVector.userInfo,uri.GetUserInfo());
        iter++;
    }
};
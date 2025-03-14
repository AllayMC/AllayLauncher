#include "test_suite.h" // IWYU pragma: keep

#include "util/string.h"

using namespace util::string;

TEST(TestUtilString, StartsWith) {
    EXPECT_TRUE(starts_with("docs.allaymc.org", "docs"));
    EXPECT_FALSE(starts_with("docs.allaymc.org", "org"));
}

TEST(TestUtilString, EndsWith) {
    EXPECT_TRUE(ends_with("docs.allaymc.org", "org"));
    EXPECT_FALSE(ends_with("docs.allaymc.org", "docs"));
}

TEST(TestUtilString, RemovePrefix) {
    std::string      str    = "docs.allaymc.org";
    std::string_view strv   = "docs.allaymc.org";
    std::string      prefix = "docs";
    remove_prefix(str, prefix);
    remove_prefix(strv, prefix);
    EXPECT_EQ(str, ".allaymc.org");
    EXPECT_EQ(strv, ".allaymc.org");
}

TEST(TestUtilString, RemoveSuffix) {
    std::string      str    = "docs.allaymc.org";
    std::string_view strv   = "docs.allaymc.org";
    std::string      suffix = "org";
    remove_suffix(str, suffix);
    remove_suffix(strv, suffix);
    EXPECT_EQ(str, "docs.allaymc.");
    EXPECT_EQ(strv, "docs.allaymc.");
}

TEST(TestUtilString, ToInt32) {
    std::string str = "445514";
    uint32_t    val = 445514;
    EXPECT_EQ(to_int32(str), val);
}

TEST(TestUtilString, ToInt32_Neg) {
    std::string str = "-445514";
    uint32_t    val = -445514;
    EXPECT_EQ(to_int32(str), val);
}

TEST(TestUtilString, Split_Nocopy) {
    std::string str = "to be, or not to be, that is the question";
    auto        ws  = split_nocopy(str, " ");
    EXPECT_EQ(ws.size(), 10);
    EXPECT_EQ(ws.at(0), "to");
    EXPECT_EQ(ws.at(1), "be,");
    EXPECT_EQ(ws.at(2), "or");
    EXPECT_EQ(ws.at(3), "not");
    EXPECT_EQ(ws.at(4), "to");
    EXPECT_EQ(ws.at(5), "be,");
    EXPECT_EQ(ws.at(6), "that");
    EXPECT_EQ(ws.at(7), "is");
    EXPECT_EQ(ws.at(8), "the");
    EXPECT_EQ(ws.at(9), "question");
}

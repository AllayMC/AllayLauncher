#include "test_suite.h" // IWYU pragma: keep

#include "base/version.h"

TEST(BaseVersionTest, SimpleParser) {
    auto v1 = Version::parse("1.0.0");
    EXPECT_EQ(v1->m_major, 1);
    EXPECT_EQ(v1->m_minor, 0);
    EXPECT_EQ(v1->m_revision, 0);
}

TEST(BaseVersionTest, SimpleParser_2) {
    auto v1 = Version::parse("47.3.1");
    EXPECT_EQ(v1->m_major, 47);
    EXPECT_EQ(v1->m_minor, 3);
    EXPECT_EQ(v1->m_revision, 1);
}

TEST(BaseVersionTest, SimpleParser_4) {
    auto v1 = Version::parse("0.1.1");
    EXPECT_EQ(v1->m_major, 0);
    EXPECT_EQ(v1->m_minor, 1);
    EXPECT_EQ(v1->m_revision, 1);
}

TEST(BaseVersionTest, ToleranceParser_IgnoreRedundant) {
    auto v1 = Version::parse("2.3.7.4.4.4");
    EXPECT_EQ(v1->m_major, 2);
    EXPECT_EQ(v1->m_minor, 3);
    EXPECT_EQ(v1->m_revision, 7);
}

TEST(BaseVersionTest, ToleranceParser_AutoZeroPadding) {
    auto v1 = Version::parse("2");
    EXPECT_EQ(v1->m_major, 2);
    EXPECT_EQ(v1->m_minor, 0);
    EXPECT_EQ(v1->m_revision, 0);
}

TEST(BaseVersionTest, ParseBadVersion) {
    auto v1 = Version::parse("###");
    EXPECT_EQ(v1.has_value(), false);
}

TEST(BaseVersionTest, ParseBadVersion_AllZero) {
    auto v1 = Version::parse("0.0.0");
    EXPECT_EQ(v1.has_value(), false);
}

TEST(BaseVersionTest, Comparator_All) {
    auto v1 = *Version::parse("1.0.0");
    auto v2 = *Version::parse("1.0.2");
    EXPECT_LT(v1, v2);
    EXPECT_LE(v1, v2);
    EXPECT_GT(v2, v1);
    EXPECT_GE(v2, v1);
    EXPECT_EQ(v1 == v2, false);
    EXPECT_EQ(v1 != v2, true);
}

TEST(BaseVersionTest, OStream) {
    auto               v1 = *Version::parse("5.6.7");
    std::ostringstream oss;
    oss << v1;
    EXPECT_EQ(oss.str(), "5.6.7");
}

TEST(BaseVersionTest, Formatter) {
    auto v1  = *Version::parse("5.6.7");
    auto res = fmt::format("{}", v1);
    EXPECT_EQ(res, "5.6.7");
}

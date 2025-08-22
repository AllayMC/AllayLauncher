#include "test_suite.h" // IWYU pragma: keep

#include "util/java.h"

using namespace util::java;

TEST(TestJavaVersionDetection, Java_8) {
    auto version  = installed_version(R"(java version "1.8.0_421"
Java(TM) SE Runtime Environment (build 1.8.0_421-b08)
Java HotSpot(TM) 64-Bit Server VM (build 25.421-b08, mixed mode)
)");
    auto expected = Version{1, 8, 0};
    EXPECT_EQ(version, expected);
}

TEST(TestJavaVersionDetection, OpenJDK_11) {
    auto version  = installed_version(R"(openjdk version "11.0.23" 2024-04-16
OpenJDK Runtime Environment (build 11.0.23+9-LTS)
OpenJDK 64-Bit Server VM (build 11.0.23+9-LTS, mixed mode)
)");
    auto expected = Version{11, 0, 23};
    EXPECT_EQ(version, expected);
}

TEST(TestJavaVersionDetection, OpenJDK_17) {
    auto version  = installed_version(R"(openjdk version "17.0.11" 2024-04-16
OpenJDK Runtime Environment (build 17.0.11+9-LTS)
OpenJDK 64-Bit Server VM (build 17.0.11+9-LTS, mixed mode, sharing)
)");
    auto expected = Version{17, 0, 11};
    EXPECT_EQ(version, expected);
}

TEST(TestJavaVersionDetection, OpenJDK_21) {
    auto version  = installed_version(R"(openjdk version "21.0.3" 2024-04-16 LTS
OpenJDK Runtime Environment (build 21.0.3+9-LTS)
OpenJDK 64-Bit Server VM (build 21.0.3+9-LTS, mixed mode, sharing)
)");
    auto expected = Version{21, 0, 3};
    EXPECT_EQ(version, expected);
}

TEST(TestJavaVersionDetection, OpenJDK_23) {
    auto version  = installed_version(R"(openjdk version "23"
2024-09-17 OpenJDK Runtime Environment (build 23+37-1231)
OpenJDK 64-Bit Server VM (build 23+37-1231, mixed mode, sharing)
)");
    auto expected = Version{23, 0, 0};
    EXPECT_EQ(version, expected);
}

TEST(TestJavaVersionDetection, OpenJDK_25_EA) {
    auto version  = installed_version(R"(openjdk version "25-ea" 2026-03-17
OpenJDK Runtime Environment (build 25-ea+3-19)
OpenJDK 64-Bit Server VM (build 25-ea+3-19, mixed mode, sharing)
)");
    auto expected = Version{25, 0, 0};
    EXPECT_EQ(version, expected);
}

TEST(TestJavaVersionDetection, OpenJDK_GraalVM_11) {
    auto version  = installed_version(R"(openjdk version "11.0.16" 2022-07-19
OpenJDK Runtime Environment GraalVM EE 22.2.0 (build 11.0.16+8-jvmci-22.2-b08)
OpenJDK 64-Bit Server VM GraalVM EE 22.2.0 (build 11.0.16+8-jvmci-22.2-b08, mixed mode, sharing)

)");
    auto expected = Version{11, 0, 16};
    EXPECT_EQ(version, expected);
}

TEST(TestJavaVersionDetection, GraalVM_21) {
    auto version  = installed_version(R"(java version "21.0.8" 2025-07-15 LTS
Java(TM) SE Runtime Environment Oracle GraalVM 21.0.8+12.1 (build 21.0.8+12-LTS-jvmci-23.1-b72)
Java HotSpot(TM) 64-Bit Server VM Oracle GraalVM 21.0.8+12.1 (build 21.0.8+12-LTS-jvmci-23.1-b72, mixed mode, sharing)
)");
    auto expected = Version{21, 0, 8};
    EXPECT_EQ(version, expected);
}

TEST(TestJavaVersionDetection, GraalVM_24) {
    auto version  = installed_version(R"(java version "24.0.2" 2025-07-15
Java(TM) SE Runtime Environment Oracle GraalVM 24.0.2+11.1 (build 24.0.2+11-jvmci-b01)
Java HotSpot(TM) 64-Bit Server VM Oracle GraalVM 24.0.2+11.1 (build 24.0.2+11-jvmci-b01, mixed mode, sharing)
)");
    auto expected = Version{24, 0, 2};
    EXPECT_EQ(version, expected);
}

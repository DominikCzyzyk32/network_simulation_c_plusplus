#include "gtest/gtest.h"
#include "gmock/gmock.h"
//#include "../../googletest-master/googletest/include/gtest/gtest.h"
//#include "../../googletest-master/googlemock/include/gmock/gmock.h"


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

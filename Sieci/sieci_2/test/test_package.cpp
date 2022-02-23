//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "package.hpp"
#include "types.hpp"

//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "package.hpp"
#include "types.hpp"

TEST(PackageTest, IsAssignedIdLowest) {
    // przydzielanie ID o jeden większych -- utworzenie dwóch obiektów pod rząd

    Package p1;
    Package p2;

    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(PackageTest, IsIdReused) {
    // przydzielanie ID po zwolnionym obiekcie

    {
        Package p1;
    }
    Package p2;

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsMoveConstructorCorrect) {
    Package p1;
    Package p2(std::move(p1));

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsAssignmentOperatorCorrect) {
    Package p1;
    Package p2 = std::move(p1);

    EXPECT_EQ(p2.get_id(), 1);
}

//// Test napisany przez Bartka ;)
//TEST(PackageTest, IsDestructorCorrect){
//    Package p1;
//
//    Package p_with_id_2;
//
//    {
//        Package p2(7);
//        EXPECT_EQ(p2.get_id(), 7);
//    }
//
//    EXPECT_EQ(p1.freed_IDs.empty(), false);
//    EXPECT_EQ(p1.freed_IDs.find(7), p1.freed_IDs.begin());
//
//    Package p3;
//    EXPECT_EQ(p1.freed_IDs.empty(), true);
//
//    EXPECT_EQ(p3.get_id(), 7);
//}
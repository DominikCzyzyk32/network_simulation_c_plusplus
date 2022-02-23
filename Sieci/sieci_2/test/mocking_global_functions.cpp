#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "global_functions_mock.hpp"

#include <nodes.hpp>
#include <nodes_mocks.hpp>


// === TESTY ===
using ::testing::Return;

//TEST_F(ReceiverPreferencesChoosingTest, TestSomething) {
//
//  EXPECT_CALL(global_function_mock, generate_canonical_mocked_implementation()).WillOnce(Return(0.3)).WillOnce(Return(0.7));
//
//  ReceiverPreferences rp;
//
//  MockReceiver r1, r2;
//  rp.add_receiver(&r1);
//  rp.add_receiver(&r2);
//
//  if (rp.begin()->first == &r1) {
//      EXPECT_EQ(rp.choose_receiver(), &r1);
//      EXPECT_EQ(rp.choose_receiver(), &r2);
//  } else {
//      EXPECT_EQ(rp.choose_receiver(), &r2);
//      EXPECT_EQ(rp.choose_receiver(), &r1);
//  }
//}



//TEST_F(ReceiverPreferencesChoosingTest, ChooseReceiver2) {
//    // Upewnij się, że odbiorcy wybierani są z właściwym prawdopodobieństwem.
//
//    EXPECT_CALL(global_function_mock, generate_canonical()).WillOnce(Return(0.3)).WillOnce(Return(0.7));
//
//    ReceiverPreferences rp;
//
//    MockReceiver r1, r2;
//    rp.add_receiver(&r1);
//    rp.add_receiver(&r2);
//
//    if (rp.begin()->first == &r1) {
//        std::cout << "IF" << std::endl;
//        EXPECT_EQ(rp.choose_receiver(), &r1);
//        EXPECT_EQ(rp.choose_receiver(), &r2);
//    } else {
//        std::cout << "ELSE" << std::endl;
//        EXPECT_EQ(rp.choose_receiver(), &r2);
//        EXPECT_EQ(rp.choose_receiver(), &r1);
//    }
//}
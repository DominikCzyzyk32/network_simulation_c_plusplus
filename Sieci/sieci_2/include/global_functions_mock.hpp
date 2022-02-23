// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#ifndef NET_SIMULATION_GLOBAL_FUNCTIONS_MOCK_HPP
#define NET_SIMULATION_GLOBAL_FUNCTIONS_MOCK_HPP

#include "nodes.hpp"
#include <nodes_mocks.hpp>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <functional>

class GlobalFunctionMock {
public:
    MOCK_METHOD0(generate_canonical_mocked_implementation, double());
};


class GlobalFunctionFixture: public ::testing::Test {
public:
    GlobalFunctionFixture() {
        generate_canonical = [&]() {
            return global_function_mock.generate_canonical_mocked_implementation();
        };
    }

    ~GlobalFunctionFixture() {
        generate_canonical = default_generate_canonical_implementation;
    }
protected:
    GlobalFunctionMock global_function_mock;
};


class ReceiverPreferencesChoosingTest: public GlobalFunctionFixture {
};

#endif //NET_SIMULATION_GLOBAL_FUNCTIONS_MOCK_HPP

// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#ifndef NET_SIMULATION_TYPES_HPP
#define NET_SIMULATION_TYPES_HPP

#include <functional>

/*
            aliasy
*/

using ElementID = int;
using ProbabilityGenerator = std::function<double()>;
using Time = int;
using TimeOffset = int;

#endif //NET_SIMULATION_TYPES_HPP

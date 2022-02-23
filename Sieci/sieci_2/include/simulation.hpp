// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#ifndef NET_SIMULATION_SIMULATION_HPP
#define NET_SIMULATION_SIMULATION_HPP

/*
        funkcja simulate()
*/

#include "factory.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf);

#endif //NET_SIMULATION_SIMULATION_HPP

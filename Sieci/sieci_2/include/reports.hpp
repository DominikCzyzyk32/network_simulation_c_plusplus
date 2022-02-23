// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#ifndef NET_SIMULATION_REPORTS_HPP
#define NET_SIMULATION_REPORTS_HPP

/*
        klasy SpecificTurnsReportNotifier, IntervalReportNotifier
        funkcje generate_structure_report(), generate_simulation_turn_report()

*/
#include <iostream>
#include <list>
#include <algorithm>
#include <sstream>
#include <vector>
#include <istream>
#include <string>
#include "types.hpp"
#include "nodes.hpp"
#include "factory.hpp"

void generate_structure_report(const Factory& factory, std::ostream& os);
void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time t);


class SpecificTurnsReportNotifier{
public:
    SpecificTurnsReportNotifier(std::set<Time> turns): turns_(turns) {}

    bool should_generate_report(Time t) { return turns_.find(t) != turns_.end(); }

private:
    std::set<Time> turns_;
};


class IntervalReportNotifier{
public:
    IntervalReportNotifier(TimeOffset to): to_(to) {}

    bool should_generate_report(Time t) { return (t-1) % to_ == 0; }

private:
    TimeOffset to_;
};

#endif //NET_SIMULATION_REPORTS_HPP

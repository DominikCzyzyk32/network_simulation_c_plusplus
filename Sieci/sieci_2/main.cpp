#include <iostream>
#include <set>
#include <map>
#include <functional>
#include <iomanip>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>

#include "nodes_mocks.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include "nodes.hpp"
#include "factory.hpp"
#include "reports.hpp"
#include "simulation.hpp"


int main(void){



//// testy
    std::filebuf fb_input;
    std::filebuf fb_output;
    std::filebuf fb_structure_report_output;
    std::filebuf fb_simulation_turn_report_output;

    if(fb_input.open("../factory_structure_files/struct-input_invers.txt", std::ios::in)
    && fb_output.open("../factory_structure_files/struct-output.txt", std::ios::out)
    && fb_structure_report_output.open("../reports/structure_report.txt", std::ios::out)
    && fb_simulation_turn_report_output.open("../reports/simulation_turn_report.txt", std::ios::out)){

        std::istream is(&fb_input);
        Factory factory = load_factory_structure(is);
        fb_input.close();

        if(factory.is_consistent()){
            std::cout << "FACTORY IS CONSISTENT" << std::endl;
        }else{
            std::cout << "ERROR - FACTORY IS NOT CONSISTENT" << std::endl;
        }


        bool no_structure_report = true;

        std::ostream os_structure_report(&fb_structure_report_output);
        std::ostream os_simulation_turn_report(&fb_simulation_turn_report_output);


//        SpecificTurnsReportNotifier spec_notifier(std::set<Time>{1,4});
//        simulate(factory, 5, [&spec_notifier, &os_simulation_turn_report, &os_structure_report, &no_structure_report](Factory& f, TimeOffset t_offset) {
//            if (spec_notifier.should_generate_report(t_offset)) {
//                if(no_structure_report){
//                    generate_structure_report(f, os_structure_report);
//                    no_structure_report = false;
//                }
//                generate_simulation_turn_report(f, os_simulation_turn_report, t_offset);
//            }
//        });

        IntervalReportNotifier interval_notifier(2);
        simulate(factory, 5, [&interval_notifier, &os_simulation_turn_report, &os_structure_report, &no_structure_report](Factory& f, TimeOffset t_offset) {
            if (interval_notifier.should_generate_report(t_offset)) {
                if(no_structure_report){
                    generate_structure_report(f, os_structure_report);
                    no_structure_report = false;
                }
                generate_simulation_turn_report(f, os_simulation_turn_report, t_offset);
            }
        });

        fb_structure_report_output.close();
        fb_simulation_turn_report_output.close();


        std::ostream os(&fb_output);
        save_factory_structure(factory, os);
        fb_output.close();

    }else{
        std::cout << "error" << std::endl;
    }

////

//    std::string line = "\n";
//    std::cout << *line.begin() << std::endl;
//    if(line.empty()){
//        std::cout << "PUSTA" << std::endl;
//    }
//    ParsedLineData p_l_d = parse_line(line);
//    if(p_l_d.element_type == ElementType::LINK){
//        std::cout << "TYP:  LINK" << std::endl;
//    }
//    for (auto [key, value] : p_l_d.parameters){
//        std::cout << key << "=" << value << std::endl;
//    }
//
//    ReceiverPreferences rp;
//
//    MockReceiver r1;
//    rp.add_receiver(&r1);
//
//    for(auto& [recipient, probability] : rp.get_preferences()){
//        std::cout << "R = " << recipient << "  ;   p = " << probability << std::endl;
//    }
//
//    std::cout << std::endl;
//    std::cout << std::endl;
//
//    MockReceiver r2;
//    rp.add_receiver(&r2);
//
//    for(auto& [recipient, probability] : rp.get_preferences()){
//        std::cout << "R = " << recipient << "  ;   p = " << probability << std::endl;
//    }
//
//    std::cout << std::endl;
//    std::cout << std::endl;
//
//    rp.remove_receiver(&r2);
//
//    for(auto& [recipient, probability] : rp.get_preferences()){
//        std::cout << "R = " << recipient << "  ;   p = " << probability << std::endl;
//    }
//
//    MockReceiver r3;
//    if(rp.get_preferences().end() == rp.get_preferences().end()){
//        std::cout << "TRUE" << std::endl;
//    }else{
//        std::cout << "FALSE" << std::endl;
//    }
//
//
//    std::cout << std::endl;
//    std::cout << std::endl;


////    TEST DODAWANIA - PUSTY
////------------------------------------------------------------------------------------
//    std::map<std::string, double> preferences_t = {};
//
//    std::string r_1 = "R_1";
//
//    if (preferences_t.empty()){
//        preferences_t.insert({r_1, 1});
//    }
//
//    for (auto& [recipient, probability] : preferences_t) {
//        std::cout << recipient << " ,  p = " << probability << std::endl;
//    }
////------------------------------------------------------------------------------------

////    TEST DODAWANIA - NIEPUSTY
////------------------------------------------------------------------------------------

//std::map<std::string, double> preferences_t = {};
//
//    double total_p = 0;
//
//    std::string r_1 = "R_1";
//    std::string r_2 = "R_2";
//
//    preferences_t.insert({r_1, 0.5});
//    preferences_t.insert({r_2, 0.5});
//
//    std::cout << "Przed dodaniem "<< std::endl;
//
//    for (auto& [recipient, probability] : preferences_t) {
//        std::cout << recipient << " ,  p = " << probability << std::endl;
//    }
//    std::cout << std::endl;

////--------------------------------------------------------

//    std::string r_3 = "R_3";
//
//    if (preferences_t.empty()){
//        preferences_t.insert({r_3, 1});
//    }
//    else{
//        double scaling_rate = 1 - probability_generator();
//        for (auto& [recipient, probability] : preferences_t) {
//            probability = probability * scaling_rate;
//            total_p += probability;
//        }
//        preferences_t.insert({r_3,1 - scaling_rate});
//        total_p += 1 - scaling_rate;
//    }
//
//    std::cout << "Po dodaniu "<< std::endl;
//
//    for (auto& [recipient, probability] : preferences_t) {
//        std::cout << recipient << " ,  p = " << probability << std::endl;
//    }
//
//    std::cout << "total_p = "<< total_p << std::endl;
//    std::cout << std::endl;

////--------------------------------------------------------


////--------------------------------------------------------

//    total_p = 0;
//    std::string r_4 = "R_4";
//
//    if (preferences_t.empty()){
//        preferences_t.insert({r_4, 1});
//    }
//    else{
//        double scaling_rate = 1 - probability_generator();
//        for (auto& [recipient, probability] : preferences_t) {
//            probability = probability * scaling_rate;
//            total_p += probability;
//        }
//        preferences_t.insert({r_4,1 - scaling_rate});
//        total_p += 1 - scaling_rate;
//    }
//
//    std::cout << "Po dodaniu "<< std::endl;
//
//    for (auto& [recipient, probability] : preferences_t) {
//        std::cout << recipient << " ,  p = " << probability << std::endl;
//    }
//
//    std::cout << "total_p = "<< total_p << std::endl;
//    std::cout << std::endl;

////--------------------------------------------------------


////--------------------------------------------------------

//    total_p = 0;
//    std::string r_5 = "R_5";
//
//    if (preferences_t.empty()){
//        preferences_t.insert({r_5, 1});
//    }
//    else{
//        double scaling_rate = 1 - probability_generator();
//        for (auto& [recipient, probability] : preferences_t) {
//            probability = probability * scaling_rate;
//            total_p += probability;
//        }
//        preferences_t.insert({r_5,1 - scaling_rate});
//        total_p += 1 - scaling_rate;
//    }
//
//    std::cout << "Po dodaniu "<< std::endl;
//
//    for (auto& [recipient, probability] : preferences_t) {
//        std::cout << recipient << " ,  p = " << probability << std::endl;
//    }
//
//    std::cout << "total_p = "<< total_p << std::endl;
//    std::cout << std::endl;

////--------------------------------------------------------

////------------------------------------------------------------------------------------


////    TEST DESTRUKTORA
////------------------------------------------------------------------------------------
//    Package p1;
//    std::cout << "id_1 = " << p1.get_id() << std::endl;
//    {
//        Package p2(7);
//        std::cout << "id_2 = " << p2.get_id() << std::endl;
//    }
//    if(p1.freed_IDs.find(7) == p1.freed_IDs.end()){
//        std::cout << "Nie ma" << std::endl;
//    }else{
//        std::cout << "Jest" << std::endl;
//    }
//    Package p3;
//    std::cout << "id_3 = " << p3.get_id() << std::endl;
//    if(p3.freed_IDs.find(7) == p3.freed_IDs.end()){
//        std::cout << "Nie ma" << std::endl;
//    }else{
//        std::cout << "Jest" << std::endl;
//    }
////------------------------------------------------------------------------------------

////      TEST MAPOWANIA
////-------------------------------------------------------------
//std::map<std::string, int> mapa;
//mapa.insert({"A",1});
//for (const auto& [name, height] : mapa) {
//    std::cout <<name << " | " << height<<std::endl;
//}
//mapa.erase(mapa.find({"A",1}));
//std::cout << mapa.size();
////-------------------------------------------------------------


    return EXIT_SUCCESS;
}
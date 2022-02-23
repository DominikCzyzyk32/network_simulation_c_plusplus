// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#ifndef NET_SIMULATION_FACTORY_HPP
#define NET_SIMULATION_FACTORY_HPP

/*
        szablon klasy NodeCollection
        klasa Factory
        funkcje do zapisu i odczytu struktury fabryki z pliku
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


template <class Node>
class NodeCollection{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() { return container_t_.begin();}
    iterator end() { return container_t_.end();}
    const_iterator begin() const { return container_t_.begin();}
    const_iterator end() const { return container_t_.end();}
    const_iterator cbegin() const { return container_t_.cbegin();}
    const_iterator cend() const { return container_t_.cend();}

    void add(Node&& node) {container_t_.push_back(std::move(node));}

    void remove_by_id(ElementID id) {
        auto it = find_by_id(id);
        if (it != container_t_.end()){
            container_t_.erase(it);
        }
    }

    iterator find_by_id(ElementID id) {
        auto it = std::find_if(container_t_.begin(), container_t_.end(),
                               [id](const auto& elem){ return id == elem.get_id();});
        return it;
    }

    const_iterator find_by_id(ElementID id) const{
        const auto it = std::find_if(container_t_.cbegin(), container_t_.cend(),
                               [id](const auto& elem){ return id == elem.get_id();});
        return it;
    }

private:
    std::list<Node> container_t_;

};


class Factory{

public:
    void add_ramp(Ramp&& Ramp) {Ramp_.add(std::move(Ramp));}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return Ramp_.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return Ramp_.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return Ramp_.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return Ramp_.cend();}
    void remove_ramp(ElementID id) {Ramp_.remove_by_id(id);}


    void add_worker(Worker&& Worker) {Worker_.add(std::move(Worker));}
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return Worker_.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return Worker_.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return Worker_.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() const { return Worker_.cend();}


    void add_storehouse(Storehouse&& Storehouse) {Storehouse_.add(std::move(Storehouse));}
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return Storehouse_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return Storehouse_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return Storehouse_.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return Storehouse_.cend();}


    bool is_consistent();
    void do_deliveries(Time t);
    void do_work(Time t);
    void do_package_passing();

    //// TA FUNKCJA POWINNA BYĆ PRYWATNA ALE NA POTRZEBY TESTÓW JĄ WYCIĄGŁEM
//    void remove_worker(NodeCollection<WORKER>& collection, ElementID id);
    void remove_worker(ElementID id);
    void remove_storehouse(ElementID id);

private:
    template <class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id){

        auto it = std::find_if(collection.begin(), collection.end(),
                               [id](const auto& elem){ return (elem.get_id() == id);});

        if(it != collection.end()){
            for (auto &ramp: Ramp_) {
                const ReceiverPreferences::preferences_t &preferences = ramp.receiver_preferences_.get_preferences();
                for (auto&[worker, probability]: preferences) {
                    if (worker->get_id() == id) {
                        ramp.receiver_preferences_.remove_receiver(worker);
                        break;
                    }
                }
            }
            collection.remove_by_id(id);
        }
    }
    NodeCollection<Ramp> Ramp_;
    NodeCollection<Worker> Worker_;
    NodeCollection<Storehouse> Storehouse_;

};


Factory load_factory_structure(std::istream& is);

void save_factory_structure(const Factory& factory, std::ostream& os);

#endif //NET_SIMULATION_FACTORY_HPP

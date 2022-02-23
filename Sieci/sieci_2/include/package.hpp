// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#ifndef NET_SIMULATION_PACKAGE_HPP
#define NET_SIMULATION_PACKAGE_HPP

#include "types.hpp"
#include <set>


class Package{
public:

    inline static std::set<ElementID> assigned_IDs;
    inline static std::set<ElementID> freed_IDs;
    inline static const ElementID blank_id = -1;
//    inline static const ElementID base_ID_for_worker = -2;

    Package();

    Package(ElementID id);

    Package(Package& p) = delete;

    Package(Package&& p);

    ~Package();

    Package& operator=(Package& other) = delete;

    Package& operator=(Package&& other);

    ElementID get_id() const {return id_;}

private:
    ElementID id_;
};

#endif //NET_SIMULATION_PACKAGE_HPP

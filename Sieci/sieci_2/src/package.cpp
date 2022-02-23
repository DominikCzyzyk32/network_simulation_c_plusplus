// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#include "package.hpp"

Package::Package(){
    if(freed_IDs.empty()){
        if(Package::assigned_IDs.empty()){
            id_ = 1;
        }else{
            id_ = *assigned_IDs.rbegin() + 1;
        }
    }else{
        id_ = *freed_IDs.begin();
        freed_IDs.erase(freed_IDs.begin());
    }
    assigned_IDs.insert(id_);
}

Package::Package(ElementID id){
    if (assigned_IDs.find(id) == assigned_IDs.end()){
        id_ = id;
        if (freed_IDs.find(id) != freed_IDs.end()){
            freed_IDs.erase(id);
        }

        assigned_IDs.insert(id);

//        if(id != base_ID_for_worker){
//            assigned_IDs.insert(id);
//        }
    }else{
        throw std::invalid_argument("Given ID is already used!");
    }
}

Package::Package(Package&& p){
    id_ = p.get_id();
    p.id_ = blank_id;
}

Package::~Package() {
    if(this->id_ != blank_id){          // && this->id_ != base_ID_for_worker
        freed_IDs.insert(id_);
        assigned_IDs.erase(id_);
    }
}


//Package& Package::operator=(Package&& p){
//    ElementID id = p.get_id();
//    p.~Package();
//    freed_IDs.erase(freed_IDs.find(id_));
//    Package p1(id);
//    return p1;
//}

Package& Package::operator=(Package&& other){
    if(this->id_ != blank_id){        // && this->id_ != base_ID_for_worker
        assigned_IDs.erase(this->id_);
        freed_IDs.insert(this->id_);
    }
    this->id_ = other.get_id();
    other.id_ = blank_id;
    return *this;
}
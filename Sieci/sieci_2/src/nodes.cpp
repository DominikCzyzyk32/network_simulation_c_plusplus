// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#include <iostream>
#include "nodes.hpp"

////    DO TESTÓW
////------------------------------------------------------------
double default_generate_canonical_implementation() { return 0; }
std::function<double()> generate_canonical = default_generate_canonical_implementation;
////------------------------------------------------------------


void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    if (preferences_t_.size() == 0){
        preferences_t_.insert({r, 1});
    }
    else{
        double scaling_rate = 1 - (1 / (double(preferences_t_.size()) + 1));           //// WERSJA W KTÓREJ 'p' DLA WSZYSTKICH ODBIORCOW JEST TAKIE SAMO
//        double scaling_rate = 1 - pg_();                                     //// WERSJA W KTÓREJ 'p' DLA DODAWANEGO ODBIORCY ZALEZY OD FUNKCJI LOSUJACEJ
        for (auto& [recipient, probability] : preferences_t_) {
            probability = probability * scaling_rate;
        }
        preferences_t_.insert({r,1 - scaling_rate});
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    double scaling_rate = 1 - preferences_t_.find(r)->second;
    preferences_t_.erase(preferences_t_.find(r));
    if (scaling_rate != 0){
        for (auto& [recipient, probability] : preferences_t_) {
            probability = probability / scaling_rate;
        }
    }
}


IPackageReceiver* ReceiverPreferences::choose_receiver() {

////    DO TESTÓW
////------------------------------------------------------------
//    double p = generate_canonical();
////------------------------------------------------------------

    double p = pg_();
    double distributor = 0;
    for (auto& [recipient, probability] : preferences_t_) {
        if(p > distributor && p <= distributor + probability){
            return recipient;
        }
        distributor += probability;
        }
    return preferences_t_.rbegin()->first;
}


//// CHYBA TAK TO MA BYĆ
void PackageSender::send_package() {
    if(opt_.has_value()){
        receiver_preferences_.choose_receiver()->receive_package(std::move(opt_.value())); //// TU PRZEKAZUJEMY PACZKĘ WYBRANEMU ODBIORCY
        opt_.reset();
    }
}


void Ramp::deliver_goods(Time t) {
    if((t- 1) % di_ == 0){           //// TO '-1' BO ZACZYNAMY OD CZASU 't = 1' I WTEDY MA BYĆ WYSYŁKA
        push_package(Package());
    }
}


void Worker::do_work(Time t) {

    if(how_long_work_lasts_ == 0 && !q_->empty()){
        t_ = t;
        processing_buffer_.emplace(q_->pop());
    }

    if(processing_buffer_.has_value()){
        how_long_work_lasts_ = t - t_ + 1;
        if(how_long_work_lasts_ == pd_){
            push_package(std::move(processing_buffer_.value()));
            processing_buffer_.reset();
            how_long_work_lasts_ = 0;
        }
    }
}


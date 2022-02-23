// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#include "reports.hpp"

std::vector<std::set<ElementID>> sorted_IDs(const Factory& f){

    std::vector<std::set<ElementID>> ramp_worker_storehouse;
    std::set<ElementID> Node_IDs = {};

    std::for_each(f.ramp_cbegin(), f.ramp_cend(), [&Node_IDs](const Ramp& ramp){ Node_IDs.insert(ramp.get_id()); });
    ramp_worker_storehouse.push_back(Node_IDs);
    Node_IDs.clear();

    std::for_each(f.worker_cbegin(), f.worker_cend(), [&Node_IDs](const Worker& worker){ Node_IDs.insert(worker.get_id()); });
    ramp_worker_storehouse.push_back(Node_IDs);
    Node_IDs.clear();

    std::for_each(f.storehouse_cbegin(), f.storehouse_cend(), [&Node_IDs](const Storehouse& storehouse){ Node_IDs.insert(storehouse.get_id()); });
    ramp_worker_storehouse.push_back(Node_IDs);

    return ramp_worker_storehouse;
}


void generate_ramp(const Ramp& ramp, std::ostream& os) {

    os << "\nLOADING RAMP #" << ramp.get_id() << "\n  Delivery interval: " << ramp.get_delivery_interval()
       << "\n  Receivers:";

    std::map<IPackageReceiver *, double> receivers = ramp.receiver_preferences_.get_preferences();
    std::vector<int> workers_with_sorted_id;
    std::vector<int> storehouses_with_sorted_id;
    for (auto &receiver: receivers) {
        if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
            workers_with_sorted_id.push_back(receiver.first->get_id());
        }
        else{
            storehouses_with_sorted_id.push_back(receiver.first->get_id());
        }
    }
    std::sort(workers_with_sorted_id.begin(), workers_with_sorted_id.end());
    std::sort(storehouses_with_sorted_id.begin(), storehouses_with_sorted_id.end());
    for (auto id: storehouses_with_sorted_id){
        os << "\n    storehouse #" << id;
    }
    for (auto id: workers_with_sorted_id){
        os << "\n    worker #" << id;
    }
    os << '\n';
}

void generate_worker(const Worker& worker, std::ostream& os) {

    os << "\nWORKER #" << worker.get_id() << "\n  Processing time: " << worker.get_processing_duration();

    std::string q_t;
    switch(worker.get_queue()->get_queue_type()){

        case PackageQueueType::FIFO:
            q_t = "FIFO";
            break;
        case PackageQueueType::LIFO:
            q_t = "LIFO";
            break;
    }
    os << "\n  Queue type: " << q_t << "\n  Receivers:";

    std::map<IPackageReceiver *, double> receivers = worker.receiver_preferences_.get_preferences();
    std::vector<int> workers_with_sorted_id;
    std::vector<int> storehouses_with_sorted_id;
    for (auto &receiver: receivers) {
        if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
            workers_with_sorted_id.push_back(receiver.first->get_id());
//            os << "\n    worker #" << receiver.first->get_id();
        }
        else{
//            os << "\n    storehouse #" << receiver.first->get_id();
            storehouses_with_sorted_id.push_back(receiver.first->get_id());
        }
    }
    std::sort(workers_with_sorted_id.begin(), workers_with_sorted_id.end());
    std::sort(storehouses_with_sorted_id.begin(), storehouses_with_sorted_id.end());
    for (auto id: storehouses_with_sorted_id){
        os << "\n    storehouse #" << id;
    }
    for (auto id: workers_with_sorted_id){
        os << "\n    worker #" << id;
    }
    os << '\n';
}

void generate_storehouse(const Storehouse& storehouse, std::ostream& os){
    os << "\nSTOREHOUSE #" << storehouse.get_id() << '\n';
}


void generate_structure_report(const Factory& factory, std::ostream& os){

    std::vector<std::set<ElementID>> ramp_worker_storehouse = sorted_IDs(factory);

    os << "\n== LOADING RAMPS ==\n";
    std::for_each(ramp_worker_storehouse[0].begin(), ramp_worker_storehouse[0].end(), [&os, &factory](ElementID ID){ generate_ramp(*factory.find_ramp_by_id(ID), os); });
//    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp){ generate_ramp(ramp, os);});

    os << "\n\n== WORKERS ==\n";
    std::for_each(ramp_worker_storehouse[1].begin(), ramp_worker_storehouse[1].end(), [&os, &factory](ElementID ID){ generate_worker(*factory.find_worker_by_id(ID), os); });
//    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const Worker& worker){ generate_worker(worker, os);});

    os << "\n\n== STOREHOUSES ==\n";
    std::for_each(ramp_worker_storehouse[2].begin(), ramp_worker_storehouse[2].end(), [&os, &factory](ElementID ID){ generate_storehouse(*factory.find_storehouse_by_id(ID), os); });
//    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&os](const Storehouse& storehouse){ generate_storehouse(storehouse, os);});

    os << "\n";
    os.flush();
}


void turn_report_worker(const Worker& worker, std::ostream& os, Time t){

    os << "\nWORKER #" << worker.get_id();
    if (worker.get_processing_buffer().has_value()){
        os << "\n  PBuffer: #" << worker.get_processing_buffer()->get_id() << " (pt = " << (t - worker.get_package_processing_start_time() + 1) << ")";
    } else { os << "\n  PBuffer: (empty)";}

    std::size_t counter_for_comma = 0;
    os << "\n  Queue: ";
    if (worker.get_queue()->size() == 0){
        os << "(empty)";}
    for(auto p = worker.get_queue()->cbegin(); p != worker.get_queue()->cend(); p++){
        if (worker.get_queue()->size() - counter_for_comma == 1){
            os << "#" << p->get_id();
        }else {os << "#" << p->get_id() << ", ";}
    }

    if (worker.get_sending_buffer().has_value()){
        os << "\n  SBuffer: #" << worker.get_sending_buffer()->get_id();
    } else { os << "\n  SBuffer: (empty)";}
    os << '\n';
}

void turn_report_storehouse(const Storehouse& storehouse, std::ostream& os){
    os << "\nSTOREHOUSE #" << storehouse.get_id();

    std::size_t counter_for_comma = 0;
    os << "\n  Stock: ";
    if (storehouse.size() == 0){
        os << "(empty)";}
    for(auto p = storehouse.cbegin(); p != storehouse.cend(); p++){
        if (storehouse.size() - counter_for_comma == 1){
            os << "#" << p->get_id();
        }else {os << "#" << p->get_id() << ", ";}
    }
    os << '\n';
}


void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time t){

    os << "=== [ Turn: " << t << " ] ===";
    os << "\n\n== WORKERS ==\n";
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os, t](const Worker& worker){ turn_report_worker(worker, os, t); });
    os << "\n\n== STOREHOUSES ==\n";
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&os](const Storehouse& storehouse){ turn_report_storehouse(storehouse, os); });
    os << '\n';
    os.flush();
}

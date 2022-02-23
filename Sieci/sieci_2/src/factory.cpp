// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#include "factory.hpp"

enum class NodeColor { UNVISITED, VISITED, VERIFIED };


bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors){
    if (node_colors.find(sender)->second == NodeColor::VERIFIED){
        return true;
    }
    node_colors.find(sender)->second = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().size() == 0){
        throw std::logic_error("The sender does not have any outbound connection.");
    }

    bool sender_has_receiver_different_than_him = false;

    for (auto& [receiver, probability]: sender->receiver_preferences_.get_preferences()){
        if (receiver->get_receiver_type() == ReceiverType::STOREHOUSE){
            sender_has_receiver_different_than_him = true;
        }
        else if (receiver->get_receiver_type() == ReceiverType::WORKER){
            IPackageReceiver* receiver_ptr = receiver;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender){
                continue;
            }
            sender_has_receiver_different_than_him = true;

            if (node_colors.find(sendrecv_ptr)->second == NodeColor::UNVISITED){
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }
    }
    node_colors.find(sender)->second = NodeColor::VERIFIED;

    if (sender_has_receiver_different_than_him){
        return true;
    }
    else{
        throw std::logic_error("STOREHOUSE is not achievable!");
    }
}


bool Factory::is_consistent() {
    std::map<const PackageSender *, NodeColor> node_colors;
    for (const auto& elem: Ramp_){
        const auto sender_ptr = dynamic_cast<const PackageSender*>(&elem);
        node_colors.insert({sender_ptr, NodeColor::UNVISITED});
    }
    for (const auto& elem: Worker_){
        const auto sender_ptr = dynamic_cast<const PackageSender*>(&elem);
        node_colors.insert({sender_ptr, NodeColor::UNVISITED});
    }

    for (const auto& elem: Ramp_){
        const auto sender_ptr = dynamic_cast<const PackageSender*>(&elem);
        try {
            has_reachable_storehouse(sender_ptr, node_colors);
        }
        catch (std::logic_error&){
            return false;
        }
    }
    return true;
}


void Factory::do_deliveries(Time t) {
    for (auto& elem: Ramp_){
        elem.deliver_goods(t);
    }
}


void Factory::do_package_passing() {
    for (auto& elem: Ramp_){
        elem.send_package();
    }
    for (auto& elem: Worker_){
        elem.send_package();
    }
}


void Factory::do_work(Time t) {
    for (auto& elem: Worker_){
        elem.do_work(t);
    }
}


void Factory::remove_worker(ElementID id) {         //// TERAZ TO JEST CHYBA FUNKCJA DELEGUJACA ;)
    remove_receiver(Worker_, id);               //// Chyba to załatwia sprawę


//        for (auto& ramp: Ramp_){
//        ReceiverPreferences::preferences_t& preferences = ramp.receiver_preferences_.get_preferences();
//        for (auto& [worker, probability]: preferences){
//            if (worker->get_id() == id){
//                ramp.receiver_preferences_.remove_receiver(worker);
//                break; // bez tego returnuje śmieci
//            }
//        }
//    }
//    Worker_.remove_by_id(id); // to tez powoduje returnowanie śmieci
}           /// ------DONE------


void Factory::remove_storehouse(ElementID id) {     //// TERAZ TO JEST CHYBA FUNKCJA DELEGUJACA ;)
    remove_receiver(Storehouse_, id);    //// Chyba to załatwia sprawę


//    for (auto& worker: Worker_){
//        ReceiverPreferences::preferences_t& preferences = worker.receiver_preferences_.get_preferences();
//        for (auto& [storehouse, probability]: preferences){
//            if (storehouse->get_id() == id){
//                worker.receiver_preferences_.remove_receiver(storehouse);
//                break; // bez tego returnuje śmieci
//            }
//        }
//    }
//    Storehouse_.remove_by_id(id); // to tez powoduje returnowanie śmieci
}       /// ------DONE------

////    LOAD
////-------------------------------------
enum class ElementType{
    RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLineData{
    ParsedLineData(ElementType e_t, std::map<std::string, std::string> p): element_type(e_t), parameters(std::move(p)) {}
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

ParsedLineData parse_line(std::string line){
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

////  TU SPRAWDZAMY JAKI JEST TYP ELEMENTU
////--------------------------------------------------------------
    std::string type_as_string = tokens[0];
    tokens.erase(tokens.begin());

// JEŚLI TYP NIE JEST JEDNYM Z PREDEFINIOWANYCH ZNACZNIKÓW TO RZUCAMY WYJĄTEK
//---------------------------------
    std::vector<std::string> correct_type_names = {"LOADING_RAMP", "WORKER", "STOREHOUSE", "LINK"};
    auto it = std::find(correct_type_names.begin(), correct_type_names.end(), type_as_string);
    if(it == correct_type_names.end()){
        throw std::logic_error("Wrong type name");
    }
//---------------------------------

    ElementType elem_type = ElementType::RAMP;
    if(type_as_string == "WORKER"){
        elem_type = ElementType::WORKER;
    }
    if(type_as_string == "STOREHOUSE"){
        elem_type = ElementType::STOREHOUSE;
    }
    if(type_as_string == "LINK"){
        elem_type = ElementType::LINK;
    }
////--------------------------------------------------------------

////  TU TWORZYMY MAPĘ KLUCZ-WARTOŚĆ
////--------------------------------------------------------------
    std::map<std::string, std::string> parameters = {};
    delimiter = '=';
    std::string key_or_value;
    std::vector<std::string> divided_key_and_value;
    for(std::string key_value: tokens){
        std::istringstream key_value_stream(key_value);
        while (std::getline(key_value_stream, key_or_value, delimiter)) {
            divided_key_and_value.push_back(key_or_value);
        }
        parameters[divided_key_and_value[0]] = divided_key_and_value[1];
        divided_key_and_value.clear();
    }
////--------------------------------------------------------------

    ParsedLineData p = ParsedLineData(elem_type, parameters);
    return p;
}

void check_correctness_of_the_line(ParsedLineData& parsed_line){

////    NOWA WERSJA
////-----------------------------------------------------------------------------------
    switch(parsed_line.element_type){

        case ElementType::RAMP: {
            if (parsed_line.parameters.size() != 2){
                throw std::logic_error("Incorrect number of features for ramp");
            }
            auto it1 = parsed_line.parameters.find("id");
            auto it2 = parsed_line.parameters.find("delivery-interval");
            if (it1 == parsed_line.parameters.end() || it2 == parsed_line.parameters.end()){
                throw std::logic_error("Wrong data for ramp");
            }
            break;
        }

        case ElementType::WORKER: {
            if (parsed_line.parameters.size() != 3){
                throw std::logic_error("Incorrect number of features for worker");
            }
            auto it1 = parsed_line.parameters.find("id");
            auto it2 = parsed_line.parameters.find("processing-time");
            auto it3 = parsed_line.parameters.find("queue-type");

            if (it1 == parsed_line.parameters.end() || it2 == parsed_line.parameters.end() || it3 == parsed_line.parameters.end()){
                throw std::logic_error("Wrong data for worker");
            }
            break;
        }

        case ElementType::STOREHOUSE: {
            if (parsed_line.parameters.size() != 1){
                throw std::logic_error("Incorrect number of features for storehouse");
            }
            auto it1 = parsed_line.parameters.find("id");
            if (it1 == parsed_line.parameters.end()){
                throw std::logic_error("Wrong data for storehouse");
            }
            break;
        }

        case ElementType::LINK: {
            if (parsed_line.parameters.size() != 2){
                throw std::logic_error("Incorrect number of features for link");
            }
            auto it1 = parsed_line.parameters.find("src");
            auto it2 = parsed_line.parameters.find("dest");

            if (it1 == parsed_line.parameters.end() || it2 == parsed_line.parameters.end()){
                throw std::logic_error("Wrong data for link");
            }
            break;
        }

        default: {
            throw std::logic_error("Wrong ElementType");
        }

    }
////-----------------------------------------------------------------------------------


////    STARA WERSJA
////-----------------------------------------------------------------------------------
//    if (parsed_line.element_type == ElementType::RAMP){
//        if (parsed_line.parameters.size() != 2){
//            throw std::logic_error("Incorrect number of features for ramp");
//        }
//        auto it1 = parsed_line.parameters.find("id");
//        auto it2 = parsed_line.parameters.find("delivery-interval");
//        if (it1 == parsed_line.parameters.end() || it2 == parsed_line.parameters.end()){
//            throw std::logic_error("Wrong data for ramp");
//        }
//    }
//
//    if (parsed_line.element_type == ElementType::WORKER){
//        if (parsed_line.parameters.size() != 3){
//            throw std::logic_error("Incorrect number of features for worker");
//        }
//        auto it1 = parsed_line.parameters.find("id");
//        auto it2 = parsed_line.parameters.find("processing-time");
//        auto it3 = parsed_line.parameters.find("queue-type");
//
//        if (it1 == parsed_line.parameters.end() || it2 == parsed_line.parameters.end() || it3 == parsed_line.parameters.end()){
//            throw std::logic_error("Wrong data for worker");
//        }
//    }
//
//    if (parsed_line.element_type == ElementType::STOREHOUSE){
//        if (parsed_line.parameters.size() != 1){
//            throw std::logic_error("Incorrect number of features for storehouse");
//        }
//        auto it1 = parsed_line.parameters.find("id");
//        if (it1 == parsed_line.parameters.end()){
//            throw std::logic_error("Wrong data for storehouse");
//        }
//    }
//
//    if (parsed_line.element_type == ElementType::LINK){
//        if (parsed_line.parameters.size() != 2){
//            throw std::logic_error("Incorrect number of features for link");
//        }
//        auto it1 = parsed_line.parameters.find("src");
//        auto it2 = parsed_line.parameters.find("dest");
//
//        if (it1 == parsed_line.parameters.end() || it2 == parsed_line.parameters.end()){
//            throw std::logic_error("Wrong data for link");
//        }
//    }
////-----------------------------------------------------------------------------------
}


Factory load_factory_structure(std::istream& is){
    Factory factory;

    std::string line;
    while(std::getline(is, line)){
        if(*line.begin() == ';' || line.empty()){
            continue;
        }else{
            ParsedLineData parsed_line = parse_line(line);
            check_correctness_of_the_line(parsed_line);

////            zainicjalizowanie obiektu właściwego typu węzła i dodanie go do obiektu fabryki,
///             albo
///             utworzenie połączenia między zadanymi węzłami sieci
            switch(parsed_line.element_type){

                case ElementType::RAMP:
                    factory.add_ramp(Ramp(
                            std::stoi(parsed_line.parameters["id"]),
                            std::stoi(parsed_line.parameters["delivery-interval"])));
                    break;

                case ElementType::WORKER: {
                    PackageQueueType queue_type = PackageQueueType::LIFO;
                    if(parsed_line.parameters["queue-type"] == "FIFO"){
                        queue_type = PackageQueueType::FIFO;
                    }
                    factory.add_worker(Worker(
                            std::stoi(parsed_line.parameters["id"]),
                            std::stoi(parsed_line.parameters["processing-time"]),
                            std::make_unique<PackageQueue>(queue_type)));
                    break;
                }

                case ElementType::STOREHOUSE:
                    factory.add_storehouse(Storehouse(std::stoi(parsed_line.parameters["id"])));
                    break;

                case ElementType::LINK: {

                    char delimiter = '-';

//// SOURCE
////-------------------------------------------------------------
                    std::vector<std::string> src_and_id;
                    std::string src_or_id;

                    std::istringstream src_and_id_stream(parsed_line.parameters["src"]);

                    while (std::getline(src_and_id_stream, src_or_id, delimiter)) {
                        src_and_id.push_back(src_or_id);
                    }

                    ElementID src_id = std::stoi(src_and_id[1]);
////-------------------------------------------------------------


//// DESTINATION
////-------------------------------------------------------------
                    std::vector<std::string> dest_and_id;
                    std::string dest_or_id;

                    std::istringstream dest_and_id_stream(parsed_line.parameters["dest"]);

                    while (std::getline(dest_and_id_stream, dest_or_id, delimiter)) {
                        dest_and_id.push_back(dest_or_id);
                    }

                    ElementID dest_id = std::stoi(dest_and_id[1]);
////-------------------------------------------------------------

                    if(src_and_id[0] == "ramp"){
                        if(dest_and_id[0] == "worker"){
                            factory.find_ramp_by_id(src_id)->receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(dest_id)));
                        }
                        if(dest_and_id[0] == "store"){
                            factory.find_ramp_by_id(src_id)->receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(dest_id)));
                        }
                    }
                    if(src_and_id[0] == "worker"){
                        if(dest_and_id[0] == "worker"){
                            factory.find_worker_by_id(src_id)->receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(dest_id)));
                        }
                        if(dest_and_id[0] == "store"){
                            factory.find_worker_by_id(src_id)->receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(dest_id)));
                        }
                    }
                    break;
                }
            }
        }
    }
    return factory;
}
////-------------------------------------


////    SAVE
////-------------------------------------
void save_Ramp(const Ramp& ramp, std::ostream& os){
    os << "LOADING_RAMP id=" << ramp.get_id() << " delivery-interval=" << ramp.get_delivery_interval() << '\n';
}

void save_Worker(const Worker& worker, std::ostream& os){
    std::string q_t = "";
    switch(worker.get_queue()->get_queue_type()){

        case PackageQueueType::FIFO:
            q_t = "FIFO";
            break;
        case PackageQueueType::LIFO:
            q_t = "LIFO";
            break;
    }
    os << "WORKER id=" << worker.get_id() << " processing-time=" << worker.get_processing_duration() << " queue-type=" << q_t << '\n';
}

void save_Storehouse(const Storehouse& storehouse, std::ostream& os){
    os << "STOREHOUSE id=" << storehouse.get_id() << '\n';
}

void save_Link(const PackageSender& source, std::ostream& os, std::string src_name_with_ID){

    for (auto& [destination, probability] : source.receiver_preferences_.get_preferences()){
        std::string dest = "";
        switch(destination->get_receiver_type()) {

            case ReceiverType::WORKER:
                dest = "worker-";
                break;
            case ReceiverType::STOREHOUSE:
                dest = "store-";
                break;
        }
        os << "LINK src=" << src_name_with_ID << " dest=" << dest << destination->get_id() << '\n';
    }
}


void save_factory_structure(const Factory& factory, std::ostream& os){

    os << "; == LOADING RAMPS ==\n\n";
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp){ save_Ramp(ramp, os); });

    os << "\n; == WORKERS ==\n\n";
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const Worker& worker){ save_Worker(worker, os); });

    os << "\n; == STOREHOUSES ==\n\n";
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&os](const Storehouse& storehouse){ save_Storehouse(storehouse, os); });

    os << "\n; == LINKS ==\n\n";
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const Ramp& ramp){
        std::string src_name_with_id = "ramp-" + std::to_string(ramp.get_id());
        save_Link(ramp, os, src_name_with_id);
        os << '\n';
    });

    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const Worker& worker){
        std::string src_name_with_id = "worker-" + std::to_string(worker.get_id());
        save_Link(worker, os, src_name_with_id);
        os << '\n';
    });

    os.flush();

}
////-------------------------------------

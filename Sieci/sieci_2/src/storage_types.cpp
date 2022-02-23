// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#include "storage_types.hpp"

Package PackageQueue::pop() {
    Package p;
    switch(q_type_){
        case PackageQueueType::FIFO:
            p = std::move(package_queue_.front());
            package_queue_.pop_front();
            break;
        case PackageQueueType::LIFO:
            p = std::move(package_queue_.back());
            package_queue_.pop_back();
            break;
    }
    return p;
}
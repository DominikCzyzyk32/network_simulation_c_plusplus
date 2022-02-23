// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#ifndef NET_SIMULATION_STORAGE_TYPES_HPP
#define NET_SIMULATION_STORAGE_TYPES_HPP

/*
        klasy IPackageStockpile, IPackageQueue, PackageQueue
        typ wyliczeniowy PackageQueueType
*/

#include <stdlib.h>
#include "package.hpp"
#include <list>


enum class PackageQueueType{
    FIFO, LIFO
};

class const_iterator;

class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& p) = 0;

    virtual bool empty() const = 0;

    virtual std::size_t size() const = 0;

    virtual const_iterator cbegin() const = 0;

    virtual const_iterator cend() const = 0;

    virtual const_iterator begin() = 0;

    virtual const_iterator end() = 0;

    virtual ~IPackageStockpile() = default;
};


class IPackageQueue : public IPackageStockpile{
public:
    virtual Package pop() = 0;

    virtual PackageQueueType  get_queue_type() const = 0;
};


class PackageQueue : public IPackageQueue{
public:

    PackageQueue(PackageQueueType pqt): q_type_(pqt) {};

    PackageQueueType get_queue_type() const override {return q_type_;}

    Package pop() override;

    IPackageStockpile::const_iterator cbegin() const override {return package_queue_.cbegin();}

    IPackageStockpile::const_iterator cend() const override {return package_queue_.cend();}

    IPackageStockpile::const_iterator begin() override {return package_queue_.begin();};

    IPackageStockpile::const_iterator end() override {return package_queue_.end();};

    bool empty() const override {return package_queue_.empty();}

    std::size_t size() const override {return package_queue_.size();}

    void push(Package&& p) override {package_queue_.emplace_back(std::move(p));}


private:
    PackageQueueType q_type_;
    std::list<Package> package_queue_;
};

#endif //NET_SIMULATION_STORAGE_TYPES_HPP

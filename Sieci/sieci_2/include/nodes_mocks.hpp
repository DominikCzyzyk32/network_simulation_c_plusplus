// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#ifndef NET_SIMULATION_NODES_MOCKS_HPP
#define NET_SIMULATION_NODES_MOCKS_HPP
#include "nodes.hpp"

class MockReceiver: public IPackageReceiver{
public:
    MockReceiver(ElementID id = 1, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)): id_(id), d_(std::move(d)) {}

    void receive_package(Package&& p) override {d_->push(std::move(p));}

    ElementID get_id() const override {return id_;}

    ReceiverType get_receiver_type() const override { return ReceiverType::WORKER;}

    std::size_t size() const {return d_->size();}

    bool empty() const {return d_->empty();}

    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin();}
    IPackageStockpile::const_iterator cend() const override {return d_->cend();}
    IPackageStockpile::const_iterator begin() const override {return d_->begin();}
    IPackageStockpile::const_iterator end() const override {return d_->end();}

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};

#endif //NET_SIMULATION_NODES_MOCKS_HPP

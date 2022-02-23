// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

#ifndef NET_SIMULATION_NODES_HPP
#define NET_SIMULATION_NODES_HPP

#include <functional>
#include "helpers.hpp"
#include "storage_types.hpp"
#include <map>
#include <memory>
#include <utility>

////    DO TESTÓW
////------------------------------------------------------------
double default_generate_canonical_implementation();

extern std::function<double()> generate_canonical;
////------------------------------------------------------------


/*  klasy ujęte na diagramie UML w pakiecie Nodes
        typ wyliczeniowy ReceiverType
        klasy IPackageReceiver, Storehouse, ReceiverPreferences, PackageSender, Ramp, Worker
*/

enum class ReceiverType{
    WORKER, STOREHOUSE
};


class IPackageReceiver{
public:
    virtual void receive_package(Package&& p) = 0;

    virtual ElementID get_id() const = 0;

    virtual ReceiverType get_receiver_type() const = 0;

    virtual IPackageStockpile::const_iterator cbegin() const = 0;

    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual IPackageStockpile::const_iterator begin() const = 0;

    virtual IPackageStockpile::const_iterator end() const = 0;
};


class Storehouse: public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)): id_(id), d_(std::move(d)) {}

    void receive_package(Package&& p) override {d_->push(std::move(p));}

    ElementID get_id() const override {return id_;}

    ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE;}

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


class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator): pg_(pg) {}

    const_iterator cbegin() const {return preferences_t_.cbegin();}
    const_iterator cend() const {return preferences_t_.cend();}
    const_iterator begin() const  {return preferences_t_.begin();}
    const_iterator end() const  {return preferences_t_.end();}

    const preferences_t& get_preferences() const {return preferences_t_;}

    void add_receiver(IPackageReceiver* r);

    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

protected:
    preferences_t preferences_t_;
    ProbabilityGenerator pg_;
};


class PackageSender{
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender() = default;

    PackageSender(PackageSender&&) = default;

    const std::optional<Package>& get_sending_buffer() const {return opt_;}

    void send_package();

private:
    std::optional<Package> opt_;

protected:
    void push_package(Package&& p) {opt_.emplace(std::move(p));}
};


class Ramp: public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di): id_(id), di_(di) {}

    void deliver_goods(Time t);

    TimeOffset get_delivery_interval() const { return di_; }

    ElementID get_id() const { return id_; }

private:
    ElementID id_;
    TimeOffset di_;
};


class Worker: public PackageSender, public IPackageReceiver{
public:

    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q): id_(id), pd_(pd), q_(std::move(q)) {}

    void do_work(Time t);

    void receive_package(Package&& p) override {q_->push(std::move(p));}

    TimeOffset get_processing_duration() const { return pd_;}

    ElementID get_id() const override { return id_;}

    ReceiverType get_receiver_type() const override { return ReceiverType::WORKER;}

    Time get_package_processing_start_time() const { return t_;}

    const IPackageQueue* get_queue() const { return &(*q_);}

    const std::optional<Package>& get_processing_buffer() const { return processing_buffer_;}

    IPackageStockpile::const_iterator cbegin() const override {return q_->cbegin();}
    IPackageStockpile::const_iterator cend() const override {return q_->cend();}
    IPackageStockpile::const_iterator begin() const override {return q_->begin();}
    IPackageStockpile::const_iterator end() const override {return q_->end();}

private:

    std::optional<Package> processing_buffer_;
    TimeOffset how_long_work_lasts_ = 0;
    ElementID id_;
    TimeOffset pd_;
    Time t_;
    std::unique_ptr<IPackageQueue> q_;
};

#endif //NET_SIMULATION_NODES_HPP

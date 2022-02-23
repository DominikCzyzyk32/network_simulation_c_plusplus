// 9: Czyżyk (401858), Więcek (400421), Kawiak (400241)

# include "simulation.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf){

    if(f.is_consistent()){
        for(Time turn = 1; turn <= d; turn++){

//// 1. DOSTAWA
            f.do_deliveries(turn);

//// 2. PRZEKAZANIE
            f.do_package_passing();

//// 3. PRZETWORZENIE
            f.do_work(turn);

//// 4. RAPORTOWANIE o stanie symulacji (jeśli dotyczy danej tury)
            rf(f, turn);
        }
    }else{
        throw std::logic_error("Factory is not consistent");
    }
}

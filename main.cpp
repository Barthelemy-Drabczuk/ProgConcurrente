#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/barrier.hpp>

#include <unistd.h>             // sleep()

#include "SleepChronogram.h"

using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace nsUtil;


barrier* p_ma_barrier;
void RendezVous () {
    //cout << "waiting ..." << endl;
    p_ma_barrier->wait();
    //cout << "finish." << endl;
}

mutex mut;

class CPassant {

   //id
    static unsigned id;
    unsigned myId;

    // times
    unsigned time_to_arrive;
    unsigned time_to_go;

public:

    CPassant (unsigned timeArr, unsigned timeDep) :  myId (++id) , time_to_arrive (timeArr), time_to_go (timeDep) {}

    void operator () (void) {
        unsigned Col = 1;
        nsUtil::SleepChronogram (this->myId, Col, '-', this->time_to_arrive, mut);
        ::RendezVous ();
        nsUtil::SleepChronogram (this->myId, Col, '_', this->time_to_go, mut);
        nsUtil::SleepChronogram (this->myId, Col, 'X', 1, mut);
    }

}; // CPassant

unsigned CPassant::id = 0;

int main () {

    thread_group mesPassants;

    unsigned nb_threads, temps_avant_rdv, temps_apres_rdv (0);

    //cout << "entrer un nombre de threads a lancer : ";
    cin >> nb_threads;

    p_ma_barrier = new barrier (nb_threads);

    cout << clrscr;
    for (unsigned i (0); i <  nb_threads; ++i) {
        //cout << "entrer le delais d'arrivee : ";
        cin >> temps_avant_rdv >> temps_apres_rdv;
        //cout << "entrer le delais de depart : ";

        mesPassants.add_thread (new thread (CPassant (temps_avant_rdv, temps_apres_rdv)));
    }

    mesPassants.join_all();

    return 0;
}

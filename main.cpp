#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>

using namespace std;

/**exo1**/
void Fct () {
    cout << "Hello World!" << endl;
    cout << "bonjour" << flush;
    cout << " tout" << flush << " le monde" << endl;
}

void exo1 () {
    cout << "Ma machine a : " << thread::hardware_concurrency() << " coeurs" << endl;
}

/**exo2**/
void exo2 () {
    thread corobizar (Fct);
    corobizar.join();
    thread sardoche (Fct);
    sardoche.join();
}

/**exo3**/
void exo3 () {
    thread corobizar (Fct);
    cout << "id = " << corobizar.get_id() << endl;
    corobizar.join();
}

/**exo4**/
void exo4 () {
    thread corobizar (Fct);
    cout << "id = " << corobizar.get_id() << endl;
    corobizar.join();
    thread sardoche (Fct);
    cout << "id = " << sardoche.get_id() << endl;
    sardoche.join();
}

/**exo5**/
void exo5 () {
    thread corobizar (Fct);
    cout << "id = " << corobizar.get_id() << endl;
    corobizar.join();
    thread sardoche (Fct);
    cout << "id = " << sardoche.get_id() << endl;
    sardoche.join();
}

/**exo6**/
mutex mutIO;
void fct2 (unsigned & i) {
    cout << "id : " << this_thread::get_id() << endl;
    mutIO.lock ();
    cout << "thread numero " << ++i << endl;
    mutIO.unlock ();
}

mutex insec;
void exo6 (unsigned & i) {
    thread corobizar (fct2, ref (i));
    thread sardoche (fct2, ref (i));
    insec.lock();
    cout << "corobizar : " << corobizar.get_id() << endl;
    insec.unlock();
    corobizar.join();
    insec.lock();
    cout << "sardoche : " << sardoche.get_id() << endl;
    insec.unlock();
    sardoche.join();
}

/**exo7**/
void exo7 () {
    vector<thread> mesFreds;
    unsigned i = 0;
    mesFreds.push_back(thread (fct2, ref(i)));
    mesFreds.push_back(thread (fct2, ref(i)));
    mesFreds.push_back(thread (Fct));

    for (thread & monFred : mesFreds) {
        monFred.join ();
    }
}

/**exo8**/
mutex tex;
unsigned i (0);
void inc () {
    tex.lock();
    ++i;
    cout << i << endl;
    tex.unlock();
}

void exo8 () {
    vector <thread> mesFreds;

    for (unsigned i (0); i < 10; ++i)
        mesFreds.push_back(thread (inc));

    for (thread & monFred : mesFreds) {
        monFred.join();
    }

}

/**exo9**/
vector<unsigned> createVector (unsigned size, vector<unsigned> & myVec) {
    srand (time (NULL));
    for (unsigned (0); i < size; ++i) {
        myVec.push_back((unsigned)rand()%500);
    }
    return myVec;
}

void printVector (vector<unsigned> myVec) {
    for (unsigned i : myVec)
        cout << i << endl;
}

void exo9 () {
    vector<unsigned> myVec;

    thread sardoche (createVector, 10, ref(myVec));
    sardoche.join();
    thread corobizar (printVector, myVec);
    corobizar.join();
}

/**exo10**/
enum Etat {PENSER, MANGER, A_FAIM};
mutex mut;
vector<shared_ptr<mutex>> dodo;
vector<Etat> etat_des_philo;

void test_manger (unsigned i){
    if (etat_des_philo [(i+1) % etat_des_philo.size()] != MANGER &&
            etat_des_philo [(i-1) % etat_des_philo.size()] != MANGER &&
            etat_des_philo [i] == A_FAIM) {
        etat_des_philo [i] = MANGER;
        cout << "philosophe : " << i << endl;
        cout << "Je mange." << endl;
        dodo [i]->unlock();
    }
}

void prendre (unsigned i) {
    lock_guard<mutex> lock (mut);
    cout << "philosophe : " << i << endl;
    etat_des_philo[i] = A_FAIM;
    cout << "J'ai faim." << endl;
    test_manger(i);
    dodo[i]->lock();
}

void poser(unsigned i) {
    lock_guard<mutex> lock (mut);
    etat_des_philo [i] = PENSER;
    cout << "philosophe : " << i << endl;
    cout << "Je pense." << endl;
    test_manger((i+1) % etat_des_philo.size());
    test_manger((i-1) % etat_des_philo.size());
}

void exo10 () {

    vector<thread> philo_qui_ont_faim;
    vector<thread> philo_qui_n_ont_plus_faim;

    /*****************************/
    /**** INITIALISE LES TABS ****/
    /****       ABRUTI        ****/
    /*****************************/

    #pragma openmp
    for (unsigned i (0); i < 10; ++i)
        etat_des_philo.push_back(PENSER);

    #pragma openmp
    for (int i (0); i < 10; ++i)
        dodo.push_back(shared_ptr<mutex> (new mutex()));

    #pragma openmp
    for (unsigned i (0); i < 10; ++i)
        philo_qui_ont_faim.push_back(thread (prendre, i));

    #pragma openmp
    for (unsigned i (0); i < 10; ++i)
        philo_qui_n_ont_plus_faim.push_back(thread (poser, i));

    #pragma openmp
    for (thread & philo : philo_qui_ont_faim)
        philo.join();

    #pragma openmp
    for (thread & philo : philo_qui_n_ont_plus_faim)
        philo.join();
}

int main() {

    exo10();

    return 0;
}

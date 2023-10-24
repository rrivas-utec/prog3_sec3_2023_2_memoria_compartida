#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <cstdio>
using namespace std;

mutex mtx_g;

class Client {
    atomic<double> saldo = 0;
    mutex mtx_c;
public:
    Client() = default;
    //Client(mutex& mtx): mtx_c(mtx) {}
    Client(double saldo):saldo(saldo) {}
    void deposito(double importe) {
        this_thread::sleep_for(chrono::milliseconds(10));
//        lock_guard lg(mtx_c);
        saldo += importe;
    }
    void retiro(double importe) {
        this_thread::sleep_for(chrono::milliseconds(10));
//        unique_lock ul(mtx_c);
        saldo -= importe;
    }
    friend ostream& operator<<(ostream& out, const Client& c) {
        out << c.saldo;
        return out;
    }
};

void ejercicio_3(int nd, int nr, double importe, Client& c) {
    // Vector de hilos
    vector<thread> vtd(nd);
    vector<thread> vtr(nr);
    // Asignacion de metodos y parametros
    for (auto& t: vtd) t = thread(&Client::deposito, &c, importe);
    for (auto& t: vtr) t = thread(&Client::retiro, &c, importe);
    // Espera de culminacion
    for (auto& t: vtd) t.join();
    for (auto& t: vtr) t.join();
}

void ejercicio_4(int m) {
    mutex mtx_l;
    vector <Client> vc(m);
    // Realizamos las operaciones de deposito y retiro de cada Cliente
    for (auto& c: vc)
        ejercicio_3(100, 50, 1, c);
    // Impresion del saldo de cada cliente
    for (auto& c: vc)
        cout << c << " ";
}

shared_mutex smtx;

void leer(int& value, int id) {
    shared_lock ul(smtx);
//    unique_lock ul(smtx);
    printf("value: %d es leido por: %d\n", value, id);
//    cout << "value: " << value << " es leido por: " << id << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void escribir(int& value, int inc, int id) {
    value += inc;
    unique_lock ul(smtx);
    printf("new value: %d es modificado por: %d\n", value, id);
    //cout << "new value: " << value << " fue modificado por: " << id << endl;
    this_thread::sleep_for(chrono::milliseconds(3000));
}

void ejercicio_5() {
    int x = 10;
    thread t1(leer, ref(x), 1);
    thread t2(leer, ref(x), 2);
    thread t3(leer, ref(x), 3);
    thread t4(leer, ref(x), 4);
    thread t5(escribir, ref(x), 5, 5);

    t1.join();
    t2.join();
    t5.join();
    t3.join();
    t4.join();
}

recursive_mutex rmtx;

void sumar(int n, int& resultado) {
    if (n == 0) return;
    unique_lock ul(rmtx);
    resultado += n;
//    ul.unlock();
    sumar(n-1, resultado);
}

void restar(int dec, int& resultado) {
    unique_lock ul(rmtx);
    resultado -= dec;
}

void ejercicio_6() {
    int resultado = 0;
    sumar(10, resultado);
    restar(10, resultado);
    cout << resultado << endl;
}

int main() {
//    Client c;
//    ejercicio_3(100, 50, 1, c);
//    cout << c;
//    ejercicio_4(40);
//    ejercicio_5();
    ejercicio_6();
    return 0;
}

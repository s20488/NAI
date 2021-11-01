#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <vector>

using namespace std;

random_device rd;
mt19937 mt_generator(rd());

ostream& operator<<(ostream& o, vector<double> v)
{
    for (auto e : v) {
        o << std::fixed << std::setprecision(5) << "\t" << e;
    }
    return o;
}

vector<double> hill_climbing(function<double(vector<double>)> f, function<bool(vector<double>)> f_domain, vector<double> p0, int iterations)
{
    auto p = p0;
    uniform_int_distribution<> distrib(0, p.size() - 1);
    uniform_real_distribution<> distrib_r(-0.1, 0.1);

    if (!f_domain(p)) throw std::invalid_argument("The p0 point must be in domain");
    for (int i = 0; i < iterations; i++) {
        auto p2 = p;

        p2[distrib(mt_generator)] += distrib_r(mt_generator);
        double y2 = f(p2);
        if (y2 < f(p)) {
            p = p2;
            //cout << p << "->" << f(p) << endl;
        }
    }
    return p;
}

vector<double> simulated_annealing(
        function<double(vector<double>)> f,
        function<bool(vector<double>)> f_domain,
        vector<double> p0,
        int iterations,
        function<vector<double>(vector<double>)> N,
        function<double(int)> T)
{
    auto s_current = p0;
    auto s_global_best = p0;

    uniform_real_distribution<> u_k(0.0, 1.0);

    if (!f_domain(s_current)) throw std::invalid_argument("The p0 point must be in domain");

    for (int k = 0; k < iterations; k++) {
        auto s_next = N(s_current);
        if (f(s_next) < f(s_current)) {
            s_current = s_next;
        } else {
            double u = u_k(mt_generator);
            if (u < exp(-abs(f(s_next) - f(s_current)) / T(k))) {
                s_current = s_next;
            } else {
            }
        }
        if (f(s_current) < f(s_global_best)) {
            s_global_best = s_current;
        }
        //cout << s_current << " -> " << f(s_current) << endl;
    }
    return s_global_best;
}

auto booth = [](vector<double> v) {
    double x = v.at(0), y = v.at(1);
    return (x + 2*y - 7)*(x + 2*y - 7) + (2*x + y - 5)*(2*x + y - 5);
};

auto booth_domain = [](vector<double> v) {
    return (abs(v[0]) <= 10) && (abs(v[1]) <= 10);
};

uniform_real_distribution<> booth_distrib_r(-10, 10);
vector<double> booth_p0 = {
        booth_distrib_r(mt_generator),
        booth_distrib_r(mt_generator),
};

auto himmelblaus = [](vector<double> v) {
    double x = v.at(0), y = v.at(1);
    return (x*x + y - 11)*(x*x + y - 11) + (x + y*y - 7)*(x + y*y - 7);
};

auto himmelblaus_domain = [](vector<double> v) {
    return (abs(v[0]) <= 5) && (abs(v[1]) <= 5);
};

uniform_real_distribution<> himmelblaus_distrib_r(-5, 5);
vector<double> himmelblaus_p0 = {
        himmelblaus_distrib_r(mt_generator),
        himmelblaus_distrib_r(mt_generator),
};

int main()
{
    int choose_function;
    int choose_algorithm;
    int iterations;

    cout << "Wybierz optymalizowana funkcje:\n1. Booth function;\n2. Himmelblau's function.\nOdpowiedz: ";
    cin >> choose_function;

    cout << "\nWybierz algorytm:\n1. Algorytm wspinaczkowy;\n2. Algorytm Symulowanego Wyzarzani.\nOdpowiedz: ";
    cin >> choose_algorithm;

    if (choose_function == 1) {
        if (choose_algorithm == 1) {
            cout << "\nWpisz ilosc iteracji: \nOdpowiedz: ";
            cin >> iterations;

            auto booth_result_1 = hill_climbing(booth, booth_domain, booth_p0, iterations);
            cout << booth_result_1 << " -> " << booth(booth_result_1) << endl;
        } else if (choose_algorithm == 2) {
            cout << "\nWpisz ilosc iteracji: \nOdpowiedz: ";
            cin >> iterations;

            auto booth_result_2 = simulated_annealing(
                    booth, booth_domain, booth_p0, iterations,
                    [](auto p) {
                        normal_distribution<double> n(0.0, 0.3);
                        for (auto& e : p) {
                            e = e + n(mt_generator);
                        }
                        return p;
                    },
                    [](int k) { return 1000.0 / k; });
            cout << booth_result_2<< " -> " << booth(booth_result_2) << endl;
        } else {
            cout << "Wybierz algorytm 1 lub 2!";
        }
    } else if (choose_function == 2) {
        if (choose_algorithm == 1) {
            cout << "\nWpisz ilosc iteracji: \nOdpowiedz: ";
            cin >> iterations;

            auto himmelblaus_result_1 = hill_climbing(himmelblaus, himmelblaus_domain, himmelblaus_p0, iterations);
            cout << himmelblaus_result_1 << " -> " << himmelblaus(himmelblaus_result_1) << endl;
        } else if (choose_algorithm == 2) {
            cout << "\nWpisz ilosc iteracji: \nOdpowiedz: ";
            cin >> iterations;

            auto himmelblaus_result_2 = simulated_annealing(
                    himmelblaus, himmelblaus_domain, himmelblaus_p0, iterations,
                    [](auto p) {
                        normal_distribution<double> n(0.0, 0.3);
                        for (auto& e : p) {
                            e = e + n(mt_generator);
                        }
                        return p;
                    },
                    [](int k) { return 1000.0 / k; });
            cout << himmelblaus_result_2 << " -> " << himmelblaus(himmelblaus_result_2) << endl;
        } else {
            cout << "Wybierz algorytm 1 lub 2!";
        }
    } else {
        cout << "Wybierz funkcje 1 lub 2!";
    }
    return 0;
}

#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <vector>

using namespace std;

random_device rd;
mt19937 gen(rd());

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

        p[distrib(gen)] += distrib_r(gen);
        double y2 = f(p2);
        if (y2 < f(p)) {
            p = p2;
        }
    }
    return p;
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
        booth_distrib_r(gen),
        booth_distrib_r(gen),
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
        himmelblaus_distrib_r(gen),
        himmelblaus_distrib_r(gen),
};

int main()
{
    int choose;
    int iterations;

    cout << "Wybierz optymalizowana funkcje:\n1. Booth function;\n2. Himmelblau's function.\nOdpowiedz: ";
    cin >> choose;

    if(choose == 1) {
        cout << "\nWpisz ilosc iteracji: \nOdpowiedz: ";
        cin >> iterations;

        auto booth_result = hill_climbing(booth, booth_domain, booth_p0, iterations);
        cout << booth_result << " -> " << booth(booth_result) << endl;
    } else if(choose == 2) {
            cout << "\nWpisz ilosc iteracji: \nOdpowiedz: ";
            cin >> iterations;

            auto himmelblaus_result = hill_climbing(himmelblaus, himmelblaus_domain, himmelblaus_p0, iterations);
            cout << himmelblaus_result << " -> " << himmelblaus(himmelblaus_result) << endl;
    } else {
        cout << "Wybierz funkcje 1 lub 2!";
    }

    return 0;
}

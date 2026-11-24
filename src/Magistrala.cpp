#include "Magistrala.h"

#include <algorithm>
#include <stdexcept>
#include <cstddef>

Magistrala::Magistrala(const std::string &cod,
                       const std::string &nume,
                       double tarifCalatorie)
    : cod(cod),
      nume(nume),
      tarifCalatorie(tarifCalatorie) {
    if (cod.empty()) {
        throw std::invalid_argument("Codul magistralei nu poate fi vid.");
    }
    if (nume.empty()) {
        throw std::invalid_argument("Numele magistralei nu poate fi vid.");
    }
    if (tarifCalatorie < 0.0) {
        throw std::invalid_argument("Tariful calatoriei nu poate fi negativ.");
    }
}

Magistrala::Magistrala(const Magistrala &other)
    : cod(other.cod),
      nume(other.nume),
      tarifCalatorie(other.tarifCalatorie),
      statii(other.statii),
      garnituri(other.garnituri) {
}

Magistrala &Magistrala::operator=(const Magistrala &other) {
    if (this == &other) {
        return *this;
    }
    cod = other.cod;
    nume = other.nume;
    tarifCalatorie = other.tarifCalatorie;
    statii = other.statii;
    garnituri = other.garnituri;
    return *this;
}

Magistrala::~Magistrala() = default;

double Magistrala::calculeazaCostReviziiNecesare() const {
    double costTotal = 0.0;
    for (const auto &garnitura: garnituri) {
        if (!garnitura.esteAptCirculatie()) {
            costTotal += garnitura.calculeazaCostRevizie();
        }
    }
    return costTotal;
}

void Magistrala::adaugaStatie(const Statie &statie) {
    statii.push_back(statie);
}

void Magistrala::insereazaStatie(std::size_t index, const Statie &statie) {
    if (index > statii.size()) {
        throw std::out_of_range("Index invalid pentru inserarea statiei.");
    }
    statii.insert(statii.begin() + static_cast<std::ptrdiff_t>(index), statie);
}

void Magistrala::eliminaStatieLaIndex(std::size_t index) {
    if (index >= statii.size()) {
        throw std::out_of_range("Index invalid pentru stergerea statiei.");
    }
    statii.erase(statii.begin() + static_cast<std::ptrdiff_t>(index));
}

bool Magistrala::eliminaStatie(const std::string &numeStatie) {
    const std::size_t nrSterse = std::erase_if(statii, [&numeStatie](const Statie &statie) {
        return statie.areAcelasiNume(numeStatie);
    });
    return nrSterse > 0;
}

void Magistrala::adaugaGarnitura(const GarnituraMetrou &garnitura) {
    garnituri.push_back(garnitura);
}

bool Magistrala::areAcelasiCod(const std::string &codCautat) const {
    return cod == codCautat;
}

int Magistrala::calculeazaCapacitateTotala() const {
    int capacitateTotala = 0;
    for (const auto &garnitura: garnituri) {
        capacitateTotala += garnitura.calculeazaCapacitateTotala();
    }
    return capacitateTotala;
}

int Magistrala::calculeazaCerereTotala(int ora) const {
    if (ora < 0 || ora > 23) {
        throw std::invalid_argument("Ora trebuie sa fie intre 0 si 23.");
    }
    int cerereTotala = 0;
    for (const auto &statie: statii) {
        cerereTotala += statie.calculeazaCererePasageri(ora);
    }
    return cerereTotala;
}

double Magistrala::calculeazaCostOperareOra() const {
    double costTotal = 0.0;
    for (const auto &garnitura: garnituri) {
        if (garnitura.esteAptCirculatie()) {
            costTotal += garnitura.getCostOperareOra();
        }
    }
    return costTotal;
}

double Magistrala::simuleazaOraTrafic(int ora) {
    const int cerereTotala = calculeazaCerereTotala(ora);
    const int capacitateTotala = calculeazaCapacitateTotala();
    const int calatoriTransportati = std::min(cerereTotala, capacitateTotala);

    for (auto &garnitura: garnituri) {
        if (garnitura.esteAptCirculatie()) {
            garnitura.simuleazaOraFunctionare(); //garniturile se uzeaza si noaptea deoarce circula si noaptea pentru
            //teste si curatenie
        }
    }

    return static_cast<double>(calatoriTransportati) * tarifCalatorie;
}

double Magistrala::efectueazaReviziiNecesare() {
    double costTotalRevizii = 0.0;
    for (auto &garnitura: garnituri) {
        if (!garnitura.esteAptCirculatie()) {
            costTotalRevizii += garnitura.efectueazaRevizie();
        }
    }
    return costTotalRevizii;
}

std::ostream &operator<<(std::ostream &os, const Magistrala &magistrala) {
    os << "Magistrala " << magistrala.cod << " (" << magistrala.nume << ")"
            << " | Tarif: " << magistrala.tarifCalatorie << " lei"
            << " | Capacitate: " << magistrala.calculeazaCapacitateTotala() << " calatori/ora\n";

    os << "  Statii (" << magistrala.statii.size() << "):\n";
    for (const auto &statie: magistrala.statii) {
        os << "    - " << statie << "\n";
    }

    os << "  Garnituri (" << magistrala.garnituri.size() << "):\n";
    for (const auto &garnitura: magistrala.garnituri) {
        os << "    - " << garnitura << "\n";
    }
    return os;
}

bool Magistrala::operator==(const Magistrala &other) const {
    return cod == other.cod;
}

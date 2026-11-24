#include "Statie.h"
#include <stdexcept>

namespace {
    constexpr double MULTIPLICATOR_NOD_CORESPONDENTA = 1.35;
    constexpr double MULTIPLICATOR_STANDARD = 1.0;

    constexpr int ORA_MINIMA = 0;
    constexpr int ORA_MAXIMA = 23;
    constexpr int ORA_VARF_DIMINEATA_START = 7;
    constexpr int ORA_VARF_DIMINEATA_STOP = 9;
    constexpr int ORA_VARF_SEARA_START = 16;
    constexpr int ORA_VARF_SEARA_STOP = 19;
    constexpr int ORA_NOAPTE_START = 23;
    constexpr int ORA_NOAPTE_STOP = 5;

    constexpr double FACTOR_ORA_VARF = 1.6;
    constexpr double FACTOR_ORA_NORMALA = 1.0;

    void valideazaOra(int oraCurenta) {
        if (oraCurenta < ORA_MINIMA || oraCurenta > ORA_MAXIMA) {
            throw std::invalid_argument("Ora curenta trebuie sa fie in intervalul [0, 23].");
        }
    }
} // namespace
Statie::Statie(const std::string &nume,
               const std::string &cartier,
               int fluxCalatoriOra,
               double costConstructie,
               bool nodCorespondenta)
    : nume(nume),
      cartier(cartier),
      fluxCalatoriOra(fluxCalatoriOra),
      costConstructie(costConstructie),
      nodCorespondenta(nodCorespondenta) {
    if (this->nume.empty() || this->cartier.empty()) {
        throw std::invalid_argument("Numele statiei si cartierul nu pot fi goale.");
    }
    if (this->fluxCalatoriOra < 0 || this->costConstructie < 0.0) {
        throw std::invalid_argument("Fluxul de calatori si costul de constructie nu pot fi negative.");
    }
}

bool Statie::esteDeschisaLaOra(int oraCurenta) {
    valideazaOra(oraCurenta);
    return oraCurenta >= ORA_NOAPTE_STOP && oraCurenta < ORA_NOAPTE_START;
}

int Statie::calculeazaCererePasageri(int oraCurenta) const {
    if (!this->esteDeschisaLaOra(oraCurenta)) {
        return 0;
    }
    const bool esteOraVarf =
            (oraCurenta >= ORA_VARF_DIMINEATA_START && oraCurenta <= ORA_VARF_DIMINEATA_STOP) ||
            (oraCurenta >= ORA_VARF_SEARA_START && oraCurenta <= ORA_VARF_SEARA_STOP);

    const double factorOrar = esteOraVarf ? FACTOR_ORA_VARF : FACTOR_ORA_NORMALA;
    const double multiplicatorNod = this->nodCorespondenta
                                        ? MULTIPLICATOR_NOD_CORESPONDENTA
                                        : MULTIPLICATOR_STANDARD;

    return static_cast<int>(static_cast<double>(fluxCalatoriOra) * factorOrar * multiplicatorNod);
}

bool Statie::areAcelasiNume(const std::string &numeCautat) const {
    return this->nume == numeCautat;
}

double Statie::getCostConstructie() const {
    return this->costConstructie;
}

std::ostream &operator<<(std::ostream &os, const Statie &statie) {
    os << "Statie[" << statie.nume
            << " | Cartier: " << statie.cartier
            << " | Flux/h: " << statie.fluxCalatoriOra
            << " | Cost: " << statie.costConstructie << " RON"
            << (statie.nodCorespondenta ? " | NOD CORESPONDENTA]" : "]");
    return os;
}

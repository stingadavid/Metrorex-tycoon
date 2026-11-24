#include "GarnituraMetrou.h"

#include <algorithm>
#include <stdexcept>

namespace {
    constexpr double STARE_MAXIMA = 100.0;
    constexpr double PRAG_MINIM_CIRCULATIE = 20.0;
    constexpr double UZURA_PE_ORA = 1.5;
    constexpr double PROCENT_COST_REVIZIE_COMPLETA = 0.25;
    constexpr int MAX_VAGOANE = 12;
    constexpr int MAX_CAPACITATE_VAGON = 350;
} // namespace

GarnituraMetrou::GarnituraMetrou(const std::string &model,
                                 int numarVagoane,
                                 int capacitatePerVagon,
                                 double costOperareOra,
                                 double costAchizitie)
    : model(model),
      numarVagoane(numarVagoane),
      capacitatePerVagon(capacitatePerVagon),
      costOperareOra(costOperareOra),
      costAchizitie(costAchizitie),
      stareTehnica(STARE_MAXIMA) {
    if (model.empty()) {
        throw std::invalid_argument("Modelul garniturii nu poate fi vid.");
    }
    if (numarVagoane <= 0 || numarVagoane > MAX_VAGOANE) {
        throw std::invalid_argument("Numarul de vagoane trebuie sa fie intre 1 si 12.");
    }
    if (capacitatePerVagon <= 0 || capacitatePerVagon > MAX_CAPACITATE_VAGON) {
        throw std::invalid_argument("Capacitatea per vagon trebuie sa fie intre 1 si 350.");
    }
    if (costOperareOra < 0.0 || costAchizitie < 0.0) {
        throw std::invalid_argument("Costurile garniturii nu pot fi negative.");
    }
}

int GarnituraMetrou::calculeazaCapacitateTotala() const {
    if (!esteAptCirculatie())
        return 0;
    return numarVagoane * capacitatePerVagon;
}

bool GarnituraMetrou::esteAptCirculatie() const {
    return stareTehnica >= PRAG_MINIM_CIRCULATIE;
}

double GarnituraMetrou::getCostOperareOra() const {
    return costOperareOra;
}

double GarnituraMetrou::getCostAchizitie() const {
    return costAchizitie;
}

void GarnituraMetrou::simuleazaOraFunctionare() {
    if (!esteAptCirculatie())
        return;
    stareTehnica = std::max(0.0, stareTehnica - UZURA_PE_ORA);
}

double GarnituraMetrou::calculeazaCostRevizie() const {
    const double uzuraAcumulata = (STARE_MAXIMA - stareTehnica) / STARE_MAXIMA;
    return costAchizitie * PROCENT_COST_REVIZIE_COMPLETA * uzuraAcumulata;
}

double GarnituraMetrou::efectueazaRevizie() {
    const double costRevizie = calculeazaCostRevizie();
    stareTehnica = STARE_MAXIMA;
    return costRevizie;
}

std::ostream &operator<<(std::ostream &os, const GarnituraMetrou &garnitura) {
    os << "Garnitura [" << garnitura.model << "] | Vagoane: " << garnitura.numarVagoane
            << " | Capacitate: " << garnitura.calculeazaCapacitateTotala() << " calatori"
            << " | Stare: " << garnitura.stareTehnica << "%"
            << " | Status: " << (garnitura.esteAptCirculatie() ? "Apt" : "Necesita revizie");
    return os;
}

#include "Dispecerat.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

Dispecerat::Dispecerat(const std::string &numeCompanie, double bugetInitial, int oraStart)
    : numeCompanie(numeCompanie),
      bugetLei(bugetInitial),
      oraCurenta(oraStart),
      ziCurenta(1) {
    if (numeCompanie.empty()) {
        throw std::invalid_argument("Numele companiei nu poate fi vid.");
    }
    if (bugetInitial < 0.0) {
        throw std::invalid_argument("Bugetul initial nu poate fi negativ.");
    }
    if (oraStart < 0 || oraStart > 23) {
        throw std::invalid_argument("Ora de start trebuie sa fie intre 0 si 23.");
    }
}

Magistrala *Dispecerat::gasesteMagistrala(const std::string &codMagistrala) {
    for (auto &magistrala: magistrale) {
        if (magistrala.areAcelasiCod(codMagistrala)) {
            return &magistrala;
        }
    }
    return nullptr;
}

void Dispecerat::adaugaMagistrala(const Magistrala &magistrala) {
    for (const auto &existenta: magistrale) {
        if (existenta == magistrala) {
            throw std::invalid_argument("Exista deja o magistrala cu acest cod.");
        }
    }
    magistrale.push_back(magistrala);
}

void Dispecerat::adaugaStatiePeMagistrala(const std::string &codMagistrala, const Statie &statie) {
    Magistrala *magistrala = gasesteMagistrala(codMagistrala);
    if (magistrala == nullptr) {
        throw std::invalid_argument("Magistrala specificata nu exista: " + codMagistrala);
    }
    magistrala->adaugaStatie(statie);
}

void Dispecerat::adaugaGarnituraPeMagistrala(const std::string &codMagistrala, const GarnituraMetrou &garnitura) {
    Magistrala *magistrala = gasesteMagistrala(codMagistrala);
    if (magistrala == nullptr) {
        throw std::invalid_argument("Magistrala specificata nu exista: " + codMagistrala);
    }
    magistrala->adaugaGarnitura(garnitura);
}

void Dispecerat::construiesteStatiePeMagistrala(const std::string &codMagistrala, const Statie &statie) {
    Magistrala *magistrala = gasesteMagistrala(codMagistrala);
    if (magistrala == nullptr) {
        throw std::invalid_argument("Magistrala specificata nu exista: " + codMagistrala);
    }
    const double cost = statie.getCostConstructie();
    if (bugetLei < cost) {
        throw std::runtime_error("Buget insuficient pentru constructia statiei.");
    }
    bugetLei -= cost;
    magistrala->adaugaStatie(statie);
}

void Dispecerat::insereazaStatiePeMagistrala(const std::string &codMagistrala, std::size_t index,
                                             const Statie &statie) {
    Magistrala *magistrala = gasesteMagistrala(codMagistrala);
    if (magistrala == nullptr) {
        throw std::invalid_argument("Magistrala specificata nu exista: " + codMagistrala);
    }
    const double cost = statie.getCostConstructie();
    if (bugetLei < cost) {
        throw std::runtime_error("Buget insuficient pentru constructia statiei.");
    }
    magistrala->insereazaStatie(index, statie);
    bugetLei -= cost;
}

void Dispecerat::eliminaStatieDePeMagistrala(const std::string& codMagistrala, const std::string& numeStatie) {
    Magistrala* magistrala = gasesteMagistrala(codMagistrala);
    if (magistrala == nullptr) {
        throw std::invalid_argument("Magistrala specificata nu exista: " + codMagistrala);
    }
    if (!magistrala->eliminaStatie(numeStatie)) {
        throw std::invalid_argument("Statia '" + numeStatie + "' nu exista pe magistrala " + codMagistrala + ".");
    }
}
void Dispecerat::eliminaStatieDePeMagistralaLaIndex(const std::string &codMagistrala, std::size_t index) {
    Magistrala *magistrala = gasesteMagistrala(codMagistrala);
    if (magistrala == nullptr) {
        throw std::invalid_argument("Magistrala specificata nu exista: " + codMagistrala);
    }
    magistrala->eliminaStatieLaIndex(index);
}

void Dispecerat::cumparaGarnituraPeMagistrala(const std::string &codMagistrala, const GarnituraMetrou &garnitura) {
    Magistrala *magistrala = gasesteMagistrala(codMagistrala);
    if (magistrala == nullptr) {
        throw std::invalid_argument("Magistrala specificata nu exista: " + codMagistrala);
    }
    const double cost = garnitura.getCostAchizitie();
    if (bugetLei < cost) {
        throw std::runtime_error("Buget insuficient pentru achizitia garniturii.");
    }
    bugetLei -= cost;
    magistrala->adaugaGarnitura(garnitura);
}

double Dispecerat::avanseazaOra() {
    double venituriTotale = 0.0;
    double costuriTotale = 0.0;

    for (auto &magistrala: magistrale) {
        costuriTotale += magistrala.calculeazaCostOperareOra();
        venituriTotale += magistrala.simuleazaOraTrafic(oraCurenta);
    }

    const double profitNet = venituriTotale - costuriTotale;
    bugetLei += profitNet;

    ++oraCurenta;
    if (oraCurenta >= 24) {
        oraCurenta = 0;
        ++ziCurenta;
    }

    return profitNet;
}

double Dispecerat::efectueazaReviziiToateMagistralele() {
    double costTotalRevizii = 0.0;
    for (const auto &magistrala: magistrale) {
        costTotalRevizii += magistrala.calculeazaCostReviziiNecesare();
    }

    if (bugetLei < costTotalRevizii) {
        throw std::runtime_error("Buget insuficient pentru efectuarea reviziilor.");
    }

    for (auto &magistrala: magistrale) {
        (void) magistrala.efectueazaReviziiNecesare();
    }

    bugetLei -= costTotalRevizii;
    return costTotalRevizii;
}

double Dispecerat::getBugetLei() const {
    return bugetLei;
}

int Dispecerat::getOraCurenta() const {
    return oraCurenta;
}

int Dispecerat::getZiCurenta() const {
    return ziCurenta;
}

Dispecerat Dispecerat::incarcaDinFisier(const std::string &caleFisier) {
    std::ifstream fisier(caleFisier);
    if (!fisier.is_open()) {
        throw std::runtime_error("Nu s-a putut deschide fisierul: " + caleFisier);
    }

    std::string linie;
    bool areCompanie = false;
    Dispecerat dispecerat("Temp", 0.0, 6);

    while (std::getline(fisier, linie)) {
        if (!linie.empty() && linie.back() == '\r') {
            linie.pop_back();
        }
        if (linie.empty() || linie[0] == '#') {
            continue;
        }

        std::stringstream ss(linie);
        std::string tip;
        std::getline(ss, tip, ';');

        if (tip == "COMPANIE") {
            if (areCompanie) {
                throw std::runtime_error("Fisier invalid: linia COMPANIE apare de mai multe ori.");
            }
            std::string nume;
            std::string bugetStr;
            std::string oraStr;
            if (!std::getline(ss, nume, ';') || !std::getline(ss, bugetStr, ';')) {
                throw std::runtime_error("Linie COMPANIE incompleta in fisier.");
            }
            if (std::getline(ss, oraStr, ';') && !oraStr.empty()) {
                dispecerat = Dispecerat(nume, std::stod(bugetStr), std::stoi(oraStr));
            } else {
                dispecerat = Dispecerat(nume, std::stod(bugetStr));
            }
            areCompanie = true;
        } else if (tip == "MAGISTRALA") {
            if (!areCompanie) {
                throw std::runtime_error("Fisier invalid: linia COMPANIE trebuie sa fie prima.");
            }
            std::string cod;
            std::string nume;
            std::string tarifStr;
            if (!std::getline(ss, cod, ';') || !std::getline(ss, nume, ';') || !std::getline(ss, tarifStr, ';')) {
                throw std::runtime_error("Linie MAGISTRALA incompleta in fisier.");
            }
            dispecerat.adaugaMagistrala(Magistrala(cod, nume, std::stod(tarifStr)));
        } else if (tip == "STATIE") {
            if (!areCompanie) {
                throw std::runtime_error("Fisier invalid: linia COMPANIE trebuie sa fie prima.");
            }
            std::string codMag, nume, zona, fluxStr, costStr, nodStr;
            if (!std::getline(ss, codMag, ';') || !std::getline(ss, nume, ';') ||
                !std::getline(ss, zona, ';') || !std::getline(ss, fluxStr, ';') ||
                !std::getline(ss, costStr, ';') || !std::getline(ss, nodStr, ';')) {
                throw std::runtime_error("Linie STATIE incompleta in fisier.");
            }
            if (nodStr != "0" && nodStr != "1" && nodStr != "true" && nodStr != "false") {
                throw std::runtime_error("Valoare invalida pentru nod de corespondenta: " + nodStr);
            }
            const bool esteNod = (nodStr == "1" || nodStr == "true");
            dispecerat.adaugaStatiePeMagistrala(
                codMag,
                Statie(nume, zona, std::stoi(fluxStr), std::stod(costStr), esteNod)
            );
        } else if (tip == "GARNITURA") {
            if (!areCompanie) {
                throw std::runtime_error("Fisier invalid: linia COMPANIE trebuie sa fie prima.");
            }
            std::string codMag, model, vagoaneStr, capVagonStr, costOperareStr, costAchizitieStr;
            if (!std::getline(ss, codMag, ';') || !std::getline(ss, model, ';') ||
                !std::getline(ss, vagoaneStr, ';') || !std::getline(ss, capVagonStr, ';') ||
                !std::getline(ss, costOperareStr, ';') || !std::getline(ss, costAchizitieStr, ';')) {
                throw std::runtime_error("Linie GARNITURA incompleta in fisier.");
            }
            dispecerat.adaugaGarnituraPeMagistrala(
                codMag,
                GarnituraMetrou(
                    model,
                    std::stoi(vagoaneStr),
                    std::stoi(capVagonStr),
                    std::stod(costOperareStr),
                    std::stod(costAchizitieStr)
                )
            );
        } else {
            throw std::runtime_error("Tip necunoscut in fisierul de configurare: " + tip);
        }
    }

    if (!areCompanie) {
        throw std::runtime_error("Fisierul nu contine definitia COMPANIE.");
    }

    return dispecerat;
}

std::ostream &operator<<(std::ostream &os, const Dispecerat &dispecerat) {
    os << "=== DISPECERAT CENTRAL: " << dispecerat.numeCompanie << " ===\n"
            << "Ziua: " << dispecerat.ziCurenta
            << " | Ora: " << (dispecerat.oraCurenta < 10 ? "0" : "") << dispecerat.oraCurenta << ":00"
            << " | Buget: " << dispecerat.bugetLei << " lei\n\n";

    for (const auto &magistrala: dispecerat.magistrale) {
        os << magistrala << "\n";
    }

    return os;
}

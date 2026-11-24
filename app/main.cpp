#include "Dispecerat.h"

#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

namespace {

std::string determinaCaleAssets() {
    if (std::ifstream("assets/retea_initiala.txt").good()) {
        return "assets/retea_initiala.txt";
    }
    return "../assets/retea_initiala.txt";
}

void afiseazaMeniu() {
    std::cout << "\n--- MENIU DISPECERAT METROREX TYCOON ---\n"
              << "1. Afiseaza starea retelei\n"
              << "2. Avanseaza timpul (N ore)\n"
              << "3. Construieste statie noua pe magistrala\n"
              << "4. Cumpara garnitura noua pe magistrala\n"
              << "5. Efectueaza revizii pe toate magistralele\n"
              << "6. Adauga magistrala noua\n"
              << "7. Insereaza statie pe traseu la o pozitie\n"
              << "8. Inchide statie dupa nume\n"
              << "9. Inchide statie dupa pozitie (index)\n"
              << "0. Iesire din simulare\n"
              << "Optiune: ";
}

void curataBufferInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

} // namespace

int main() {
    try {
        Dispecerat dispecerat = Dispecerat::incarcaDinFisier(determinaCaleAssets());
        std::cout << "Reteaua a fost incarcata cu succes!\n\n";
        std::cout << dispecerat;

        int optiune = -1;
        while (true) {
            afiseazaMeniu();
            if (!(std::cin >> optiune)) {
                if (std::cin.eof()) {
                    std::cout << "\nSfarsit flux de intrare. Inchidere simulare.\n";
                    break;
                }
                curataBufferInput();
                std::cout << "Eroare: introdu o optiune numerica valida.\n";
                continue;
            }

            if (optiune == 0) {
                std::cout << "Simularea Metrorex Tycoon s-a incheiat.\n";
                break;
            }

            try {
                if (optiune == 1) {
                    std::cout << "\n" << dispecerat;
                } else if (optiune == 2) {
                    int numarOre = 0;
                    std::cout << "Numar de ore de simulat: ";
                    if (!(std::cin >> numarOre) || numarOre <= 0) {
                        curataBufferInput();
                        std::cout << "Numar invalid de ore.\n";
                        continue;
                    }

                    double profitCumulat = 0.0;
                    for (int i = 0; i < numarOre; ++i) {
                        profitCumulat += dispecerat.avanseazaOra();
                    }
                    std::cout << "S-au simulat " << numarOre << " ore. Profit net perioada: "
                              << profitCumulat << " lei | Ziua " << dispecerat.getZiCurenta()
                              << ", Ora " << dispecerat.getOraCurenta() << ":00 | Buget: "
                              << dispecerat.getBugetLei() << " lei.\n";
                } else if (optiune == 3) {
                    std::string codMagistrala;
                    std::string numeStatie;
                    std::string zona;
                    int fluxBaza = 0;
                    double costConstructie = 0.0;
                    int esteNodInt = 0;

                    std::cout << "Cod magistrala: ";
                    std::cin >> codMagistrala;
                    curataBufferInput();

                    std::cout << "Nume statie: ";
                    std::getline(std::cin, numeStatie);

                    std::cout << "Zona / Sector: ";
                    std::getline(std::cin, zona);

                    std::cout << "Flux calatori de baza/ora: ";
                    std::cin >> fluxBaza;

                    std::cout << "Cost constructie (lei): ";
                    std::cin >> costConstructie;

                    std::cout << "Este nod de corespondenta? (1 = Da, 0 = Nu): ";
                    std::cin >> esteNodInt;

                    Statie statieNoua{
                        numeStatie,
                        zona,
                        fluxBaza,
                        costConstructie,
                        esteNodInt == 1
                    };
                    dispecerat.construiesteStatiePeMagistrala(codMagistrala, statieNoua);
                    std::cout << "Statia '" << numeStatie << "' a fost construita pe "
                              << codMagistrala << ". Buget ramas: " << dispecerat.getBugetLei() << " lei.\n";
                } else if (optiune == 4) {
                    std::string codMagistrala;
                    std::string model;
                    int numarVagoane = 0;
                    int capacitateVagon = 0;
                    double costOperare = 0.0;
                    double costAchizitie = 0.0;

                    std::cout << "Cod magistrala: ";
                    std::cin >> codMagistrala;
                    curataBufferInput();

                    std::cout << "Model garnitura: ";
                    std::getline(std::cin, model);

                    std::cout << "Numar vagoane: ";
                    std::cin >> numarVagoane;

                    std::cout << "Capacitate per vagon: ";
                    std::cin >> capacitateVagon;

                    std::cout << "Cost operare/ora (lei): ";
                    std::cin >> costOperare;

                    std::cout << "Cost achizitie (lei): ";
                    std::cin >> costAchizitie;

                    GarnituraMetrou garnituraNoua{
                        model,
                        numarVagoane,
                        capacitateVagon,
                        costOperare,
                        costAchizitie
                    };
                    dispecerat.cumparaGarnituraPeMagistrala(codMagistrala, garnituraNoua);
                    std::cout << "Garnitura '" << model << "' a fost achizitionata pentru "
                              << codMagistrala << ". Buget ramas: " << dispecerat.getBugetLei() << " lei.\n";
                } else if (optiune == 5) {
                    const double costRevizii = dispecerat.efectueazaReviziiToateMagistralele();
                    std::cout << "Revizii finalizate pe intreaga retea. Cost total: "
                              << costRevizii << " lei | Buget ramas: "
                              << dispecerat.getBugetLei() << " lei.\n";
                } else if (optiune == 6) {
                    std::string codMagistrala;
                    std::string numeMagistrala;
                    double tarif = 0.0;

                    std::cout << "Cod magistrala noua: ";
                    std::cin >> codMagistrala;
                    curataBufferInput();

                    std::cout << "Nume magistrala: ";
                    std::getline(std::cin, numeMagistrala);

                    std::cout << "Tarif calatorie (lei): ";
                    std::cin >> tarif;

                    dispecerat.adaugaMagistrala(Magistrala{codMagistrala, numeMagistrala, tarif});
                    std::cout << "Magistrala " << codMagistrala << " (" << numeMagistrala
                              << ") a fost deschisa.\n";
                } else if (optiune == 7) {
                    std::string codMagistrala;
                    std::size_t index = 0;
                    std::string numeStatie;
                    std::string zona;
                    int fluxBaza = 0;
                    double costConstructie = 0.0;
                    int esteNodInt = 0;

                    std::cout << "Cod magistrala: ";
                    std::cin >> codMagistrala;
                    std::cout << "Pozitie inserare (index de la 0): ";
                    std::cin >> index;
                    curataBufferInput();

                    std::cout << "Nume statie: ";
                    std::getline(std::cin, numeStatie);
                    std::cout << "Zona / Sector: ";
                    std::getline(std::cin, zona);
                    std::cout << "Flux calatori de baza/ora: ";
                    std::cin >> fluxBaza;
                    std::cout << "Cost constructie (lei): ";
                    std::cin >> costConstructie;
                    std::cout << "Este nod de corespondenta? (1 = Da, 0 = Nu): ";
                    std::cin >> esteNodInt;

                    Statie statieNoua{numeStatie, zona, fluxBaza, costConstructie, esteNodInt == 1};
                    dispecerat.insereazaStatiePeMagistrala(codMagistrala, index, statieNoua);
                    std::cout << "Statia '" << numeStatie << "' a fost inserata pe "
                              << codMagistrala << " la pozitia " << index << ".\n";
                } else if (optiune == 8) {
                    std::string codMagistrala;
                    std::string numeStatie;

                    std::cout << "Cod magistrala: ";
                    std::cin >> codMagistrala;
                    curataBufferInput();
                    std::cout << "Nume statie de inchis: ";
                    std::getline(std::cin, numeStatie);

                    dispecerat.eliminaStatieDePeMagistrala(codMagistrala, numeStatie);
                    std::cout << "Statia '" << numeStatie << "' a fost eliminata de pe " << codMagistrala << ".\n";
                } else if (optiune == 9) {
                    std::string codMagistrala;
                    std::size_t index = 0;

                    std::cout << "Cod magistrala: ";
                    std::cin >> codMagistrala;
                    std::cout << "Pozitie statie de inchis (index): ";
                    std::cin >> index;

                    dispecerat.eliminaStatieDePeMagistralaLaIndex(codMagistrala, index);
                    std::cout << "Statia de la pozitia " << index << " a fost eliminata de pe " << codMagistrala << ".\n";
                } else {
                    std::cout << "Optiune necunoscuta.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "[EROARE OPERATIUNE] " << e.what() << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[EROARE FATALA INITIALIZARE] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
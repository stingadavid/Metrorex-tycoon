#undef NDEBUG
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include "Statie.h"
#include "GarnituraMetrou.h"
#include "Magistrala.h"
#include "Dispecerat.h"

void testeazaStatie() {
    const Statie unirii{"Piata Unirii", "Centru", 1000, 500000.0, true};
    const Statie timpuriNoi{"Timpuri Noi", "Sector 3", 1000, 300000.0, false};

    // 1. Nume si cost
    assert(unirii.areAcelasiNume("Piata Unirii"));
    assert(!unirii.areAcelasiNume("Victoriei"));
    assert(std::abs(unirii.getCostConstructie() - 500000.0) < 1e-6);

    // 2. Program de functionare (05:00 - 22:59 deschis, 23:00 - 04:59 inchis)
    assert(unirii.esteDeschisaLaOra(5));
    assert(unirii.esteDeschisaLaOra(22));
    assert(!unirii.esteDeschisaLaOra(23));
    assert(!unirii.esteDeschisaLaOra(4));

    // 3. Calcul cerere pasageri (ora normala vs varf vs noapte, nod vs normal)
    assert(timpuriNoi.calculeazaCererePasageri(12) == 1000); // 1000 * 1.0 * 1.0
    assert(timpuriNoi.calculeazaCererePasageri(8) == 1600); // 1000 * 1.6 * 1.0
    assert(unirii.calculeazaCererePasageri(12) == 1350); // 1000 * 1.0 * 1.35
    assert(unirii.calculeazaCererePasageri(8) == 2160); // 1000 * 1.6 * 1.35
    assert(unirii.calculeazaCererePasageri(2) == 0); // noapte -> 0

    // 4. Operator<<
    std::ostringstream oss;
    oss << unirii;
    assert(!oss.str().empty());

    // 5. Validari si exceptii (constructor si ore invalide)
    bool threw = false;
    try {
        const Statie invalida{"", "Centru", 1000, 100.0, false};
        (void) invalida;
    } catch (const std::invalid_argument &) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        (void) unirii.esteDeschisaLaOra(24);
    } catch (const std::invalid_argument &) {
        threw = true;
    }
    assert(threw);
}

void testeazaGarnituraMetrou() {
    GarnituraMetrou caf{"CAF Inneo", 6, 200, 1200.0, 5000000.0};

    // 1. Initializare, capacitate si costuri
    assert(caf.esteAptCirculatie());
    assert(caf.calculeazaCapacitateTotala() == 1200);
    assert(std::abs(caf.getCostOperareOra() - 1200.0) < 1e-6);
    assert(std::abs(caf.getCostAchizitie() - 5000000.0) < 1e-6);

    // 2. Uzura si retragere din circulatie (54 ore * 1.5% = 81% uzura -> 19% < 20%)
    GarnituraMetrou iva{"IVA Astra", 4, 150, 800.0, 2000000.0};
    for (int ora = 0; ora < 54; ++ora) {
        iva.simuleazaOraFunctionare();
    }
    assert(!iva.esteAptCirculatie());
    assert(iva.calculeazaCapacitateTotala() == 0);

    // 3. Revizie (20 ore * 1.5% = 30% uzura -> cost = 4.000.000 * 0.25 * 0.30 = 300.000)
    GarnituraMetrou bombardier{"Bombardier Movia", 6, 180, 1000.0, 4000000.0};
    for (int ora = 0; ora < 20; ++ora) {
        bombardier.simuleazaOraFunctionare();
    }
    const double costRevizie = bombardier.efectueazaRevizie();
    assert(std::abs(costRevizie - 300000.0) < 1e-6);
    assert(bombardier.esteAptCirculatie());
    assert(bombardier.calculeazaCapacitateTotala() == 1080);

    // 4. Operator<<
    std::ostringstream oss;
    oss << caf;
    assert(!oss.str().empty());

    // 5. Validari constructor
    bool threw = false;
    try {
        const GarnituraMetrou invalid{"", 6, 200, 1000.0, 4000000.0};
        (void) invalid;
    } catch (const std::invalid_argument &) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        const GarnituraMetrou invalid{"CAF", 15, 200, 1000.0, 4000000.0};
        (void) invalid;
    } catch (const std::invalid_argument &) {
        threw = true;
    }
    assert(threw);
}

void testeazaMagistrala() {
    Magistrala m2{"M2", "Berceni - Pipera", 3.0};

    // 1. adaugaStatie, insereazaStatie, eliminaStatieLaIndex, eliminaStatie + exceptii out_of_range
    m2.adaugaStatie(Statie{"Berceni", "Sector 4", 800, 200000.0, false});
    m2.adaugaStatie(Statie{"Piata Unirii", "Centru", 1000, 500000.0, true});
    m2.insereazaStatie(1, Statie{"Timpuri Noi", "Sector 3", 900, 300000.0, false});
    m2.insereazaStatie(2, Statie{"Universitate", "Centru", 700, 350000.0, false});

    m2.eliminaStatieLaIndex(2); // scoate "Universitate"
    const bool aEliminatTimpuriNoi = m2.eliminaStatie("Timpuri Noi");
    assert(aEliminatTimpuriNoi);

    const bool aEliminatInexistenta = m2.eliminaStatie("Statie Inexistenta");
    assert(!aEliminatInexistenta);

    bool aAruncatOutOfRange = false;
    try {
        m2.insereazaStatie(99, Statie{"Pipera", "Sector 2", 1200, 400000.0, false});
    } catch (const std::out_of_range&) {
        aAruncatOutOfRange = true;
    }
    assert(aAruncatOutOfRange);

    aAruncatOutOfRange = false;
    try {
        m2.eliminaStatieLaIndex(99);
    } catch (const std::out_of_range&) {
        aAruncatOutOfRange = true;
    }
    assert(aAruncatOutOfRange);

    // 2. simuleazaOraTrafic (cerere > capacitate, cerere < capacitate, noapte) + efectueazaReviziiNecesare
    // Cererea pe m2 la ora 12 (normala): 800 + 1350 = 2150 calatori
    // Cererea pe m2 la ora 8 (varf, x1.6): 1280 + 2160 = 3440 calatori
    assert(m2.calculeazaCerereTotala(12) == 2150);
    assert(m2.calculeazaCerereTotala(8) == 3440);

    // Adaugam 2 garnituri (capacitate totala: 1200 + 1200 = 2400 calatori/ora)
    m2.adaugaGarnitura(GarnituraMetrou{"CAF Inneo", 6, 200, 1200.0, 5000000.0});
    m2.adaugaGarnitura(GarnituraMetrou{"Bombardier Movia", 6, 200, 1100.0, 4500000.0});
    assert(m2.calculeazaCapacitateTotala() == 2400);

    // Caz A: cerere > capacitate (ora 8: cerere 3440 > capacitate 2400 -> transporta 2400 * 3.0 = 7200.0 lei)
    const double venitCazA = m2.simuleazaOraTrafic(8);
    assert(std::abs(venitCazA - 7200.0) < 1e-6);

    // Caz B: cerere < capacitate (ora 12: cerere 2150 < capacitate 2400 -> transporta 2150 * 3.0 = 6450.0 lei)
    const double venitCazB = m2.simuleazaOraTrafic(12);
    assert(std::abs(venitCazB - 6450.0) < 1e-6);

    // Caz C: noapte (ora 2: cerere 0 -> venit 0.0 lei, dar trenurile ruleaza pt probe/curatenie si se uzeaza)
    const double venitCazC = m2.simuleazaOraTrafic(2);
    assert(std::abs(venitCazC - 0.0) < 1e-6);

    // 3. Rule of 3: constructor de copiere si operator=
    Magistrala copieConstructor = m2;
    copieConstructor.adaugaStatie(Statie{"Pipera", "Sector 2", 2000, 400000.0, false});
    assert(m2.calculeazaCerereTotala(12) == 2150);
    assert(copieConstructor.calculeazaCerereTotala(12) == 4150);

    Magistrala copieAtribuire{"M1", "Pantelimon - Dristor", 3.0};
    copieAtribuire = m2;
    copieAtribuire.eliminaStatie("Berceni");
    assert(m2.calculeazaCerereTotala(12) == 2150);
    assert(copieAtribuire.calculeazaCerereTotala(12) == 1350);

    // 4. Uzura completa (au trecut deja 3 ore: ora 8, 12 si 2; mai simulam 51 ore => 54 ore * 1.5% = 81% uzura => 19% stare < 20%)
    for (int i = 0; i < 51; ++i) {
        (void)m2.simuleazaOraTrafic(12);
    }
    assert(m2.calculeazaCapacitateTotala() == 0);
    const double costRevizii = m2.efectueazaReviziiNecesare();
    assert(costRevizii > 0.0);
    assert(m2.calculeazaCapacitateTotala() == 2400);

    // 5. Compunere operator<<
    std::ostringstream oss;
    oss << m2;
    const std::string textAfisat = oss.str();
    assert(textAfisat.find("Berceni") != std::string::npos);
    assert(textAfisat.find("Piata Unirii") != std::string::npos);
    assert(textAfisat.find("CAF Inneo") != std::string::npos);
    assert(textAfisat.find("Bombardier Movia") != std::string::npos);
}

void testeazaDispecerat() {
    // 1. Incarcare retea initiala din fisierul assets/retea_initiala.txt
    Dispecerat dispecerat = Dispecerat::incarcaDinFisier("assets/retea_initiala.txt");
    assert(std::abs(dispecerat.getBugetLei() - 12000000.0) < 1e-6);
    assert(dispecerat.getOraCurenta() == 6);
    assert(dispecerat.getZiCurenta() == 1);

    // 2. Investitii cu scadere din buget: construiesteStatiePeMagistrala si cumparaGarnituraPeMagistrala
    const double bugetInainte = dispecerat.getBugetLei();
    dispecerat.construiesteStatiePeMagistrala(
        "M2",
        Statie{"Universitate", "Centru", 1500, 500000.0, false}
    );
    assert(std::abs(dispecerat.getBugetLei() - (bugetInainte - 500000.0)) < 1e-6);

    dispecerat.cumparaGarnituraPeMagistrala(
        "M1",
        GarnituraMetrou{"CAF Inneo", 6, 200, 1200.0, 5000000.0}
    );
    assert(std::abs(dispecerat.getBugetLei() - (bugetInainte - 5500000.0)) < 1e-6);

    // 3. Avansarea orelor si trecerea de la ora 23:00 la 00:00 (incrementarea zilei)
    // Plecam de la ora 6:00 -> 17 apeluri duc la ora 23:00 (tot Ziua 1)
    for (int i = 0; i < 17; ++i) {
        (void)dispecerat.avanseazaOra();
    }
    assert(dispecerat.getOraCurenta() == 23);
    assert(dispecerat.getZiCurenta() == 1);

    // Al 18-lea apel trece ceasul la 00:00 si incrementeaza ziua la 2
    (void)dispecerat.avanseazaOra();
    assert(dispecerat.getOraCurenta() == 0);
    assert(dispecerat.getZiCurenta() == 2);

    // 4. Uzura pana sub 20% + efectueazaReviziiToateMagistralele (inclusiv buget insuficient)
    // Au trecut deja 18 ore; mai simulam 36 ore => 54 ore * 1.5% = 81% uzura => 19% stare (< 20%)
    for (int i = 0; i < 36; ++i) {
        (void)dispecerat.avanseazaOra();
    }

    // Testam intai cazul cu buget insuficient pe un dispecerat cu fonduri mici
    Dispecerat dispeceratSarac{"Metrorex Test", 1000.0, 8};
    dispeceratSarac.adaugaMagistrala(Magistrala{"M4", "Gara de Nord - Straulesti", 3.0});
    dispeceratSarac.adaugaGarnituraPeMagistrala(
        "M4",
        GarnituraMetrou{"Astra IVA", 6, 180, 950.0, 3500000.0}
    );
    for (int i = 0; i < 54; ++i) {
        (void)dispeceratSarac.avanseazaOra();
    }

    bool aAruncatBugetInsuficient = false;
    try {
        (void)dispeceratSarac.efectueazaReviziiToateMagistralele();
    } catch (const std::runtime_error&) {
        aAruncatBugetInsuficient = true;
    }
    assert(aAruncatBugetInsuficient);

    // Pe dispeceratul principal avem buget suficient -> reviziile reusesc si scad bugetul
    const double bugetInainteRevizii = dispecerat.getBugetLei();
    const double costRevizii = dispecerat.efectueazaReviziiToateMagistralele();
    assert(costRevizii > 0.0);
    assert(std::abs(dispecerat.getBugetLei() - (bugetInainteRevizii - costRevizii)) < 1e-6);

    // 5. Compunere operator<<
    std::ostringstream oss;
    oss << dispecerat;
    const std::string output = oss.str();
    assert(output.find("Metrorex Bucuresti") != std::string::npos);
    assert(output.find("M1") != std::string::npos);
    assert(output.find("M2") != std::string::npos);
    assert(output.find("Universitate") != std::string::npos);
}

int main() {
    testeazaStatie();
    testeazaGarnituraMetrou();
    testeazaMagistrala();
    testeazaDispecerat();
    return 0;
}

//
// Created by Lotik on 11/24/2026.
//

#ifndef OOP_MAGISTRALA_H
#define OOP_MAGISTRALA_H

#include "GarnituraMetrou.h"
#include "Statie.h"

#include <ostream>
#include <string>
#include <vector>
#include <cstddef>

class Magistrala {
private:
    std::string cod; //M2, M3
    std::string nume; // REPUBLICA-DRISTOR2
    double tarifCalatorie; // pretul unui bilet pe aceasta magistrala (lei)
    std::vector<Statie> statii;
    std::vector<GarnituraMetrou> garnituri;

public:
    Magistrala(const std::string &cod, const std::string &nume, double tarifCalatorie);

    // Rule of 3: constructor de copiere, operator= de copiere, destructor
    Magistrala(const Magistrala &other);

    [[nodiscard]] bool operator==(const Magistrala &other) const;

    Magistrala &operator=(const Magistrala &other);

    ~Magistrala();

    void adaugaStatie(const Statie &statie);

    void insereazaStatie(std::size_t index, const Statie &statie);

    void eliminaStatieLaIndex(std::size_t index);

    bool eliminaStatie(const std::string &numeStatie);

    void adaugaGarnitura(const GarnituraMetrou &garnitura);
    
    [[nodiscard]] double calculeazaCostReviziiNecesare() const;

    [[nodiscard]] bool areAcelasiCod(const std::string &codCautat) const;

    [[nodiscard]] int calculeazaCapacitateTotala() const;

    [[nodiscard]] int calculeazaCerereTotala(int ora) const;

    [[nodiscard]] double calculeazaCostOperareOra() const;

    // Simuleaza o ora de trafic pe magistrala si returneaza venitul obtinut din bilete
    [[nodiscard]] double simuleazaOraTrafic(int ora);

    // Efectueaza revizia tuturor garniturilor care nu mai sunt apte si returneaza costul total
    [[nodiscard]] double efectueazaReviziiNecesare();

    friend std::ostream &operator<<(std::ostream &os, const Magistrala &magistrala);
};

#endif //OOP_MAGISTRALA_H

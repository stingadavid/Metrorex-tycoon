//
// Created by Lotik on 11/24/2026.
//

#ifndef OOP_STATIE_H
#define OOP_STATIE_H

#include <ostream>
#include <string>

class Statie {
private:
    std::string nume;
    std::string cartier;
    int fluxCalatoriOra;
    double costConstructie;
    bool nodCorespondenta;

public:
    Statie(const std::string &nume,
           const std::string &cartier,
           int fluxCalatoriOra,
           double costConstructie,
           bool nodCorespondenta);

    [[nodiscard]] bool esteDeschisaLaOra(int oraCurenta) const;

    [[nodiscard]] int calculeazaCererePasageri(int oraCurenta) const;

    [[nodiscard]] bool areAcelasiNume(const std::string &numeCautat) const;

    [[nodiscard]] double getCostConstructie() const;

    friend std::ostream &operator<<(std::ostream &os, const Statie &statie);
};

#endif //OOP_STATIE_H

//
// Created by Lotik on 11/24/2026.
//

#ifndef OOP_GARNITURAMETROU_H
#define OOP_GARNITURAMETROU_H

#include <ostream>
#include <string>

class GarnituraMetrou {
private:
    std::string model;
    int numarVagoane;
    int capacitatePerVagon;
    double costOperareOra;
    double costAchizitie;
    double stareTehnica; // procent [0.0, 100.0]

public:
    GarnituraMetrou(const std::string &model,
                    int numarVagoane,
                    int capacitatePerVagon,
                    double costOperareOra,
                    double costAchizitie);

    [[nodiscard]] bool esteAptCirculatie() const;

    [[nodiscard]] double calculeazaCostRevizie() const;

    [[nodiscard]] int calculeazaCapacitateTotala() const;

    [[nodiscard]] double getCostOperareOra() const;

    [[nodiscard]] double getCostAchizitie() const;

    void simuleazaOraFunctionare();

    // Reface starea tehnica la 100% si returneaza costul reviziei (de scazut din buget).
    [[nodiscard]] double efectueazaRevizie();

    friend std::ostream &operator<<(std::ostream &os, const GarnituraMetrou &garnitura);
};
#endif //OOP_GARNITURAMETROU_H

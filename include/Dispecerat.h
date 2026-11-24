#ifndef OOP_DISPECERAT_H
#define OOP_DISPECERAT_H

#include "Magistrala.h"

#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

class Dispecerat {
private:
    std::string numeCompanie;
    double bugetLei;
    int oraCurenta;
    int ziCurenta;
    std::vector<Magistrala> magistrale;

    [[nodiscard]] Magistrala* gasesteMagistrala(const std::string& codMagistrala);

public:
    Dispecerat(const std::string& numeCompanie, double bugetInitial, int oraStart = 6);

    void adaugaMagistrala(const Magistrala& magistrala);

    // Pentru retea deja existenta (ex. incarcare din fisier, fara cost din buget)
    void adaugaStatiePeMagistrala(const std::string& codMagistrala, const Statie& statie);
    void adaugaGarnituraPeMagistrala(const std::string& codMagistrala, const GarnituraMetrou& garnitura);

    // Pentru investitii in timpul jocului (verifica si scade costul din bugetLei)
    void construiesteStatiePeMagistrala(const std::string& codMagistrala, const Statie& statie);
    void insereazaStatiePeMagistrala(const std::string& codMagistrala, std::size_t index, const Statie& statie);
    void eliminaStatieDePeMagistrala(const std::string& codMagistrala, const std::string& numeStatie);
    void eliminaStatieDePeMagistralaLaIndex(const std::string& codMagistrala, std::size_t index);
    void cumparaGarnituraPeMagistrala(const std::string& codMagistrala, const GarnituraMetrou& garnitura);

    double avanseazaOra();
    double efectueazaReviziiToateMagistralele();

    [[nodiscard]] double getBugetLei() const;
    [[nodiscard]] int getOraCurenta() const;
    [[nodiscard]] int getZiCurenta() const;

    [[nodiscard]] static Dispecerat incarcaDinFisier(const std::string& caleFisier);

    friend std::ostream& operator<<(std::ostream& os, const Dispecerat& dispecerat);
};

#endif // OOP_DISPECERAT_H
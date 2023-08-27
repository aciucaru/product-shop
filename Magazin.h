#ifndef MAGAZIN_H
#define MAGAZIN_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <exception>

enum TipProdus
{
    FRIGIDER,
    MASINA_SPALAT,
    NEDEFINIT
};

class Produs
{
protected:
    int id = 0;
    std::string nume = "";
    double pret = 0.0;

    Produs(int id, std::string nume, double pret)
    {
        this->id = id;
        this->nume = nume;
        this->pret = pret;
    }

public:
    // getteri, deoarece clasa 'Produs' nu are voie sa fie instantiata, deci nu are voie sa aiba prieteni
    // deci clasa 'Magazin' nu va putea accesa direct nici proprietatile clasei 'Produs'
    int iaId() { return id; }
    std::string iaNume() { return nume; }
    double iaPret() { return pret; }

    /* metoda virtuala adaugata de nevoie, deoarece dynamic_cast<Derivat*>(baza) nu merge daca
    clasa 'Baza' nu are macar o metoda virtuala !!! */
    virtual std::string catreString()
    {
        std::string rezultat = "";
        return rezultat + "id: " + std::to_string(this->id) + ", denumire: " + this->nume 
                + ", pret: " + std::to_string(this->pret);
    }
};

class Frigider : public Produs
{
    friend class Magazin;
protected:
    int volum = 0;
    bool areCongelator = false;

public:
    Frigider(int id, std::string nume, double pret, int volum, bool areCongelator) : Produs(id, nume, pret)
    {
        this->volum = volum;
        this->areCongelator = areCongelator;
    }

    // override
    std::string catreString() override
    {
        return Produs::catreString() + ", tip: frigider" + ", volum: " + std::to_string(this->volum)
            + ", are congelator: " + std::to_string(this->areCongelator);
    }
};

class MasinaDeSpalat : public Produs
{
    friend class Magazin;
protected:
    int rotatiiPeMinut;
    std::string clasaEnergetica;

public:
    MasinaDeSpalat(int id, std::string nume, double pret, int rpm, std::string clasa) : Produs(id, nume, pret)
    {
        this->rotatiiPeMinut = rpm;
        this->clasaEnergetica = clasa;
    }

    // override
    std::string catreString() override
    {
        return Produs::catreString() + ", tip: masina spalat" + ", rpm: " + std::to_string(this->rotatiiPeMinut)
            + ", clasa energetica: " + this->clasaEnergetica;
    }
};

class MagazinPlinException : std::length_error
{
public:
    MagazinPlinException() : length_error("eroare: magazinul este plin")
    { }
};

class ProdusInexistentException : std::runtime_error
{
public:
    ProdusInexistentException() : std::runtime_error("eroare: produsul nu exista in magazin")
    { }
};

class Magazin
{
private:
    std::vector<Produs*> sirProduse;
    int nrMaximProduse = std::numeric_limits<int>::max();

public:
    Magazin(int nrMaximProduse)
    {
        if(nrMaximProduse >= 1)
            this->nrMaximProduse = nrMaximProduse;
    }

    ~Magazin() // destructor
    {
        sirProduse.clear();
    }

    void adaugaProdus(Produs* produs) noexcept(false)
    {
        if(produs != nullptr)
        {
            if(sirProduse.size() + 1 <= nrMaximProduse)
                sirProduse.push_back(produs);
            else
                throw MagazinPlinException();
        }
    }

    void eliminaProdus(int idProdus) // throw(ProdusInexistentException)
    {
        if(idProdus >= 0)
        {
            // mai intai se cauta produsul in magazin, pentru a vedea daca macar exista
            int indexGasit = -1; // indexul la care se va gasi produsul (daca exista); valoarea sa initiala este -1
            int i = 0; // index pentru iterare
            for(Produs* produs : sirProduse)
            {
                if(produs->iaId() == idProdus) // daca s-a gasit produsul dupa id
                {
                    indexGasit = i; // atunci se memoreaza indexul la care s-a gasit
                    break; // si se opreste bucla for, caci nu mai are sens sa continue
                }
                i++;
            }
            // daca s-a gait elementul
            if(indexGasit != -1)
                // atunci se sterge
                sirProduse.erase(std::next(sirProduse.begin(), indexGasit), std::next(sirProduse.begin(), indexGasit + 1));
            else
                // altfel se arunca exceptie
                throw ProdusInexistentException();
        }
    }

    Produs* cautaProdus(int idProdus)
    {
        Produs* produsGasit = nullptr; // la inceput se presupune ca nu se va gasi nimic
        if(idProdus >= 0)
        {
            int indexGasit = -1;
            int i = 0;
            for(Produs* produs : sirProduse)
            {
                if(produs->iaId() == idProdus)
                {
                    indexGasit = i;
                    break;
                }
                i++;
            }
            // daca s-a gasit elementul
            if(indexGasit != -1)
                // atunci se stocheaza
                produsGasit = sirProduse.at(indexGasit);
            else
                // altfel se arunca exceptie
                throw ProdusInexistentException();
        }
        return produsGasit;
    }

    int numaraMasiniDeSpalat()
    {
        int nrMasiniDeSpalat = 0; // numarul total de masini de spalat
        for(Produs* produs : sirProduse)
        {
            // dynamic_cast<> o sa returneze pointer null daca obiectul nu este de tipul specificat
            // acest lucru poate fi folosit intr-o structura 'if' pentru a verifica si numara cate obiecte
            // de tipul MasinaDeSpalat sunt
            if(dynamic_cast<MasinaDeSpalat*>(produs))
                nrMasiniDeSpalat++;
        }
        return nrMasiniDeSpalat;
    }

    void scrieProduse()
    {
        for(Produs* produs : sirProduse)
        {
            std::cout << produs->catreString() << std::endl;
        }
    }

    // overload operator cautare produs (operatorul cauta un produs in magazin, dupa id)
    Produs& operator [] (int id)
    {
        return *cautaProdus(id); // se refoloseste metoda cautaProdus()
    }
};

// overload operator adaugare produs (operatorul permite adaugarea unui produs in magazin)
// operatorul returneaza magazinul in sine, deci poate fi folosit pentru adaugare produse in lant
Magazin& operator << (Magazin& magazin, Produs& produs)
{
    magazin.adaugaProdus(&produs); // se refoloseste metoda adaugaProdus()
    return magazin;
}

// overload operator afisare produs (operatorul afiseaza produsul cu id-ul indicat
// operatorul returneaza magazinul in sine, deci poate fi folosit pentru afisare produse in lant
Magazin& operator >> (Magazin& magazin, int id)
{
    Produs* produs = magazin.cautaProdus(id);
    std::cout << produs->catreString() << std::endl;
    return magazin;
}

#endif // MAGAZIN_H
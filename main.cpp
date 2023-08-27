#include <iostream>
#include <vector>
#include <deque>
#include <string>

#include "utilitar_input.h"
#include "Magazin.h"


int main()
{
    // cat timp aceasta variabila este adevarata, programul continua sa accepte comenzi
    bool continua_rularea = true;
    
    std::cout << "Acest program simuleaza un magazin de produse" << std::endl;
    
    // mai intai se ia numarul maxim de produse; se permit nenumarate incarcari, pana cand numarul este
    // introdus corect
    int nrMaximProduse;
    std::string stringNrMaximProduse;
    bool nrMaximProduseIntrodusGresit = true;
    while(nrMaximProduseIntrodusGresit) // cat timp numarul maxim nu a fost introdus corect
    {
        // se incearca citirea lui
        std::cout << "Va rugam introduceti numarul maxim permis de produse (pentru nelimitat introduceti 0): ";
        getline(std::cin, stringNrMaximProduse);
        if(valideazaStringNumar(stringNrMaximProduse))
        {
            nrMaximProduse = std::stoi(stringNrMaximProduse);
            nrMaximProduseIntrodusGresit = false; // se intrerupe bucla
        }
        else
            std::cout << "eroare: numar maxim de produse invalid, va rugam incercati din nou" << std::endl;
    }

    // std::cin >> nrMaximProduse;
    // dupa ce se ia un numar, trebuie folosit ignore(), altfel urmatorul string nu se poate lua corect
    // std::cin.ignore();

    Magazin* magazin = new Magazin(nrMaximProduse);
    
    std::cout << std::endl;
    std::cout << "Lista de comenzi disponibile:" << std::endl;
    std::cout << "A frigider, id, nume, pret, volum, are_congelator – adauga produs de tip frigider" << std::endl;
    std::cout << "A masina_spalat, id, nume, pret, rpm, clasa energetica – adauga produs de tip masina de spalat" << std::endl;
    std::cout << "E id – elimina produs dupa id" << std::endl;
    std::cout << "C id – cauta produs dupa id" << std::endl;
    std::cout << "N – afiseaza numarul de masini de spalat" << std::endl;
    std::cout << "L – afiseaza informatii despre fiecare produs" << std::endl;
    std::cout << "S - stop" << std::endl;
    
    std::cout << std::endl;
    std::cout << "exemple de utilizare:" << std::endl;
    std::cout << "A frigider, 1, Arctic AD60310M30W, 1.501, 306, da" << std::endl;
    std::cout << "   adauga un frigider cu id 1, nume 'Arctic AD60310M30W', pret 1.501, volum 306L, cu congelator" << std::endl;

    std::cout << "A masina_spalat, 2, Whirlpool TDLR7221BSEUN, 2.199, 1200, A++" << std::endl;
    std::cout << "   adauga o masina de spalat cu id 2, nume 'Whirlpool TDLR7221BSEUN', pret 2.199, 1200rpm, clasa energetica A++" << std::endl;
   
    std::cout << "E 2" << std::endl;
    std::cout << "   elimina produsul cu id 2 (daca acesta exista)" << std::endl;
    
    std::cout << "C 2" << std::endl;
    std::cout << "   cauta produsul cu id 2 (daca acesta exista)" << std::endl;
    
    std::cout << "restul comenzilor nu au argumente" << std::endl;
    
    std::string linie_comanda_curenta = ""; // string ce contine intreaga linie introdusa de utilizator
    std::vector<std::string> sir_token; // sir ce contine linia impartita in componentele ei (comanda + argumente)
    char litera_comanda_curenta; // litera corespunzatoare unei comenzi, se axtrage din primul string al sirului 'sir_token'
    std::vector<std::string> sir_argumente; // sir ce contine doar argumentele unei comenzi, se extrage din 'sir_token'
    
    std::cout << std::endl;
    while(continua_rularea)
    {
        std::cout << "introduceti o comanda: ";
        getline(std::cin, linie_comanda_curenta);
        
        sir_token = desparteLinieComanda(linie_comanda_curenta);
        
        // daca s-a introdus o comanda cu macar un cuvant (un token)
        if(sir_token.size() > 0)
        {
            // primul cuvant al comenzii este primul element al sirului 'sir_token' si se poate lua cu 'front()'
            // daca acel cuvant nu este o comanda, metoda 'extrageComanda()' va returna caracterul null
            litera_comanda_curenta = extrageComanda(sir_token.front());
            
            switch(litera_comanda_curenta)
            {
                case 'A': // adauga produs
                {
                    // se determina ce tip de produs se doreste a fi introdus
                    // pentru comanda 'A', tipul produsului este chiar primul argument, adica al 2-lea
                    // element al sirului de token-uri
                    std::string arg_produs = sir_token.at(1);
                    TipProdus tip_produs = determinaTipProdus(arg_produs);

                    // daca tipul produsului ce se vrea adaugat este un frigider
                    if(tip_produs == FRIGIDER)
                    {
                        // se extrage sirul de argumente din sirul de cuvinte (token-uri)
                        // extragerea se face de la al 3-lea pana la ultimul element
                        // deoarece primul element este comanda in sine, iar al doilea arg
                        // este tipul produsului (pe aceasta ramura 'frigider')
                        sir_argumente = std::vector<std::string>(sir_token.begin() + 2, sir_token.end());
                        
                        // se verifica daca argumentele sunt valide
                        bool argumente_valide = valideazaArgAdaugareFrigider(sir_argumente);
                        
                        if(argumente_valide) // daca argumentele sunt valide
                        {
                            // atunci se iau valorile corepsunzatoare argumentelor
                            int id = std::stoi(sir_argumente.at(0));
                            std::string nume = sir_argumente.at(1);
                            double pret = std::stod(sir_argumente.at(2));
                            int volum = std::stoi(sir_argumente.at(3));
                            bool areCongelator = stringCatreBool(sir_argumente.at(4));
                            
                            // si se construieste un obiect de tip 'Frigider', care apoi se adauga la magazin
                            Frigider* frigiderNou = new Frigider(id, nume, pret, volum, areCongelator);
                            // obiectul se adauga cu operatorul supraincarcat <<
                            *magazin << *frigiderNou;
                            std::cout << "produs adaugat cu succes: " << frigiderNou->catreString() << std::endl;
                        }
                        else
                        {
                            std::cout << "eroare: argumentele pentru comanda 'A' nu sunt valide, va rugam incercati din nou" << std::endl;
                            std::cout << "sugestie: argumentele trebuie separate prin virgula" << std::endl;
                        }
                    }

                                        // daca tipul produsului ce se vrea adaugat este un frigider
                    if(tip_produs == MASINA_SPALAT)
                    {
                        // se extrage sirul de argumente din sirul de cuvinte (token-uri)
                        // extragerea se face de la al 3-lea pana la ultimul element
                        // deoarece primul element este comanda in sine, iar al doilea arg
                        // este tipul produsului (pe aceasta ramura 'frigider')
                        sir_argumente = std::vector<std::string>(sir_token.begin() + 2, sir_token.end());
                        
                        // se verifica daca argumentele sunt valide
                        bool argumente_valide = valideazaArgAdaugareMasinaSpalat(sir_argumente);
                        
                        if(argumente_valide) // daca argumentele sunt valide
                        {
                            // atunci se iau valorile corepsunzatoare argumentelor
                            int id = std::stoi(sir_argumente.at(0));
                            std::string nume = sir_argumente.at(1);
                            double pret = std::stod(sir_argumente.at(2));
                            int rpm = std::stoi(sir_argumente.at(3));
                            std::string clasa_energetica = sir_argumente.at(4);
                            
                            // si se construieste un obiect de tip 'MasinaDeSpalat', care apoi se adauga la magazin
                            MasinaDeSpalat* masinaSpalatNoua = new MasinaDeSpalat(id, nume, pret, rpm, clasa_energetica);
                            // obiectul se adauga cu operatorul supraincarcat <<
                            *magazin << *masinaSpalatNoua;
                            std::cout << "produs adaugat cu succes: " << masinaSpalatNoua->catreString() << std::endl;
                        }
                        else
                        {
                            std::cout << "eroare: argumentele pentru comanda 'A' nu sunt valide, va rugam incercati din nou" << std::endl;
                            std::cout << "sugestie: argumentele trebuie separate prin virgula" << std::endl;
                        }
                    }
                }
                    break;
                case 'E': // elimina produs (se citeste id produs de la tastatura)
                {
                    // daca linia comenzii are 2 cuvinte (token-uri) (adica o comanda si un argument)
                    if(sir_token.size() == 2)
                    {
                        // se extrage sirul de argumente din sirul de cuvinte (token-uri)
                        // extragerea se face de la al 2-lea pana la ultimul element
                        sir_argumente = std::vector<std::string>(sir_token.begin() + 1, sir_token.end());
                        
                        // se verifica daca argumentul pasat pentru id-ul prodsului este corect
                        // ar trebui sa fie un singur argument, iar metoda valideazaArgId() ia in
                        // considerare acest lucru si da eroare daca detecteaza mai multe argumente
                        bool argumente_valide = valideazaArgId(sir_argumente);
                        
                        if(argumente_valide) // daca argumentul este valid (este doar unul singur)
                        {
                            // atunci se iau valoarea corespunzatoare argumentului 'id'
                            int id = std::stoi(sir_argumente.at(0));
                            
                            // si se incearca eliminarea produsului din magazin
                            // metoda 'eliminaProdus()' arunca exceptie daca produsul nu se gaseste in magazin
                            try
                            {
                                magazin->eliminaProdus(id);
                                std::cout << "produs eliminat cu succes" << std::endl;
                            }
                            catch(const ProdusInexistentException& excep)
                            {
                                std::cout << "produsul cu id " << id << " nu s-a sters (nu se gaseste in magazin)" << std::endl;
                            }
                        }
                        else
                            std::cout << "eroare: argumentele pentru comanda 'E' nu sunt valide, va rugam incercati din nou" << std::endl;
                    }
                    else
                    {
                        std::cout << "eroare: argumentele pentru comanda 'E' nu sunt valide, va rugam incercati din nou" << std::endl;
                        std::cout << "sugestie: argumentele trebuie separate prin virgula" << std::endl;
                    }
                }
                    break;
                case 'C': // cauta produs
                {
                    // daca linia comenzii are 2 cuvinte (token-uri) (adica o comanda si un argument)
                    if(sir_token.size() == 2)
                    {
                        // se extrage sirul de argumente din sirul de cuvinte (token-uri)
                        // extragerea se face de la al 2-lea pana la ultimul element
                        sir_argumente = std::vector<std::string>(sir_token.begin() + 1, sir_token.end());
                        
                        // se verifica daca argumentul pasat pentru id-ul produsului este corect
                        // ar trebui sa fie un singur argument, iar metoda valideazaArgId() ia in
                        // considerare acest lucru si da eroare daca detecteaza mai multe argumente
                        bool argumente_valide = valideazaArgId(sir_argumente);
                        
                        if(argumente_valide) // daca argumentul este valid (este doar unul singur)
                        {
                            // atunci se iau valoarea corespunzatoare argumentului 'id'
                            int id = std::stoi(sir_argumente.at(0));
                            
                            try
                            {
                                // pentru cautare se foloseste operatorul supraincarcat []
                                // acest operator cauta dupa 'id' (nu returneaza dupa index)
                                Produs produsCautat = (*magazin)[id];
                                std::cout << "produs gasit: " << produsCautat.catreString() << std::endl;
                            }
                            catch(const ProdusInexistentException& excep)
                            {
                                std::cout << "produsul cu id " << id << " nu s-a gasit in magazin" << std::endl;
                            }
                        }
                        else
                        {
                            std::cout << "eroare: argumentele pentru comanda 'C' nu sunt valide, va rugam incercati din nou" << std::endl;
                            std::cout << "sugestie: comanda 'C' are un singur argument" << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "eroare: argumentele pentru comanda 'C' nu sunt valide, va rugam incercati din nou" << std::endl;
                        std::cout << "sugestie: comanda 'C' are un singur argument" << std::endl;
                    }
                }
                    break;
                case 'N': // numara masinile de spalat
                {
                    // daca linia comenzii are exact un cuvant (adica nu sunt si argumente)
                    if(sir_token.size() == 1)
                        std::cout << "in magazin sunt: " << magazin->numaraMasiniDeSpalat() << " masini de spalat" << std::endl;
                    else
                        std::cout<< "eroare: comanda 'N' nu trebuie sa aiba argumente, va rugam incercati din nou" << std::endl;
                }
                    break;
                case 'L': // scrie produse
                    magazin->scrieProduse();
                    break;
                case 'S': // stop aplicatie
                    continua_rularea = false; // se opreste aplicatie
                    std::cout << "programul s-a oprit" << std::endl;
                    break;
                    
                case '\0':
                default:
                    std::cout << "eroare: linia introdusa nu contine o comanda valida, va rugam incercati din nou" << std::endl;
                    break;
            }
        }
        else
            std::cout << "eroare: nu s-a introdus nici o comanda, va rugam incercati din nou" << std::endl;

    }
    
    return 0;
}

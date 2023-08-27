#ifndef UTILITAR_INPUT_H
#define UTILITAR_INPUT_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Magazin.h"

/* functie utilitara ce elimina spatiile albe numai de la inceputul si de la sfarsitul unui string 
   exemplu de functionare:
   - eliminaSpatiiAlbe("     C++ Programming Language   "): "C++ Programming Language"
   - eliminaSpatiiAlbe("   Bjarne Stroustrup"): "Bjarne Stroustrup"
   - eliminaSpatiiAlbe("C++: Effective Modern C++      "): "C++: Effective Modern C++"
   - eliminaSpatiiAlbe("Paul Laurence"): "Paul Laurence" (fara efect, in acest caz) */
std::string eliminaSpatiiAlbe(std::string& token)
{
    /* aceasta functie functioneaza in felul urmator:
       - se determina indexul la care se termina spatiile albe de la inceputul string-ului
       - se determina indexul la care incep spatiile albe de la sfarsitul stringului
       - intre aceste doua index-uri va fi stringul fara spatii albe de inceput si sfarsit, adica
        string-ul de returnat */
    std::string rezultat;
    
    if(token.length() == 0)
        rezultat = "";
    else
    {
        // se gaseste indexul primei litere ce nu este spatiu " "
        auto index_inceput_rezultat = token.find_first_not_of(" ");
        // se gaseste indexul ultimei litere ce nu este spatiu " "
        auto index_sfarsit_rezultat = token.find_last_not_of(" ");
        
        // daca nu s-a gasit nimic diferit de " ", inseamna ca token-ul are doar spatii albe sau este gol
        if(index_inceput_rezultat == std::string::npos)
            rezultat = ""; // in acest caz se returneaza un string gol
        else
        {
            auto lungime_rezultat = index_sfarsit_rezultat - index_inceput_rezultat + 1;
            // se extrage string-ul necesar
            rezultat = token.substr(index_inceput_rezultat, lungime_rezultat);
        }
    }
    return rezultat;
}

// functie utilitara ce desparte o intreaga linie in token-urile (cuvintele) ei (adica in litera comenzii si argumente)
std::vector<std::string> desparteLinieComanda(std::string& linie_comanda)
{
    /* pentru acest program, o comanda arata astfel: C arg1, arg2, arg3, arg4, ...
     * unde:
     * - C este litera comenzii (A, L, E, I, P)
     * - arg1, arg2, arg3, arg4, ... sunt argumentele comenzii (unele comenzi pot sa nu aiba argumente)
     * aceasta functie desparte intreaga linie de comanda in aceste elemente si le returneaza ca vector
     * */
    
    /* pentru ca trebuie inserate elemente si la inceputul sirului, se va lucra cu un deque, iar apoi
       se va converti deque-ul intr-un vector (vectorul va fi de fapt returnat de aceasta functie) */
    std::deque<std::string> deque_token; // sir cu care se va lucra efectiv
    std::vector<std::string> vector_token; // sir ce va fi returnat
    
    // pasul 1. se separa string-ul liniei de comanda dupa delimitatorul ','
    std::stringstream s_stream(linie_comanda);
    std::string token_curent;
    char delimitator = ',';
    while(std::getline(s_stream, token_curent, delimitator))
    {
        deque_token.push_back(token_curent);
//      vector_token.push_back(std::move(item)); // C++11
    }
    /* dupa separare, de exemplu, comanda 'A 10, C++ Programming Language, Bjarne Stroustrup, 2013' va fi:
            A 10
            Bjarne Stroustrup
            C++ Programming Language
            2013 
       se observa ca litera comenzii (A) este amestecata cu primul argument (adica 10); aceasta problema se
       rezolva in pasul urmator */
    
    // pasul 2. se separa litera comenzii de primul argument
    if(deque_token.size() > 0) // daca sirul nu este gol
    {
        // atunci se ia primul element (care sigur exista)
        std::string primul_token = deque_token.front();
        // se elimina primul element, deoarece el reprezinta de fapt 2 elemente si va fi inlocuit cu cele 2 elemente separate
        deque_token.pop_front();
        
        /* acum ca s-a obtinut primul token inainte de virgula, exista doua posibilitati:
           - comanda este de tip 'fara argumente', deci primul token contine doar un cuvant (comanda in sine)
           - comanda este de tip 'cu argumente', deci primul token contine 2 cuvinte (comanda si un argument) */
        
        /* ca sa se afle daca primul token contine unul sau doua cuvinte, se testeaza existenta spatiilor deoarece,
           daca sunt mai multe cuvinta, garantat exista macar un spatiu */
        if(primul_token.find(' ') != std::string::npos) // daca exista macar un spatiu
        {
            // atunci primul token contine o comanda si un argument
            std::string comanda = primul_token.substr(0, primul_token.find(' '));
            std::string primul_argument = primul_token.substr(primul_token.find(' '), primul_token.length() - 1);
            
//             // se extrage fiecare parte a primului token
//             std::stringstream stream_primul_token(primul_token);
//             stream_primul_token >> comanda; // se extrage primul token(cuvant) in string-ul 'comanda'
//             stream_primul_token >> primul_argument; // se extrage al doilea token in string-ul 'primul_argument'
            
            // se adauga string-urile corecte la inceputul deque-ului de token-uri
            deque_token.push_front(primul_argument);
            deque_token.push_front(comanda);
        }
        else
        {
            // atunci primul token contine doar comanda
            std::string comanda = primul_token.substr(0, primul_token.find(' '));
            
//             std::stringstream stream_primul_token(primul_token);
//             stream_primul_token >> comanda;
            
            // se adauga string-ul comenzii la inceputul deque-ului de token-uri
            deque_token.push_front(comanda); 
        }
        /* acum comanda 'A 10, C++ Programming Language, Bjarne Stroustrup, 2013' ar trebui sa fie despartita asa:
            A
            10
            Bjarne Stroustrup
            C++ Programming Language
            2013 */
    }
    
    // pasul 3. se elimina spatiile albe de la inceputul si sfarsitul fiecarui string, ramase in urma separarii
    // liniei de comanda
    // si, pentru ca este mai comod sa se lucreze cu vectori, deque-ul obtinut se converteste intr-un vector
    // si, in final, se va returna vectorul
    for(std::string token_curent : deque_token) // pentru fiecare string din deque
    {
         vector_token.push_back(eliminaSpatiiAlbe(token_curent));
    }

    return vector_token;
}

// functie ce determina daca un cuvant (de tip string) este cu adevarat o comanda valida
// iar daca este o comanda valida, functia extrage si returneaza comanda sub forma de char, ca sa nu mai
// fie nevoie de inca o functie care sa extraga litara comenzii din cuvant
char extrageComanda(const std::string& token)
{
    
    char litera_comanda = '\0';
    
    // daca token-ul (cuvantul) are exact un caracter, inseamna ca ar putea fi o comanda (comenzile au un singur caracter)
    if(token.length() == 1)
    {
        const int NUMAR_COMENZI = 6;
        const char comenzi_posibile[NUMAR_COMENZI] = {'A', 'E', 'C', 'N', 'L', 'S'};
        
        // se verifica daca token-ul (cuvantul) primit (ce are sigur un caracter) contine una dintre comenzile posibile
        for(int i=0; i<NUMAR_COMENZI; i++)
        {
            if(token.front() == comenzi_posibile[i])
            {
                litera_comanda = token.front();
                break; // daca s-a gasit o asociere, se intrerupe bucla for
            }
        }
    }
    
    return litera_comanda;
}

// functie utilitara ce converteste un string cu valori posibile 'da' sau 'nu' in echivalentul bool
// aceasta functie presupune ca stringul contine numai 'da' sau 'nu', nu face nici o verificare
// verificarea este facuta in functiile ce valideaza argumentele si garanteaza corectitudinea lor
bool stringCatreBool(std::string& text)
{
    if(text == "da")
        return true;
    if(text == "nu")
        return false;
}

// functie ce returneaza tipul produsului (sub forma de enum), pe baza unui string ce ar trebui sa reprezinte
// tipul produsului
TipProdus determinaTipProdus(std::string& string_produs)
{
    TipProdus tip_produs = NEDEFINIT;

    if(string_produs.compare("frigider") == 0)
        tip_produs = FRIGIDER;
    if(string_produs.compare("masina_spalat") == 0)
        tip_produs = MASINA_SPALAT;

    return tip_produs;
}

/******************* functii pentru validarea argumentelor primite de la utilizator **********************/

// functie ce vaideaza argumentele pentru comanda adaugare frigider
bool valideazaArgAdaugareFrigider(std::vector<std::string>& sir_argumente)
{
    // variabila ce stocheaza rezultatul validarii
    // la inceput, se presupune ca argumentele sunt corecte; la orice abatere, rezultatul va deveni fals
    bool rezultat_validare = true;
    
    // variabile ce reprezinta argumentele pasate, sunt variabilele ce vor fi validate intr-un final
    int id;
    std::string string_nume;
    double pret;
    int volum;
    bool areCongelator;
    
    // 1. pentru comanda de adaugare frigider, este nevoie de 5 argumente
    // daca numarul de argumente este cel corect, atunci se pot verifica si argumentele in sine
    if(sir_argumente.size() == 5)
    {
        // 2. se verifica daca argumentul 'id' este numar, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stoi()' ce arunca exceptie daca string-ul nu este un numar
        std::string string_id = sir_argumente.at(0); // string-ul corespunzator arg. 'id' (primul argument)
        try
        {
            int id = std::stoi(string_id);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
            // dar id-ul nu poate fi orice numar intreg, trebuie sa fie pozitiv
            if(id <= 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
        catch(std::exception& excep)
        {
            std::cout << "valideazaArgAdaugareFrigider(): id ivalid" << std::endl;
            rezultat_validare = false;
        }
        
        // 3. se verifica daca argumentul 'nume' este gol
        string_nume = sir_argumente.at(1); // numele autorului este al 2-lea argument
        if(string_nume.empty())
            rezultat_validare = false;
        
        // 4. se verifica daca argumentul 'pret' este un numar double, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stod()' ce arunca exceptie daca string-ul nu este un double
        std::string string_pret = sir_argumente.at(2); // pretul este al 3-lea argument
        try
        {
            pret = std::stod(string_id);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stod()' a returnat un nr. double
            // dar pretul nu poate fi negativ sau zero
            if(pret <= 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un double, deci nu trece de validare
        catch(std::exception& excep)
        {
            std::cout << "valideazaArgAdaugareFrigider(): pret ivalid" << std::endl;
            rezultat_validare = false;
        }
        
        // 5. se verifica daca argumentul 'volum' este numar intreg, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stoi()' ce arunca exceptie daca string-ul nu este un intreg
        std::string string_volum = sir_argumente.at(3); // string-ul coresp. argumentului 'volum' (al 4-lea argument)
        try
        {
            volum = std::stoi(string_volum);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
            // dar volumul frigiderului nu poate fi negativ sau zero
            if(volum <= 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
        catch(std::exception& excep)
        {
            std::cout << "valideazaArgAdaugareFrigider(): volum ivalid" << std::endl;
            rezultat_validare = false;
        }

        // 6. se verifica daca argumentul 'are_congelator' este 'da' sau 'nu' (alte valori nu sunt acceptate)
        std::string string_are_congelator = sir_argumente.at(4); // 'are_congelator' este al 5-lea argument
        // daca 'string_are_congelator' nu este nici 'da' nici 'nu', inseamna ca este o alta valoare, deci neacceptat
        if(string_are_congelator != "da" && string_are_congelator != "nu")
        {
            rezultat_validare = false;
            std::cout << "valideazaArgAdaugareFrigider(): areFrigider ivalid" << std::endl;
        }
    }
    else
    {
        std::cout << "valideazaArgAdaugareFrigider(): numar argumente ivalid" << std::endl;
        rezultat_validare = false;
    }
    
    return rezultat_validare;
}

// functie ce vaideaza argumentele pentru comanda adaugare masina spalat
bool valideazaArgAdaugareMasinaSpalat(std::vector<std::string>& sir_argumente)
{
    // variabila ce stocheaza rezultatul validarii
    // la inceput, se presupune ca argumentele sunt corecte; la orice abatere, rezultatul va deveni fals
    bool rezultat_validare = true;
    
    // variabile ce reprezinta argumentele pasate, sunt variabilele ce vor fi validate intr-un final
    int id;
    std::string string_nume;
    double pret;
    int rotatii_pe_minut;
    std::string clasa_energetica;
    
    // 1. pentru comanda de adaugare masina e spalat, este nevoie de 5 argumente
    // daca numarul de argumente este cel corect, atunci se pot verifica si argumentele in sine
    if(sir_argumente.size() == 5)
    {
        // 2. se verifica daca argumentul 'id' este numar, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stoi()' ce arunca exceptie daca string-ul nu este un numar
        std::string string_id = sir_argumente.at(0); // string-ul corespunzator arg. 'id' (primul argument)
        try
        {
            int id = std::stoi(string_id);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
            // dar id-ul nu poate fi orice numar intreg, trebuie sa fie pozitiv
            if(id <= 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
        catch(std::exception& excep)
        {
            std::cout << "valideazaArgAdaugareMasinaSpalat(): id ivalid" << std::endl;
            rezultat_validare = false;
        }
        
        // 3. se verifica daca argumentul 'nume' este gol
        string_nume = sir_argumente.at(1); // numele autorului este al 2-lea argument
        if(string_nume.empty())
            rezultat_validare = false;
        
        // 4. se verifica daca argumentul 'pret' este un numar double, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stod()' ce arunca exceptie daca string-ul nu este un double
        std::string string_pret = sir_argumente.at(2); // pretul este al 3-lea argument
        try
        {
            pret = std::stod(string_id);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stod()' a returnat un nr. double
            // dar pretul nu poate fi negativ sau zero
            if(pret <= 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un double, deci nu trece de validare
        catch(std::exception& excep)
        {
            std::cout << "valideazaArgAdaugareMasinaSpalat(): pret ivalid" << std::endl;
            rezultat_validare = false;
        }
        
        // 5. se verifica daca argumentul 'string_rotatii_pe_minut' este numar intreg, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stoi()' ce arunca exceptie daca string-ul nu este un intreg
        std::string string_rotatii_pe_minut = sir_argumente.at(3); // string-ul coresp. argumentului 'volum' (al 4-lea argument)
        try
        {
            rotatii_pe_minut = std::stoi(string_rotatii_pe_minut);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
            // dar volumul frigiderului nu poate fi negativ sau zero
            if(rotatii_pe_minut <= 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
        catch(std::exception& excep)
        {
            std::cout << "valideazaArgAdaugareMasinaSpalat(): rpm ivalid" << std::endl;
            rezultat_validare = false;
        }

        // 6. se verifica daca argumentul 'clasa_energetica' este string gol
        std::string string_clasa_energetica = sir_argumente.at(4); // 'clasa_energetica' este al 5-lea argument
        // daca 'clasa_energetica' nu este gol
        if(string_clasa_energetica.empty())
        {
            rezultat_validare = false;
            std::cout << "valideazaArgAdaugareMasinaSpalat(): clasa_energetica ivalid" << std::endl;
        }
    }
    else
    {
        std::cout << "valideazaArgAdaugareMasinaSpalat(): numar argumente ivalid" << std::endl;
        rezultat_validare = false;
    }
    
    return rezultat_validare;
}

bool valideazaArgId(std::vector<std::string>& sir_argumente)
{
    // variabila ce stocheaza rezultatul validarii
    // la inceput, se presupune ca argumentele sunt corecte; la orice abatere, rezultatul va deveni fals
    bool rezultat_validare = true;
    
    // variabile ce reprezinta singurul argument pasat, este folosita pentru validarea finala
    int id;
    
    // 1. pentru comenzile ce au ca argument doar id-ul produsului, este nevoie de un argument
    // se verifica daca sirul de argumente are intr-adevar un singur argument
    if(sir_argumente.size() == 1) // daca numarul de argumente este cel corect, atunci se pot verifica si argumentele in sine
    {
        // 2. se verifica daca argumentul 'id' este numar, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stoi()' ce arunca exceptie daca string-ul nu este un numar
        std::string string_id = sir_argumente.at(0); // string-ul coresp. argumentului 'id' (este primul argument)
        try
        {
            id = std::stoi(string_id);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
            // dar id-ul nu poate fi orice numar intreg, trebuie sa fie pozitiv
            if(id < 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
        catch(std::exception& excep) { rezultat_validare = false; }
    }
    else
        rezultat_validare = false;
    
    return rezultat_validare;
}

bool valideazaStringNumar(std::string& string_numar)
{
    // variabila ce stocheaza rezultatul validarii
    // la inceput, se presupune ca argumentele sunt corecte; la orice abatere, rezultatul va deveni fals
    bool rezultat_validare = true;
    int numar;

    try
    {
        numar = std::stoi(string_numar);
        
        // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
        // dar id-ul nu poate fi orice numar intreg, trebuie sa fie pozitiv
        if(numar < 0)
            rezultat_validare = false; // nu trece de validare daca este negativ
    }
    // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
    catch(std::exception& excep) { rezultat_validare = false; }
    
    return rezultat_validare;
}

#endif // UTILITAR_INPUT_H
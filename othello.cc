#include <iostream>
#include <fstream>

#include "othello.h"
#include "main.h"
#include "stapel.h"

using namespace std;

// open file for writing
ofstream output("./info.txt", std::ofstream::out);

// Setters
void Othellobord::zetParameters() { // Parameters voor othellospel
    while (true) {
        cout
            << "                                 \n\n"
            << "Welkom bij Othello. Kies uit:      \n"
            << "                                   \n"
            << " - Spel spelen               [S]   \n"
            << "                                   \n"
            << " - Mapgrootte veranderen     [M]   \n"
            << " - Spelers kiezen            [P]   \n"
            << " - Aantal iteraties          [I]   \n"
            << " - kleurcharacter veranderen [K]   \n" << endl;

        char keuze = inputChar();
        leegTerminal();

        switch (keuze) {
        case 'M':
        case 'm':
            zetMapgrootte();
            break;
        case 'P':
        case 'p':
            zetSpelers(); // Tegen een robot?
            break;
        case 'K':
        case 'k':
            zetSpelerKleur(); 
            break;
        case 'S':
        case 's':
            return;
        case 'I':
        case 'i':
            zetMaxIteraties();
            break;
        default:
            break;
        }
    }
}

void Othellobord::zetMapgrootte() { // Othellobord grootte
    cout << "Wat wordt de breedte van de map?" << endl;
    lengte = inputGetal();
    cout << "Wat wordt de hoogte van de map?" << endl;
    hoogte = inputGetal();
    if(hoogte != 8 || lengte != 8) optimalisatie = false;
    else optimalisatie = true;
    leegTerminal();
}

void Othellobord::zetSpelers() { // Speel je zelf of speelt een computer
    cout << "Wie speelt er als wit? \n"
         << "[S]peler of [C]omputer?" << endl;
    char input = inputChar();

    if (input == 'C' || input == 'c') isSpeler1 = false;
    else isSpeler1 = true;
                
    cout << "Wie speelt er als zwart? \n"
         << "[S]peler of [C]omputer?" << endl;
    input = inputChar();

    if (input == 'C' || input == 'c') isSpeler2 = false;
    else isSpeler2 = true;
    
    if(!isSpeler1 && !isSpeler2) {computerPlayer = true; zetVervolgpartijen(); }
    leegTerminal();
}

void Othellobord::zetSpelerKleur() { // Zet de karakter van de kleuren
    cout << "Welke kleur is de eerste speler? \n";
    speler1 = inputChar();

    cout << "Welke kleur is de tweede speler? \n";
    speler2 = inputChar();
    leegTerminal();
}

// Zet hoeveel potjes er worden gesimuleerd
void Othellobord::zetVervolgpartijen() { 
    cout << "Hoeveel vervolgpartijen wil je simuleren?" << endl;
    vervolgpartijen = inputGetal();
    leegTerminal();
}

void Othellobord::zetMaxIteraties() { // Zet aantal iteraties
    cout << "Hoeveel iteraties moet het programma doorrekenen?" << endl;
    max_iteraties = inputGetal();
    leegTerminal();
}

// Zet een karakter op een vlak van het bord
void Othellobord::zetKleur(Bordvakje* positie, char kleur) {  
    positie->kleur = kleur; 
}

// Krijg de opgegeven positie van de gebruiker
void Othellobord::krijgPositieSpeler(int& x, int& y) {
    char input;
    bool getalGevonden = false, isGetal = false;
    x = 0;
    y = 0;

    cout << 
        "Geef een positie op door twee losse coordinaten op te geven" 
         << endl;
    printBord();
    
    while (cin.get(input)) { // Zolang er geen enter is
        if ('0' <= input && '9' >= input) { 
            isGetal = true;
            if (!getalGevonden) x = x * 10 + (input - '0');
            else y = y * 10 + (input - '0');
        }
        else isGetal = false; // Als het geen getal is
        if (!getalGevonden && x > 0 && !isGetal) getalGevonden = true;  
        if(getalGevonden && y > 0 && !isGetal) {  // Tweede getal gevonden
            if(x > lengte) x = lengte;
            if(y > hoogte) y = hoogte;
            
            leegTerminal(); // Klaar2
            return;
        }
    }
}

// Initialiseert wat waarden en zet beginpositie klaar
void Othellobord::zetBeginpositie() {   
    Bordvakje *omlaag = ingang;
    Bordvakje *huidig;
    max_iteraties = 6;

    // Alle vakjes moeten leeg zijn:
    while (omlaag != nullptr) {
        huidig = omlaag;
        while (huidig != nullptr) {
            huidig->kleur = ' ';        
            huidig = huidig->buren[2];
        }
        omlaag = omlaag->buren[4];
    }

    // Dan de beginsopstelling
    int x_mid = lengte / 2;
    int y_mid = hoogte / 2;
    zetSteen(krijgVakje(x_mid,     y_mid    ), speler2);
    zetSteen(krijgVakje(x_mid + 1, y_mid    ), speler1);
    zetSteen(krijgVakje(x_mid,     y_mid + 1), speler1);
    zetSteen(krijgVakje(x_mid + 1, y_mid + 1), speler2);
    mogelijkeZetten();
}

void Othellobord::beginAuto() { // Het automatisch afspelen van de simulaties
    int score = 0;
    for (int i = 0; i < vervolgpartijen; i++) {
        zetBeginpositie();
        while(!isAfgelopen(score)) {
            randomZet();
            mogelijkeZetten();
            if(isAfgelopen(score)) break; 
            slimmeZet();
            mogelijkeZetten();
        }
    }
    cout << "Wit won " << score << 
            " van de " << vervolgpartijen << " keer." << endl;
}

void Othellobord::othelloMenu() { // Het game-menu   
    int score;
    while (true) {
        mogelijkeZetten();
        if (isAfgelopen(score)) return;
        if(!isSpeler1) slimmeZet();
        printBord();
        cout
            << "                       \n\n"
            << " [HOOFDMENU]             \n"
            << "                         \n"
            << " - Zet               [Z] \n"
            << " - Random zet        [R] \n"
            << " - Slimme zet        [S] \n"
            << " - Ongedaan maken    [O] \n"
            << " - Aantal eindes     [E] \n"
            << "                         \n"
            << " Of wil je terug?    [Q]" << endl;

        char keuze = inputChar();
        leegTerminal();

        switch (keuze)
        {
        case 'E':
        case 'e':
            eindSituaties();
            break;
        break;
        case 'Q':
        case 'q':
            return;
        case 'R':
        case 'r':
            nieuwBord();
            randomZet();
            break;
        case 'S':
        case 's':
            nieuwBord();
            slimmeZet();
            break;
        case 'Z':
        case 'z':
            nieuwBord();
            mensZet();
            break;
        case 'O':
        case 'o':
            maakOngedaan();
            break;
        default:
            cout << "Geen geldige karakter" << endl;
            leegInput();
            break;
        }
        if(!isSpeler2) slimmeZet();
    }
}

void Othellobord::mensZet() { // Alles wat nodig is voor mens om een zet doen
    Bordvakje* positie;
    int x, y;
    bool richtingZet[8] = {false}; // Welke richting gaat de zet op.
    krijgPositieSpeler(x, y);
    positie = krijgVakje(x, y);

    if(isMogelijk(positie, richtingZet)) {
        voerZetUit(positie, richtingZet);
        draaiSpelers();
    }
    else cout << "Dit is geen mogelijke zet" << endl;
}

void Othellobord::randomZet() { // Alles wat nodig is om een random zet te doen
    Bordvakje* positie;
    int x, y;
    bool richtingZet[8] = {false};
    do {
        x = (randomGetal() % lengte) + 1;
        y = (randomGetal() % hoogte) + 1;
        positie = krijgVakje(x, y);
    } while (!isMogelijk(positie, richtingZet));
    voerZetUit(positie, richtingZet);
    draaiSpelers();
}

void Othellobord::slimmeZet() {// Alles wat nodig is om een slimme zet te doen
    int x = 0, y = 0;
    int iteraties = 0;
    Bordvakje* positie, *temp = ingang;
    bool richtingZet[8] = {false};

    kopieerMap(recursief->huidig);
    ingang = recursief->huidig->ingang;
    recursieveZet(0, x, y, iteraties);
    
    bepaalDiepte(iteraties);

    ingang = temp;
    positie = krijgVakje(x, y);
    isMogelijk(positie, richtingZet);
    voerZetUit(positie, richtingZet);
    draaiSpelers();
}

void Othellobord::maakOngedaan() {
    if(stapel->huidig->vorige != nullptr) draaiSpelers();
    stapel->vorige();
    ingang = stapel->huidig->ingang;
}

void Othellobord::bepaalDiepte(int & iteraties) {
    int stenen1 = 0;
    int stenen2 = 0;
    telStenen(stenen1, stenen2);
    int score = lengte * hoogte - (stenen1 + stenen2);
    if (score > 30) max_iteraties = 3;
    if (score < 30) max_iteraties = 5;
    if (score < 18) max_iteraties = 6;
    if (score < 12) max_iteraties = score;
    output << score << " " << iteraties << endl;
  }

// Berekent alle mogelijke zetten in de huidige opstelling
void Othellobord::mogelijkeZetten() { 
    Bordvakje* positie = nullptr;
    bool temp[8] = {false};
    nrZetten = 0;

    for (int x = 1; x <= lengte; x++) {
        for (int y = 1; y <= hoogte; y++) {
            positie = krijgVakje(x, y);

            if(isMogelijk(positie, temp)) { zetKleur(positie, '.'); nrZetten++;}
            else if ( positie->kleur != speler1 && 
                      positie->kleur != speler2 ) { zetKleur(positie, ' '); }
        }
    }
}

void Othellobord::eindSituaties() { // Eindsituaties berekenen
    kopieerMap(recursief->huidig);
    Bordvakje *temp = ingang;
    ingang = recursief->huidig->ingang;
    cout << "Er zijn "
         << aantalEindsituaties()
         << " eindsituaties \n"
         << endl;
    ingang = temp;
}

// Berekend alle zetten door voor het aantal eindsituaties
int Othellobord::aantalEindsituaties() {
    int aantalZetten = 0;
    bool richting[8] = {false}; // Nodig voor de zet functie
    Bordvakje *huidig   = nullptr;
    Bordvakje *kHuidig  = nullptr;
    
    if(recursief->huidig->volgende == nullptr) recursief->voegToe();

    for (int y = 1; y <= hoogte; y++) {
        for (int x = 1; x <= lengte; x++) {   
            for (int i = 0; i < 8; i++) { richting[i] = false;}
            huidig = krijgVakje(x, y);
            if(isMogelijk(huidig, richting)) {
                recursief->volgende(); // Eerst naar volgende bord
                kopieerMap(recursief->huidig); // kopieer vorige map naar huidig 
                ingang = recursief->huidig->ingang; // verplaats ingang 
                kHuidig = krijgVakje(x, y); // Krijg pointer naar huidig vakje
                voerZetUit(kHuidig, richting);  // Doe de zet
                draaiSpelers();                // Na zet draaien van spelers
                aantalZetten += aantalEindsituaties(); // recursie
                recursief->vorige(); // We bereiden volgende zet voor
                ingang = recursief->huidig->ingang;
            }
        }   
    }
    draaiSpelers();
    if(!aantalZetten) return 1;
    return aantalZetten;
}

// Kopieert de pointermap in een nieuwe pointermap
void Othellobord::kopieerMap(Bord *kopie) { 
    Bordvakje *omlaag = ingang;
    Bordvakje *huidig;
    Bordvakje *kOmlaag = kopie->ingang;
    Bordvakje *kHuidig;
    while (omlaag != nullptr) {
        huidig = omlaag;
        kHuidig = kOmlaag;
        while (huidig != nullptr) {
            kHuidig->kleur = huidig->kleur;
            huidig = huidig->buren[2];
            kHuidig = kHuidig->buren[2];
        }
        omlaag = omlaag->buren[4];
        kOmlaag = kOmlaag->buren[4];
    }
}

bool Othellobord::isAfgelopen(int& score) {// Checkt of er nog zetten gedaan kunnen worden
    if(!nrZetten) {
        if(computerPlayer && (wieWint() == 'W')) score++;
        if(!computerPlayer) {
            printBord();
            cout << "Het spel stopt" << endl;
            char winnende = wieWint();
            if(winnende == '0')   cout << "Het is gelijkspel" << endl;
            else cout << "De winnende partij is " << winnende << endl;
        }
        //cout << "Je wordt teruggestuurd naar het hoofdmenu" << endl;
        return true;
    }
    return false;
}

char Othellobord::wieWint() { // Checkt welke speler meer stenen heeft
    int stenen1 = 0, stenen2 = 0;
    telStenen(stenen1, stenen2);
    if(stenen1 > stenen2) return speler1;
    if(stenen2 > stenen1) return speler2;
    return '.';
}

void Othellobord::telStenen(int& p1, int& p2) { // Telt het aantal stenen per speler
    Bordvakje *omlaag = ingang;
    Bordvakje *huidig;

    // Probeer deze functie niet te begrijpen
    while (omlaag != nullptr) {
        huidig = omlaag;
        while (huidig != nullptr) {
            if(huidig->kleur == speler1) p1++;
            else if (huidig->kleur == speler2) p2++;
            huidig = huidig->buren[2];
        }
        omlaag = omlaag->buren[4];
    }
}

// Checkt voor de mogelijkheid van een zet
bool Othellobord::isMogelijk(Bordvakje* begin, bool* richtingZet) {
    // Deze functie is belangrijk, dus hier is wat uitleg
    int stappen; // We houden de stappen bij
    char kleur = ' ';  // Bijhouden kleur van vak, moet geinitialiseerd worden
    bool mogelijk = false; // Is de zet mogelijk

    Bordvakje *huidig = nullptr;
    
    // Het vakje moet vrij zijn:
    if (begin->kleur == speler1 || begin->kleur == speler2) return false;
    // We kijken in alle richtingen of een zet mogelijk is
    for (int richting = 0; richting < 8; richting++) {
        stappen = 0;
        huidig = begin;

        // Zolang er stenen van de vijanden zijn, gaan we verder
        do {
            if (huidig->buren[richting] == nullptr) break;
            kleur = huidig->buren[richting]->kleur;
            stappen++;
            huidig = huidig->buren[richting];
        } while (kleur == speler2);
        
        // Als het nog steeds niet mogelijk is, sla deze richting over
        if (kleur == speler1 && stappen > 1) {
            richtingZet[richting] = true;
            mogelijk = true;
        }
    }
    return mogelijk;
}

// Zet steen op positie
void Othellobord::zetSteen(Bordvakje* positie, char spelerKleur) {   
    positie->kleur = spelerKleur;
}

// Veranderd alle stenen in een lijn naar de steen van de speler
void Othellobord::voerZetUit(Bordvakje* positie, bool* richtingzet) {
    char kleur;
    Bordvakje* huidig = positie;
    // We weten al dat de zet mogelijk is, dus we hoeven minder checks te doen
    for (int i = 0; i < 8; i++) {
        if(richtingzet[i]) {
            huidig = positie;
            do {
                huidig->kleur = speler1;
                kleur = huidig->buren[i]->kleur;
                huidig = huidig->buren[i];
            } while (kleur == speler2);
        }
    }
}

// Functie voor het recursief zetten doen tot zekere diepte
int Othellobord::recursieveZet(int diepte, int& x_best, int& y_best, int& itt) {
    // Dit is een belangrijke functie
    bool richting[8] = {false}; // Nodig voor de zet functie
    int score = 0, maximum = -1000;
    bool nietVoorbij    = false;
    Bordvakje *huidig   = nullptr;
    Bordvakje *k_huidig = nullptr;
    
    diepte++; // De diepte wordt bijgehouden
    if(recursief->huidig->volgende == nullptr) recursief->voegToe();
    for (int y = 1; y <= hoogte; y++) {
        for (int x = 1; x <= lengte; x++) {   
            huidig = krijgVakje(x, y);

            if(isMogelijk(huidig, richting)) {
                nietVoorbij = true;
                recursief->volgende();
                kopieerMap(recursief->huidig);
                ingang = recursief->huidig->ingang;
                k_huidig = krijgVakje(x, y);
                score = telStenenEnZet(k_huidig, richting);
                itt++;

                // EVALUATIE
                if(diepte < max_iteraties) {
                    draaiSpelers();
                    score -= recursieveZet(diepte, x_best, y_best, itt);
                }
                
                if (score > maximum) {
                    maximum = score;
                    if (diepte == 1) {
                        x_best = x;
                        y_best = y;
                    }
                }
                recursief->vorige();
                ingang = recursief->huidig->ingang;
                for (int i = 0; i < 8; i++) { richting[i] = false; }
            }
        }
    }
    if(!nietVoorbij) {
       if(wieWint() == speler1) maximum =  100;
       else                     maximum = -100;
    }
    if(diepte != 1) draaiSpelers();
    return maximum;
}

// Hetzelfde als voerZetUit, maar telt ook hoeveelheid stenen
int Othellobord::telStenenEnZet(Bordvakje* positie, bool* richtingzet) {   
    char kleur;
    int n_stenen = 0;
    Bordvakje* huidig = positie;
    
    for (int i = 0; i < 8; i++) {
        if(richtingzet[i]) {
            huidig = positie;
            do {
                n_stenen++;
                huidig->kleur = speler1;
                kleur = huidig->buren[i]->kleur;
                huidig = huidig->buren[i];
            } while (kleur == speler2);
        }
    }
    return n_stenen;
}

void Othellobord::printBord() { // Afdrukken
    Bordvakje *omlaag = ingang;
    Bordvakje *huidig;

    int x = 0, y = 0; 
    cout << "Huidige speler: " << speler1 << endl;
    while (omlaag != nullptr) {
        huidig = omlaag;
        y++;
        
        while (huidig != nullptr) {
            if(x == 0) {
                for (int i = 1; i <= min(lengte, 8); i++) cout << i << "   ";
                if(lengte > 8) cout << "9  ";
                for (int i = 0; i < lengte - 9; i++)  cout << i + 10 << "  ";
                cout << '\n';
                x++;
            }
            cout << (huidig->kleur) << " | ";
            huidig = huidig->buren[2];
        }
        cout << y << " \n";
        omlaag = omlaag->buren[4];
    }
}

void Othellobord::draaiSpelers() { // Draai spelers
    char temp = speler1;
    speler1 = speler2;
    speler2 = temp;
}

// returnt de pointer die hoort bij de positie
Bordvakje* Othellobord::krijgVakje(int x, int y) { 
    Bordvakje *huidige = ingang;
    for (int i = 1; i < x; i++) {
        if (huidige->buren[2] != nullptr) huidige = huidige->buren[2];
    }
    for (int j = 1; j < y; j++) {
        if (huidige->buren[4] != nullptr) huidige = huidige->buren[4];
    }
    return huidige;
}

void Othellobord::nieuwBord() { // Maak een nieuw bord en plaats ingang
    if(stapel->huidig->volgende == nullptr) stapel->voegToe();
    stapel->volgende();
    kopieerMap(stapel->huidig);
    ingang = stapel->huidig->ingang;
}

// Constructors en destructors
Othellobord::Othellobord() {
    // Gewoon normale constructor
    speler1 = 'Z';
    speler2 = 'W';

    isSpeler1      = true;
    isSpeler2      = true;
    computerPlayer = false;
    spelIsOver     = false;
    optimalisatie  = true;

    lengte          = 8;
    hoogte          = 8;
    vervolgpartijen = 0;
    max_iteraties   = 4; 
    nrZetten        = 0;

    zetParameters();                       // Juiste parameters

    stapel    = new Stapel(lengte, hoogte);// stapel voor ongedaan
    recursief = new Stapel(lengte, hoogte);// recursief voor recursieve functies
    ingang    = stapel->huidig->ingang;    // ingang wordt stapelIngang

    zetBeginpositie(); // Zet wat waarden op nul en maak beginpositie
    if (!isSpeler1 && !isSpeler2) { beginAuto(); return; } // Simulaties
    othelloMenu(); // Het gamemenu
}

Othellobord::~Othellobord() {
    delete stapel;
    delete recursief;
    recursief = nullptr;
    stapel = nullptr;
    output.close();
}

Bordvakje::Bordvakje() {
    kleur = ' ';
    for (int i = 0; i < 8; i++) { buren[i] = nullptr; }
}

Bordvakje::~Bordvakje() {
    for (int i = 0; i < 8; i++) {buren[i] = nullptr; }
}
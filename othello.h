class Stapel;

class Bord;

class Bordvakje 
{
public:
    char kleur;
    Bordvakje* buren[8];
    Bordvakje();
    ~Bordvakje();
};

class Othellobord
{
private:
    char speler1; // Speler 1 is altijd aan de beurt
    char speler2;

    Bordvakje* ingang;
    Stapel* stapel;
    Stapel* recursief;

    bool isSpeler1, isSpeler2;
    bool computerPlayer;
    bool spelIsOver;
    bool optimalisatie;

    int lengte, hoogte;
    int vervolgpartijen;
    int max_iteraties;
    int nrZetten;

public:
    Othellobord();
    Othellobord(char, char, int, int);
    ~Othellobord();
    
    void maakMap();
    void mensZet();
    void printBord();
    void beginAuto();
    void nieuwBord();
    void randomZet();
    void slimmeZet();
    void zetSpelers();
    void othelloMenu();
    
    void maakOngedaan();
    void draaiSpelers();
    void zetMapgrootte();
    void eindSituaties();
    void zetParameters();
    void zetSpelerKleur();
    void zetBeginpositie();
    void mogelijkeZetten();
    void zetMaxIteraties();
    void kopieerMap(Bord *);
    void bepaalDiepte(int &);
    void zetVervolgpartijen();
    void telStenen(int &, int &);
    void zetKleur(Bordvakje *, char);
    void zetSteen(Bordvakje *, char);
    void voerZetUit(Bordvakje *, bool *);
    void krijgPositieSpeler(int &, int &);

    bool isAfgelopen(int &);
    bool isMogelijk(Bordvakje *, bool *);

    int aantalEindsituaties();
    int meesteStenen(int &, int &);
    int recursieveZet(int, int &, int &, int &);
    int telStenenEnZet(Bordvakje *, bool *);

    char wieWint();
    
    Bordvakje *krijgVakje(int, int);
};
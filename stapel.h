class Bordvakje;

class Othellobord;

class Bord 
{
private:

public:
    Bord *vorige;
    Bord *volgende;
    Bordvakje *ingang;
    
    Bord();
    ~Bord();
};

class Stapel
{
private:
    
public:
    Bord *huidig;
    int hoogte;
    int lengte;

    Stapel(int, int);
    ~Stapel();

    void vorige();
    void maakMap();
    void voegToe();
    void volgende();
    void ongedaan();
    void deleteMap();
    void verwijder();
};
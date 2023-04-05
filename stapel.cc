#include <iostream>

#include "stapel.h"
#include "othello.h"
#include "main.h"

using namespace std;

Stapel::Stapel(int Lengte, int Hoogte) {
    huidig = new Bord;
    hoogte = Hoogte;
    lengte = Lengte;
    maakMap(); // Updated ook ingang
}

Stapel::~Stapel() {
    Bord* temp;
    // Ga naar laatste element in stapel
    while(huidig->volgende != nullptr) { huidig = huidig->volgende; }
    while (huidig != nullptr) {
        deleteMap();
        temp = huidig->vorige;
        delete huidig;
        huidig = temp;
    } 
}

void Stapel::maakMap() {
    Bordvakje *rijIngang = nullptr;
    Bordvakje *boven = nullptr;
    Bordvakje *rechtsboven = nullptr;
    Bordvakje *linksboven = nullptr;
    Bordvakje *vorige = nullptr;

    // Er zijn vijf verschillende richtingen die geknoopt moeten worden.
    for (int i = 0; i < hoogte; i++) {
        for (int j = 0; j < lengte; j++) {

            Bordvakje *temp = new Bordvakje; // Nieuw vakje wordt aangemaakt

            if (!i && !j) huidig->ingang = temp;
            if (!j) rijIngang = temp;
            if (vorige != nullptr) { // Als er een vakje voor zit, dan linken
                temp->buren[6] = vorige;
                vorige->buren[2] = temp;
            }
            if (linksboven != nullptr) { // Als er een vakje linksboven zit...
                temp->buren[7] = linksboven;
                linksboven->buren[3] = temp;
            }
            if (boven != nullptr) { // Vakje boven
                temp->buren[0] = boven;
                boven->buren[4] = temp;
            }
            if (rechtsboven != nullptr) { // Vakje rechtsboven
                temp->buren[1] = rechtsboven;
                rechtsboven->buren[5] = temp;
            }

            // Na het linken, oppassen dat je niet je hoofd stoot tegen nullptrs
            vorige = temp;
            if (boven != nullptr) linksboven = boven;
            if (rechtsboven != nullptr) {
                boven = rechtsboven;
                if (rechtsboven->buren[2] != nullptr) {
                    rechtsboven = rechtsboven->buren[2];
                }
                else rechtsboven = nullptr;
            }
        } // Na iedere rij:
        vorige = nullptr;
        linksboven = nullptr;
        boven = rijIngang;
        rechtsboven = boven->buren[2];
    }
}

void Stapel::deleteMap() {
    Bordvakje *ingang = huidig->ingang;
    while (ingang != nullptr) {
        Bordvakje *onder = ingang->buren[4];
        while (ingang->buren[2] != nullptr) {
            Bordvakje *volgend = ingang->buren[2];
            delete ingang;
            ingang = volgend;
        }
        delete ingang;
        ingang = onder;
    }
}

void Stapel::voegToe() {
    Bord* temp = huidig;
    Bord* nieuw = new Bord;
    nieuw->vorige = huidig;
    huidig->volgende = nieuw;
    huidig = nieuw;
    maakMap();
    huidig = temp;
}

void Stapel::volgende() {
    if(huidig->volgende != nullptr) huidig = huidig->volgende;
}

void Stapel::vorige() {
    if(huidig->vorige != nullptr) huidig = huidig->vorige;
}

void Stapel::verwijder() {
    if(huidig != nullptr) {
        deleteMap();
        Bord* temp = huidig->vorige;
        delete huidig;
        huidig = temp;
    }
}

Bord::Bord() {
    vorige = nullptr;
    volgende = nullptr;
    ingang = nullptr;
}

Bord::~Bord() {
    vorige = nullptr;
    volgende = nullptr;
    ingang = nullptr;
}
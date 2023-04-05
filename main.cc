/*
    Gemaakt door: Vitto van Leeuwen () en Jan Luijk (s3705625)
    Compiler: g++ (GCC) 12.2.1
    Beschrijving: Dit is een programma waarmee othello gespeeld
    kan worden. Een aantal mogelijkheden zijn:
    - zetten ongedaan maken
    - een 'slimme' zet maken
    - een 'random' zet maken
    - computersimulaties
    - mapgrootte aanpassen

    De meeste van deze functies worden mogelijk gemaakt
    door linked-list datastructuren, om zo dynamisch geheugen te
    alloceren. 
*/

#include <iostream>
#include <stdio.h>
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h> // Voor cycles CPU
#endif

#include "main.h"
#include "othello.h"

using namespace std;

// Input van een user
char inputChar() {
    char input;
    do {
        cout << "Voer een geldige letter op: ";
        cin >> input;
    } while ((input < 'a' || input > 'z') && 
             (input < 'A' || input > 'Z') && 
             (input < '0' || input > '9'));
    return input;
}

int inputGetal() {
    char input;
    int num = 0;
    bool isGetal = false;

    while (cin.get(input)) {
        if (input >= '0' && input <= '9') {
            num = num * 10 + (input - '0');
            isGetal = true;
        }
        else if (isGetal) {
            break;
        }
    }
    return num;
}

// Wiskundige functies
int genereerHash(int x) { // Shuffelt een getal
    // Allemaal willekeurige bit-operaties
    x += (x << 10);
    x ^= (x >> 6);
    x += (x << 3);
    x ^= (x >> 11);
    x += (x << 15);

    return x;
}

// returnt random d.m.v aantal clockcycles
int randomGetal() { return genereerHash(__rdtsc()); }

// Code
void leegTerminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear"); // For Linux and macOS
#endif
}

void leegInput() {
    char c;
    while (cin.get(c) && c != '\n') {}
}

void infoblock() {
    cout << "Welkom!  \nDit programma is gemaakt door Vitto van Leeuwen (studee"
         << "ert wiskunde in Leiden) en Jan Luijk (studeert wiskunde en natuurk"
         << "unde, ook in Leiden).\n\nDit is het spel Othello. Het heeft een aa"
         << "ntal handige functies, hier zijn een aantal opgesomt:\n- Ongedaan "
         << "maken van een zet.\n- Een willekeurige zet spelen.\n- Een ietswat "
         << "intelligente zet spelen.\n- Tegen een computer spelen.\n- Computer"
         << "s tegen elkaar laten spelen (simuleren)."
         << ""
         << ""
         << ""
         << ""
         << ""
         << ""
         << ""
         << ""
         << ""
         << ""
         << "" << endl;

}

void speelOthello() {
    Othellobord* othellobord = new Othellobord();
    delete othellobord;
}

int mandelbrotPrinten() {
    int width = 60;
    int height = 20;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double real = (x - width / 2.0) * 4.0 / width;
            double imag = (y - height / 2.0) * 4.0 / width;
            double zReal = 0.0;
            double zImag = 0.0;

            int i = 0;
            while (i < 80 && zReal * zReal + zImag * zImag < 4.0) {
                double temp = zReal * zReal - zImag * zImag + real;// algoritme
                zImag = 2.0 * zReal * zImag + imag;
                zReal = temp;
                i++;
            }
            // als 80 dan spatie, anders karakter op basis van iteraties
            cout << (i == 80 ? ' ' : ".,-~:;=!*#$@"[i % 12]);
        }
        cout << '\n';
    }
    return 0;
}

void menu() {
    char keuze;

    while(true) {
        cout 
        << "\n\n[MENU]                \n\n"
        << "Kies uit:                   \n"
        << " \n"
        << " - Othello spelen [O]       \n"
        << " - Mandelbrot genereren [M] \n"
        << " \n"
        << " Of wil je stoppen? Druk op [Q]" << endl;

        keuze = inputChar();
        leegTerminal();
        switch(keuze) 
        {
        case 'O':
        case 'o':
            speelOthello();
            break;
        case 'M':
        case 'm':
            mandelbrotPrinten();
            break;
        case 'Q':
        case 'q':
            return;
        default:
            break;
        }
    }
    
    return;
}

int main() {

// create an output file stream and open the file
    
    leegTerminal();
    infoblock();
    menu();
    cout << "Tot ziens!" << endl;

    return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ERRORMEMORY -2  // deff konstante za gresku pri alokaciji memorije
#define MAX 100         // deff raspon za slucajno generirane vrijednosti
#define MIN 10
#define MAXPRIORITET 5          // deff raspon prioriteta reda
#define MINPRIORITET 1
#define GRESKAMAIN -1

// Struktura cvor, u kojoj se nalazi element, pokazivač na sljedeći element i prioritet (samo za red)
struct cvor {
    int element;             // Vrijednost čvora
    struct cvor* next;       // Pokazivač na sljedeći čvor u vezanoj listi
    int prioritetcvora;      // Prioritet čvora (koristi se samo za red)
};

typedef struct cvor* Position;  // Deklaracija tipa Position kao pokazivača na strukturu cvor

// Funkcije
int PushStog(Position, int, int);  // Dodavanje elementa u cirkularni stog
int Pop(Position);                  // Uklanjanje elementa sa stoga ili reda
int Print(Position);                // Ispis elemenata u stogu ili u redu
int getRandom();                    // Generiranje slučajnog broja u zadanom rasponu
int randomPriority();               // Generiranje slučajnog prioriteta
int PushRed(Position, int, int);    // Dodavanje elementa u red s prioritetom
int deletAll(Position);             // Brisanje svih elemenata liste

int main()
{
    struct cvor headStog;  // Glava za cirkularni stog
    headStog.next = NULL;   // Inicijaliziramo pokazivač na NULL         

    struct cvor headRed;   // Glava za red s prioritetom
    headRed.next = NULL;   // Inicijaliziramo pokazivač na NULL

    int brojelemenatastog = 0;
    int rezultatoperacije = 0;
    int i = 0;
    int korisnikovizbor = 0;

    do
    {
        printf("Unesi sto zelis raditi:\n");
        printf("1. Push (dodaj element) u cirkularni stog\n");
        printf("2. Pop (ukloni element) sa stoga\n");
        printf("3. Ispis elemenata u stogu\n");
        printf("4. Ispis elemenata u redu\n");
        printf("5. Push (dodaj element) u red\n");
        printf("6. Pop (ukloni element) iz reda\n");

        printf("Unesi broj: \n");
        scanf(" %d", &korisnikovizbor);  // Tražimo od korisnika unos opcije
        if (korisnikovizbor < 1 || korisnikovizbor > 6)
        {
            printf("Error, netočan unos\n"); // Provjera ispravnosti unosa opcije
            return GRESKAMAIN;
        }
        else
        {
            switch (korisnikovizbor)
            {
            case 1:
                printf("Unesi koliko elemenata želiš unijeti u stog: \n");
                scanf("%d", &brojelemenatastog);
                for (i = 0; i < brojelemenatastog; i++)
                {
                    rezultatoperacije = PushStog(&headStog, getRandom(), brojelemenatastog); // Dodaj slučajno generirane elemente u stog
                }
                Print(headStog.next);  // Ispis sadržaja stoga
                if (rezultatoperacije == -2)
                {
                    printf("Greška.\n");
                    return GRESKAMAIN;
                }
                else
                {
                    printf("Sve je proslo u redu.\n");
                }
                break;
            case 2:
                rezultatoperacije = Pop(&headStog); // Uklanjanje elementa sa stoga
                break;
            case 3:
                printf("Stog je:\n");   // Ispis stoga
                Print(headStog.next);
                break;
            case 4:
                printf("Red je:\n");   // Ispis reda
                Print(headRed.next);
                break;
            case 5:
                srand((unsigned int)time(NULL));  // Dodavanje elementa u red s nasumičnim brojem i prioritetom
                rezultatoperacije = PushRed(&headRed, getRandom(), randomPriority());
                if (rezultatoperacije == -2)
                {
                    printf("Greška.\n");
                    return GRESKAMAIN;
                }
                else
                {
                    printf("Sve je prošlo u redu.\n");
                }
                break;
            case 6:
                rezultatoperacije = Pop(&headRed);  // Uklanjanje elementa sa stoga
                break;
            }
        }
        printf("Izlaz iz programa: -1, za nastaviti: 0\n"); // Provjera želi li korisnik nastaviti
        scanf(" %d", &korisnikovizbor);

    } while (korisnikovizbor != -1);  // Petlja se ponavlja dok korisnik ne unese -1

    deletAll(&headStog); // Brisanje svih elemenata stoga i reda
    deletAll(&headRed);

    return 0;
}

// Funkcija za dodavanje elementa u stog
int PushStog(Position P, int value, int brojelemenatastog)
{
    Position temp;

    static int brojac = 0;   // Brojač elemenata na stogu

    if (brojac >= brojelemenatastog)  // Provjera je li stog pun
    {
        printf("Stog je pun\n");
        return ERRORMEMORY;
    }
    else // Ako stog nije pun
    {
        temp = (Position)malloc(sizeof(struct cvor)); // Alociraj prostor za novi element
        if (temp == NULL)
        {
            printf("Greška, memorija nije uspješno alocirana");
            return ERRORMEMORY;
        }

        temp->element = value; //dodajemo vrijednost u novi cvor (temp), value sadrzi vrijednost koju zelimo dodati u stog
        temp->next = P->next;  //postavljamo pokazivac next novog cvora temp da pokazuje na onaj cvor na koji trenutno pokazuje p next (pokazuje na cvor koji je prethodno bio prvi na stofu)
        P->next = temp;   //p->next sad pokazuje na novi cvor 

        brojac++;  // Dodavanje elementa na vrh stoga
    }
    return 0;
}

// Funkcija za uklanjanje elementa sa stoga ili reda
int Pop(Position P)
{
    Position temp = NULL;
    if (P->next != NULL)
    {
        temp = P->next;
        P->next = temp->next;
        printf("Izbrisana vrijednost je %d\n", temp->element);
        free(temp);  // Uklanjanje prvog elementa i oslobađanje memorije
    }
    return 0;
}

// Funkcija za ispis elemenata liste
int Print(Position P)  //ispisuje sve elemente vezane liste koristenjem pokazivaca p
{
    if (P == NULL)  //provjera jel lista prazna (kad == NULL to znaci da lista nema elemenata)
    {
        printf("Prazna lista\n");
        return 0;
    }
    while (P != NULL)  //petlja prolazi kroz sve clanove liste dok god p nije NULL (p je pokazivac na trenutni cvor u listi)
    {
        printf(" %d %d\n", P->element, P->prioritetcvora);  // Ispis vrijednosti i prioriteta za svaki red
        P = P->next;
    }
    printf("\n");
    return 0;
}

// Funkcija za dodavanje elementa u red s prioritetom
int PushRed(Position P, int value, int prioritetcvora)
{
    Position temp = NULL;  // Deklaracija pokazivača temp koji će se koristiti za stvaranje novog čvora

    temp = (Position)malloc(sizeof(struct cvor)); // Alokacija memorije za novi čvor
    if (temp == NULL)  // Provjera je li alokacija uspjela
    {
        printf("Memorija nije alocirana!!!");
        return ERRORMEMORY;
    }

    temp->element = value;  // Postavljanje vrijednosti za novi čvor
    temp->prioritetcvora = prioritetcvora;  // Postavljanje prioriteta
    temp->next = NULL;

    if (P->next == NULL)  // Ako je red prazan
    {
        P->next = temp;   // Dodavanje prvog elementa u red
    }
    else
    {
        // Umetanje elementa na odgovarajuće mjesto prema prioritetu
        while (P->next != NULL && P->next->prioritetcvora >= temp->prioritetcvora)
        {
            P = P->next;
        }
        temp->next = P->next;
        P->next = temp;
    }

    return 0;
}

// Funkcija za generiranje slučajnog broja u zadanom rasponu
int getRandom()
{
    return rand() % (MAX - MIN + 1) + MIN;
}

// Funkcija za generiranje slučajnog prioriteta
int randomPriority()
{
    return rand() % (MAXPRIORITET - MINPRIORITET + 1) + MINPRIORITET;
}

// Funkcija za brisanje svih elemenata povezane liste
int deletAll(Position P)
{
    Position temp = NULL;
    while (P->next != NULL)  // Petlja se izvodi sve do kraja liste
    {
        temp = P->next;
        P->next = temp->next;  // Preskoči cvor i preusmjeri pokazivač
        free(temp);  // Oslobađanje memorije
    }
    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>    // Standardna biblioteka za ulaz/izlaz
#include <stdlib.h>   // Biblioteka za dinamičku alokaciju memorije
#include <time.h>     // Biblioteka za rad s vremenom (potrebno za generiranje slučajnih brojeva)
#include <string.h>   // Biblioteka za manipulaciju stringovima
#define ERRORMEMORY -2  // Definiranje makroa za kod greške memorijske alokacije
#define MAX 100        // Gornja granica za slučajni broj
#define MIN 10         // Donja granica za slučajni broj

// Definicija strukture čvora za vezanu listu
struct node;
typedef struct node* Position;  // Alias za pokazivač na čvor
struct node {
    int element;    // Vrijednost koju čvor sadrži
    Position next;  // Pokazivač na sljedeći čvor
};

// Deklaracije funkcija
int PushStack(Position, int);   // Funkcija za dodavanje elementa na stog
int PushQueue(Position, int);   // Funkcija za dodavanje elementa u red
int Pop(Position);              // Funkcija za uklanjanje elementa
int getRandom();                // Funkcija za generiranje slučajnog broja
int Print(Position);            // Funkcija za ispis elemenata
int deletAll(Position);         // Funkcija za brisanje svih elemenata

int main() {
    struct node headStack;      // Glava stoga
    struct node headQueue;      // Glava reda
    int resualt = 0, choice = 0; // Rezultat funkcije i izbor korisnika
    headQueue.next = NULL;      // Inicijalizacija reda kao praznog
    headStack.next = NULL;      // Inicijalizacija stoga kao praznog

    // Izbornik opcija za korisnika
    printf("Enter what you want to do:\n1.Push on the stack\n2.Pop from the stack\n3.Print stack\n4.Print Queue\n5.Push on the queue\n6.Pop from the queue\n");
    do {
        printf("Unesi 1,2,3,4,5 ili 6\n");
        scanf(" %d", &choice);  // Unos korisničkog izbora

        if (choice < 1 || choice > 6) {  // Provjera valjanosti unosa
            printf(" Netocan unos!!!\n");
        }
        else {
            // Obrada korisničkog izbora
            switch (choice) {
            case 1:  // Dodavanje elementa na stog
                resualt = PushStack(&headStack, getRandom());
                if (resualt == -2) {
                    printf(" Greska.\n");
                }
                else {
                    printf(" Sve je proslo u redu.\n");
                }
                break;
            case 2:  // Uklanjanje elementa sa stoga
                resualt = Pop(&headStack);
                break;
            case 3:  // Ispis stoga
                printf("Stack is:");
                Print(headStack.next);
                break;
            case 4:  // Ispis reda
                printf("Queue is:");
                Print(headQueue.next);
                break;
            case 5:  // Dodavanje elementa u red
                resualt = PushQueue(&headQueue, getRandom());
                if (resualt == -2) {
                    printf(" Greska.\n");
                }
                else {
                    printf(" Sve je proslo u redu.\n");
                }
                break;
            case 6:  // Uklanjanje elementa iz reda
                resualt = Pop(&headQueue);
                break;
            }
        }
        // Nastavak ili prekid petlje prema korisničkom izboru
        printf("If you want to stop press -1, if you want to continue press 0!\n");
        scanf(" %d", &choice);
    } while (choice != -1);  // Petlja radi dok korisnik ne unese -1

    // Brisanje svih elemenata na kraju programa
    deletAll(&headStack);
    deletAll(&headQueue);
    return 0;  // Završetak programa
}

// Funkcija za generiranje slučajnog broja u opsegu [MIN, MAX]
int getRandom() {
    int value = 0;
    value = rand() % (MAX - MIN + 1) + MIN;  // Generiranje slučajnog broja
    return value;
}

// Funkcija za dodavanje elementa na stog
int PushStack(Position P, int value) {
    Position temp;
    temp = (Position)malloc(sizeof(struct node));  // Alokacija memorije za novi čvor
    if (temp == NULL) {
        printf("Memory is not allocated!!!");
        return ERRORMEMORY;
    }
    temp->element = value;  // Postavljanje vrijednosti čvora
    temp->next = P->next;   // Povezivanje čvora s ostatkom liste
    P->next = temp;         // Ažuriranje glave liste
    return 0;
}

// Funkcija za uklanjanje elementa sa stoga ili reda
int Pop(Position P) {    //brise prvi cvor iz povezane liste (onog odma poslije zaglavlja)
    Position temp = NULL;  //privremeno drzi adresu cvora koji ce biti obrisan (inicijaliziran je na NULL jer mu jos nie dodijeljena memorija)
    if (P->next != NULL) {  //provjeravamo postoji li cvor za brisanje
        temp = P->next;          // temp postavljamo na prvi cvor koji dolazi iza zaglavlja tj na prvi pravi cvor u listi ako postoji (pokazujemo na cvor koji zelimo izbrisati)
        P->next = temp->next;    // Preskakanje čvora
        printf("Deleted value is %d\n", temp->element);  // Ispis obrisane vrijednosti
        free(temp);              // Oslobađanje memorije
    }
    return 0;
}

// Funkcija za ispis elemenata
int Print(Position P) {
    if (P == NULL) {
        printf("Prazna lista\n");  // Provjera je li lista prazna
        return 0;
    }
    while (P != NULL) {
        printf(" %d\t", P->element);  // Ispis vrijednosti
        P = P->next;                 // Prelazak na sljedeći čvor
    }
    printf("\n");
    return 0;
}

// Funkcija za dodavanje elementa u red
int PushQueue(Position P, int value) {
    static Position last = NULL;  // Pokazivač na zadnji element
    Position temp = NULL;

    if (last == NULL) {
        last = P;  // Inicijalizacija reda
    }
    temp = (Position)malloc(sizeof(struct node));  // Alokacija memorije
    if (temp == NULL) {
        printf("Memory is not allocated!!!");
        return ERRORMEMORY;
    }
    temp->element = value;   // Postavlja se vrijednost elementa novog cvora temp na value (value je neki podatak koji zelimo pohraniti u novi cvor)
    temp->next = last->next; // Povezivanje na kraj reda
    last->next = temp;       // Ažuriranje kraja reda, temp sad postaje novi posljednji cvor u listi
    last = last->next;       // Pokazivac uvijek pokazuje na posljednji cvor u listi nakon sto dodamo novi cvor
    return 0;
}

// Funkcija za brisanje svih elemenata iz liste
int deletAll(Position P) {
    Position temp = NULL;   //deklariramo temp kao pokazivac na cvor (sluzi za privremeno drzanje pokazivaca na cvor koji cemo obrisati), inicijaliziran je na NULL jer mu nie dodijeljena nikakva memorija
    while (P->next != NULL) { //petlja, ide kroz sve cvorove u listi (p je zaglavlje a p->next je orvi pravi element tj pokazivac na sljedeci prvi stvarni cvor)
        temp = P->next;  // postavljamo temp na p->next tj na cvor koji ce bit obrisan
        P->next = temp->next;  // Preskacemo cvor temp (koji cemo kasnije oslobodit) tkd p->next postavimo na temp->next (povezujemo trenutni cvor sa cvorom koji dolazi nakon p)
        free(temp);  // Oslobađanje memorije koju je zauzimao cvor temp (sprjecavamo curenje memorije)
    }
    return 0;
}

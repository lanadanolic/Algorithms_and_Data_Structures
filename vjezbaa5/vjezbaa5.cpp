/*Za dvije sortirane liste L1 i L2(mogu se pročitati iz datoteke ili unijeti ručno, bitno je samo da su
    sortirane), napisati program koji stvara novu vezanu listu tako da računa :
a) L1∪L2,
b) L1∩ L2.
Liste osim pokazivača na slijedeću strukturu imaju i jedan cjelobrojni element, po kojem su
sortirane.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <iostream>
#define ERRORFILE -1;
#define ERRORMEMORY -2;

struct node    //definira strukturu node, predstavlja cvor liste u povezanom popisu 
{
    int element;   //cjelobrojni podatak koji cvor sadrzi
    Position next;  //pokazivac na sljedeci cvor u listi (omogucuje povezanost izmedu cvorova, tkd se svaki cvor povezuje s narednim cvorom u popisu)
};

struct node;        //deklarira se tip struct node
typedef struct node* Position; //pokazivac na strukturu node

//ZAKLJUCAK !!!! ZA FUNKCIJE STALNO ZOVEMO FUNKCIJU POSITION KOJU SMO DEKLARIRALI I INICIJALIZIRALI KAO NEXT U STRUKTURI, A ANKON TOGA POSTAVILI POKAZIVAC NA NJU ISPOD STRUKTURE
int readfromfile(Position P); //za citanje podataka iz datoteke
int printlist(Position P);    //ispis elemenata povezane liste pocevsi od cvora na poziciji p
int deleteAll(Position P);    //oslobada svu memoriju koju zauzima povezana lista pocevsi od cvora na poziciji p
int presjek(Position P1, Position P2, Position Presjek);   //izracunava presjek 2 povezanih listi i sprema rezultat u listu presjek 
int unija(Position P1, Position P2, Position Unija);   //izracunava uniju 2 povezanih listi p1 i p2 (elementi se dodaju samo 1 bez dupliciranja)

int main()
{
    struct node head1;     //postavlja 4 razlicita zaglavlja (head) za 4 razlicite liste
    struct node head2;     //kreirali smo pocetne cvorove
    struct node headpresjek;
    struct node headunija;

    int pozivanjeglavnihfunkcija = 0;

    head1.next = NULL;   //inicijalizacija next pokazivaca na NULL, uvijek se mora inicijalizirati
    head2.next = NULL;   //kako bi oznacio da su liste prazne (ima samo zaglavlje)
    headpresjek.next = NULL;
    headunija.next = NULL;

    pozivanjeglavnihfunkcija = readfromfile(&head1);   //poziva se fja za citanje podataka iz datoteke i pohranu njih u povezanu listu

    if (pozivanjeglavnihfunkcija == -1)
    {
        printf("error, file se ne moze otvoriti \n");
        return 0;
    }
    else if (pozivanjeglavnihfunkcija == -2)
    {
        printf("error, memorija nije alocirana \n");
        deleteAll(&head1);
        return 0;
    }
    else
    {
        printf("sve je dobro proslo \n");
    }

    printf("prikaz liste 1: \n");
    printlist(head1.next);  //poziva fju za ispisivanje liste pocevsi od prvog stvarnog elementa (head1.next pokazuje na prvi element liste)

    pozivanjeglavnihfunkcija = readfromfile(&head2);

    if (pozivanjeglavnihfunkcija == -1)
    {
        printf("error, lista se nazalost ne moze pokrenuti \n");
        return -1;
    }
    else if (pozivanjeglavnihfunkcija == -2)
    {
        printf("error, memorija nije alocirana \n");
        deleteAll(&head1);  //ako nije uspjesno alocirana memorija za 1 od lista, program mora izbrisati em tu em sve prethodno alocirane memorije
        deleteAll(&head2);
        return 0;
    }
    else
    {
        printf("sve je dobro proslo \n");
    }

    printf("prikaz liste 2: \n");
    printlist(head2.next);


    return 0;
}

int readfromfile(Position P)
{
    FILE* fp = NULL;
    char imedatoteke[30];

    printf("unesite ime datoteke: \n");
    scanf("%s", imedatoteke);

    fp = fopen(imedatoteke, "r");

    if (fp == NULL)
    {
        printf("error, file se nije u mogucnosti otvoriti \n");
        return -1;
    }

    return 0;
}


int printlist(Position P)
{
    if (P == NULL)
    {
        printf("prazna lista \n");
    }
    while (P != NULL)  //prolazi kroz svaki cvor u listi ispisujuci elemente i prelazeci na sljedeci cvor pomocu p=p->next
    {
        printf("%d", P->element); //ispis elementa
        if (P->next != NULL)
        {
            printf("->"); //dodajemo strelicu izmedu brojeva, osim nakon posljednjeg
        }

        P = P->next; //prelazak na sljedeci cvor
    }

    return 0;
}

int deleteAll(Position P)
{
    Position temp = NULL;

    while (P != NULL)  //ili ide p->next? //oslobada se memorija za prvi cvor tkd ne preskacemo pocetni cvor
    {
        temp = P;
        P = P->next; //premjestanje p na sljedeci cvor

        free(temp);  //oslobadanje memorije trenutnog cvora
    }

    return 0;
}


int presjek(Position p1, Position p2, Position presjek)
{
    if (p1 == NULL)  //provjeavamo postoji li neka lista koja je prazna od te 2
    {
        printf("Lista p1 ne postoji \n");
        return -1;
    }
    if (p2 == NULL)
    {
        printf("Lista p2 ne postoji \n");
        return -1;
    }

    Position temp1 = p1->next;  // Preskačemo prvi element (head)
    Position temp2 = p2->next;  // Preskačemo prvi element (head)

    while (temp1 != NULL)
    {
        temp2 = p2->next;  // Ponovno postavljamo temp2 na početak p2 liste
        while (temp2 != NULL)
        {
            // Ako su elementi isti, dodajemo ih u listu presjek
            if (temp1->element == temp2->element)
            {
                // Dodavanje zajedničkog elementa u presjek
                Position newNode = (Position)malloc(sizeof(struct node));
                if (newNode == NULL)
                {
                    printf("Nema dovoljno memorije\n");
                    return -2;
                }
                newNode->element = temp1->element;
                newNode->next = NULL;

                // Dodajemo novi čvor na kraj liste presjek
                Position temp = presjek;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
            temp2 = temp2->next;  // Idemo na sljedeći element u p2
        }
        temp1 = temp1->next;  // Idemo na sljedeći element u p1
    }

    return 0;
}


/*
int unija(Position p1, Position p2, Position unija)
{
    Position g = NULL;

    if (p1 == NULL)
    {
        printf("lisza 1 ne postoji \n");
        return -1;
    }

    if (p2 == NULL)
    {
        printf("lista p2 ne postoji \n");
        return -1;
    }

    while (p1 != NULL && p2 != NULL)
    {
        g = (Position)malloc(sizeof(struct node));
        if (g == NULL) {
            printf("Allocation failed!!!\n");
            return -1;
        }
        if (p1 == NULL)
        {
            g->element = p2->element;
            p2 = p2->next;
        }
        else if (p2 == NULL)
        {
            g->element = p1->element;
            p1 = p1->next;
        }
        else
        {
            if (p1->element > p2->element)
            {
                g->element = p2->element;
                p2 = p2->next;
            }
            else if (p1->element < p2->element)
            {
                g->element = p1->element;
                p1 = p1->next;
            }
            else
            {
                g->element = p1->element;
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        g->next = unija->next;
        unija->next = g;
        unija = unija->next;
    }

    return 0;
}*/
int unija(Position p1, Position p2, Position unija)
{
    if (p1 == NULL)
    {
        printf("Lista p1 ne postoji \n");
        return -1;
    }

    if (p2 == NULL)
    {
        printf("Lista p2 ne postoji \n");
        return -1;
    }

    Position temp = unija;  // Pokazivač na početak liste unija (head)

    while (p1 != NULL || p2 != NULL)  // Tražimo dokle god postoji neki element u jednoj od lista
    {
        Position g = (Position)malloc(sizeof(struct node));
        if (g == NULL) {
            printf("Alokacija memorije nije uspjela!\n");
            return -1;
        }

        if (p1 == NULL) // Ako je lista p1 prazna, preostali elementi su samo iz p2
        {
            g->element = p2->element;
            p2 = p2->next;
        }
        else if (p2 == NULL) // Ako je lista p2 prazna, preostali elementi su samo iz p1
        {
            g->element = p1->element;
            p1 = p1->next;
        }
        else // Ako obje liste imaju elemente, uspoređujemo ih
        {
            if (p1->element < p2->element) // Ako je element iz p1 manji
            {
                g->element = p1->element;
                p1 = p1->next;
            }
            else if (p1->element > p2->element) // Ako je element iz p2 manji
            {
                g->element = p2->element; //postavljamo g element na vrijednost trenutnog elementa iz liste p2
                p2 = p2->next; //premjestamo pokazivac p2 na sljedei cvor u listi p2
            }
            else // Ako su elementi isti, uzimamo samo jedan primjerak
            {
                g->element = p1->element; //postavljamo vrijednost elementa novog cvora g na vrijednost trenutnog elementa iz liste p1
                p1 = p1->next; //premjestamo pokazivac p1 na sljedeci cvor u listi p1
                p2 = p2->next; //
            }
        }

        g->next = NULL;  // Novi čvor uvijek treba imati NULL kao sljedeći
        temp->next = g;  // Dodajemo novi čvor na kraj liste unija
        temp = temp->next;  // Pomakni temp na zadnji dodani čvor
    }

    return 0;
}

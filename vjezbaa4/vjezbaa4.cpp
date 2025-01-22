/*Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct
{
    int* koeficjenti;          //niz koeficjenata polinoma (npr za 3x na 2, koef je 3)
    int* eksponenti;           //niz eksponenata polinoma (npr za 3x na 2, eks je 2)
    int brojclanovapolinoma;   //cijeli broj koji pohranjuje broj clanova polinoma
                               //koristi se za iteraciju kroz nizove koeficjenti i eksponenti (koef (5,3,-7,2), eksponenti (3,2,1,0), brojclanovapolinoma (4))
}Polinomi;

Polinomi* kreirajpolinom();   //dinamicki alocira i inicijalizira novi polinom, fja vraca pokazivac na strukturu tipa polinomi (memorija za strukturu se dinamicki alocira unutar fje, a pokazivac na tu memoriju se vraca)
void dodajclanpolinoma(Polinomi* poly, int koeficjenti, int eksponenti); //koristi se za dodavanje clana polinoma u strukturu polinomi, fja ne vraca nikakvu vrijednost
                                                                         //Polinomi* poly je pokazivac na strukturu polinomi u koju dodajemo clan
                                                                         //int koeficjenti je vrijednost koeficjenta novog clana polinoma
                                                                         //int eksponent je vrijednost eksponenta novog clana polinoma
                                                                         //ima zadatak dodati novi clan polinoma u postojecu strukturu
Polinomi* citajpolinomeizdatoteke(const char* filename); //cita podatke o polinomima iz datoteke i vrati pokazivac na strukturu tipa polinomi
Polinomi* zbrojpolinoma(Polinomi* poly1, Polinomi* poly2); //zbraja 2 polinoma, fja vraca pokazivac na strukturu polinomi
                                                           //prima 2 argumenta, oba su pokazivaci na strukturu polinomi
                                                           //poly1 je pokazivac na prvi polinom
                                                           //poly2 je pokazivac na drugi polinom
                                                           //ulazni polinomi nece biti promijenjeni vec ce se stvoriti novi polinom kao rezultat njihovog zbroja
Polinomi* umnozakpolinoma(Polinomi* poly1, Polinomi* poly2);
void ispispolinoma(Polinomi* product); //ne vraca vrijednost, prima 1 argument tj pokazivac na strukturu polinomi
                                       //product je pokazivac koji pokazuje na polinom koji treba ispisati

int main()
{
    char filename[100];  //za pohranu imena datoteke koju korisnik unosi

    printf("unesite ime datoteke: (npr TextFile1.txt)");
    scanf("%s", filename);

    if (strcmp(filename, "TextFile1.txt") != 0)  //provjera postoji li datoteka
    {
        printf("Datoteka '%s' ne postoji. \n", filename);
        return 1;  //izlaz iz programa
    }

    //pozivaju se 2 iste funkcije, koje citaju podatke o polinomima iz datoteke. obe fje pokusavaju ucitati polinome iz iste datoteke (filename)
    //varijable poly1 i poly2 su pokazivaci na strukture tipa polinomi koji ce sadrzavati podatke o polinomu (koeficjente eksponente i br clanova polinoma)
    Polinomi* poly1 = citajpolinomeizdatoteke(filename);
    Polinomi* poly2 = citajpolinomeizdatoteke(filename);

    if (poly1 == NULL || poly2 == NULL)  //provjera uspjesnosti ucitavanja polinoma
    {
        printf("greska prilikom ucitavanja polinoma iz filea \n");
        return 1;
    }


    //zbrajanje polinoma
    Polinomi* suma = zbrojpolinoma(poly1, poly2); //rezultat zbrajanja vraca se kao pokazivac na novu strukturu polinomi
                                                  //rezultat zbrajanja sprema se u varijablu suma koja je pokazivac na strukturu tipa polinomi
    printf("zbroj polinoma: \n");                 // ako polinomima jednaki eksponenti -> koeficjenti se zbrajaju
    ispispolinoma(suma);                          //fja ispisuje rezultat zbrajanja

    Polinomi* produkt = umnozakpolinoma(poly1, poly2);
    printf("produkt polinoma: \n");               //svaki clan prvog polinoma mnozi za svakim clanom drugog polinoma
    ispispolinoma(produkt);


    //oslobadanje memorije koja je prethodno dinamicki alocirana
    free(poly1->koeficjenti);
    free(poly1->eksponenti);
    free(poly1);
    free(poly2->koeficjenti);
    free(poly2->eksponenti);
    free(poly2);
    free(suma->koeficjenti);
    free(suma->eksponenti);
    free(suma);
    free(produkt->koeficjenti);
    free(produkt->eksponenti);
    free(produkt);

    //ne mozemo samo freeat produkt i sumu, jer oni sadrze i unutarnje pokazivace eksponent i koeficjen
    //kad oslobodimo memoriju za sumu, oslobada se memorija rezerv samo za tu strukturu, ali memorija
    //koju pokazivaci unutar te strukture koriste nece bit oslobodena
    //memorija se mora brisati da ne dode do njenog iscrpljivanja (jer je ogranicen resurs), moze doci do pada programa ili nemogucnosti alokacije nove memorije
    return 0;
}

//alocira i inicijalizira novu strukturu tipa Polinomi i vraca pokazivac na nju
Polinomi* kreirajpolinom()
{
    Polinomi* poly = (Polinomi*)malloc(sizeof(Polinomi)); //poly je pokazivac koji pokazuje na pocetak novododijeljene memorije

    poly->koeficjenti = NULL; //inicijalizira pokazivac koeficjenti unutar strukture na NULL (ne postoji dinamicki alocirno polje za koef)
    poly->eksponenti = NULL; //memorija za eksponente nije rezervirana
    poly->brojclanovapolinoma = 0; //polinom trenutno nema nijedan clan

    return poly; //vraca pokazivac poly, pokazuje na inicijaliz al praznu strukturu Polinomi
}


//dodaje novi clan polinoma tako sto povecava broj clanova a zatim koristi realloc za prilagodbu 
//velicine izova koeficjenata i eksponenata
void dodajclanpolinoma(Polinomi* poly, int koeficjent, int eksponent) //dodaje novi clan u postojeci polinom
{
    poly->brojclanovapolinoma++; //povecaje broj clanova polinoma za 1, to azurira strukturu tkd zna da sad ima jos 1 clan
    poly->koeficjenti = (int*)realloc(poly->koeficjenti, poly->brojclanovapolinoma * sizeof(int)); //realloc ponovno alocira memoriju za koeficjente, prilagodavajuci velicinu tkd moze smjestiti jos 1 dodatni clan
    poly->eksponenti = (int*)realloc(poly->eksponenti, poly->brojclanovapolinoma * sizeof(int));   //prilagodava se velicina memorije za polje eksponenti
    poly->koeficjenti[poly->brojclanovapolinoma - 1] = koeficjent; //nakon prosirivanja polja, novi koeficjent se dodaje na posljednje mjesto u polju koeficjenti
    poly->eksponenti[poly->brojclanovapolinoma - 1] = eksponent;

    //1.povecava br clanova polinoma
    //2.ponovno alocira memoriju za polja eksponenti i koef
    //3.dodaje novi koef i eksponent na odgov pozicije u poljima
}

//zbraja 2 polinoma tako sto dodaje sve clanove prvog polinoma i zatim sve clanove
//drugog polinoma u rezultat
Polinomi* zbrojpolinoma(Polinomi* poly1, Polinomi* poly2)
{
    Polinomi* result = kreirajpolinom();  //fja kreira novi prazni polinom result pomocu kreirajpolinom()
                                          //sadrzava zbroj clanova iz poly1 i poly2

    // Dodavanje članova prvog polinoma
    for (int i = 0; i < poly1->brojclanovapolinoma; i++) {
        dodajclanpolinoma(result, poly1->koeficjenti[i], poly1->eksponenti[i]);
    }

    // Dodavanje članova drugog polinoma
    for (int i = 0; i < poly2->brojclanovapolinoma; i++) {
        dodajclanpolinoma(result, poly2->koeficjenti[i], poly2->eksponenti[i]);
    }

    return result;
}


//mnozi 2 polinoma tako sto uzima svaki clan iz prvog polinoma i mnozi ga sa svakim clanom 
//iz drugog polinoma
//novi koeficjent -> umnozak koeficjenata
//novi eksponent -> zbroj eksponenata
//rezultat se dodaje u novi polinom
Polinomi* umnozakpolinoma(Polinomi* poly1, Polinomi* poly2)
{
    Polinomi* result = kreirajpolinom();  //kreira novi prazni polinom result koji ce sadrzavat produkt polinoma

    for (int i = 0; i < poly1->brojclanovapolinoma; i++)  //prolazi kroz sve clanove poly1
    {
        for (int j = 0; j < poly2->brojclanovapolinoma; j++) //za svaki clan iz poly1, prolazi kroz sve clanove polinoma poly2
        {
            int new_coefficient = poly1->koeficjenti[i] * poly2->koeficjenti[j]; //koeficjenti se mnoze
            int new_exponent = poly1->eksponenti[i] + poly2->eksponenti[j];      //eksponenti se zbrajaju
            dodajclanpolinoma(result, new_coefficient, new_exponent);  //rezultat se dodaje u polinom result pozivom fje dodajclanpolinoma
        }
    }
    return result;
}

//ispisuje polinom u obliku stringa
void ispispolinoma(Polinomi* poly)
{
    for (int i = 0; i < poly->brojclanovapolinoma; i++) //prolazak kroz sve clanove polinoma
    {
        if (i > 0 && poly->koeficjenti[i] >= 0)
        {
            printf(" + "); //dodavanje znaka + za pozitivne koeficjente, neg clanovi automatski dobiju znak - isprid koeficjenta, prvi clan nema nepotrebni znak + isprid sebe
        }
        printf("%dx^%d", poly->koeficjenti[i], poly->eksponenti[i]); //npr 5x^2 ili -3x^2
    }
    printf("\n");
}

//cita koeficjente i eksponente iz dat redom i dodaje ih u polinom koristecu dodajclanpolinoma
Polinomi* citajpolinomeizfilea(const char* filename)
{
    Polinomi* poly = kreirajpolinom(); //poziva se fja kreiraj polinom kako bi se stvorio novi prazni polinom
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Greška prilikom otvaranja datoteke.\n");
        return NULL;
    }
    int coefficient, exponent;
    while (fscanf(file, "%d %d", &coefficient, &exponent) == 2) { //cita 2 uzastopna cijela broja iz datoteke
        dodajclanpolinoma(poly, coefficient, exponent);           //ako fscanf vraca vrijednost 2, znaci da su oba broja uspjesno procitana (pa se clan s tim koef i eksp dodaje u polinom
    }                                                             //petlja se prekida ako fscanf vrati vrijednost koja nije 2 npr kraj datoteke ili neispravan format, petlja se prekida
    fclose(file); //oslobadanje resursa, os dodjeljuje odredene resurse npr memoriju za rad s tom dat, ako ju ne zatvorimo, ti resursi ostaju zauzeti
    return poly;
}
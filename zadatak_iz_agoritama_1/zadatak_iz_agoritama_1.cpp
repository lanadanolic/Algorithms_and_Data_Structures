#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>              
#include <stdlib.h>             
#include <string.h>             
#include <time.h>               

#define MAX_PEOPLE 200          // max br osoba u listi
#define MAX_NAME_LENGTH 50      
#define ID_MIN 50               // min vrijed generiranog ID-a
#define ID_MAX 75               // max vrijed generiranog ID-a

typedef struct
{
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    int id;

} Person;

void sortByLastName(Person people[], int count);  //za sortiranje prema prez
void sortByDescendingID(Person people[], int count);  //za sortir prema ID-ju u silaznom redosl
int generateUniqueID();                            //za generir ID-a
void readFromFile(Person people[], int* count, const char* filename);  //za citanje iz datot
void printList(Person people[], int count);        //za ispis liste s ID-evima
void printlist(Person people[], int count);        //za ispis liste bez ID-eva
void moveToStart(Person people[], int* count, int move_id);  // za premjestanje osobe na pocetak
void deleteBelowThreshold(Person people[], int* count, int threshold);  //za brisanje osoba ispod praga
void deleteInReverseOrder(Person people[], int* count);  //za brisanje u obrnutom redoslijedu

int main()
{
    Person people[MAX_PEOPLE];  //inicij niza osoba
    int count = 0;              // brojac za br osoba u nizu

    srand((unsigned)time(NULL));  // generira sluc br

    readFromFile(people, &count, "osobe.txt");  // cita podatke o osobama iz datot

    sortByLastName(people, count);  // sortira osobe prema prez

    printf("Zadatak 1: Ispis sortirane liste\n\n");  
    printf("Sortirana lista (prezime pa ime):\n");       
    printlist(people, count);                        // ispis sortir liste bez ID-eva

    for (int i = 0; i < count; i++) {  // Petlja kroz sve osobe.
        people[i].id = generateUniqueID();  // dodj jedinstveni ID svakoj osobi
    }

    printf("\nZadatak 2: Generiranje identifikatora (50-75):\n\n");  
    printf("\nLista s jedinstvenim ID-evima:\n");                     
    printList(people, count);                                         //ispis liste s ID-evima

    printf("\nZadatak 5: Unos identifikatora i premjestanje tog elementa na pocetak liste\n\n");  
    int move_id;                            // varij za ID koji treba premjestiti
    printf("\nUnesite ID osobe za premjestanje na pocetak: ");  
    scanf("%d", &move_id);                 
    moveToStart(people, &count, move_id);  // premjesta osobu s tim ID-om na pocetak.
    printf("\nLista nakon premjestanja osobe na pocetak:\n");  // Ispis liste nakon premjestanja.
    printList(people, count);             // Ispis azurirane liste

    printf("\nZadatak 3: Brisanje iz liste svih osoba s vrijednoscu identifikatora manje od unesenog:\n\n");  
    int threshold;                          // prag za brisanje
    printf("\nUnesite prag za ID: ");      
    scanf("%d", &threshold);                
    deleteBelowThreshold(people, &count, threshold);  // brise osobe s ID-om manjim od praga
    printf("\nLista nakon brisanja:\n");   
    printList(people, count);              

    printf("\nZadatak 4: Brisanje svih ostalih elemenata (prvo najveci identifikatori prema najmanjem)\n");  
    deleteInReverseOrder(people, &count);  // brise sve osobe redom prema ID-ju

    return 0;  // Zavrsava program.
}

void sortByLastName(Person people[], int count) 
{
    for (int i = 0; i < count - 1; i++) {  //za prolazak kroz sve elem
        for (int j = 0; j < count - i - 1; j++)  //uspored susjed elem
        {  // za uspored susjednih elemenata
            if (strcmp(people[j].last_name, people[j + 1].last_name) > 0 ||
                (strcmp(people[j].last_name, people[j + 1].last_name) == 0 &&
                    strcmp(people[j].first_name, people[j + 1].first_name) > 0)) 
            {
                Person temp = people[j];
                people[j] = people[j + 1];
                people[j + 1] = temp;
            }
        }
    }
}

void sortByDescendingID(Person people[], int count) 
{
    for (int i = 0; i < count - 1; i++) //prolazi kroz sve elem niza ozim zadnjeg
    {
        for (int j = 0; j < count - i - 1; j++) //usporeduje susjed elem, count-i-1 je zato sta svaki put kad prolazi vanjska petlja kroz i najveci elem nie potribno uspored
        {
            if (people[j].id < people[j + 1].id) {  // Ako je ID 1. elem manji od sljed, zamijeni ih
                Person temp = people[j];
                people[j] = people[j + 1];
                people[j + 1] = temp;
            }
        }
    }
}

int generateUniqueID() 
{
    return rand() % (ID_MAX - ID_MIN + 1) + ID_MIN;
}

void readFromFile(Person people[], int* count, const char* filename) 
{
    FILE* file = fopen(filename, "r");
    if (!file) 
    {
        printf("Ne mogu otvoriti datoteku.\n");
        exit(1);
    }

    while (fscanf(file, "%s %s", people[*count].first_name, people[*count].last_name) != EOF) //cita 2 stringa istovrem
    {
        (*count)++; //svaki put kad se procita uspjeno neki redak osobe, count se povec za 1
        if (*count >= MAX_PEOPLE) { //ako count dosegne max br ljudi
            printf("Dosegnut max broj osoba.\n");
            break;
        }
    }
    fclose(file);
}

void printList(Person people[], int count) 
{
    for (int i = 0; i < count; i++) {
        printf(" %s, %s, ID: %d \n", people[i].last_name, people[i].first_name, people[i].id);
    }
}

void printlist(Person people[], int count) 
{
    for (int i = 0; i < count; i++) 
    {
        printf(" %s, %s \n", people[i].last_name, people[i].first_name);
    }
}

void moveToStart(Person people[], int* count, int move_id) 
{
    
        int index = 0;  // za umetanje osoba s odgovarajućim ID-om na poc, ona je 0 jer je zelimo umetnit na poc
        for (int i = 0; i < *count; i++) //prolazi kroz sve osobe u nizu people[i]
        {
            if (people[i].id == move_id)  //ako je id osobe jednak move:id ulazi u unutarnju logiku
            {
                Person temp = people[i]; //pohrani osobu u privrem varijab
                for (int j = i; j > index; j--)  // pomak sve elem između poc i trenutne pozicije unatrag
                {
                    people[j] = people[j - 1];
                }
                people[index] = temp;  //postavi pronadenu osobu na poc
                index++;  // povecaj indeks za umetanje sljed osobe
            }
        }

       
        if (index == 0) 
        {
            printf("Nijedna osoba s ID-om %d nije pronađena.\n", move_id);
        }
    
}

void deleteBelowThreshold(Person people[], int* count, int threshold) 
{
    int new_count = 0; //br osoba koje ostaju nakon filtrir u listi
    for (int i = 0; i < *count; i++) //prolazi kroz sve osobe
    {
        if (people[i].id >= threshold) //ako je osoba veca il jednaka pragu, ostaje ulisti
        {
            people[new_count++] = people[i]; //osoba se kop na novu pozic u nizu (new_count) a brojac se povecava
        }
    }
    *count = new_count; //broj osoba count se postavlja na novu vrijednost newcount tj ona sad predstavlja br osoba koje su ostale nakon sta se filtriranje izvrsilo
}

void deleteInReverseOrder(Person people[], int* count) 
{
    sortByDescendingID(people, *count); //osoba s najvecim id ce bit na poc liste (lista ide sad u silaznom redoslijedu)

    for (int i = 0; i < *count; i++) 
    {
        printf("Brisanje: Prezime: %s, Ime: %s, ID: %d\n", people[i].last_name, people[i].first_name, people[i].id);
    }
    *count = 0; //lista je prazna, nema vise osoba u nizu
}

/*Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa rezultat. Stog
je potrebno realizirati preko vezane liste. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>


struct cvor {
    int element;             // Vrijednost čvora
    struct cvor* next;       // Pokazivač na sljedeći čvor u vezanoj listi
    int prioritetcvora;      // Prioritet čvora (za stog nije potreban, ali uključen zbog zadatka)
};

typedef struct cvor* Position;  // Deklaracija tipa Position kao pokazivača na strukturu cvor

// Funkcije za rad sa stogom
int PushStog(Position, int);          // Dodavanje elementa na stog
int PopStog(Position, int*);          // Uklanjanje elementa sa stoga
int IsEmpty(Position);                // Provjera je li stog prazan
int EvaluatePostfix(const char*);     // Funkcija za evaluaciju postfix izraza
int PerformOperation(int, int, char); // Izvršava osnovne operacije (+, -, *, /)

// Glavna funkcija
int main() {
    char filename[50];
    char postfix[100];
    FILE* file = NULL;

    printf("Unesite ime datoteke s postfix izrazom: ");
    scanf("%s", filename);

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Greška: Ne mogu otvoriti datoteku %s.\n", filename);
        return -1;
    }

    fscanf(file, "%s", postfix); // Pretpostavlja se da je postfix izraz u jednom redu
    fclose(file);

    printf("Postfix izraz: %s\n", postfix);

    int rezultat = EvaluatePostfix(postfix);
    printf("Rezultat: %d\n", rezultat);

    return 0;
}

// Dodavanje elementa na stog
int PushStog(Position head, int value) {
    Position newNode = (Position)malloc(sizeof(struct cvor));
    if (newNode == NULL) {
        printf("Greška: Ne mogu alocirati memoriju.\n");
        return -1;
    }
    newNode->element = value;
    newNode->next = head->next;
    head->next = newNode;

    return 0;
}

// Uklanjanje elementa sa stoga
int PopStog(Position head, int* value) {
    if (head->next == NULL) {
        printf("Greška: Stog je prazan.\n");
        return -1;
    }

    Position temp = head->next;
    *value = temp->element;
    head->next = temp->next;
    free(temp);

    return 0;
}

// Provjera je li stog prazan
int IsEmpty(Position head) {
    return head->next == NULL;
}

// Evaluacija postfix izraza
int EvaluatePostfix(const char* postfix) {
    struct cvor head = { 0, NULL, 0 }; // Stog
    int i = 0, operand1 = 0, operand2 = 0, result = 0;

    while (postfix[i] != '\0') {
        if (isdigit(postfix[i])) {
            // Ako je broj, dodajemo ga na stog
            PushStog(&head, postfix[i] - '0'); // Pretvaranje char -> int
        }
        else {
            // Ako je operator, skidamo dva operanda sa stoga
            PopStog(&head, &operand2); // Zadnji uneseni broj je operand2
            PopStog(&head, &operand1); // Pretposljednji uneseni broj je operand1
            result = PerformOperation(operand1, operand2, postfix[i]);
            PushStog(&head, result); // Rezultat operacije vraćamo na stog
        }
        i++;
    }

    // Na kraju, na vrhu stoga ostaje rezultat
    PopStog(&head, &result);
    return result;
}

// Izvršavanje osnovnih operacija
int PerformOperation(int operand1, int operand2, char operator) 
{
    switch (operator) 
    {
    case '+': 
        return operand1 + operand2;
        
    case '-': 
        return operand1 - operand2;
        
    case '*': 
        return operand1 * operand2;
       
    case '/':
        if (operand2 == 0) 
        {
            printf("Greška: Dijeljenje s nulom.\n");
            exit(EXIT_FAILURE);
        }
        return operand1 / operand2;
    default:
        printf("Greška: Nepoznat operator %c.\n", operator);
        exit(EXIT_FAILURE);
    }
}

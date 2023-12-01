#ifndef SYSTEM_H
#define SYSTEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define FILENAME "C:/Users/Samo/OneDrive/Skrivbord/Cards.txt"


typedef struct {
    int cardNumber;
    bool hasAccess;
    time_t dateAdded;
} Card;

typedef struct {
    Card *cardDatabase;
    int totalCards;
    int maxCards;
} CardSystem;


void displayAdminMenu(CardSystem *system);
void remoteOpenDoor();
void listAllCards(CardSystem *system);
bool cardExists(CardSystem *system, int cardNumber);
void addCard(CardSystem *system);
void fakeScanTestCard(CardSystem *system);
void saveToFile(CardSystem *system);
void loadFromFile(CardSystem *system);
void manageAccess(CardSystem *system);

#endif 
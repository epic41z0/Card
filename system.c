#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>


void displayAdminMenu(CardSystem *system) {
    int option;
    do {
        printf("\nAdmin Menu:\n");
        printf("1. Remote open door.\n");
        printf("2. List all cards.\n");
        printf("3. Add a card.\n");
        printf("4. Add or remove access to a card.\n");
        printf("9. Fake scan test card.\n");
        printf("5. Exit.\n");
        printf("Enter your choice: ");

        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a valid option.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (option) {
            case 1:
                remoteOpenDoor();
                break;
            case 2:
                listAllCards(system);
                break;
            case 3:
                addCard(system);
                break;
            case 4:
                manageAccess(system);
                break;
            case 9:
                fakeScanTestCard(system);
                break;
            case 5:
                saveToFile(system);
                free(system->cardDatabase);
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (option != 5);
}

void remoteOpenDoor() {
    printf("Door opened remotely!\n");
    printf("Closing in \n3s...\n");
    sleep(1);
    printf("2s..\n");
    sleep(1);
    printf("1s.\n");
    sleep(1);
    printf("Door closed.\n");
}

// Function to list all cards
void listAllCards(CardSystem *system) {
    if (system->totalCards == 0) {
        printf("\nNo cards found. Please add a card first.\n");
        return;
    }

    printf("\nList of Cards:\n");
    for (int i = 0; i < system->totalCards; ++i) {
        printf("Card %d - Access: %s - Date Added: %s", system->cardDatabase[i].cardNumber, 
               system->cardDatabase[i].hasAccess ? "Granted" : "Revoked", ctime(&system->cardDatabase[i].dateAdded));
    }
}

bool cardExists(CardSystem *system, int cardNumber) {
    for (int i = 0; i < system->totalCards; ++i) {
        if (system->cardDatabase[i].cardNumber == cardNumber) {
            return true; // Card number already exists
        }
    }
    return false; // Card number does not exist
}
// Function to add a new card
void addCard(CardSystem *system) {
    
    int newCardNumber;
    printf("Enter card number: ");
    if (scanf("%d", &newCardNumber) != 1) {
        printf("Invalid input. Please enter a valid card number.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    if (cardExists(system, newCardNumber)) {
        printf("\nCard number already exists. Please use a diffrent card number.\n");
        return;
    }


    if (system->totalCards >= system->maxCards) {
        system->maxCards += 10; // Increase maxCards by 10
        system->cardDatabase = realloc(system->cardDatabase, system->maxCards * sizeof(Card));
        if (system->cardDatabase == NULL) {
            printf("Error allocating memory. Unable to add a new card.\n");
            return;
        }
    }

    system->cardDatabase[system->totalCards].cardNumber = newCardNumber;
    system->cardDatabase[system->totalCards].hasAccess = true;
    system->cardDatabase[system->totalCards].dateAdded = time(NULL);
    system->totalCards++;

    printf("\nCard added successfully!\n");
}

void fakeScanTestCard(CardSystem *system) {
    int cardNumber;
    printf("Enter card number to scan: ");
    if (scanf("%d", &cardNumber) != 1) {
        printf("Invalid input. Please enter a valid card number.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    bool found = false;
    for (int i = 0; i < system->totalCards; ++i) {
        if (system->cardDatabase[i].cardNumber == cardNumber) {
            found = true;
            if (system->cardDatabase[i].hasAccess) {
                printf("\nGreen light - Access granted for card %d\n", cardNumber);
            } else {
                printf("\nRed light - Access denied for card %d\n", cardNumber);
            }
            break;
        }
    }

    if (!found) {
        printf("Card not found.\n");
    }
}

void saveToFile(CardSystem *system) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fwrite(system->cardDatabase, sizeof(Card), system->totalCards, file);

    fclose(file);
}

void loadFromFile(CardSystem *system) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Error opening file. Starting with an empty dadabase.\n");
        return;
    }

    fseek(file, 0, SEEK_END); // Move file pointer to end of file
    long fileSize = ftell(file); // Get current file pointer
    rewind(file); // Move file pointer to beginning of file
    
    system->totalCards = fileSize / sizeof(Card);
    system->maxCards = system->totalCards + 10; // Add 10 extra slots

    system->cardDatabase = malloc(system->maxCards * sizeof(Card));
    if (system->cardDatabase == NULL) {
        printf("Error allocating memory! Unable to load cards.\n");
        fclose(file);
        return;
    }

    fread(system->cardDatabase, sizeof(Card), system->totalCards, file);

    fclose(file);
}

// Function to add or remove access to a card
void manageAccess(CardSystem *system) {
    if (system->totalCards == 0) {
        printf("\nNo cards available to manage access.\n");
        return;
    }

    int cardNumber;
    printf("Enter card number to manage access: ");
    scanf("%d", &cardNumber);

    bool found = false;
    for (int i = 0; i < system->totalCards; ++i) {
        if (system->cardDatabase[i].cardNumber == cardNumber) {
            found = true;
            printf("Do you want to grant (1) or revoke (0) access to card %d: ", cardNumber);
            int choice;
            scanf("%d", &choice);
            if (choice == 1){
                system->cardDatabase[i].hasAccess = true;
                printf("\nAccess granted to card %d.\n", cardNumber);
            }
            else if (choice == 0) {
                system->cardDatabase[i].hasAccess = false;
                printf("\nAccess revoked from card %d.\n", cardNumber);
            }
            else
                printf("\nInvalid choice.\n");
            break;
        }
    }

    if (!found)
        printf("\nCard not found.\n");
}
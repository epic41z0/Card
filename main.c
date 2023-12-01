#include <stdio.h>
#include "safeinput.h"
#include "system.h"


int main() {
    
    CardSystem system;
    system.cardDatabase = NULL;
    system.totalCards = 0;
    system.maxCards = 0;
    
    loadFromFile(&system);
    displayAdminMenu(&system);
    return 0;
}

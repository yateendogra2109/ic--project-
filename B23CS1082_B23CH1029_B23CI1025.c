#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Structure to represent a medicine
typedef struct {
    char name[50];
    int quantity;
    float price;
    struct tm expiryDate; // Expiry date of the medicine
} Medicine;

// Function prototypes
void loadInitialData(Medicine inventory[], int *numMedicines);
void addMedicine(Medicine inventory[], int *numMedicines);
void updateMedicine(Medicine inventory[], int numMedicines);
void deleteMedicine(Medicine inventory[], int *numMedicines);
void displayInventory(Medicine inventory[], int numMedicines);
float calculateTotalPrice(Medicine inventory[], int numMedicines);
void stockAlerts(Medicine inventory[], int numMedicines);
void generateBill(Medicine inventory[], int numMedicines);

int main() {
    Medicine inventory[100]; // Assuming a maximum of 100 medicines in the inventory
    int numMedicines = 0;
    int choice;
    char escapeChoice;

    // Load initial data from the file
    loadInitialData(inventory, &numMedicines);

    do {
        // Display menu
        printf("\nPharma Store Management System\n");
        printf("1. Add Medicine\n");
        printf("2. Update Medicine\n");
        printf("3. Delete Medicine\n");
        printf("4. Display Inventory\n");
        printf("5. Calculate Total Price\n");
        printf("6. Stock Alerts\n");
        printf("7. Generate Bill\n");
        printf("0. Exit\n");
        printf("Enter your choice (or 'Q' to quit): ");

        // Accept a single character for escape
        scanf(" %c", &escapeChoice);

        // Check if the user wants to exit
        if (toupper(escapeChoice) == 'Q') {
            printf("Exiting the program. Goodbye!\n");
            break; // Exit the loop
        }

        // If not, proceed with the numeric choice
        sscanf(&escapeChoice, "%d", &choice);

        switch (choice) {
            case 1:
                addMedicine(inventory, &numMedicines);
                break;
            case 2:
                updateMedicine(inventory, numMedicines);
                break;
            case 3:
                deleteMedicine(inventory, &numMedicines);
                break;
            case 4:
                displayInventory(inventory, numMedicines);
                break;
            case 5:
                printf("Total Price: $%.2f\n", calculateTotalPrice(inventory, numMedicines));
                break;
            case 6:
                stockAlerts(inventory, numMedicines);
                break;
            case 7:
                generateBill(inventory, numMedicines);
                break;
            case 0:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 0);

    return 0;
}

void loadInitialData(Medicine inventory[], int *numMedicines) {
    FILE *file = fopen("initial_inventory.txt", "r");
    if (file == NULL) {
        printf("Error opening the file. Exiting...\n");
        exit(1);
    }

    while (fscanf(file, "%s %d %f %d %d %d",
                  inventory[*numMedicines].name,
                  &inventory[*numMedicines].quantity,
                  &inventory[*numMedicines].price,
                  &inventory[*numMedicines].expiryDate.tm_year,
                  &inventory[*numMedicines].expiryDate.tm_mon,
                  &inventory[*numMedicines].expiryDate.tm_mday) == 6) {
        (*numMedicines)++;
        if (*numMedicines >= 100) {
            printf("Maximum capacity reached. Ignoring additional data.\n");
            break;
        }
    }

    fclose(file);
}

void addMedicine(Medicine inventory[], int *numMedicines) {
    // Add a new medicine to the inventory
    printf("Enter medicine name: ");
    scanf("%s", inventory[*numMedicines].name);
    printf("Enter quantity: ");
    scanf("%d", &inventory[*numMedicines].quantity);
    printf("Enter price: ");
    scanf("%f", &inventory[*numMedicines].price);

    printf("Enter expiration date (YYYY MM DD): ");
    scanf("%d %d %d", &inventory[*numMedicines].expiryDate.tm_year,
          &inventory[*numMedicines].expiryDate.tm_mon,
          &inventory[*numMedicines].expiryDate.tm_mday);

    (*numMedicines)++;

    printf("Medicine added successfully!\n");
}

void updateMedicine(Medicine inventory[], int numMedicines) {
    // Update existing medicine in the inventory
    char searchName[50];
    int found = 0;

    printf("Enter the name of the medicine to update: ");
    scanf("%s", searchName);

    for (int i = 0; i < numMedicines; i++) {
        if (strcmp(inventory[i].name, searchName) == 0) {
            printf("Enter new quantity: ");
            scanf("%d", &inventory[i].quantity);
            printf("Enter new price: ");
            scanf("%f", &inventory[i].price);

            printf("Enter new expiration date (YYYY MM DD): ");
            scanf("%d %d %d", &inventory[i].expiryDate.tm_year,
                  &inventory[i].expiryDate.tm_mon,
                  &inventory[i].expiryDate.tm_mday);

            printf("Medicine updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Medicine not found in the inventory.\n");
    }
}

void deleteMedicine(Medicine inventory[], int *numMedicines) {
    // Delete a medicine from the inventory
    char searchName[50];
    int found = 0;

    printf("Enter the name of the medicine to delete: ");
    scanf("%s", searchName);

    for (int i = 0; i < *numMedicines; i++) {
        if (strcmp(inventory[i].name, searchName) == 0) {
            // Shift elements to fill the gap
            for (int j = i; j < *numMedicines - 1; j++) {
                strcpy(inventory[j].name, inventory[j + 1].name);
                inventory[j].quantity = inventory[j + 1].quantity;
                inventory[j].price = inventory[j + 1].price;
                inventory[j].expiryDate = inventory[j + 1].expiryDate;
            }

            (*numMedicines)--;
            printf("Medicine deleted successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Medicine not found in the inventory.\n");
    }
}

void displayInventory(Medicine inventory[], int numMedicines) {
    // Display the current inventory
    printf("\nMedicine Inventory:\n");
    printf("%-20s %-10s %-10s %-15s\n", "Medicine Name", "Quantity", "Price", "Expiry Date");

    for (int i = 0; i < numMedicines; i++) {
        printf("%-20s %-10d $%.2f %02d-%02d-%04d\n", inventory[i].name, inventory[i].quantity, inventory[i].price,
               inventory[i].expiryDate.tm_mday, inventory[i].expiryDate.tm_mon, inventory[i].expiryDate.tm_year + 1900);
    }
}

float calculateTotalPrice(Medicine inventory[], int numMedicines) {
    // Calculate the total price of the inventory
    float totalPrice = 0;

    for (int i = 0; i < numMedicines; i++) {
        totalPrice += inventory[i].quantity * inventory[i].price;
    }

    return totalPrice;
}

void stockAlerts(Medicine inventory[], int numMedicines) {
    // Display alerts for different stock levels
    printf("\nStock Alerts:\n");

    struct tm currentDate;

    printf("Enter the current date (YYYY MM DD): ");
    scanf("%d %d %d", &currentDate.tm_year, &currentDate.tm_mon, &currentDate.tm_mday);

    // Adjust year to be years since 1900
    currentDate.tm_year -= 1900;

    for (int i = 0; i < numMedicines; i++) {
        // Adjust year to be years since 1900
        inventory[i].expiryDate.tm_year -= 1900;

        // Check if the medicine is expired
        if (mktime(&currentDate) > mktime(&inventory[i].expiryDate)) {
            printf("Expired stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        } else if (inventory[i].quantity < 2) {
            printf("Ultra Low stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        } else if (inventory[i].quantity <= 50) {
            printf("Low stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        } else if (inventory[i].quantity <= 100) {
            printf("Medium stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        } else {
            printf("High stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        }
        
        // Check if the medicine is expired separately and print if expired
        if (mktime(&currentDate) > mktime(&inventory[i].expiryDate)) {
            printf("Expired stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        }
    }
}



void generateBill(Medicine inventory[], int numMedicines) {
    // Generate a bill for the customer
    printf("\nGenerate Bill:\n");

    float totalBill = 0;
    int numItems;

    struct tm currentDate;

    printf("Enter the current date (YYYY MM DD): ");
    scanf("%d %d %d", &currentDate.tm_year, &currentDate.tm_mon, &currentDate.tm_mday);

    // Adjust year to be years since 1900
    currentDate.tm_year -= 1900;

    printf("Enter the number of items to purchase: ");
    scanf("%d", &numItems);

    for (int i = 0; i < numItems; i++) {
        char itemName[50];
        int quantity;

        printf("Enter the name of the medicine: ");
        scanf("%s", itemName);

        // Find the medicine in the inventory
        int found = 0;
        for (int j = 0; j < numMedicines; j++) {
            if (strcmp(inventory[j].name, itemName) == 0) {
                found = 1;

                // Check if the medicine is expired
                struct tm expiryDate = inventory[j].expiryDate;
                expiryDate.tm_year -= 1900;

                if (mktime(&currentDate) > mktime(&expiryDate)) {
                    printf("Expired medicine: %s. Cannot be purchased.\n", inventory[j].name);
                    found = 0;
                    break;
                }

                // Check if there is enough stock
                printf("Enter quantity for %s: ", itemName);
                scanf("%d", &quantity);

                if (quantity > 0 && quantity <= inventory[j].quantity) {
                    totalBill += quantity * inventory[j].price;

                    // Update stock in the inventory
                    inventory[j].quantity -= quantity;

                    // Display the item bought
                    printf("Bought: %s, Quantity: %d, Price: $%.2f\n", itemName, quantity, quantity * inventory[j].price);
                } else {
                    printf("Invalid quantity or insufficient stock.\n");
                }

                break;
            }
        }

        if (!found) {
            printf("Medicine %s not found in the inventory.\n", itemName);
            i--; // Decrement i to re-enter the current item
        }
    }

    printf("\nTotal Bill: $%.2f\n", totalBill);
}

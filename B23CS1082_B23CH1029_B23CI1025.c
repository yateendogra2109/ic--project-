#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

//framing structure.
typedef struct {
    char name[50];
    int quantity;
    float price;
} Medicine;

//defining the functions.
void EnteringData(Medicine inventory[], int *numMedicines) { //entering the data about medicines to the inventory.
    FILE *file = fopen("initial_inventory.txt", "r");
    if (file == NULL) {
        printf("Sorry! the file you searched for was not found in the inventory, exiting......\n");
        exit(1);
    }

    while (fscanf(file, "%s %d %f",
                  inventory[*numMedicines].name,
                  &inventory[*numMedicines].quantity,
                  &inventory[*numMedicines].price) == 3) {
        (*numMedicines)++;
        if (*numMedicines >= 100) {
            printf("Oops! The inventory is full so no additional can be uploaded.\n");
            break;
        }
    }

    fclose(file);
}

void FillingTheInventory(Medicine inventory[], int *numMedicines);  //Function to add medicines to the inventory.


void upgradation(Medicine inventory[], int numMedicines) { //Function to update medicines or to add some new medicines.
    
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

            printf("Congratulations! New medicine is successfully added to the inventory.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Sorry! No such medicine is present in the inventory at present moment.\n");
    }
}


void removeMedicine(Medicine inventory[], int *numMedicines); //Function to remove any of the medicine.

void ShowingTheInventory(Medicine inventory[], int numMedicines) { //Function to display inventory to the customer.
    printf("\nMedicine Inventory:\n");
    printf("%-20s %-10s %-10s\n", "Medicine Name", "Quantity", "Price");

    for (int i = 0; i < numMedicines; i++) {
        printf("%-20s %-10d $%.2f\n", inventory[i].name, inventory[i].quantity, inventory[i].price);
    }
}

float totalPrice(Medicine inventory[], int numMedicines);

void OutOfOrder(Medicine inventory[], int numMedicines) { //display alerts regarding the stocks of different medicines.
    printf("\nStock Alerts:\n");

    for (int i = 0; i < numMedicines; i++) {
        if (inventory[i].quantity < 2) {
            printf("Ultra Low stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        } else if (inventory[i].quantity <= 50) {
            printf("Low stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        } else if (inventory[i].quantity <= 100) {
            printf("Medium stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        } else {
            printf("High stock for %s. Quantity: %d\n", inventory[i].name, inventory[i].quantity);
        }
    }
}
void generateBill(Medicine inventory[], int numMedicines); //function to generate the bill.

int main() {
    Medicine inventory[100]; // Assuming a maximum of 100 medicines in the inventory
    int numMedicines = 0;
    int choice;
    char escapeChoice;

    printf("How do you want to enter data?\n");
    printf("1. Manually\n");
    printf("2. Load from file\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 2) {  // Load initial data from the file.
        EnteringData(inventory, &numMedicines);
    }

    do {
       //Display menu for the user.
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

        // Accept a single character for escape.
        scanf(" %c", &escapeChoice);

        //Asking the user if he wants to quit the program.
        if (toupper(escapeChoice) == 'Q') {
            printf("Exiting the program. Goodbye!\n");
            break; // Exit the loop
        }

        // If not, proceed with the numeric choice.
        sscanf(&escapeChoice, "%d", &choice);

        switch (choice) {
            case 1:
                FillingTheInventory(inventory, &numMedicines);
                break;
            case 2:
                upgradation(inventory, numMedicines);
                break;
            case 3:
                removeMedicine(inventory, &numMedicines);
                break;
            case 4:
                ShowingTheInventory(inventory, numMedicines);
                break;
            case 5:
                printf("Total Price: $%.2f\n", totalPrice(inventory, numMedicines));
                break;
            case 6:
                OutOfOrder(inventory, numMedicines);
                break;
            case 7:
                generateBill(inventory, numMedicines);
                break;
            case 0:
                printf("quitting the program,thankyou!!\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 0);

    return 0;
}


void FillingTheInventory(Medicine inventory[], int *numMedicines) {   // Add a new medicine to the inventory
  
    printf("Enter medicine name: ");
    scanf("%s", inventory[*numMedicines].name);
    printf("Enter quantity: ");
    scanf("%d", &inventory[*numMedicines].quantity);
    printf("Enter price: ");
    scanf("%f", &inventory[*numMedicines].price);

    (*numMedicines)++;

    printf("Congratulations! New medicine is added successfully.\n");
}



void removeMedicine(Medicine inventory[], int *numMedicines) {
    // Delete a medicine from the inventory
    char searchName[50];
    int found = 0;

    printf("Enter the name of the medicine to delete: ");
    scanf("%s", searchName);

    for (int i = 0; i < *numMedicines; i++) {
        if (strcmp(inventory[i].name, searchName) == 0) {
           //Shifting the elements to fill the gap.
            for (int j = i; j < *numMedicines - 1; j++) {
                strcpy(inventory[j].name, inventory[j + 1].name);
                inventory[j].quantity = inventory[j + 1].quantity;
                inventory[j].price = inventory[j + 1].price;
            }

            (*numMedicines)--;
            printf("The medicine is deleted successfully from the inventory!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Sorry! No such medicine is found in the inventory try re-entering the medicine name correctly.\n");
    }
}


float totalPrice(Medicine inventory[], int numMedicines) {
    // Calculate the total price of the inventory
    float totalPrice = 0;

    for (int i = 0; i < numMedicines; i++) {
        totalPrice += inventory[i].quantity * inventory[i].price;
    }

    return totalPrice;
}



void generateBill(Medicine inventory[], int numMedicines) {
    // Generate a bill for the customer
    printf("\nGenerate Bill:\n");

    float totalBill = 0;
    int numItems;

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
//hi
//22 march 2024

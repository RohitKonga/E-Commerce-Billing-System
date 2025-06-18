#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // For password input without echo

#define MAX_ITEMS 100
#define MAX_CART 50
#define MAX_BILLS 100
#define TAX_RATE 0.05   

// Structure for inventory items
typedef struct {
    int id;
    char name[50];
    float price;
    int stock;
} Item;

// Structure for cart items
typedef struct {
    int itemId;
    int quantity;
} CartItem;

// Structure for Bill History
typedef struct {
    int billNumber;
    CartItem cartItems[MAX_CART];
    int cartCount;
    float totalAmount;
} BillHistory;

// Declare global variables
Item items[MAX_ITEMS];
int itemCount = 0;
CartItem cart[MAX_CART];
int cartCount = 0;
BillHistory bills[MAX_BILLS];
int billCount = 0;

// Function prototypes
void loadItemsFromFile();
void saveItemsToFile();
void displayItems();
int findItemById(int id);
void addNewItem();
void updateItem();
void removeItem();
void addItemToCart();
void viewCart();
void removeItemFromCart();
void calculateBill();
void userMenu();
void adminMenu();
int authenticateAdmin();
void saveBillHistoryToFile();
void loadBillHistoryFromFile();
void displayBillHistory();

// Main function - the entry point of the program
int main() {
    int role;

    loadItemsFromFile(); // Load items from the file at the beginning
    loadBillHistoryFromFile(); // Load bill history from the file at the beginning

    // Display welcome message
    printf("\n\n****************************************************************\n");
    printf("*                                                              *\n");
    printf("*         Welcome to the E-Commerce Billing System             *\n");
    printf("*                                                              *\n");
    printf("****************************************************************\n\n");
    
    // Ask the user to choose a role
    printf("Select your role:\n");
    printf("1. Admin\n");
    printf("2. Customer\n");
    printf("3. Exit\n");
    printf("\nEnter your choice: ");
    scanf("%d", &role);

    // Handle different roles
    if (role == 1) {
        if (authenticateAdmin()) {
            adminMenu(); // If admin authenticated, show admin menu
        } else {
            printf("Invalid credentials. Exiting...\n");
        }
    } else if (role == 2) {
        userMenu(); // Show customer menu
    } else if (role == 3) {
        printf("\nExited Successfully...\n");
        return 0;
    } else {
        printf("Invalid role. Exiting...\n");
    }

    return 0;
}

// Admin authentication function (username: Rohit, password: rohit)
int authenticateAdmin() {
    char username[50], password[50];
    char ch;
    int i = 0;

    while (1) {  // Start a loop to repeatedly ask for credentials
        printf("Enter Admin Username: ");
        scanf("%s", username);

        printf("Enter Admin Password: ");
        i = 0;  // Reset the password input index for each attempt
        while (1) {
            ch = getch(); // Get password input without echoing
            if (ch == '\n' || ch == '\r') {
                password[i] = '\0'; // Null-terminate the password string
                break;
            } else {
                password[i++] = ch;
                printf("*");  // Print * for each character typed
            }
        }
        printf("\n");

        // Check if the username and password are correct
        if (strcmp(username, "Rohit") == 0 && strcmp(password, "rohit") == 0) {
            return 1;  // Admin authenticated successfully
        } else {
            printf("Invalid credentials. Please try again.\n");
        }
    }
}

// Customer menu function
void userMenu() {
    int choice;

    while (1) {
        // Show menu options
        printf("\n\n****************************************************************\n");
        printf("*                     Customer Menu                            *\n");
        printf("****************************************************************\n\n");
        printf("1. Display Items\n");
        printf("2. Add Item to Cart\n");
        printf("3. View Cart\n");
        printf("4. Remove Item from Cart\n");
        printf("5. Calculate Bill\n");
        printf("6. Exit and Main menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        // Handle different menu choices
        switch (choice) {
            case 1:
                displayItems();
                break;
            case 2:
                addItemToCart();
                break;
            case 3:
                viewCart();
                break;
            case 4:
                removeItemFromCart();
                break;
            case 5:
                calculateBill();
                break;
            case 6:
                printf("Thank you for shopping with us!\n");
                return; // Return to main menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Admin menu function
void adminMenu() {
    int choice;

    while (1) {
        // Show admin menu options
        printf("\n\n****************************************************************\n");
        printf("*                        Admin Menu                            *\n");
        printf("****************************************************************\n\n");
        printf("1. Display Items\n");
        printf("2. Add New Item\n");
        printf("3. Update Item\n");
        printf("4. Remove Item\n");
        printf("5. Save\n");
        printf("6. View Bill History\n");
        printf("7. Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        // Handle admin menu choices
        switch (choice) {
            case 1:
                displayItems();
                break;
            case 2:
                addNewItem();
                break;
            case 3:
                updateItem();
                break;
            case 4:
                removeItem();
                break;
            case 5:
                saveItemsToFile();
                printf("Changes saved\n");
                break;
            case 6:
                displayBillHistory();
                break;
            case 7:
                main(); // Go back to main menu
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to load items from file
void loadItemsFromFile() {
    FILE *file = fopen("items.txt", "r");
    if (!file) {
        printf("No inventory file found. Starting with an empty inventory.\n");
        return;
    }

    itemCount = 0;
    while (fscanf(file, "%d,%49[^,],%f,%d\n", &items[itemCount].id, items[itemCount].name,
                  &items[itemCount].price, &items[itemCount].stock) != EOF) {
        itemCount++;
    }
    fclose(file);
}

// Function to save items to file
void saveItemsToFile() {
    FILE *file = fopen("items.txt", "w");
    if (!file) {
        printf("Error saving items to file.\n");
        return;
    }

    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%d,%s,%.2f,%d\n", items[i].id, items[i].name, items[i].price, items[i].stock);
    }
    fclose(file);
}

// Function to save bill history to file
void saveBillHistoryToFile() {
    FILE *file = fopen("bills.txt", "w");
    if (!file) {
        printf("Error saving bill history.\n");
        return;
    }

    for (int i = 0; i < billCount; i++) {
        fprintf(file, "Bill Number: %d\n", bills[i].billNumber);
        for (int j = 0; j < bills[i].cartCount; j++) {
            int index = findItemById(bills[i].cartItems[j].itemId);
            fprintf(file, "Item Name: %s, Quantity: %d, Total: %.2f\n", items[index].name,
                    bills[i].cartItems[j].quantity,
                    items[index].price * bills[i].cartItems[j].quantity);
        }
        fprintf(file, "Total Amount: %.2f\n\n", bills[i].totalAmount);
    }
    fclose(file);
}

// Function to load bill history from file
void loadBillHistoryFromFile() {
    FILE *file = fopen("bills.txt", "r");
    if (!file) {
        printf("No bill history file found.\n");
        return;
    }

    billCount = 0;
    while (fscanf(file, "Bill Number: %d\n", &bills[billCount].billNumber) != EOF) {
        bills[billCount].cartCount = 0;
        char line[100];
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, "Item Name") != NULL) {
                int itemId, quantity;
                float total;
                sscanf(line, "Item Name: %d, Quantity: %d, Total: %f", &itemId, &quantity, &total);
                bills[billCount].cartItems[bills[billCount].cartCount].itemId = itemId;
                bills[billCount].cartItems[bills[billCount].cartCount].quantity = quantity;
                bills[billCount].cartCount++;
            } else if (strstr(line, "Total Amount") != NULL) {
                sscanf(line, "Total Amount: %f", &bills[billCount].totalAmount);
                billCount++;
                break;
            }
        }
    }
    fclose(file);
}

// Function to display bill history
void displayBillHistory() {
    if (billCount == 0) {
        printf("No bills found.\n");
        return;
    }

    printf("\n\n=== Bill History ===\n");
    for (int i = 0; i < billCount; i++) {
        printf("Bill Number: %d\n", bills[i].billNumber);
        for (int j = 0; j < bills[i].cartCount; j++) {
            int index = findItemById(bills[i].cartItems[j].itemId);
            printf("Item Name: %s, Quantity: %d, Total: %.2f\n", items[index].name,
                   bills[i].cartItems[j].quantity,
                   items[index].price * bills[i].cartItems[j].quantity);
        }
        printf("Total Amount: %.2f\n\n", bills[i].totalAmount);
    }
}

// Function to calculate the bill
void calculateBill() {
    if (cartCount == 0) {
        printf("Your cart is empty.\n");
        return;
    }

    float grandTotal = 0.0;

    printf("\n=== Billing Summary ===\n");
    printf("ID\tName\t\tPrice\tQuantity\tTotal\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < cartCount; i++) {
        int index = findItemById(cart[i].itemId);
        if (index != -1) {
            float total = items[index].price * cart[i].quantity;
            grandTotal += total;
            printf("%d\t%s\t\tRs %.2f\t%d\t\tRs %.2f\n",
                   items[index].id, items[index].name, items[index].price,
                   cart[i].quantity, total);
        }
    }

    float tax = grandTotal * TAX_RATE;
    float finalAmount = grandTotal + tax;

    // Save bill to history
    bills[billCount].billNumber = billCount + 1;
    bills[billCount].totalAmount = finalAmount;
    for (int i = 0; i < cartCount; i++) {
        bills[billCount].cartItems[i] = cart[i];
    }
    bills[billCount].cartCount = cartCount;
    billCount++;

    printf("--------------------------------------------------\n");
    printf("Subtotal: Rs %.2f\n", grandTotal);
    printf("Tax (%.2f%%): Rs %.2f\n", TAX_RATE * 100, tax);
    printf("Total Amount: Rs %.2f\n", finalAmount);

    // Clear the cart after purchase
    cartCount = 0;

    // Save the bill history to file
    saveBillHistoryToFile();
}

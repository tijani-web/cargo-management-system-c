#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_CARGO 100
#define MAX_STRING_LENGTH 100
#define MAX_ITEMS 10
#define DATA_FILE "cargo_data.txt"

// Cargo Item structure
typedef struct {
    char name[MAX_STRING_LENGTH];
    int quantity;
    float unitWeight;
} CargoItem;

// Cargo structure definition
typedef struct {
    int id;
    CargoItem items[MAX_ITEMS];
    int itemCount;
    float totalWeight;
    char sender[MAX_STRING_LENGTH];
    char senderAddress[MAX_STRING_LENGTH];
    char destination[MAX_STRING_LENGTH];
    char status[MAX_STRING_LENGTH];
    char trackingNumber[MAX_STRING_LENGTH];
} Cargo;

// Global variables
Cargo cargoRecords[MAX_CARGO];
int cargoCount = 0;

// Function prototypes
void initializeSystem();
void displayMenu();
void addCargo();
void viewAllCargo();
void calculateTotalWeight();
void searchByDestination();
void searchByStatus();
void trackCargo();
void saveToFile();
void loadFromFile();
void exitProgram();
int isIdUnique(int id);
int validateWeight(float weight);
void toLowerCase(char *str);
void clearInputBuffer();
void generateTrackingNumber(char *trackingNum);
void displayCargoTableHeader();
int caseInsensitiveCompare(const char *a, const char *b);

int main() {
    initializeSystem();
    displayMenu();
    return 0;
}

// Initialize the system by loading data from file
void initializeSystem() {
    printf("Initializing Cargo Management System...\n");
    loadFromFile();
    printf("System ready. %d cargo records loaded.\n", cargoCount);
}

// Display the main menu and handle user choices
void displayMenu() {
    int choice;
    
    do {
        printf("\n=== CARGO MANAGEMENT SYSTEM ===\n");
        printf("1. Add New Cargo\n");
        printf("2. View All Cargo\n");
        printf("3. Calculate Total Weight\n");
        printf("4. Search by Destination\n");
        printf("5. Search by Status\n");
        printf("6. Track Cargo\n");
        printf("7. Save Data to File\n");
        printf("8. Exit\n");
        printf("Please enter your choice (1-8): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 8.\n");
            clearInputBuffer();
            continue;
        }
        
        switch(choice) {
            case 1:
                addCargo();
                break;
            case 2:
                viewAllCargo();
                break;
            case 3:
                calculateTotalWeight();
                break;
            case 4:
                searchByDestination();
                break;
            case 5:
                searchByStatus();
                break;
            case 6:
                trackCargo();
                break;
            case 7:
                saveToFile();
                break;
            case 8:
                exitProgram();
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 8.\n");
        }
    } while (choice != 8);
}

// Add a new cargo record to the system
void addCargo() {
    if (cargoCount >= MAX_CARGO) {
        printf("Error: Maximum cargo capacity reached (%d records).\n", MAX_CARGO);
        return;
    }
    
    Cargo newCargo;
    newCargo.itemCount = 0;
    newCargo.totalWeight = 0;
    
    printf("\n--- Add New Cargo ---\n");
    
    // Get and validate ID
    printf("Enter cargo ID: ");
    if (scanf("%d", &newCargo.id) != 1) {
        printf("Error: Invalid ID format. Cargo not added.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    if (!isIdUnique(newCargo.id)) {
        printf("Error: ID %d already exists. Cargo not added.\n", newCargo.id);
        return;
    }
    
    // Get sender information
    printf("Enter sender name: ");
    fgets(newCargo.sender, MAX_STRING_LENGTH, stdin);
    newCargo.sender[strcspn(newCargo.sender, "\n")] = '\0';
    
    printf("Enter sender address: ");
    fgets(newCargo.senderAddress, MAX_STRING_LENGTH, stdin);
    newCargo.senderAddress[strcspn(newCargo.senderAddress, "\n")] = '\0';
    
    // Get destination
    printf("Enter destination: ");
    fgets(newCargo.destination, MAX_STRING_LENGTH, stdin);
    newCargo.destination[strcspn(newCargo.destination, "\n")] = '\0';
    
    // Add cargo items
    printf("\nAdding cargo items (max %d items):\n", MAX_ITEMS);
    char addMore;
    do {
        if (newCargo.itemCount >= MAX_ITEMS) {
            printf("Maximum items reached.\n");
            break;
        }
        
        printf("\nItem %d:\n", newCargo.itemCount + 1);
        printf("Enter item name: ");
        fgets(newCargo.items[newCargo.itemCount].name, MAX_STRING_LENGTH, stdin);
        newCargo.items[newCargo.itemCount].name[strcspn(newCargo.items[newCargo.itemCount].name, "\n")] = '\0';
        
        printf("Enter quantity: ");
        scanf("%d", &newCargo.items[newCargo.itemCount].quantity);
        clearInputBuffer();
        
        printf("Enter unit weight (kg): ");
        if (scanf("%f", &newCargo.items[newCargo.itemCount].unitWeight) != 1 || 
            !validateWeight(newCargo.items[newCargo.itemCount].unitWeight)) {
            printf("Error: Invalid weight. Must be a positive number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        // Calculate item weight and add to total
        float itemWeight = newCargo.items[newCargo.itemCount].quantity * 
                          newCargo.items[newCargo.itemCount].unitWeight;
        newCargo.totalWeight += itemWeight;
        
        newCargo.itemCount++;
        
        if (newCargo.itemCount < MAX_ITEMS) {
            printf("Add another item? (y/n): ");
            scanf(" %c", &addMore);
            clearInputBuffer();
        } else {
            addMore = 'n';
        }
    } while (addMore == 'y' || addMore == 'Y');
    
    // Get status
    printf("Enter status (e.g., In Transit, Delivered, Warehouse): ");
    fgets(newCargo.status, MAX_STRING_LENGTH, stdin);
    newCargo.status[strcspn(newCargo.status, "\n")] = '\0';
    
    // Generate tracking number
    generateTrackingNumber(newCargo.trackingNumber);
    
    // Add to records
    cargoRecords[cargoCount] = newCargo;
    cargoCount++;
    
    printf("\nCargo added successfully!\n");
    printf("Tracking Number: %s\n", newCargo.trackingNumber);
    printf("Total Weight: %.2f kg\n", newCargo.totalWeight);
}

// Display all cargo records in a formatted table
void displayCargoTableHeader() {
    printf("===============================================================================================================\n");
    printf("%-5s %-12s %-15s %-20s %-15s %-10s %-15s\n", 
           "ID", "Tracking No", "Sender", "Destination", "Items", "Weight(kg)", "Status");
    printf("===============================================================================================================\n");
}

void viewAllCargo() {
    if (cargoCount == 0) {
        printf("No cargo records available.\n");
        return;
    }
    
    printf("\n--- All Cargo Records ---\n");
    displayCargoTableHeader();
    
    for (int i = 0; i < cargoCount; i++) {
        printf("%-5d %-12s %-15s %-20s %-15d %-10.2f %-15s\n", 
               cargoRecords[i].id, 
               cargoRecords[i].trackingNumber,
               cargoRecords[i].sender, 
               cargoRecords[i].destination, 
               cargoRecords[i].itemCount,
               cargoRecords[i].totalWeight, 
               cargoRecords[i].status);
    }
    printf("===============================================================================================================\n");
    printf("Total records: %d\n", cargoCount);
}

// Calculate and display the total weight of all cargo
void calculateTotalWeight() {
    float totalWeight = 0;
    
    for (int i = 0; i < cargoCount; i++) {
        totalWeight += cargoRecords[i].totalWeight;
    }
    
    printf("\nTotal weight of all cargo: %.2f kg\n", totalWeight);
}

// Case-insensitive string comparison
int caseInsensitiveCompare(const char *a, const char *b) {
    for (; *a && *b; a++, b++) {
        if (tolower(*a) != tolower(*b)) {
            return 0;
        }
    }
    return *a == *b;
}

// Search for cargo by destination (case-insensitive)
void searchByDestination() {
    if (cargoCount == 0) {
        printf("No cargo records available to search.\n");
        return;
    }
    
    char searchTerm[MAX_STRING_LENGTH];
    int found = 0;
    
    printf("\n--- Search by Destination ---\n");
    printf("Enter destination to search for: ");
    clearInputBuffer();
    fgets(searchTerm, MAX_STRING_LENGTH, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    printf("\nSearch results for destination: %s\n", searchTerm);
    displayCargoTableHeader();
    
    for (int i = 0; i < cargoCount; i++) {
        if (caseInsensitiveCompare(cargoRecords[i].destination, searchTerm)) {
            printf("%-5d %-12s %-15s %-20s %-15d %-10.2f %-15s\n", 
                   cargoRecords[i].id, 
                   cargoRecords[i].trackingNumber,
                   cargoRecords[i].sender, 
                   cargoRecords[i].destination, 
                   cargoRecords[i].itemCount,
                   cargoRecords[i].totalWeight, 
                   cargoRecords[i].status);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No cargo found for destination: %s\n", searchTerm);
    }
    printf("===============================================================================================================\n");
}

// Search for cargo by status (case-insensitive)
void searchByStatus() {
    if (cargoCount == 0) {
        printf("No cargo records available to search.\n");
        return;
    }
    
    char searchTerm[MAX_STRING_LENGTH];
    int found = 0;
    
    printf("\n--- Search by Status ---\n");
    printf("Enter status to search for: ");
    clearInputBuffer();
    fgets(searchTerm, MAX_STRING_LENGTH, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    printf("\nSearch results for status: %s\n", searchTerm);
    displayCargoTableHeader();
    
    for (int i = 0; i < cargoCount; i++) {
        if (caseInsensitiveCompare(cargoRecords[i].status, searchTerm)) {
            printf("%-5d %-12s %-15s %-20s %-15d %-10.2f %-15s\n", 
                   cargoRecords[i].id, 
                   cargoRecords[i].trackingNumber,
                   cargoRecords[i].sender, 
                   cargoRecords[i].destination, 
                   cargoRecords[i].itemCount,
                   cargoRecords[i].totalWeight, 
                   cargoRecords[i].status);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No cargo found with status: %s\n", searchTerm);
    }
    printf("===============================================================================================================\n");
}

// Track a specific cargo by ID or tracking number
void trackCargo() {
    if (cargoCount == 0) {
        printf("No cargo records available to track.\n");
        return;
    }
    
    int option;
    char searchInput[20];
    int found = 0;
    
    printf("\n--- Track Cargo ---\n");
    printf("Enter 1 to search by ID\n");
    printf("Enter 2 to search by Tracking Number\n");
    printf("Enter option: ");
    scanf("%d", &option);
    clearInputBuffer();
    
    if (option == 1) {
        printf("Enter cargo ID to track: ");
        fgets(searchInput, 20, stdin);
        searchInput[strcspn(searchInput, "\n")] = '\0';
        int searchId = atoi(searchInput);
        
        for (int i = 0; i < cargoCount; i++) {
            if (cargoRecords[i].id == searchId) {
                found = 1;
                printf("\nCargo found!\n");
                printf("==================================================\n");
                printf("ID: %d\n", cargoRecords[i].id);
                printf("Tracking Number: %s\n", cargoRecords[i].trackingNumber);
                printf("Sender: %s\n", cargoRecords[i].sender);
                printf("Sender Address: %s\n", cargoRecords[i].senderAddress);
                printf("Destination: %s\n", cargoRecords[i].destination);
                printf("Status: %s\n", cargoRecords[i].status);
                printf("Total Weight: %.2f kg\n", cargoRecords[i].totalWeight);
                printf("Items:\n");
                for (int j = 0; j < cargoRecords[i].itemCount; j++) {
                    printf("  %d x %s (%.2f kg each)\n", 
                           cargoRecords[i].items[j].quantity,
                           cargoRecords[i].items[j].name,
                           cargoRecords[i].items[j].unitWeight);
                }
                printf("==================================================\n");
                break;
            }
        }
        
        if (!found) {
            printf("No cargo found with ID: %d\n", searchId);
        }
    } else if (option == 2) {
        printf("Enter tracking number to track: ");
        fgets(searchInput, MAX_STRING_LENGTH, stdin);
        searchInput[strcspn(searchInput, "\n")] = '\0';
        
        for (int i = 0; i < cargoCount; i++) {
            if (strcmp(cargoRecords[i].trackingNumber, searchInput) == 0) {
                found = 1;
                printf("\nCargo found!\n");
                printf("==================================================\n");
                printf("ID: %d\n", cargoRecords[i].id);
                printf("Tracking Number: %s\n", cargoRecords[i].trackingNumber);
                printf("Sender: %s\n", cargoRecords[i].sender);
                printf("Sender Address: %s\n", cargoRecords[i].senderAddress);
                printf("Destination: %s\n", cargoRecords[i].destination);
                printf("Status: %s\n", cargoRecords[i].status);
                printf("Total Weight: %.2f kg\n", cargoRecords[i].totalWeight);
                printf("Items:\n");
                for (int j = 0; j < cargoRecords[i].itemCount; j++) {
                    printf("  %d x %s (%.2f kg each)\n", 
                           cargoRecords[i].items[j].quantity,
                           cargoRecords[i].items[j].name,
                           cargoRecords[i].items[j].unitWeight);
                }
                printf("==================================================\n");
                break;
            }
        }
        
        if (!found) {
            printf("No cargo found with tracking number: %s\n", searchInput);
        }
    } else {
        printf("Invalid option.\n");
    }
}

// Save all cargo records to a text file
void saveToFile() {
    FILE *file = fopen(DATA_FILE, "w");
    
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    
    for (int i = 0; i < cargoCount; i++) {
        fprintf(file, "%d|%s|%s|%s|%s|%s|%.2f|%d|",
                cargoRecords[i].id,
                cargoRecords[i].trackingNumber,
                cargoRecords[i].sender,
                cargoRecords[i].senderAddress,
                cargoRecords[i].destination,
                cargoRecords[i].status,
                cargoRecords[i].totalWeight,
                cargoRecords[i].itemCount);
        
        for (int j = 0; j < cargoRecords[i].itemCount; j++) {
            fprintf(file, "%s|%d|%.2f|",
                    cargoRecords[i].items[j].name,
                    cargoRecords[i].items[j].quantity,
                    cargoRecords[i].items[j].unitWeight);
        }
        
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("Data saved successfully to %s. %d records written.\n", DATA_FILE, cargoCount);
}

// Load cargo records from a text file
void loadFromFile() {
    FILE *file = fopen(DATA_FILE, "r");
    
    if (file == NULL) {
        printf("No existing data file found. Starting with empty records.\n");
        return;
    }
    
    cargoCount = 0;
    char line[1000];
    
    while (fgets(line, sizeof(line), file) && cargoCount < MAX_CARGO) {
        // Tokenize the line using pipe as delimiter
        char *token = strtok(line, "|");
        if (token == NULL) continue;
        
        cargoRecords[cargoCount].id = atoi(token);
        
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(cargoRecords[cargoCount].trackingNumber, token, MAX_STRING_LENGTH);
        
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(cargoRecords[cargoCount].sender, token, MAX_STRING_LENGTH);
        
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(cargoRecords[cargoCount].senderAddress, token, MAX_STRING_LENGTH);
        
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(cargoRecords[cargoCount].destination, token, MAX_STRING_LENGTH);
        
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(cargoRecords[cargoCount].status, token, MAX_STRING_LENGTH);
        
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        cargoRecords[cargoCount].totalWeight = atof(token);
        
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        cargoRecords[cargoCount].itemCount = atoi(token);
        
        // Load items
        for (int i = 0; i < cargoRecords[cargoCount].itemCount; i++) {
            token = strtok(NULL, "|");
            if (token == NULL) break;
            strncpy(cargoRecords[cargoCount].items[i].name, token, MAX_STRING_LENGTH);
            
            token = strtok(NULL, "|");
            if (token == NULL) break;
            cargoRecords[cargoCount].items[i].quantity = atoi(token);
            
            token = strtok(NULL, "|");
            if (token == NULL) break;
            cargoRecords[cargoCount].items[i].unitWeight = atof(token);
        }
        
        cargoCount++;
    }
    
    fclose(file);
}

// Exit the program with a goodbye message
void exitProgram() {
    char response;
    
    printf("\nDo you want to save before exiting? (y/n): ");
    scanf(" %c", &response);
    clearInputBuffer();
    
    if (response == 'y' || response == 'Y') {
        saveToFile();
    }
    
    printf("\nThank you for using the Cargo Management System!\n");
    printf("Goodbye!\n");
    exit(0);
}

// Check if an ID is unique in the system
int isIdUnique(int id) {
    for (int i = 0; i < cargoCount; i++) {
        if (cargoRecords[i].id == id) {
            return 0;
        }
    }
    return 1;
}

// Validate that weight is a positive number
int validateWeight(float weight) {
    return (weight > 0);
}

// Convert a string to lowercase for case-insensitive comparisons
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Clear the input buffer to handle invalid inputs
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Generate a unique tracking number
void generateTrackingNumber(char *trackingNum) {
    static int counter = 1000;
    sprintf(trackingNum, "TRK%d", counter++);
}
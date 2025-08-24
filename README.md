Cargo Management System in C
A professional, modular Cargo Management System written in C, featuring:

Add, view, and manage cargo records with detailed item information

Search by destination, status, ID, and tracking number

Calculate total cargo weight

File persistence (save and load cargo data)

Menu-driven interface for user-friendly navigation

Case-insensitive search functionality

Input validation and error handling

📦 Features
addCargo() → Add new cargo with multiple items, sender info, and destination

viewAllCargo() → Display all cargo in a formatted table

calculateTotalWeight() → Show total weight of all cargo

searchByDestination() → Find cargo by destination (case-insensitive)

searchByStatus() → Find cargo by status (case-insensitive)

trackCargo() → Track cargo by ID or tracking number (e.g., TRK1001)

saveToFile() → Save cargo records to text file

loadFromFile() → Load saved cargo at startup

menu() → User-friendly interface

exitProgram() → Save before exit and display goodbye message


🚀 How to Run
Compile the Program
bash
gcc -o cargo_management cargo_management.c

Run the Program
bash
./cargo_management

📋 Usage Instructions
Adding Cargo: Select option 1 to add new cargo. You'll be prompted for:

Cargo ID (must be unique)

Sender information (name and address)

Destination

Multiple items (name, quantity, weight for each)

Status (e.g., "In Transit", "Delivered", "Warehouse")

Viewing All Cargo: Select option 2 to see all cargo records in a formatted table.

Calculating Total Weight: Select option 3 to see the combined weight of all cargo.

Searching by Destination: Select option 4 and enter a destination to find matching cargo.

Searching by Status: Select option 5 and enter a status to find matching cargo.

Tracking Cargo: Select option 6 and choose to search by:

ID: Enter the numeric ID (e.g., 1001)

Tracking Number: Enter the tracking number (e.g., TRK1001)

Saving Data: Select option 7 to manually save all cargo records to file.

Exiting: Select option 8 to exit. You'll be prompted to save before exiting.

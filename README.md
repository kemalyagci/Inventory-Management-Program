![image](https://github.com/user-attachments/assets/6c199a56-6283-4c67-b49c-44b082acdc2b)

# Inventory Management System

## Overview
The **Inventory Management System** is a C++/CLI application designed to manage inventory and stock levels for items in a database. The program allows users to add, update, search, and delete items, as well as manage categories and generate low stock reports. This system is connected to a SQL Server database and provides a user-friendly interface for interacting with the inventory.

## Key Features
- **Add Categories**: Users can add new categories to classify the items.
- **Add Items**: Users can add new inventory items, specifying item names, categories, quantity, and unit prices.
- **Search Items**: Users can search for items based on item name, category, price range, and quantity range.
- **Update Items**: Users can update the information of existing items.
- **Delete Items**: Users can delete items from the inventory.
- **Low Stock Report**: Users can generate and save a report of items with stock below a certain threshold.
- **Database Initialization**: The application automatically creates and initializes the required database and tables during the first run.

## Installation & Setup

### Prerequisites
- **Visual Studio**: Make sure you have Visual Studio installed with C++/CLI support.
- **SQL Server LocalDB**: The application uses Microsoft SQL Server LocalDB. Ensure that you have it installed and configured properly.
- **Git**: (Optional) If you are working with the source code from GitHub, ensure Git is installed.

### Database Setup
The program automatically creates a database and necessary tables when run for the first time. You do not need to manually create the database.

1. When you run the program, it will connect to `MSSQLLocalDB` and check if the `ProductDatabaseMuhammedKemalYagci` database exists.
2. If the database does not exist, it will create one along with the `Categories`, `Items`, and `InsertLog` tables.
3. A success message will show the file path where the database is created.

### Running the Program
1. Clone or download the project from GitHub.
2. Open the solution file (`.sln`) in Visual Studio.
3. Build and run the solution.
4. The program will create the necessary database on startup.

### Code Structure
- **MyForm.h**: This is the main form where the graphical user interface is defined.
- **MyForm.cpp**: Contains the implementation of the form's methods.
- **LoginForm.cpp, LoginForm.h, LoginForm.resx**: This handles user authentication (if enabled).
- **Project13.vcxproj**: The project file used by Visual Studio to manage and compile the project.
- **SQL Database**: SQL commands are embedded in the code to manage inventory items and categories in the LocalDB instance.

### How to Use
1. **Add a Category**: Enter the category name in the `Category Name` text box and click **Add Category**.
2. **Add an Item**: 
   - Enter the item name, quantity, and price in the respective fields.
   - Select a category from the dropdown list and click **Add Item**.
3. **Search Items**: Enter search criteria such as item name, price range, or quantity range, and click **Search Item**.
4. **Update an Item**: Select an item from the table, modify its details, and click **Update Item**.
5. **Delete an Item**: Select an item from the table and click **Delete Item**.
6. **Generate Low Stock Report**: Click **Create Low Stock Report** to generate a report of items with quantities below 5 units.

### Technologies Used
- **C++/CLI**: The main programming language used for the application.
- **Windows Forms**: For creating the user interface.
- **SQL Server LocalDB**: For managing inventory data.
- **.NET Framework**: Provides the underlying platform for running the Windows Forms application.

### Future Improvements
- **User Authentication**: Add user roles (e.g., Admin, User) with different access levels.
- **Advanced Search**: Implement more detailed search options, including filtering by multiple criteria at once.
- **Export/Import**: Allow users to export and import inventory data in CSV or Excel formats.
- **Notifications**: Add alerts for low stock directly on the UI.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

## Contact
For any questions or issues, feel free to contact the developer:
- **Developer**: Muhammed Kemal Yağcı
- (https://www.linkedin.com/in/muhammed-yagci/)


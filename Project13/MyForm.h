#pragma once 

namespace Project13 {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Data::SqlClient;
    using namespace System::IO;
    using namespace System::Threading::Tasks;
    using namespace System::Threading;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
            InitializeDatabase(); // Automatically initializes the database
            this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load); // Call LoadCategories after the form is ready
            dataGridViewItems->SelectionChanged += gcnew System::EventHandler(this, &MyForm::dataGridViewItems_SelectionChanged);
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private: System::Windows::Forms::TextBox^ textBoxCategoryName;
    private: System::Windows::Forms::Button^ buttonAddCategory;
    private: System::Windows::Forms::TextBox^ textBoxItemName;
    private: System::Windows::Forms::TextBox^ textBoxQuantity;
    private: System::Windows::Forms::TextBox^ textBoxUnitPrice; // To be used for price range
    private: System::Windows::Forms::ComboBox^ comboBoxCategories;
    private: System::Windows::Forms::Button^ buttonAddItem;
    private: System::Windows::Forms::Button^ buttonSearchItem;
    private: System::Windows::Forms::Button^ buttonDeleteItem;
    private: System::Windows::Forms::Button^ buttonUpdateItem;
    private: System::Windows::Forms::Button^ buttonListItems;
    private: System::Windows::Forms::DataGridView^ dataGridViewItems;
    private: System::Windows::Forms::Button^ buttonLowStockReport;
    private: System::Windows::Forms::Label^ LabelCategoryadd2;
    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::Label^ label2;
    private: System::Windows::Forms::Label^ label3;
    private: System::Windows::Forms::Label^ label4;
    private: System::Windows::Forms::Label^ label5;
    private: System::Windows::Forms::Button^ buttonQuestionMark_Click1;

    private: System::Windows::Forms::Label^ labelinfo1;
    private: System::Windows::Forms::Timer^ timer1;

    private: String^ connectionString = "Server=localhost\\SQLEXPRESS;Database=master;Integrated Security=True;";

           // Load event handler to ensure categories are loaded after the form is ready
    private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
        LoadCategories(); // Loads categories after the form is initialized
    }

           // Async method to load categories
    private: void LoadCategoriesAsync() {
        Task::Run(gcnew Action(this, &MyForm::LoadCategories)); // Asynchronously call the LoadCategories method
    }

           // Load categories from the database
    private: System::Void LoadCategories() {
        try {
            SqlConnection^ connection = gcnew SqlConnection("Server=(localdb)\\MSSQLLocalDB;Database=ProductDatabaseMuhammedKemalYagci;Integrated Security=True;");

            connection->Open();

            String^ query = "SELECT CategoryID, CategoryName FROM Categories";
            SqlCommand^ command = gcnew SqlCommand(query, connection);
            SqlDataReader^ reader = command->ExecuteReader();

            comboBoxCategories->Invoke(gcnew Action(this, &MyForm::ClearComboBox)); // Asynchronously clear ComboBox items

            while (reader->Read()) {
                String^ categoryDisplay = reader["CategoryID"]->ToString() + " - " + reader["CategoryName"]->ToString();
                comboBoxCategories->Invoke(gcnew Action<String^>(this, &MyForm::AddToComboBox), categoryDisplay); // Asynchronously add items to ComboBox
            }

            reader->Close();
            connection->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error loading categories: " + ex->Message);
        }
    }

           // Clears the ComboBox items
    private: System::Void ClearComboBox() {
        comboBoxCategories->Items->Clear();
    }

           // Adds an item to the ComboBox
    private: System::Void AddToComboBox(String^ item) {
        comboBoxCategories->Items->Add(item);
    }



    private: void buttonLowStockReport_Click(System::Object^ sender, System::EventArgs^ e) {
        Task::Run(gcnew Action(this, &MyForm::GenerateLowStockReport));
    }

    private: System::Void GenerateLowStockReport() {
        try {
            SqlConnection^ connection = gcnew SqlConnection("Server=(localdb)\\MSSQLLocalDB;Database=ProductDatabaseMuhammedKemalYagci;Integrated Security=True;");

            connection->Open();

            // Query low stock items
            String^ lowStockQuery = "SELECT Items.ItemID, Items.ItemName, Items.Quantity, Items.UnitPrice, Categories.CategoryID, Categories.CategoryName "
                "FROM Items INNER JOIN Categories ON Items.CategoryID = Categories.CategoryID WHERE Items.Quantity < 5";

            SqlCommand^ lowStockCommand = gcnew SqlCommand(lowStockQuery, connection);
            SqlDataAdapter^ dataAdapter = gcnew SqlDataAdapter(lowStockCommand);
            DataTable^ lowStockTable = gcnew DataTable();
            dataAdapter->Fill(lowStockTable);

            if (lowStockTable->Rows->Count == 0) {
                MessageBox::Show("No low stock found.");
                connection->Close();
                return;
            }

            // Use Invoke to display the SaveFileDialog in the UI thread
            this->Invoke(gcnew Action<DataTable^>(this, &MyForm::ShowSaveFileDialog), lowStockTable);

            connection->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error generating report: " + ex->Message);
        }
    }

           // Helper function to display the SaveFileDialog in the UI thread
    private: System::Void ShowSaveFileDialog(DataTable^ lowStockTable) {
        SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
        saveFileDialog->Filter = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*";
        saveFileDialog->Title = "Save Low Stock Report";

        if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            String^ reportPath = saveFileDialog->FileName;
            StreamWriter^ writer = gcnew StreamWriter(reportPath);
            writer->WriteLine("ItemID;ItemName;Quantity;UnitPrice;CategoryID;CategoryName");

            for each (DataRow ^ row in lowStockTable->Rows) {
                writer->WriteLine(row["ItemID"] + ";" + row["ItemName"] + ";" + row["Quantity"] + ";" + row["UnitPrice"] + ";" + row["CategoryID"] + ";" + row["CategoryName"]);
            }

            writer->Close();
            MessageBox::Show("Low stock report saved in CSV format at " + reportPath);
        }
    }




    private: System::Void buttonAddCategory_Click(System::Object^ sender, System::EventArgs^ e) {
        String^ categoryName = textBoxCategoryName->Text;
        if (!String::IsNullOrWhiteSpace(categoryName)) {
            Task::Run(gcnew Action(this, &MyForm::AddCategoryAsync));
        }
    }

    private: System::Void AddCategoryAsync() {
        try {
            SqlConnection^ connection = gcnew SqlConnection("Server=(localdb)\\MSSQLLocalDB;Database=ProductDatabaseMuhammedKemalYagci;Integrated Security=True;");

            connection->Open();

            // Check if the category name already exists
            String^ checkCategoryQuery = "SELECT COUNT(*) FROM Categories WHERE CategoryName = @name";
            SqlCommand^ checkCategoryCommand = gcnew SqlCommand(checkCategoryQuery, connection);
            checkCategoryCommand->Parameters->AddWithValue("@name", textBoxCategoryName->Text);
            int count = (int)checkCategoryCommand->ExecuteScalar();

            if (count > 0) {
                MessageBox::Show("This category already exists.");
            }
            else {
                String^ insertCategoryQuery = "INSERT INTO Categories (CategoryName) VALUES (@name)";
                SqlCommand^ command = gcnew SqlCommand(insertCategoryQuery, connection);
                command->Parameters->AddWithValue("@name", textBoxCategoryName->Text);
                command->ExecuteNonQuery();
                MessageBox::Show("Category added: " + textBoxCategoryName->Text);

                // Call LoadCategories to refresh the ComboBox
                comboBoxCategories->Invoke(gcnew Action(this, &MyForm::LoadCategories));
            }

            connection->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error: " + ex->Message);
        }
    }




    private: System::Void buttonAddItem_Click(System::Object^ sender, System::EventArgs^ e) {
        String^ itemName = textBoxItemName->Text;
        String^ quantityText = textBoxQuantity->Text;
        String^ unitPriceText = textBoxUnitPrice->Text;

        int quantity;
        Decimal unitPrice;

        // Name validation
        if (String::IsNullOrWhiteSpace(itemName)) {
            MessageBox::Show("Please enter the item name.");
            return;
        }

        // Quantity validation
        if (!Int32::TryParse(quantityText, quantity)) {
            MessageBox::Show("Please enter a valid stock value.");
            return;
        }

        // Price validation
        if (!Decimal::TryParse(unitPriceText, unitPrice)) {
            MessageBox::Show("Please enter a valid price value.");
            return;
        }

        if (comboBoxCategories->SelectedIndex < 0) {
            MessageBox::Show("Category cannot be empty. Please select a category.");
            return;
        }

        String^ selectedCategory = comboBoxCategories->SelectedItem->ToString();
        array<String^>^ categoryParts = selectedCategory->Split('-');
        int categoryID = Convert::ToInt32(categoryParts[0]->Trim());

        try {
            SqlConnection^ connection = gcnew SqlConnection("Server=(localdb)\\MSSQLLocalDB;Database=ProductDatabaseMuhammedKemalYagci;Integrated Security=True;");

            connection->Open();

            // Check if the item name already exists
            String^ checkItemQuery = "SELECT COUNT(*) FROM Items WHERE ItemName = @itemName";
            SqlCommand^ checkItemCommand = gcnew SqlCommand(checkItemQuery, connection);
            checkItemCommand->Parameters->AddWithValue("@itemName", itemName);
            int itemCount = (int)checkItemCommand->ExecuteScalar();

            if (itemCount > 0) {
                MessageBox::Show("This item already exists.");
                connection->Close();
                return;
            }

            // Start a transaction
            SqlTransaction^ transaction = connection->BeginTransaction();

            try {
                // Insert new item
                String^ insertItemQuery = "INSERT INTO Items (ItemName, CategoryID, Quantity, UnitPrice, LastUpdated) VALUES (@itemName, @categoryID, @quantity, @unitPrice, GETDATE())";
                SqlCommand^ insertItemCommand = gcnew SqlCommand(insertItemQuery, connection, transaction);

                insertItemCommand->Parameters->AddWithValue("@itemName", itemName);
                insertItemCommand->Parameters->AddWithValue("@categoryID", categoryID);
                insertItemCommand->Parameters->AddWithValue("@quantity", quantity);
                insertItemCommand->Parameters->AddWithValue("@unitPrice", unitPrice);

                insertItemCommand->ExecuteNonQuery();

                // Insert log record
                String^ logQuery = "INSERT INTO InsertLog (ItemName, InsertedBy, InsertDate) VALUES (@itemName, @insertedBy, GETDATE())";
                SqlCommand^ logCommand = gcnew SqlCommand(logQuery, connection, transaction);

                logCommand->Parameters->AddWithValue("@itemName", itemName);
                logCommand->Parameters->AddWithValue("@insertedBy", "Admin");

                logCommand->ExecuteNonQuery();

                // Commit the transaction
                transaction->Commit();

                MessageBox::Show("Item successfully added: " + itemName);
            }
            catch (Exception^ ex) {
                transaction->Rollback();
                MessageBox::Show("Error during item addition: " + ex->Message);
            }

            connection->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Connection error: " + ex->Message);
        }
    }














    private: System::Void buttonSearchItem_Click(System::Object^ sender, System::EventArgs^ e) {
        String^ itemName = textBoxItemName->Text;
        Decimal minPrice, maxPrice;
        int minQuantity, maxQuantity;

        // Check price range
        array<String^>^ priceRange = textBoxUnitPrice->Text->Split('-');
        bool isPriceRangeValid = priceRange->Length == 2 && Decimal::TryParse(priceRange[0], minPrice) && Decimal::TryParse(priceRange[1], maxPrice);

        // If not a range, check for a single value
        bool isSinglePriceValid = priceRange->Length == 1 && Decimal::TryParse(priceRange[0], minPrice);
        if (isSinglePriceValid) {
            maxPrice = minPrice;  // If not a range, search with a single value
        }

        // Check quantity range
        array<String^>^ quantityRange = textBoxQuantity->Text->Split('-');
        bool isQuantityRangeValid = quantityRange->Length == 2 && Int32::TryParse(quantityRange[0], minQuantity) && Int32::TryParse(quantityRange[1], maxQuantity);

        // If not a range, check for a single value
        bool isSingleQuantityValid = quantityRange->Length == 1 && Int32::TryParse(quantityRange[0], minQuantity);
        if (isSingleQuantityValid) {
            maxQuantity = minQuantity;  // If not a range, search with a single value
        }

        // Check if at least one search criteria is provided
        if (comboBoxCategories->SelectedIndex < 0 && String::IsNullOrWhiteSpace(itemName) && !isPriceRangeValid && !isSinglePriceValid && !isQuantityRangeValid && !isSingleQuantityValid) {
            MessageBox::Show("Please enter at least one search criterion (Item Name, Category, Price range, Quantity range).");
            return;
        }

        try {
            SqlConnection^ connection = gcnew SqlConnection("Server=(localdb)\\MSSQLLocalDB;Database=ProductDatabaseMuhammedKemalYagci;Integrated Security=True;");

            connection->Open();

            // Create SQL search query
            String^ searchQuery = "SELECT * FROM Items WHERE 1=1";

            // Search by item name
            if (!String::IsNullOrWhiteSpace(itemName)) {
                searchQuery += " AND ItemName = @itemName";
            }

            int categoryID = -1; // Define categoryID here

            // Search by category
            if (comboBoxCategories->SelectedIndex >= 0) {
                String^ selectedCategory = comboBoxCategories->SelectedItem->ToString();
                array<String^>^ categoryParts = selectedCategory->Split('-');
                categoryID = Convert::ToInt32(categoryParts[0]->Trim()); // Get ID

                searchQuery += " AND CategoryID = @categoryID";
            }

            // Search by price range or single price
            if (isPriceRangeValid || isSinglePriceValid) {
                searchQuery += " AND UnitPrice BETWEEN @minPrice AND @maxPrice";
            }

            // Search by quantity range or single quantity
            if (isQuantityRangeValid || isSingleQuantityValid) {
                searchQuery += " AND Quantity BETWEEN @minQuantity AND @maxQuantity";
            }

            SqlCommand^ searchCommand = gcnew SqlCommand(searchQuery, connection);

            // Add parameters to the query
            if (!String::IsNullOrWhiteSpace(itemName)) {
                searchCommand->Parameters->AddWithValue("@itemName", itemName);
            }
            if (comboBoxCategories->SelectedIndex >= 0) {
                searchCommand->Parameters->AddWithValue("@categoryID", categoryID);
            }
            if (isPriceRangeValid || isSinglePriceValid) {
                searchCommand->Parameters->AddWithValue("@minPrice", minPrice);
                searchCommand->Parameters->AddWithValue("@maxPrice", maxPrice);
            }
            if (isQuantityRangeValid || isSingleQuantityValid) {
                searchCommand->Parameters->AddWithValue("@minQuantity", minQuantity);
                searchCommand->Parameters->AddWithValue("@maxQuantity", maxQuantity);
            }

            SqlDataAdapter^ dataAdapter = gcnew SqlDataAdapter(searchCommand);
            DataTable^ dataTable = gcnew DataTable();
            dataAdapter->Fill(dataTable);

            dataGridViewItems->DataSource = dataTable;  // Bind results to DataGridView
            connection->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("An error occurred while searching for the item: " + ex->Message);
        }
    }

    private: System::Void buttonUpdateItem_Click(System::Object^ sender, System::EventArgs^ e) {
        if (dataGridViewItems->SelectedRows->Count > 0) {
            int itemID = Convert::ToInt32(dataGridViewItems->SelectedRows[0]->Cells["ItemID"]->Value);
            String^ itemName = textBoxItemName->Text;
            String^ quantityText = textBoxQuantity->Text;
            String^ unitPriceText = textBoxUnitPrice->Text;

            int quantity;
            Decimal unitPrice;

            // Name validation
            if (String::IsNullOrWhiteSpace(itemName)) {
                MessageBox::Show("Please enter the item name.");
                return;
            }

            // Quantity validation
            if (!Int32::TryParse(quantityText, quantity)) {
                MessageBox::Show("Please enter a valid stock value.");
                return;
            }

            // Price validation
            if (!Decimal::TryParse(unitPriceText, unitPrice)) {
                MessageBox::Show("Please enter a valid price value.");
                return;
            }

            // Category selection validation
            if (comboBoxCategories->SelectedIndex < 0) {
                MessageBox::Show("Category cannot be empty. Please select a category.");
                return;
            }

            String^ selectedCategory = comboBoxCategories->SelectedItem->ToString();
            array<String^>^ categoryParts = selectedCategory->Split('-');
            int categoryID = Convert::ToInt32(categoryParts[0]->Trim());

            try {
                // Open connection
                SqlConnection^ connection = gcnew SqlConnection("Server=(localdb)\\MSSQLLocalDB;Database=ProductDatabaseMuhammedKemalYagci;Integrated Security=True;");

                connection->Open();

                // Start a transaction
                SqlTransaction^ transaction = connection->BeginTransaction();

                try {
                    // First query: Update the item
                    String^ updateQuery = "UPDATE Items SET ItemName = @itemName, CategoryID = @categoryID, Quantity = @quantity, UnitPrice = @unitPrice, LastUpdated = GETDATE() WHERE ItemID = @itemID";
                    SqlCommand^ updateItemCommand = gcnew SqlCommand(updateQuery, connection, transaction); // Adding the transaction

                    // Add parameters to the query
                    updateItemCommand->Parameters->AddWithValue("@itemID", itemID);
                    updateItemCommand->Parameters->AddWithValue("@itemName", itemName);
                    updateItemCommand->Parameters->AddWithValue("@categoryID", categoryID);
                    updateItemCommand->Parameters->AddWithValue("@quantity", quantity);
                    updateItemCommand->Parameters->AddWithValue("@unitPrice", unitPrice);

                    updateItemCommand->ExecuteNonQuery();

                    // Second query: Add an update log
                    String^ logQuery = "INSERT INTO UpdateLog (ItemID, UpdatedBy, UpdateDate) VALUES (@itemID, @updatedBy, GETDATE())";
                    SqlCommand^ logCommand = gcnew SqlCommand(logQuery, connection, transaction); // Adding the transaction

                    logCommand->Parameters->AddWithValue("@itemID", itemID);
                    logCommand->Parameters->AddWithValue("@updatedBy", "Admin"); // Assuming 'Admin' as the updated by person

                    logCommand->ExecuteNonQuery();

                    // Commit the transaction if both queries succeed
                    transaction->Commit();

                    MessageBox::Show("Item successfully updated: " + itemName);
                }
                catch (Exception^ ex) {
                    // Rollback the transaction if an error occurs
                    transaction->Rollback();
                    MessageBox::Show("Error during update: " + ex->Message);
                }

                connection->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Connection error: " + ex->Message);
            }
        }
        else {
            MessageBox::Show("Please search for the item to update and select it from the table.");
        }
    }







    private: System::Void buttonDeleteItem_Click(System::Object^ sender, System::EventArgs^ e) {
        if (dataGridViewItems->SelectedRows->Count > 0) {
            int itemID = Convert::ToInt32(dataGridViewItems->SelectedRows[0]->Cells["ItemID"]->Value);

            // Confirmation dialog for deletion
            System::Windows::Forms::DialogResult result = MessageBox::Show("Are you sure you want to delete this item?", "Delete Item", MessageBoxButtons::YesNo, MessageBoxIcon::Warning);

            if (result == System::Windows::Forms::DialogResult::Yes) {
                try {
                    // Open connection
                    SqlConnection^ connection = gcnew SqlConnection("Server=(localdb)\\MSSQLLocalDB;Database=ProductDatabaseMuhammedKemalYagci;Integrated Security=True;");

                    connection->Open();

                    // Start a transaction
                    SqlTransaction^ transaction = connection->BeginTransaction();

                    try {
                        // First query: Delete the item
                        String^ deleteQuery = "DELETE FROM Items WHERE ItemID = @itemID";
                        SqlCommand^ deleteCommand = gcnew SqlCommand(deleteQuery, connection, transaction); // Adding the transaction

                        deleteCommand->Parameters->AddWithValue("@itemID", itemID);
                        deleteCommand->ExecuteNonQuery();

                        // Second query: Insert a log for the deleted item
                        String^ logQuery = "INSERT INTO DeleteLog (ItemID, DeletedBy, DeleteDate) VALUES (@itemID, @deletedBy, GETDATE())";
                        SqlCommand^ logCommand = gcnew SqlCommand(logQuery, connection, transaction); // Adding the transaction

                        logCommand->Parameters->AddWithValue("@itemID", itemID);
                        logCommand->Parameters->AddWithValue("@deletedBy", "Admin"); // Assuming 'Admin' as the deleted by person
                        logCommand->ExecuteNonQuery();

                        // Commit the transaction
                        transaction->Commit();

                        MessageBox::Show("Item successfully deleted.");
                    }
                    catch (Exception^ ex) {
                        // Rollback the transaction if an error occurs
                        transaction->Rollback();
                        MessageBox::Show("Error while deleting the item: " + ex->Message);
                    }

                    connection->Close();
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Connection error: " + ex->Message);
                }
            }
            else {
                MessageBox::Show("Item deletion canceled.");
            }
        }
        else {
            MessageBox::Show("Please search for the item to delete and select it from the table.");
        }
    }


    private: System::Void dataGridViewItems_SelectionChanged(System::Object^ sender, System::EventArgs^ e) {
        if (dataGridViewItems->SelectedRows->Count > 0) {
            DataGridViewRow^ selectedRow = dataGridViewItems->SelectedRows[0];

            // Transfer data to TextBoxes
            textBoxItemName->Text = selectedRow->Cells["ItemName"]->Value->ToString();
            textBoxQuantity->Text = selectedRow->Cells["Quantity"]->Value->ToString();
            textBoxUnitPrice->Text = selectedRow->Cells["UnitPrice"]->Value->ToString();

            // Transfer data to ComboBox based on the category ID
            int categoryID = Convert::ToInt32(selectedRow->Cells["CategoryID"]->Value);
            for (int i = 0; i < comboBoxCategories->Items->Count; i++) {
                String^ categoryItem = comboBoxCategories->Items[i]->ToString();
                array<String^>^ categoryParts = categoryItem->Split('-');
                if (categoryID == Convert::ToInt32(categoryParts[0]->Trim())) {
                    comboBoxCategories->SelectedIndex = i;
                    break;
                }
            }
        }
    }








    private: System::Void buttonListItems_Click(System::Object^ sender, System::EventArgs^ e) {
        try {
            SqlConnection^ connection = gcnew SqlConnection("Server=(localdb)\\MSSQLLocalDB;Database=ProductDatabaseMuhammedKemalYagci;Integrated Security=True;");

            connection->Open();

            // Query to list items
            String^ listItemsQuery = "SELECT Items.ItemID, Items.ItemName, Categories.CategoryID, Categories.CategoryName, Items.Quantity, Items.UnitPrice, Items.LastUpdated "
                "FROM Items INNER JOIN Categories ON Items.CategoryID = Categories.CategoryID";

            SqlCommand^ listItemsCommand = gcnew SqlCommand(listItemsQuery, connection);

            SqlDataAdapter^ dataAdapter = gcnew SqlDataAdapter(listItemsCommand);
            DataTable^ dataTable = gcnew DataTable();
            dataAdapter->Fill(dataTable);  // Fill the DataTable with the data

            dataGridViewItems->DataSource = dataTable;  // Bind data to the DataGridView
            connection->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("An error occurred while listing items: " + ex->Message);
        }
    }
    private: System::ComponentModel::IContainer^ components;

    protected:


#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            this->textBoxCategoryName = (gcnew System::Windows::Forms::TextBox());
            this->buttonAddCategory = (gcnew System::Windows::Forms::Button());
            this->textBoxItemName = (gcnew System::Windows::Forms::TextBox());
            this->textBoxQuantity = (gcnew System::Windows::Forms::TextBox());
            this->textBoxUnitPrice = (gcnew System::Windows::Forms::TextBox());
            this->comboBoxCategories = (gcnew System::Windows::Forms::ComboBox());
            this->buttonAddItem = (gcnew System::Windows::Forms::Button());
            this->buttonSearchItem = (gcnew System::Windows::Forms::Button());
            this->buttonDeleteItem = (gcnew System::Windows::Forms::Button());
            this->buttonUpdateItem = (gcnew System::Windows::Forms::Button());
            this->buttonListItems = (gcnew System::Windows::Forms::Button());
            this->dataGridViewItems = (gcnew System::Windows::Forms::DataGridView());
            this->buttonLowStockReport = (gcnew System::Windows::Forms::Button());
            this->LabelCategoryadd2 = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->buttonQuestionMark_Click1 = (gcnew System::Windows::Forms::Button());
            this->labelinfo1 = (gcnew System::Windows::Forms::Label());
            this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewItems))->BeginInit();
            this->SuspendLayout();
            // 
            // textBoxCategoryName
            // 
            this->textBoxCategoryName->Location = System::Drawing::Point(117, 15);
            this->textBoxCategoryName->Name = L"textBoxCategoryName";
            this->textBoxCategoryName->Size = System::Drawing::Size(363, 22);
            this->textBoxCategoryName->TabIndex = 0;
            // 
            // buttonAddCategory
            // 
            this->buttonAddCategory->Location = System::Drawing::Point(353, 54);
            this->buttonAddCategory->Name = L"buttonAddCategory";
            this->buttonAddCategory->Size = System::Drawing::Size(127, 28);
            this->buttonAddCategory->TabIndex = 1;
            this->buttonAddCategory->Text = L"Add Category";
            this->buttonAddCategory->UseVisualStyleBackColor = true;
            this->buttonAddCategory->Click += gcnew System::EventHandler(this, &MyForm::buttonAddCategory_Click);
            // 
            // textBoxItemName
            // 
            this->textBoxItemName->Location = System::Drawing::Point(117, 208);
            this->textBoxItemName->Name = L"textBoxItemName";
            this->textBoxItemName->Size = System::Drawing::Size(363, 22);
            this->textBoxItemName->TabIndex = 2;
            // 
            // textBoxQuantity
            // 
            this->textBoxQuantity->Location = System::Drawing::Point(117, 252);
            this->textBoxQuantity->Name = L"textBoxQuantity";
            this->textBoxQuantity->Size = System::Drawing::Size(363, 22);
            this->textBoxQuantity->TabIndex = 3;
            // 
            // textBoxUnitPrice
            // 
            this->textBoxUnitPrice->Location = System::Drawing::Point(117, 300);
            this->textBoxUnitPrice->Name = L"textBoxUnitPrice";
            this->textBoxUnitPrice->Size = System::Drawing::Size(363, 22);
            this->textBoxUnitPrice->TabIndex = 4;
            // 
            // comboBoxCategories
            // 
            this->comboBoxCategories->FormattingEnabled = true;
            this->comboBoxCategories->Location = System::Drawing::Point(117, 166);
            this->comboBoxCategories->Name = L"comboBoxCategories";
            this->comboBoxCategories->Size = System::Drawing::Size(363, 24);
            this->comboBoxCategories->TabIndex = 5;
            // 
            // buttonAddItem
            // 
            this->buttonAddItem->Location = System::Drawing::Point(12, 359);
            this->buttonAddItem->Name = L"buttonAddItem";
            this->buttonAddItem->Size = System::Drawing::Size(100, 28);
            this->buttonAddItem->TabIndex = 6;
            this->buttonAddItem->Text = L"Add Item";
            this->buttonAddItem->UseVisualStyleBackColor = true;
            this->buttonAddItem->Click += gcnew System::EventHandler(this, &MyForm::buttonAddItem_Click);
            // 
            // buttonSearchItem
            // 
            this->buttonSearchItem->Location = System::Drawing::Point(395, 359);
            this->buttonSearchItem->Name = L"buttonSearchItem";
            this->buttonSearchItem->Size = System::Drawing::Size(100, 28);
            this->buttonSearchItem->TabIndex = 7;
            this->buttonSearchItem->Text = L"Search Item";
            this->buttonSearchItem->UseVisualStyleBackColor = true;
            this->buttonSearchItem->Click += gcnew System::EventHandler(this, &MyForm::buttonSearchItem_Click);
            // 
            // buttonDeleteItem
            // 
            this->buttonDeleteItem->Location = System::Drawing::Point(273, 359);
            this->buttonDeleteItem->Name = L"buttonDeleteItem";
            this->buttonDeleteItem->Size = System::Drawing::Size(91, 28);
            this->buttonDeleteItem->TabIndex = 8;
            this->buttonDeleteItem->Text = L"Delete Item";
            this->buttonDeleteItem->UseVisualStyleBackColor = true;
            this->buttonDeleteItem->Click += gcnew System::EventHandler(this, &MyForm::buttonDeleteItem_Click);
            // 
            // buttonUpdateItem
            // 
            this->buttonUpdateItem->Location = System::Drawing::Point(135, 359);
            this->buttonUpdateItem->Name = L"buttonUpdateItem";
            this->buttonUpdateItem->Size = System::Drawing::Size(115, 28);
            this->buttonUpdateItem->TabIndex = 9;
            this->buttonUpdateItem->Text = L"Update Item";
            this->buttonUpdateItem->UseVisualStyleBackColor = true;
            this->buttonUpdateItem->Click += gcnew System::EventHandler(this, &MyForm::buttonUpdateItem_Click);
            // 
            // buttonListItems
            // 
            this->buttonListItems->Location = System::Drawing::Point(156, 430);
            this->buttonListItems->Name = L"buttonListItems";
            this->buttonListItems->Size = System::Drawing::Size(133, 28);
            this->buttonListItems->TabIndex = 10;
            this->buttonListItems->Text = L"List All Items";
            this->buttonListItems->UseVisualStyleBackColor = true;
            this->buttonListItems->Click += gcnew System::EventHandler(this, &MyForm::buttonListItems_Click);
            // 
            // dataGridViewItems
            // 
            this->dataGridViewItems->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewItems->Location = System::Drawing::Point(561, 36);
            this->dataGridViewItems->Name = L"dataGridViewItems";
            this->dataGridViewItems->RowHeadersWidth = 51;
            this->dataGridViewItems->RowTemplate->Height = 24;
            this->dataGridViewItems->Size = System::Drawing::Size(1008, 810);
            this->dataGridViewItems->TabIndex = 11;
            // 
            // buttonLowStockReport
            // 
            this->buttonLowStockReport->Location = System::Drawing::Point(320, 430);
            this->buttonLowStockReport->Name = L"buttonLowStockReport";
            this->buttonLowStockReport->Size = System::Drawing::Size(195, 28);
            this->buttonLowStockReport->TabIndex = 12;
            this->buttonLowStockReport->Text = L"Create Low Stock Report 5<";
            this->buttonLowStockReport->UseVisualStyleBackColor = true;
            this->buttonLowStockReport->Click += gcnew System::EventHandler(this, &MyForm::buttonLowStockReport_Click);
            // 
            // LabelCategoryadd2
            // 
            this->LabelCategoryadd2->AutoSize = true;
            this->LabelCategoryadd2->Location = System::Drawing::Point(9, 18);
            this->LabelCategoryadd2->Name = L"LabelCategoryadd2";
            this->LabelCategoryadd2->Size = System::Drawing::Size(93, 16);
            this->LabelCategoryadd2->TabIndex = 13;
            this->LabelCategoryadd2->Text = L"Add Category:";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(9, 121);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(121, 16);
            this->label1->TabIndex = 14;
            this->label1->Text = L"Add or Search Item";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(21, 169);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(85, 16);
            this->label2->TabIndex = 15;
            this->label2->Text = L"ID/ Category:";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(23, 214);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(75, 16);
            this->label3->TabIndex = 16;
            this->label3->Text = L"Item Name:";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(23, 255);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(58, 16);
            this->label4->TabIndex = 17;
            this->label4->Text = L"Quantity:";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(21, 303);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(67, 16);
            this->label5->TabIndex = 18;
            this->label5->Text = L"Unit Price:";
            // 
            // buttonQuestionMark_Click1
            // 
            this->buttonQuestionMark_Click1->BackColor = System::Drawing::SystemColors::Menu;
            this->buttonQuestionMark_Click1->Location = System::Drawing::Point(497, 252);
            this->buttonQuestionMark_Click1->Name = L"buttonQuestionMark_Click1";
            this->buttonQuestionMark_Click1->Size = System::Drawing::Size(48, 23);
            this->buttonQuestionMark_Click1->TabIndex = 19;
            this->buttonQuestionMark_Click1->Text = L"\?";
            this->buttonQuestionMark_Click1->UseVisualStyleBackColor = false;
            this->buttonQuestionMark_Click1->Click += gcnew System::EventHandler(this, &MyForm::buttonQuestionMark_Click1_Click);
            // 
            // labelinfo1
            // 
            this->labelinfo1->AutoSize = true;
            this->labelinfo1->BackColor = System::Drawing::SystemColors::Info;
            this->labelinfo1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(162)));
            this->labelinfo1->Location = System::Drawing::Point(269, 229);
            this->labelinfo1->Name = L"labelinfo1";
            this->labelinfo1->Size = System::Drawing::Size(593, 20);
            this->labelinfo1->TabIndex = 20;
            this->labelinfo1->Text = L"Enter a stock or price range (e.g., 40-50) or a single value (e.g., 45) for filte"
                L"ring";
            this->labelinfo1->Visible = false;
            // 
            // timer1
            // 
            this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1608, 515);
            this->Controls->Add(this->labelinfo1);
            this->Controls->Add(this->buttonQuestionMark_Click1);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->LabelCategoryadd2);
            this->Controls->Add(this->buttonLowStockReport);
            this->Controls->Add(this->dataGridViewItems);
            this->Controls->Add(this->buttonListItems);
            this->Controls->Add(this->buttonUpdateItem);
            this->Controls->Add(this->buttonDeleteItem);
            this->Controls->Add(this->buttonSearchItem);
            this->Controls->Add(this->buttonAddItem);
            this->Controls->Add(this->comboBoxCategories);
            this->Controls->Add(this->textBoxUnitPrice);
            this->Controls->Add(this->textBoxQuantity);
            this->Controls->Add(this->textBoxItemName);
            this->Controls->Add(this->buttonAddCategory);
            this->Controls->Add(this->textBoxCategoryName);
            this->Name = L"MyForm";
            this->Text = L"Product Management";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewItems))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }

        // Database initialization and table creation function
    private: System::Void InitializeDatabase() {
        try {
            // The LocalDB connection string works dynamically on any computer.
            String^ masterConnectionString = "Server=(localdb)\\MSSQLLocalDB;Database=master;Integrated Security=True;";
            SqlConnection^ masterConnection = gcnew SqlConnection(masterConnectionString);

            masterConnection->Open();

            // Command to create the database with the new name
            String^ createDatabaseQuery = "IF NOT EXISTS (SELECT * FROM sys.databases WHERE name = 'ProductDatabaseMuhammedKemalYagci') CREATE DATABASE ProductDatabaseMuhammedKemalYagci;";
            SqlCommand^ createDatabaseCommand = gcnew SqlCommand(createDatabaseQuery, masterConnection);
            createDatabaseCommand->ExecuteNonQuery();

            // Retrieve the file path of the newly created database
            String^ getFilePathQuery = "SELECT physical_name FROM sys.master_files WHERE database_id = DB_ID('ProductDatabaseMuhammedKemalYagci') AND type = 0;";
            SqlCommand^ getFilePathCommand = gcnew SqlCommand(getFilePathQuery, masterConnection);
            String^ databaseFilePath = (String^)getFilePathCommand->ExecuteScalar();

            masterConnection->Close();

            // Connecting to the newly created ProductDatabaseMuhammedKemalYagci
            SqlConnection^ connection = gcnew SqlConnection("Server=(localdb)\\MSSQLLocalDB;Database=ProductDatabaseMuhammedKemalYagci;Integrated Security=True;");
            connection->Open();

            // Creating the Categories table
            String^ createCategoriesTableQuery = "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'Categories') "
                "CREATE TABLE Categories (CategoryID INT PRIMARY KEY IDENTITY(1,1), CategoryName NVARCHAR(50));";
            SqlCommand^ createCategoriesTableCommand = gcnew SqlCommand(createCategoriesTableQuery, connection);
            createCategoriesTableCommand->ExecuteNonQuery();

            // Creating the Items table
            String^ createItemsTableQuery = "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'Items') "
                "CREATE TABLE Items (ItemID INT PRIMARY KEY IDENTITY(1,1), ItemName NVARCHAR(100), "
                "CategoryID INT FOREIGN KEY REFERENCES Categories(CategoryID), Quantity INT, "
                "UnitPrice DECIMAL(10,2), LastUpdated DATETIME DEFAULT GETDATE());";
            SqlCommand^ createItemsTableCommand = gcnew SqlCommand(createItemsTableQuery, connection);
            createItemsTableCommand->ExecuteNonQuery();

            // **Creating the InsertLog table** (for logging insert operations)
            String^ createInsertLogTableQuery = "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'InsertLog') "
                "CREATE TABLE InsertLog (LogID INT PRIMARY KEY IDENTITY(1,1), ItemName NVARCHAR(100), "
                "InsertedBy NVARCHAR(50), InsertDate DATETIME DEFAULT GETDATE());";
            SqlCommand^ createInsertLogTableCommand = gcnew SqlCommand(createInsertLogTableQuery, connection);
            createInsertLogTableCommand->ExecuteNonQuery();

            connection->Close();

            // Informational message about the database creation with file path
            MessageBox::Show("Database 'ProductDatabaseMuhammedKemalYagci' created successfully. File path: " + databaseFilePath);
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error occurred while creating the database: " + ex->Message);
        }
    }











    private: System::Void buttonQuestionMark_Click1_Click(System::Object^ sender, System::EventArgs^ e) {
        labelinfo1->Visible = true;  // Make the Label visible
        timer1->Interval = 5000;    // Set timer to 3 seconds (5000 milliseconds = 5 seconds)
        timer1->Start();            // Start the timer
    }
    private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
        labelinfo1->Visible = false;  // Hide the Label again
        timer1->Stop();              // Stop the timer
    }
    };
}
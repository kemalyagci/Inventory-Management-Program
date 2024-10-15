#include "LoginForm.h"
#include "MyForm.h"  // Import the main form

using namespace System;
using namespace System::Windows::Forms;

void Project13::LoginForm::buttonLogin_Click(System::Object^ sender, System::EventArgs^ e)
{
    String^ correctUsername = "muhammed";  // Correct username
    String^ correctPassword = "kemal";     // Correct password

    String^ enteredUsername = textBoxUsername->Text;  // Get username from the TextBox
    String^ enteredPassword = textBoxPassword->Text;  // Get password from the TextBox

    // If login information is correct
    if (enteredUsername == correctUsername && enteredPassword == correctPassword)
    {
        MessageBox::Show("Login successful! Welcome, " + correctUsername + ".");

        // Open the main form when login is successful
        this->Hide();  // Hide the LoginForm
        MyForm^ mainForm = gcnew MyForm();  // Create the main form
        mainForm->ShowDialog();  // Show the main form
        this->Close();  // Close the LoginForm
    }
    else
    {
        // If login information is incorrect
        MessageBox::Show("Incorrect username or password.");
    }
}

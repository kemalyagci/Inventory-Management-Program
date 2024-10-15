#include "LoginForm.h"
#include "MyForm.h"  // Import the main form

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // First, we open the login form
    Project13::LoginForm loginForm;

    // If login is successful, LoginForm will return OK
    if (loginForm.ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        // Login successful, opening the main form
        Project13::MyForm^ mainForm = gcnew Project13::MyForm(); // Use gcnew to allocate managed memory
        Application::Run(mainForm); // Pass the form pointer here
    }

    return 0;
}

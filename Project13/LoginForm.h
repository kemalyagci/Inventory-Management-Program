#pragma once

namespace Project13 {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class LoginForm : public System::Windows::Forms::Form
    {
    public:
        LoginForm(void)
        {
            InitializeComponent();
        }

    protected:
        ~LoginForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::TextBox^ textBoxUsername;
        System::Windows::Forms::TextBox^ textBoxPassword;
        System::Windows::Forms::Button^ buttonLogin;
    private: System::Windows::Forms::Label^ usernamelabel;
    private: System::Windows::Forms::Label^ passwordlabel;
    private: System::Windows::Forms::Label^ infolabel;
           System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
           void InitializeComponent(void)
           {
               this->textBoxUsername = (gcnew System::Windows::Forms::TextBox());
               this->textBoxPassword = (gcnew System::Windows::Forms::TextBox());
               this->buttonLogin = (gcnew System::Windows::Forms::Button());
               this->usernamelabel = (gcnew System::Windows::Forms::Label());
               this->passwordlabel = (gcnew System::Windows::Forms::Label());
               this->infolabel = (gcnew System::Windows::Forms::Label());
               this->SuspendLayout();
               // 
               // textBoxUsername
               // 
               this->textBoxUsername->Location = System::Drawing::Point(159, 42);
               this->textBoxUsername->Name = L"textBoxUsername";
               this->textBoxUsername->Size = System::Drawing::Size(150, 22);
               this->textBoxUsername->TabIndex = 0;
               this->textBoxUsername->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &LoginForm::textBox_KeyDown);
               // 
               // textBoxPassword
               // 
               this->textBoxPassword->Location = System::Drawing::Point(159, 81);
               this->textBoxPassword->Name = L"textBoxPassword";
               this->textBoxPassword->Size = System::Drawing::Size(150, 22);
               this->textBoxPassword->TabIndex = 1;
               this->textBoxPassword->UseSystemPasswordChar = true;
               this->textBoxPassword->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &LoginForm::textBox_KeyDown);
               // 
               // buttonLogin
               // 
               this->buttonLogin->Location = System::Drawing::Point(209, 138);
               this->buttonLogin->Name = L"buttonLogin";
               this->buttonLogin->Size = System::Drawing::Size(100, 23);
               this->buttonLogin->TabIndex = 2;
               this->buttonLogin->Text = L"Login";
               this->buttonLogin->UseVisualStyleBackColor = true;
               this->buttonLogin->Click += gcnew System::EventHandler(this, &LoginForm::buttonLogin_Click);
               // 
               // usernamelabel
               // 
               this->usernamelabel->AutoSize = true;
               this->usernamelabel->Location = System::Drawing::Point(55, 42);
               this->usernamelabel->Name = L"usernamelabel";
               this->usernamelabel->Size = System::Drawing::Size(73, 16);
               this->usernamelabel->TabIndex = 3;
               this->usernamelabel->Text = L"Username:";
               // 
               // passwordlabel
               // 
               this->passwordlabel->AutoSize = true;
               this->passwordlabel->Location = System::Drawing::Point(58, 84);
               this->passwordlabel->Name = L"passwordlabel";
               this->passwordlabel->Size = System::Drawing::Size(70, 16);
               this->passwordlabel->TabIndex = 4;
               this->passwordlabel->Text = L"Password:";
               // 
               // infolabel
               // 
               this->infolabel->AutoSize = true;
               this->infolabel->Location = System::Drawing::Point(25, 237);
               this->infolabel->Name = L"infolabel";
               this->infolabel->Size = System::Drawing::Size(253, 16);
               this->infolabel->TabIndex = 5;
               this->infolabel->Text = L"Username: muhammed Password: kemal";
               // 
               // LoginForm
               // 
               this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
               this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
               this->ClientSize = System::Drawing::Size(429, 292);
               this->Controls->Add(this->infolabel);
               this->Controls->Add(this->passwordlabel);
               this->Controls->Add(this->usernamelabel);
               this->Controls->Add(this->buttonLogin);
               this->Controls->Add(this->textBoxPassword);
               this->Controls->Add(this->textBoxUsername);
               this->Name = L"LoginForm";
               this->Text = L"Login";
               this->ResumeLayout(false);
               this->PerformLayout();

           }
#pragma endregion

    private:
        System::Void buttonLogin_Click(System::Object^ sender, System::EventArgs^ e);

        // Function to handle the KeyDown event for Enter key
        System::Void textBox_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
            if (e->KeyCode == Keys::Enter) {
                // Call the login button click event when Enter is pressed
                buttonLogin_Click(sender, e);
            }
        }
    };
}

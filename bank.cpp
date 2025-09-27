#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
using namespace std;

class account
{
    int acno;
    char name[50];
    int deposit;
    char type;
    int loanAmount;

public:
    void create_account();
    void show_account() const;
    void modify();
    void dep(int);
    void draw(int);
    void report() const;
    int retacno() const;
    int retdeposit() const;
    char rettype() const;
    int retLoanAmount() const;
    void applyLoan(int amount);
    void repayLoan(int amount);
};

void account::create_account()
{
    cout << "\nEnter The account No. :";
    cin >> acno;
    cout << "\n\nEnter The Name of The account Holder : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\nEnter Type of The account (C/S) : ";
    cin >> type;
    type = toupper(type);
    cout << "\nEnter The Initial amount(>=500 for Saving and >=1000 for current ) : ";
    cin >> deposit;
    loanAmount = 0;
    cout << "\n\n\nAccount Created..";
}

void account::show_account() const
{
    cout << "\nAccount No. : " << acno;
    cout << "\nAccount Holder Name : " << name;
    cout << "\nType of Account : " << type;
    cout << "\nBalance amount : " << deposit;
    cout << "\nLoan amount : " << loanAmount;
}

void account::modify()
{
    cout << "\nAccount No. : " << acno;
    cout << "\nModify Account Holder Name : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\nModify Type of Account : ";
    cin >> type;
    type = toupper(type);
    cout << "\nModify Balance amount : ";
    cin >> deposit;
}

void account::dep(int x)
{
    deposit += x;
}

void account::draw(int x)
{
    deposit -= x;
}

void account::report() const
{
    cout << acno << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << setw(10) << loanAmount << endl;
}

int account::retacno() const
{
    return acno;
}

int account::retdeposit() const
{
    return deposit;
}

char account::rettype() const
{
    return type;
}

int account::retLoanAmount() const
{
    return loanAmount;
}

void account::applyLoan(int amount)
{
    loanAmount += amount;
}

void account::repayLoan(int amount)
{
    loanAmount -= amount;
}

void write_account()
{
    account ac;
    ofstream outFile;
    outFile.open("account.dat", ios::binary | ios::app);
    ac.create_account();
    outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
    outFile.close();
}

void display_sp(int n)
{
    account ac;
    bool flag = false;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    cout << "\nBALANCE DETAILS\n";

    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account)))
    {
        if (ac.retacno() == n)
        {
            ac.show_account();
            flag = true;
        }
    }
    inFile.close();
    if (flag == false)
        cout << "\n\nAccount number does not exist";
}

void modify_account(int n)
{
    bool found = false;
    account ac;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File)
    {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    while (File.read(reinterpret_cast<char *>(&ac), sizeof(account)) && found == false)
    {
        if (ac.retacno() == n)
        {
            ac.show_account();
            cout << "\n\nEnter The New Details of account" << endl;
            ac.modify();
            int pos = (-1) * static_cast<int>(sizeof(account));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(account));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }
    File.close();
    if (found == false)
        cout << "\n\n Record Not Found ";
}

void delete_account(int n)
{
    account ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    outFile.open("Temp.dat", ios::binary);
    inFile.seekg(0, ios::beg);
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account)))
    {
        if (ac.retacno() != n)
        {
            outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");
    cout << "\n\n\tRecord Deleted ..";
}

void display_all()
{
    account ac;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.      NAME           Type  Balance   Loan\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account)))
    {
        ac.report();
    }
    inFile.close();
}

void deposit_withdraw(int n, int option)
{
    int amt;
    bool found = false;
    account ac;
    fstream File;

    File.open("account.dat", ios::binary | ios::in | ios::out);

    if (!File)
    {
        cout << "File could not be open !! Press any Key...";
        return;
    }

    while (File.read(reinterpret_cast<char *>(&ac), sizeof(account)) && found == false)
    {
        if (ac.retacno() == n)
        {
            ac.show_account();

            if (option == 1)
            {
                cout << "\n\n\tTO DEPOSIT AMOUNT ";
                cout << "\n\nEnter The amount to be deposited: ";
                cin >> amt;
                ac.dep(amt);
            }
            else if (option == 2)
            {
                cout << "\n\n\tTO WITHDRAW AMOUNT ";
                cout << "\n\nEnter The amount to be withdrawn: ";
                cin >> amt;

                int bal = ac.retdeposit() - amt;
                if ((bal < 500 && ac.rettype() == 'S') || (bal < 1000 && ac.rettype() == 'C'))
                {
                    cout << "Insufficiency balance";
                }
                else
                {
                    ac.draw(amt);
                }
            }
            else if (option == 3)
            {
                cout << "\n\n\tAPPLY FOR A LOAN ";
                cout << "\n\nEnter The amount of loan to be applied for: ";
                cin >> amt;
                ac.applyLoan(amt);
            }
            else if (option == 4)
            {
                cout << "\n\n\tREPAY LOAN ";
                cout << "\n\nEnter The amount to be repaid: ";
                cin >> amt;
                ac.repayLoan(amt);
            }

            int pos = (-1) * static_cast<int>(sizeof(ac));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(account));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }

    File.close();

    if (found == false)
    {
        cout << "\n\n Record Not Found ";
    }
}

void intro()
{
    cout << "\n\n\n\t                            BANK MANAGEMENT SYSTEM";
    cout << "\n\n\n\n\t                            MADE BY : ISHAN SHARMA";
    cin.get();
}

int main()
{
    char ch;
    int num;
    intro();
    do
    {
        cout << "\n\n\n\tMAIN MENU";
        cout << "\n\n\t01. NEW ACCOUNT";
        cout << "\n\n\t02. DEPOSIT AMOUNT";
        cout << "\n\n\t03. WITHDRAW AMOUNT";
        cout << "\n\n\t04. BALANCE ENQUIRY";
        cout << "\n\n\t05. ALL ACCOUNT HOLDER LIST";
        cout << "\n\n\t06. CLOSE AN ACCOUNT";
        cout << "\n\n\t07. MODIFY AN ACCOUNT";
        cout << "\n\n\t08. APPLY FOR A LOAN";
        cout << "\n\n\t09. REPAY LOAN";
        cout << "\n\n\t0. EXIT";
        cout << "\n\n\tSelect Your Option (1-10) ";
        cin >> ch;

        switch (ch)
        {
        case '1':
            write_account();
            break;
        case '2':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            deposit_withdraw(num, 1);
            break;
        case '3':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            deposit_withdraw(num, 2);
            break;
        case '4':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            display_sp(num);
            break;
        case '5':
            display_all();
            break;
        case '6':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            delete_account(num);
            break;
        case '7':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            modify_account(num);
            break;
        case '8':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            deposit_withdraw(num, 3);
            break;
        case '9':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            deposit_withdraw(num, 4);
            break;
        case '0':
            cout << "\n\n\tThanks for using the bank management system";
            break;
        default:
            cout << "\a";
        }
        cin.ignore();
        cin.get();
    } while (ch != '0');

    return 0;
}

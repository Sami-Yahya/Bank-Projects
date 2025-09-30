// Same previous after adding QuickWithdraw screen.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

void ShowMainMenu();
void ShowTransactionsMenu();
void ShowQuickWithdrawScreen();
void ShowManageUsersMenus();
void Login();

const string ClientsFileName = "ClientsFile.txt";
const string UsersFileName = "Users.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;

    bool MarkForDelete = false;
};

struct sUsers
{
    string UserName;
    string Password;
    int Permissions;

    bool MarkForDelete = false;
};

enum enMainMenuOptions { eShowClientList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eTransactions = 6, eManageUsers = 7, eLogout = 8 };

enum enTransactionsMenuOptions { eDeposit = 1, eQuickWithdraw = 2, eWithdraw = 3, eTotalBalances = 4, eBackMainMenu = 5 };

enum enManageUsersOptions { eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eBackMainMenuFromManageScreen = 6 };

enum enUserPermissions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClient = 8, pFindClient = 16, pTransactions = 32,
    pManageUsers = 64
};

bool CheckAccessPermission(enUserPermissions Permission);

sUsers CurrentUser;

vector<string> SplitString(string Text, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable

    // use find() function to get the position of delimiters
    while ((pos = Text.find(Delim)) != std::string::npos)
    {
        sWord = Text.substr(0, pos); // store the word
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        Text.erase(0, pos + Delim.length()); // erase() until position and move to next word
    }

    if (Text != "")
    {
        vString.push_back(Text); // it prints last word of the string
    }

    return vString;
}

sClient ConvertLineToRecord(string Record, string Delim)
{
    sClient Client;

    vector<string> vClientData = SplitString(Record, Delim);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Delim = "###")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Delim;
    stClientRecord += Client.PinCode + Delim;
    stClientRecord += Client.Name + Delim;
    stClientRecord += Client.Phone + Delim;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

vector<sClient> LoadClientDataFromFile(string FileName)
{
    vector<sClient> vClients;

    fstream File;
    File.open(FileName, ios::in); //read mode

    if (File.is_open())
    {
        string Line;
        sClient Client;

        while (getline(File, Line))
        {
            Client = ConvertLineToRecord(Line, "/-/");
            vClients.push_back(Client);
        }

        File.close();
    }

    return vClients;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::in); //read mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line, "/-/");

            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();
    }

    return false;
}

sClient ReadClientData()
{
    sClient Client;

    cout << "Please Enter Account Number: ";
    // Usage of std::ws will extract all the whilespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with account number [" << Client.AccountNumber << "] is already exists, Please enter another Account Number: ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Please Enter Pin Code: ";
    getline(cin, Client.PinCode);

    cout << "Please Enter Name: ";
    getline(cin, Client.Name);

    cout << "Please Enter Phone: ";
    getline(cin, Client.Phone);

    cout << "Please Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

void SaveDataLineToFile(string FileName, string sLine)
{
    fstream File;

    File.open(FileName, ios::out | ios::app); //append mode

    if (File.is_open())
    {
        File << sLine << endl;

        File.close();
    }
}

string WhatAccountToFind()
{
    string toFind;

    cout << "Please enter Account Number? ";
    cin >> toFind;

    return toFind;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the Client details: \n\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "Pin Code       : " << Client.PinCode << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& foundClient)
{
    for (sClient& client : vClients)
    {
        if (AccountNumber == client.AccountNumber)
        {
            foundClient = client;
            return true;
        }
    }

    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Please Enter Pin Code: ";
    getline(cin >> ws, Client.PinCode);

    cout << "Please Enter Name: ";
    getline(cin, Client.Name);

    cout << "Please Enter Phone: ";
    getline(cin, Client.Phone);

    cout << "Please Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

void SaveClientsDataToFile(string FileName, vector<sClient> vClients)
{
    //he used vector<sClient> istead of void

    fstream File;
    File.open(FileName, ios::out); //overwrite

    if (File.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete
                File << ConvertRecordToLine(C, "/-/") << endl;
            }
        }

        File.close();
    }

    // return vClients;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

bool DeleteClientsByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'N';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want to delete this client? [Y/N]: ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);

            //refresh clients
            vClients = LoadClientDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully." << endl;
            return true;
        }
    }
    else
    {
        cout << "\nClient with account number (" << AccountNumber << ") is Not Found!" << endl;
        return false;
    }
}

bool UpdateClientsByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'N';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want to update this client? [Y/N]: ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveClientsDataToFile(ClientsFileName, vClients);


            cout << "\n\nClient Updated Successfully." << endl;
            return true;
        }
    }
    else
    {
        cout << "\nClient with account number (" << AccountNumber << ") is Not Found!" << endl;
        return false;
    }
}

void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<sClient> vClients)
{
    cout << "\n\t\t\t\tClients List (" << vClients.size() << ") Client(s).";
    cout << "\n------------------------------------------------------";
    cout << "-----------------------------------------\n";

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n------------------------------------------------------";
    cout << "-----------------------------------------\n";

    if (vClients.size() == 0)
    {
        cout << "\n\t\t\tNo Clients Available in the System!" << endl;
    }
    else
    {
        for (sClient& client : vClients)
        {
            PrintClientRecord(client);
            cout << endl;
        }
    }

    cout << "\n------------------------------------------------------";
    cout << "-----------------------------------------\n";
}

void AddNewClient()
{
    sClient Client;
    string sLine;

    Client = ReadClientData();
    sLine = ConvertRecordToLine(Client, "/-/");

    SaveDataLineToFile(ClientsFileName, sLine);

}

bool DepositBalanceByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClients)
{
    char Answer = 'N';

    cout << "\n\nAre you sure you want to perform this transaction? [Y/N]: ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New Balance is: " << C.AccountBalance;

                return true;
            }
        }

        return false;
    }
}

short ReadQuickWithdrawOption()
{
    short Choice = 0;
    do
    {
        cout << "\nChoose what to do from [1] to [9] ? ";
        cin >> Choice;

    } while (Choice < 1 || Choice > 9);

    return Choice;
}

short getQuickWithdrawAmount(short QuickWithdrawChoice)
{
    switch (QuickWithdrawChoice)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

void PerformQuickWithdrawOption(short QuickWithdrawChoice)
{
    if (QuickWithdrawChoice == 9) //exit
        return;

    sClient Client;
    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    string AccountNumber = WhatAccountToFind();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist! \n";
        AccountNumber = WhatAccountToFind();
    }

    PrintClientCard(Client);

    short QuickWithdrawAmount = getQuickWithdrawAmount(QuickWithdrawChoice);

    if (QuickWithdrawAmount > Client.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press any key to continue...\n";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    DepositBalanceByAccountNumber(Client.AccountNumber, QuickWithdrawAmount * -1, vClients);
}

void PrintClientsBalances(sClient Client)
{
    cout << "| " << setw(16) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

double SumBalances()
{
    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    double SumB = 0;

    for (sClient& C : vClients)
    {
        SumB += C.AccountBalance;
    }

    return SumB;
}

void ShowAccessDeniedMesseage()
{
    cout << "--------------------------------------------\n";
    cout << "Access Denied, \nYou don't have Permission to do this,";
    cout << "\nPlease contact your Admin.\n";
    cout << "--------------------------------------------\n";
}

void ShowAllClientListScreen()
{
    if (!CheckAccessPermission(enUserPermissions::pListClients))
    {
        ShowAccessDeniedMesseage();
        return;
    }

    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);

    PrintAllClientsData(vClients);
}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enUserPermissions::pAddNewClient))
    {
        ShowAccessDeniedMesseage();
        return;
    }

    cout << "--------------------------------------------\n";
    cout << "\t\tAdd New Client\n";
    cout << "--------------------------------------------\n";

    char AddMore = 'Y';

    do
    {
        cout << "Adding New Client: \n\n";
        AddNewClient();

        cout << "\nClient Added Successfully, Do you want to add more? [Y/N]: ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enUserPermissions::pDeleteClient))
    {
        ShowAccessDeniedMesseage();
        return;
    }

    cout << "--------------------------------------------\n";
    cout << "\t\tDelete Client\n";
    cout << "--------------------------------------------\n";

    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    string AccountNumberToDelete = WhatAccountToFind();

    DeleteClientsByAccountNumber(AccountNumberToDelete, vClients);

}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enUserPermissions::pFindClient))
    {
        ShowAccessDeniedMesseage();
        return;
    }

    cout << "--------------------------------------------\n";
    cout << "\t\tFind Client\n";
    cout << "--------------------------------------------\n";

    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumberToFind = WhatAccountToFind();

    if (FindClientByAccountNumber(AccountNumberToFind, vClients, Client))
    {
        PrintClientCard(Client);
    }
    else
    {
        cout << "\nClient with account number (" << AccountNumberToFind << ") is Not Found!" << endl;
    }

}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enUserPermissions::pUpdateClient))
    {
        ShowAccessDeniedMesseage();
        return;
    }

    cout << "--------------------------------------------\n";
    cout << "\t\tUpdate Client\n";
    cout << "--------------------------------------------\n";

    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    string AccountNumberToUpdate = WhatAccountToFind();

    UpdateClientsByAccountNumber(AccountNumberToUpdate, vClients);
}

void ShowExitSreen()
{
    cout << "--------------------------------------------\n";
    cout << "\t\tProgram Ends :-)\n";
    cout << "--------------------------------------------\n";
}

void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "--------------------------------------------\n";

    cout << "\t[1] 20  \t[2] 50\n";
    cout << "\t[3] 100 \t[4] 200\n";
    cout << "\t[5] 400 \t[6] 600\n";
    cout << "\t[7] 800 \t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "--------------------------------------------\n";

    PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

void ShowDepositScreen()
{
    cout << "--------------------------------------------\n";
    cout << "\t\tDeposit\n";
    cout << "--------------------------------------------\n";

    sClient Client;
    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    string AccountNumber = WhatAccountToFind();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist! \n";
        AccountNumber = WhatAccountToFind();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount: ";
    cin >> Amount;

    DepositBalanceByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithdrawScreen()
{
    cout << "--------------------------------------------\n";
    cout << "\t\tWithdraw\n";
    cout << "--------------------------------------------\n";

    sClient Client;
    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    string AccountNumber = WhatAccountToFind();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist! \n";
        AccountNumber = WhatAccountToFind();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount: ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw uo to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount: ";
        cin >> Amount;
    }

    DepositBalanceByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void ShowTotalBalancesScreen()
{
    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);

    cout << "\n\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n------------------------------------------------------";
    cout << "----------------------\n";

    cout << "| " << left << setw(16) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n------------------------------------------------------";
    cout << "----------------------\n";

    if (vClients.size() == 0)
    {
        cout << "\n\t\t\tNo Clients Available in the System!" << endl;
    }
    else
    {
        for (sClient& client : vClients)
        {
            PrintClientsBalances(client);
            cout << endl;
        }
    }

    cout << "\n------------------------------------------------------";
    cout << "----------------------\n";

    cout << "\n\t\t\tTotal Balances = " << SumBalances() << endl;
}

short ReadTransactionsOption()
{
    short Choice = 0;
    do
    {
        cout << "Choose what do you want to do? [1 to 5]: ";
        cin >> Choice;

    } while (Choice < 1 || Choice > 5);

    return Choice;
}

void GoBackToTransactionsMenu()
{
    cout << "\n\nPress any key to go back to Transactions Menu... ";
    system("pause>0");
    ShowTransactionsMenu();
}

void PerformTransactionsMenuOption(enTransactionsMenuOptions TransactionsMenuOptions)
{
    switch (TransactionsMenuOptions)
    {
    case::enTransactionsMenuOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case::enTransactionsMenuOptions::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case::enTransactionsMenuOptions::eWithdraw:
    {
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case::enTransactionsMenuOptions::eTotalBalances:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case::enTransactionsMenuOptions::eBackMainMenu:
    {
        system("cls");
        ShowMainMenu();
        break;
    }
    }
}

void ShowTransactionsMenu()
{
    if (!CheckAccessPermission(enUserPermissions::pTransactions))
    {
        ShowAccessDeniedMesseage();
        return;
    }

    system("cls");
    cout << "==============================================\n";
    cout << "\t   Transactions Menu Screen\n";
    cout << "==============================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Quick Withdraw.\n";
    cout << "\t[3] Withdraw.\n";
    cout << "\t[4] Total Balances.\n";
    cout << "\t[5] Go Back to Main Menu.\n";
    cout << "==============================================\n";

    PerformTransactionsMenuOption((enTransactionsMenuOptions)ReadTransactionsOption());
}

sUsers ConvertUserLineToRecord(string Record, string Delim)
{
    sUsers User;

    vector<string> vUserData = SplitString(Record, Delim);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}

string ConvertUserRecordToLine(sUsers User, string Delim = "###")
{
    string stUserRecord = "";

    stUserRecord += User.UserName + Delim;
    stUserRecord += User.Password + Delim;
    stUserRecord += to_string(User.Permissions);

    return stUserRecord;
}

vector<sUsers> LoadUsersDataFromFile(string FileName)
{
    vector<sUsers> vUsers;

    fstream File;
    File.open(FileName, ios::in); //read mode

    if (File.is_open())
    {
        string Line;
        sUsers User;

        while (getline(File, Line))
        {
            User = ConvertUserLineToRecord(Line, "/-/");
            vUsers.push_back(User);
        }

        File.close();
    }

    return vUsers;
}

bool UserExistsByUsername(string Username, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::in); //read mode

    if (MyFile.is_open())
    {
        string Line;
        sUsers User;

        while (getline(MyFile, Line))
        {
            User = ConvertUserLineToRecord(Line, "/-/");

            if (User.UserName == Username)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();
    }

    return false;
}

int ReadPermissionToSet()
{
    int Permission = 0;
    char Answer = 'N';

    cout << "\nDo you want to give full access? [Y/N]: ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to: \n";

    cout << "\nShow Client List? [Y/N]: ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enUserPermissions::pListClients;
    }

    cout << "\nAdd New Client? [Y/N]: ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enUserPermissions::pAddNewClient;
    }

    cout << "\nDelete Client? [Y/N]: ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enUserPermissions::pDeleteClient;
    }

    cout << "\nUpdate Client? [Y/N]: ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enUserPermissions::pUpdateClient;
    }

    cout << "\nFind Client? [Y/N]: ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enUserPermissions::pFindClient;
    }

    cout << "\nTransactions? [Y/N]: ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enUserPermissions::pTransactions;
    }

    cout << "\nManage Users? [Y/N]: ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enUserPermissions::pManageUsers;
    }

    return Permission;
}

sUsers ReadUserData()
{
    sUsers User;

    cout << "Please Enter User Name: ";
    // Usage of std::ws will extract all the whilespace character
    getline(cin >> ws, User.UserName);

    while (UserExistsByUsername(User.UserName, UsersFileName))
    {
        cout << "\nUser with user name [" << User.UserName << "] is already exists, Please enter another User Name: ";
        getline(cin >> ws, User.UserName);
    }

    cout << "Please Enter Password: ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissionToSet();

    return User;
}

string WhatUserToFind()
{
    string toFind;

    cout << "Please enter Username? ";
    cin >> toFind;

    return toFind;
}

void PrintUserCard(sUsers User)
{
    cout << "\nThe following are the User details: \n\n";
    cout << "Username    : " << User.UserName << endl;
    cout << "Password    : " << User.Password << endl;
    cout << "Permissions : " << User.Permissions << endl;
}

bool FindUserByUsername(string Username, vector<sUsers> vUsers, sUsers& foundUser)
{
    for (sUsers& user : vUsers)
    {
        if (Username == user.UserName)
        {
            foundUser = user;
            return true;
        }
    }

    return false;
}

sUsers ChangeUserRecord(string UserName)
{
    sUsers User;

    User.UserName = UserName;

    cout << "Please Enter Password: ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionToSet();

    return User;
}

void SaveUsersDataToFile(string FileName, vector<sUsers> vUsers)
{
    //he used vector<sClient> istead of void

    fstream File;
    File.open(FileName, ios::out); //overwrite

    if (File.is_open())
    {
        for (sUsers U : vUsers)
        {
            if (U.MarkForDelete == false)
            {
                //we only write records that are not marked for delete
                File << ConvertUserRecordToLine(U, "/-/") << endl;
            }
        }

        File.close();
    }

    // return vClients;
}

bool MarkUserForDeleteByUsername(string Username, vector<sUsers>& vUsers)
{
    for (sUsers& U : vUsers)
    {
        if (U.UserName == Username)
        {
            U.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

bool DeleteUsersByUsername(string Username, vector<sUsers>& vUsers)
{
    if (Username == "Admin")
    {
        cout << "\n\nYou cannot delete this User!";
        return false;
    }

    sUsers User;
    char Answer = 'N';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);

        cout << "\n\nAre you sure you want to delete this user? [Y/N]: ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            MarkUserForDeleteByUsername(Username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            //refresh clients
            vUsers = LoadUsersDataFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully." << endl;
            return true;
        }
    }
    else
    {
        cout << "\nUser with user name (" << Username << ") is Not Found!" << endl;
        return false;
    }
}

bool UpdateUsersByUsername(string Username, vector<sUsers>& vUsers)
{
    sUsers User;
    char Answer = 'N';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);

        cout << "\n\nAre you sure you want to update this user? [Y/N]: ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (sUsers& U : vUsers)
            {
                if (U.UserName == Username)
                {
                    U = ChangeUserRecord(Username);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);


            cout << "\n\nUser Updated Successfully." << endl;
            return true;
        }
    }
    else
    {
        cout << "\nUser with user name (" << Username << ") is Not Found!" << endl;
        return false;
    }
}

void PrintUserRecord(sUsers User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(15) << left << User.Password;
    cout << "| " << setw(30) << left << User.Permissions;
}

void PrintAllUsersData(vector<sUsers> vUsers)
{
    cout << "\n\t\t\t\tUser List (" << vUsers.size() << ") User(s).";
    cout << "\n------------------------------------------------------";
    cout << "-----------------------------------------\n";

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(15) << "Password";
    cout << "| " << left << setw(30) << "Permissions";

    cout << "\n------------------------------------------------------";
    cout << "-----------------------------------------\n";

    if (vUsers.size() == 0)
    {
        cout << "\n\t\t\tNo Users Available in the System!" << endl;
    }
    else
    {
        for (sUsers& user : vUsers)
        {
            PrintUserRecord(user);
            cout << endl;
        }
    }

    cout << "\n------------------------------------------------------";
    cout << "-----------------------------------------\n";
}

void AddNewUser()
{
    sUsers User;
    string sLine;

    User = ReadUserData();
    sLine = ConvertUserRecordToLine(User, "/-/");

    SaveDataLineToFile(UsersFileName, sLine);
}

void ListUsersScreen()
{
    vector <sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

    PrintAllUsersData(vUsers);
}

void AddNewUserScreen()
{
    cout << "--------------------------------------------\n";
    cout << "\t\tAdd New User\n";
    cout << "--------------------------------------------\n";

    char AddMore = 'N';

    do
    {
        cout << "Adding New User: \n\n";
        AddNewUser();

        cout << "\nUser Added Successfully, Do you want to add more? [Y/N]: ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void DeleteUserScreen()
{
    cout << "--------------------------------------------\n";
    cout << "\t\tDelete User\n";
    cout << "--------------------------------------------\n";

    vector <sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UsernameToDelete = WhatUserToFind();

    DeleteUsersByUsername(UsernameToDelete, vUsers);
}

void UpdateUserScreen()
{
    cout << "--------------------------------------------\n";
    cout << "\t\tUpdate User\n";
    cout << "--------------------------------------------\n";

    vector <sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UsernameToUpdate = WhatUserToFind();

    UpdateUsersByUsername(UsernameToUpdate, vUsers);
}

void FindUserScreen()
{
    cout << "--------------------------------------------\n";
    cout << "\t\tFind User\n";
    cout << "--------------------------------------------\n";

    vector <sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    sUsers foundUser;
    string UsernameToFind = WhatUserToFind();

    if (FindUserByUsername(UsernameToFind, vUsers, foundUser))
    {
        PrintUserCard(foundUser);
    }
    else
    {
        cout << "\nUser with user name (" << UsernameToFind << ") is Not Found!" << endl;
    }
}

bool CheckAccessPermission(enUserPermissions Permission)
{
    if (CurrentUser.Permissions == enUserPermissions::eAll)
    {
        return true;
    }

    if ((Permission & CurrentUser.Permissions) == Permission)
    {
        return true;
    }
    else
    {
        return false;
    }
}

enManageUsersOptions ReadManageUsersMenu()
{
    short Choice = 0;
    do
    {
        cout << "Choose what do you want to do? [1 to 6]: ";
        cin >> Choice;

    } while (Choice < 1 || Choice > 6);

    return (enManageUsersOptions)Choice;
}

void GoBackToManageUsersMenu()
{
    cout << "\n\nPress any key to go back to Manage Users Menu... ";
    system("pause>0");
    ShowManageUsersMenus();
}

void PerformManageUsersMenu(enManageUsersOptions ManageUsersMenu)
{
    switch(ManageUsersMenu)
    {
    case::enManageUsersOptions::eListUsers:
    {
        system("cls");
        ListUsersScreen();
        GoBackToManageUsersMenu();
        break;
    }
    case::enManageUsersOptions::eAddNewUser:
    {
        system("cls");
        AddNewUserScreen();
        GoBackToManageUsersMenu();
        break;
    }
    case::enManageUsersOptions::eDeleteUser:
    {
        system("cls");
        DeleteUserScreen();
        GoBackToManageUsersMenu();
        break;
    }
    case::enManageUsersOptions::eUpdateUser:
    {
        system("cls");
        UpdateUserScreen();
        GoBackToManageUsersMenu();
        break;
    }
    case::enManageUsersOptions::eFindUser:
    {
        system("cls");
        FindUserScreen();
        GoBackToManageUsersMenu();
        break;
    }
    case::enManageUsersOptions::eBackMainMenuFromManageScreen:
    {
        system("cls");
        ShowMainMenu();
        break;
    }
    }
}

void ShowManageUsersMenus()
{
    if (!CheckAccessPermission(enUserPermissions::pManageUsers))
    {
        ShowAccessDeniedMesseage();
        return;
    }

    system("cls");
    cout << "==============================================\n";
    cout << "\t   Manage Users Menu Screen\n";
    cout << "==============================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Go Back to Main Menu.\n";
    cout << "==============================================\n";

    PerformManageUsersMenu(ReadManageUsersMenu());
}

enMainMenuOptions ReadMainMenuOption()
{
    short Choice = 0;
    do
    {
        cout << "Choose what do you want to do? [1 to 8]: ";
        cin >> Choice;

    } while (Choice < 1 || Choice > 8);

    return (enMainMenuOptions)Choice;
}

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu... ";
    system("pause>0");
    ShowMainMenu();
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case::enMainMenuOptions::eShowClientList:
    {
        system("cls");
        ShowAllClientListScreen();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eAddNewClient:
    {
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eTransactions:
    {
        system("cls");
        ShowTransactionsMenu();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eManageUsers:
    {
        system("cls");
        ShowManageUsersMenus();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eLogout:
    {
        system("cls");
       // ShowExitSreen();
        Login();
        break;
    }
    }
}

void ShowMainMenu()
{
    system("cls");
    cout << "==============================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "==============================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "==============================================\n";

    PerformMainMenuOption(ReadMainMenuOption());
}

bool IsCorrectPassword(string Username, string Password, sUsers &User)
{
    vector<sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUsers &U : vUsers)
    {
        if (U.UserName == Username && U.Password == Password)
        {
            User = U;
            return true;
        }
    }

    return false;
}

bool CheckUserAndPassword(string Username, string Password)
{
    if (IsCorrectPassword(Username, Password, CurrentUser))
    {
        return true;
    }

    return false;
}

void Login()
{
    string UserName, Password;
    bool LoginFailed = false;

    do
    {
        system("cls");

        cout << "--------------------------------------------\n";
        cout << "\t\tLogin Screen\n";
        cout << "--------------------------------------------\n";

        if(LoginFailed)
            cout << "Invalid Username/Password!\n";

        cout << "Please enter User Name: ";
        cin >> UserName;
        cout << "Please enter Password: ";
        cin >> Password;

        LoginFailed = !CheckUserAndPassword(UserName, Password);

    } while (LoginFailed);
    
    ShowMainMenu();
}

int main() {

    Login();


    // system("pause > 0");

    return 0;
}

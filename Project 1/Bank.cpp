#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include "MyLibrary.h"
using namespace std;

void ShowMainMenu();

const string ClientsFileName = "ClientsFile.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;

    bool MarkForDelete = false;
};

enum enMainMenuOptions { eShowClientList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eExit = 6 };

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

string ConvertRecordToLine(sClient Client, string Delim = "##")
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
            Client = ConvertLineToRecord(Line, "--");
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
            Client = ConvertLineToRecord(Line, "--");

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
                File << ConvertRecordToLine(C, "--") << endl;
            }
        }

        File.close();
    }

   // return vClients;
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
                File << ConvertRecordToLine(C, "--") << endl;
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

    for (sClient& client : vClients)
    {
        PrintClientRecord(client);
        cout << endl;
    }

    cout << "\n------------------------------------------------------";
    cout << "-----------------------------------------\n";
}

void AddNewClient()
{
    sClient Client;
    string sLine;

    Client = ReadClientData();
    sLine = ConvertRecordToLine(Client, "--");

    SaveDataLineToFile(ClientsFileName, sLine);

}

void ShowAllClientListScreen()
{
    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);

    PrintAllClientsData(vClients);
}

void ShowAddNewClientsScreen()
{
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
    cout << "--------------------------------------------\n";
    cout << "\t\tDelete Client\n";
    cout << "--------------------------------------------\n";

    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    string AccountNumberToDelete = WhatAccountToFind();

    DeleteClientsByAccountNumber(AccountNumberToDelete, vClients);

}

void ShowFindClientScreen()
{
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

enMainMenuOptions ReadMainMenuOption()
{
    short Choice = 0;
    do
    {
        cout << "Choose what do you want to do? [1 to 6]: ";
        cin >> Choice;

    } while (Choice < 1 || Choice > 6);

    return (enMainMenuOptions)Choice;
}

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...\n";
    system("pause>0");
    ShowMainMenu();
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch(MainMenuOption)
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
    case::enMainMenuOptions::eExit:
    {
        system("cls");
        ShowExitSreen();
        break;
    }
    }
}

void ShowMainMenu()
{
    system("cls");
    cout << "=============================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "=============================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exit.\n";
    cout << "=============================================\n";

    PerformMainMenuOption(ReadMainMenuOption());
}

int main() {

    ShowMainMenu();


    system("pause > 0");

    return 0;
}

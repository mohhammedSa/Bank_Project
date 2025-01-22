#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const string FileName = "Clients.txt";

string readAccountNumber()
{
    string accNumber;
    cout << "Please Enter account number: ";
    cin >> accNumber;
    return accNumber;
}

enum MainMenuOptions
{
    eShowList = 1,
    eAdd = 2,
    eDelete = 3,
    eUpdate = 4,
    eFind = 5,
    eTransactionsMenu = 6,
    eExit = 7
};

enum TransactionsMenuOptions
{
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalances = 3,
    eExitTransactionsMenu = 4
};

struct sClient
{
    string accountNumber;
    string pinCode;
    string name;
    string phoneNumber;
    double Balance;
};

void ShowMainMenu();

void TransactionsMenuScreen();

sClient FindClient(string accNumber);

bool isExists(string accountNumber);

vector<string> Split(string line, string delim = "#//#")
{
    vector<string> VDataStrings;
    short pos = 0;
    string word;

    while ((pos = line.find(delim)) != line.npos)
    {
        word = line.substr(0, pos);
        if (word != "")
        {
            VDataStrings.push_back(word);
            line.erase(0, pos + delim.length());
        }
    }

    if (line != "")
    {
        VDataStrings.push_back(line);
    }
    return VDataStrings;
}

sClient ReadClientInfo(string accountNumber)
{
    sClient Client;

    Client.accountNumber = accountNumber;

    cout << "Enter PinCode: ";
    getline(cin >> ws, Client.pinCode);

    cout << "Enter name: ";
    getline(cin, Client.name);

    cout << "Enter Phone number: ";
    getline(cin, Client.phoneNumber);

    cout << "Enter balane: ";
    cin >> Client.Balance;

    return Client;
}

sClient ConvertLineToRecord(string line)
{
    sClient Client;
    vector<string> vData = Split(line);

    Client.accountNumber = vData[0];
    Client.pinCode = vData[1];
    Client.name = vData[2];
    Client.phoneNumber = vData[3];
    Client.Balance = stod(vData[4]);
    return Client;
}

string ConvertRecordToLine(sClient client, string delim = "#//#")
{
    return client.accountNumber + delim + client.pinCode + delim +
           client.name + delim + client.phoneNumber + delim + to_string(client.Balance);
}

vector<sClient> LoadClientsFromFile(string filename)
{
    vector<sClient> vClients;

    fstream MyFile;
    MyFile.open(filename, ios::in);

    if (MyFile.is_open())
    {
        string line;
        while (getline(MyFile, line))
        {
            vClients.push_back(ConvertLineToRecord(line));
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecord(sClient Client)
{
    cout << "\nThe following are the client details: \n";
    cout << "----------------------------------------\n";
    cout << "Account number: " << Client.accountNumber << "\n";
    cout << "PinCode: " << Client.pinCode << "\n";
    cout << "Name: " << Client.name << "\n";
    cout << "Phone: " << Client.phoneNumber << "\n";
    cout << "Balance: " << Client.Balance << "\n";
    cout << "----------------------------------------\n\n";
}

void ShowClientsScreen()
{
    vector<sClient> vClients = LoadClientsFromFile(FileName);

    cout << "\n\t\t\tClient List (" << vClients.size() << ") Client" << (vClients.size() > 1 ? "s" : "") << "\n";
    cout << "---------------------------------------------------------------------------------\n";
    cout << "|" << setw(20) << left << "Account Number"
         << "|" << setw(10) << left << "PinCode"
         << "|" << setw(20) << left << "Name"
         << "|" << setw(10) << left << "Phone"
         << "|" << setw(10) << left << "Balance" << endl;
    cout << "---------------------------------------------------------------------------------\n";

    for (sClient &client : vClients)
    {
        cout << "|" << setw(20) << client.accountNumber
             << "|" << setw(10) << client.pinCode
             << "|" << setw(20) << client.name
             << "|" << setw(10) << client.phoneNumber
             << "|" << setw(10) << client.Balance << "\n";
    }
    cout << "---------------------------------------------------------------------------------\n";
}

void AddClientToFile(sClient Client)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << ConvertRecordToLine(Client) << "\n";
        MyFile.close();
    }
}

void AddNewClientScreen()
{
    cout << "----------------------------\n";
    cout << "\tAdd new client screen\n";
    cout << "----------------------------";

    char answer = 'y';
    while (toupper(answer) == 'Y')
    {
        cout << "\nAdding new client: \n\n";
        string accNumber = readAccountNumber();

        while (isExists(accNumber))
        {
            cout << "Client with [" << accNumber << "] already Exists, ";
            accNumber = readAccountNumber();
        }

        sClient Client = ReadClientInfo(accNumber);
        AddClientToFile(Client);
        cout << "Client Added Successfully, do you want to add more Clients? Y/N: ";
        cin >> answer;
    }
}

void ClearFile(string fileName)
{
    fstream File;
    File.open(fileName, ios::out);

    if (File.is_open())
    {
        File << "";
        File.close();
    }
}

void SaveDataToFile(vector<sClient> vClient)
{
    ClearFile(FileName);
    for (sClient C : vClient)
    {
        AddClientToFile(C);
    }
}

void DeleteClientFromFile(vector<sClient> vClient, sClient Client)
{
    ClearFile(FileName);
    for (sClient &C : vClient)
    {
        if (Client.accountNumber != C.accountNumber)
        {
            AddClientToFile(C);
        }
    }
};

void DeleteClientScreen()
{
    cout << "---------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "---------------------------------\n\n";

    vector<sClient> vClient = LoadClientsFromFile(FileName);

    string accNumber = readAccountNumber();

    if (isExists(accNumber))
    {
        sClient Client = FindClient(accNumber);
        PrintClientRecord(Client);

        char answer;
        cout << "\nAre you sure you want to delete this Client? Y/N: ";
        cin >> answer;

        if (toupper(answer) == 'Y')
            DeleteClientFromFile(vClient, Client);
    }
    else
        cout << "Client with account number [" << accNumber << "], does not exists.\n";
}

void UpdateClientInfo(vector<sClient> &vClient, sClient &Client)
{
    for (sClient &C : vClient)
    {
        if (C.accountNumber == Client.accountNumber)
        {
            C = Client;
            break;
        }
    }
}

void UpdateClientScreen()
{
    cout << "---------------------------------\n";
    cout << "\t\t Update Client Screen\n";
    cout << "---------------------------------\n\n";

    vector<sClient> vClients = LoadClientsFromFile(FileName);

    string accNumber = readAccountNumber();
    if (isExists(accNumber))
    {
        sClient Client = FindClient(accNumber);
        PrintClientRecord(Client);

        char answer;
        cout << "\nAre you sure you want to Update this Client? Y/N: ";
        cin >> answer;

        if (toupper(answer) == 'Y')
        {
            Client = ReadClientInfo(accNumber);
            UpdateClientInfo(vClients, Client);
            SaveDataToFile(vClients);
        }
    }
    else
    {
        cout << "Client with account number [" << accNumber << "], does not exists.\n";
    }
}

bool isExists(string accountNumber)
{
    vector<sClient> vClients = LoadClientsFromFile(FileName);
    for (sClient &Client : vClients)
    {
        if (Client.accountNumber == accountNumber)
        {
            return true;
        }
    }
    return false;
}

sClient FindClient(string accNumber)
{
    vector<sClient> vClients = LoadClientsFromFile(FileName);
    sClient client;

    if (isExists(accNumber))
    {
        for (sClient &C : vClients)
        {
            if (C.accountNumber == accNumber)
            {
                return C;
            }
        }
    }
    return client;
}

void FindClientScreen()
{
    cout << "===========================\n";
    cout << "\tFind Client Screen\n";
    cout << "===========================\n\n";

    string accNumber = readAccountNumber();
    if (isExists(accNumber))
    {
        sClient sClient = FindClient(accNumber);
        PrintClientRecord(sClient);
    }
    else
        cout << "\nClient with account number [" << accNumber << "], does not exists.\n";
}

void ExitApp() {};

bool Withdraw(int amount, sClient &Client)
{
    vector<sClient> vClients = LoadClientsFromFile(FileName);

    if (Client.Balance < amount)
    {
        return false;
    }
    else
    {
        Client.Balance -= amount;
        UpdateClientInfo(vClients, Client);
        SaveDataToFile(vClients);
        return true;
    }
}

void WithDrawScreen()
{
    cout << "====================================\n";
    cout << "\tWithdraw Screen\n";
    cout << "====================================\n";
    string accNumber = readAccountNumber();
    if (isExists(accNumber))
    {
        sClient Client = FindClient(accNumber);
        PrintClientRecord(Client);

        int amount;
        cout << "Enter withdraw amount: ";
        cin >> amount;

        if (Withdraw(amount, Client))
            PrintClientRecord(Client);
        else
            cout << "\nAccount Balance is " << Client.Balance << " , it is not enough for the withdraw.\n";
    }
    else
        cout << "\nClient with account number [" << accNumber << "], does not exists.\n";
}

void Deposit(int amount, sClient &Client)
{
    vector<sClient> vClients = LoadClientsFromFile(FileName);

    Client.Balance += amount;

    UpdateClientInfo(vClients, Client);
    SaveDataToFile(vClients);
}

void DepositScreen()
{
    cout << "====================================\n";
    cout << "\tDeposit Screen\n";
    cout << "====================================\n";

    string accNumber = readAccountNumber();
    if (isExists(accNumber))
    {
        sClient Client = FindClient(accNumber);
        PrintClientRecord(Client);

        int amount = 0;
        cout << "Enter deposit amount: ";
        cin >> amount;

        Deposit(amount, Client);
        PrintClientRecord(Client);
    }
    else
        cout << "Client with account number [" << accNumber << "], does not exists.\n";
}

double TotalBalances(vector<sClient> vClients)
{
    double total = 0;
    for (sClient Client : vClients)
    {
        total += Client.Balance;
    }
    return total;
}

void TotalBalancesScreen()
{

    vector<sClient> vClients = LoadClientsFromFile(FileName);

    cout << "\n\t\t\tBalances List (" << vClients.size() << ") Client" << (vClients.size() > 1 ? "s" : "") << "\n";
    cout << "---------------------------------------------------------------------------------\n";
    cout << "|" << setw(20) << left << "Account Number"
         << "|" << setw(20) << left << "Name"
         << "|" << setw(10) << left << "Balance" << endl;
    cout << "---------------------------------------------------------------------------------\n";

    if (vClients.size() == 0)
    {
        cout << "\t\tNo Client Available in the system\n";
        cout << "---------------------------------------------------------------------------------\n";
    }
    else
    {

        for (sClient &client : vClients)
        {
            cout << "|" << setw(20) << client.accountNumber
                 << "|" << setw(20) << client.name
                 << "|" << setw(10) << client.Balance << "\n";
        }
        cout << "---------------------------------------------------------------------------------\n";
        cout << setw(75) << right << "Total Balances: " << TotalBalances(vClients) << "\n";
    }
}

void ExitTransMenu() {}

void ReturnToTransactionsMenu()
{
    cout << "\nPress any key to go back to transactions menu..." << flush;
    system("bash -c 'read'");
    TransactionsMenuScreen();
}

TransactionsMenuOptions ReadTransactinMenuOption()
{
    cout << "Choose What Do you want to do? [1 -> 4]: ";
    short answer;
    cin >> answer;
    return (TransactionsMenuOptions)answer;
}

void PerformTransactionsOptions(TransactionsMenuOptions TransactionOption)
{
    switch (TransactionOption)
    {
    case TransactionsMenuOptions::eDeposit:
        system("clear");
        DepositScreen();
        ReturnToTransactionsMenu();
        break;

    case TransactionsMenuOptions::eWithdraw:
        system("clear");
        WithDrawScreen();
        ReturnToTransactionsMenu();
        break;

    case TransactionsMenuOptions::eTotalBalances:
        system("clear");
        TotalBalancesScreen();
        ReturnToTransactionsMenu();
        break;

    default:
        ExitTransMenu();
        break;
    }
}

void TransactionsMenuScreen()
{
    system("clear");
    cout << "====================================\n";
    cout << "\tTransactions Menu Screen\n";
    cout << "====================================\n";
    cout << "[1] Deposit.\n";
    cout << "[2] Withdraw.\n";
    cout << "[3] Total Balaces\n";
    cout << "[4] MainMenu.\n";
    cout << "====================================\n";
    PerformTransactionsOptions(ReadTransactinMenuOption());
}

void ReturnToMainMenu()
{
    cout << "Press any key to go back to main menu..." << flush;
    system("bash -c 'read'");
    ShowMainMenu();
}

MainMenuOptions ReadMainMenuOption()
{
    cout << "Choose What do yo want to do? [1 to 7] : ";
    short Option;
    cin >> Option;
    return MainMenuOptions(Option);
}

void PerformMainMenuOption(MainMenuOptions Option)
{

    switch (Option)
    {
    case MainMenuOptions::eShowList:
        system("clear");
        ShowClientsScreen();
        ReturnToMainMenu();
        break;

    case MainMenuOptions::eAdd:
        system("clear");
        AddNewClientScreen();
        ReturnToMainMenu();
        break;

    case MainMenuOptions::eDelete:
        system("clear");
        DeleteClientScreen();
        ReturnToMainMenu();
        break;

    case MainMenuOptions::eUpdate:
        system("clear");
        UpdateClientScreen();
        ReturnToMainMenu();
        break;

    case MainMenuOptions::eFind:
        system("clear");
        FindClientScreen();
        ReturnToMainMenu();
        break;

    case MainMenuOptions::eTransactionsMenu:
        system("clear");
        TransactionsMenuScreen();
        ShowMainMenu();
        break;

    default:
        ExitApp();
        break;
    }
}

void ShowMainMenu()
{
    system("clear");
    cout << "====================================\n";
    cout << "\tMain Menu Screen\n";
    cout << "====================================\n";
    cout << "[1] Show Clients List.\n";
    cout << "[2] Add new client.\n";
    cout << "[3] Delete Client\n";
    cout << "[4] Update Client Info.\n";
    cout << "[5] Find Client.\n";
    cout << "[6] Transactions.\n";
    cout << "[7] Exit.\n";
    cout << "====================================\n";
    PerformMainMenuOption(ReadMainMenuOption());
}

int main()
{
    ShowMainMenu();
    return 0;
}
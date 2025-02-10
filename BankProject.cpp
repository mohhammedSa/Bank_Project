#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const string FileName = "Clients.txt";
const string UserFileName = "Users.txt";

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
    eManageUsers = 7,
    eLogout = 8
};

enum TransactionsMenuOptions
{
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalances = 3,
    eExitTransactionsMenu = 4
};

enum eManageUsersOptions
{
    eListUsers = 1,
    eAddUser = 2,
    eDeleteUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eExitManageUser = 6
};

struct sClient
{
    string accountNumber;
    string pinCode;
    string name;
    string phoneNumber;
    double Balance;
};

struct sUserInfo
{
    string username;
    string password;
    short Permissions;
};

enum ePermissions
{
    allP = -1,
    listClientP = 1,
    AddClientP = 2,
    deleteClientP = 4,
    updateClientP = 8,
    findClientP = 16,
    transactionP = 32,
    manageUsersP = 64
};

void ReturnToMainMenu();

void ShowMainMenu();

void TransactionsMenuScreen();

void ManageUsersScreen();

void LoginScreen();

sClient FindClient(string accNumber);

bool isClientExists(string accountNumber);

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

sUserInfo CurrentUsser;

bool CheckPermission(short UserPermissions, ePermissions Permission)
{
    return (UserPermissions == ePermissions::allP || UserPermissions & Permission);
}

void ShowClientsScreen()
{
    vector<sClient> vClients = LoadClientsFromFile(FileName);

    if (!(CheckPermission(CurrentUsser.Permissions, ePermissions::listClientP)))
    {
        cout << "----------------------------\n";
        cout << "  Show clients list screen\n";
        cout << "----------------------------\n";
        cout << "Access Denied!\n";
        cout << "----------------------\n";
    }
    else
    {
        cout << "\n\t\t\tClient List (" << vClients.size() << ") Client" << (vClients.size() > 1 ? "s" : "") << "\n";
        cout << "---------------------------------------------------------------------------------\n";
        cout << "|" << setw(20) << left << "Account Number"
             << "|" << setw(10) << left << "PinCode"
             << "|" << setw(20) << left << "Name"
             << "|" << setw(10) << left << "Phone"
             << "|" << setw(10) << left << "Balance" << endl;
        cout << "---------------------------------------------------------------------------------\n";

        if (vClients.size() == 0)
        {
            cout << "\t\t\tNo data available...\n";
        }
        else
        {
            for (sClient &client : vClients)
            {
                cout << "|" << setw(20) << client.accountNumber
                     << "|" << setw(10) << client.pinCode
                     << "|" << setw(20) << client.name
                     << "|" << setw(10) << client.phoneNumber
                     << "|" << setw(10) << client.Balance << "\n";
            }
        }
        cout << "---------------------------------------------------------------------------------\n";
    }
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
    cout << "------------------------------------\n";
    cout << "\tAdd new client screen\n";
    cout << "------------------------------------\n";

    if (!(CheckPermission(CurrentUsser.Permissions, ePermissions::AddClientP)))
    {
        cout << "Access Denied!\n";
        cout << "----------------------\n";
    }
    else
    {
        char answer = 'y';
        while (toupper(answer) == 'Y')
        {
            cout << "\nAdding new client: \n\n";
            string accNumber = readAccountNumber();

            while (isClientExists(accNumber))
            {
                cout << "Client with [" << accNumber << "] already Exists, ";
                accNumber = readAccountNumber();
            }

            sClient Client = ReadClientInfo(accNumber);
            AddClientToFile(Client);
            cout << "\nClient Added Successfully, do you want to add more Clients? Y/N: ";
            cin >> answer;
        }
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
    cout << "------------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "------------------------------------\n";

    vector<sClient> vClient = LoadClientsFromFile(FileName);

    if (!(CheckPermission(CurrentUsser.Permissions, ePermissions::deleteClientP)))
    {
        cout << "Access Denied!\n";
        cout << "----------------------\n";
    }
    else
    {
        string accNumber = readAccountNumber();

        if (isClientExists(accNumber))
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
    cout << "------------------------------------\n";
    cout << "\tUpdate Client Screen\n";
    cout << "------------------------------------\n";

    vector<sClient> vClients = LoadClientsFromFile(FileName);

    if (!(CheckPermission(CurrentUsser.Permissions, ePermissions::updateClientP)))
    {
        cout << "Access Denied!\n";
        cout << "----------------------\n";
    }
    else
    {

        string accNumber = readAccountNumber();
        if (isClientExists(accNumber))
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
}

bool isClientExists(string accountNumber)
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

    if (isClientExists(accNumber))
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
    cout << "------------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "------------------------------------\n";

    if (!(CheckPermission(CurrentUsser.Permissions, ePermissions::findClientP)))
    {
        cout << "Access Denied!\n";
        cout << "----------------------\n";
    }
    else
    {
        string accNumber = readAccountNumber();
        if (isClientExists(accNumber))
        {
            sClient sClient = FindClient(accNumber);
            PrintClientRecord(sClient);
        }
        else
            cout << "\nClient with account number [" << accNumber << "], does not exists.\n";
    }
}

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
    if (isClientExists(accNumber))
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
    if (isClientExists(accNumber))
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

void ExitTransMenu()
{
    ShowMainMenu();
}

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
    cout << "------------------------------------\n";
    cout << "    Transactions Menu Screen\n";
    cout << "------------------------------------\n";

    if (!(CheckPermission(CurrentUsser.Permissions, ePermissions::transactionP)))
    {
        cout << "Access Denied!\n";
        cout << "----------------------\n";
        ReturnToMainMenu();
    }
    else
    {
        cout << "[1] Deposit.\n";
        cout << "[2] Withdraw.\n";
        cout << "[3] Total Balaces\n";
        cout << "[4] MainMenu.\n";
        cout << "====================================\n";
        PerformTransactionsOptions(ReadTransactinMenuOption());
    }
}

string ReadUserName()
{
    string username;
    cout << "\nEnter username: ";
    getline(cin >> ws, username);
    return username;
}

short ReadPermissions()
{
    short Permissions;
    cout << "\nDo you want to give full access? y/n: ";
    char answer;
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
    {
        return -1;
    }

    cout << "\ndo you want to give access to: \n";
    cout << "\nShow Client List? y/n: ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        Permissions += ePermissions::listClientP;
    }

    cout << "\nAdd new Client? y/n: ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        Permissions += ePermissions::AddClientP;
    }

    cout << "\nDelete Client? y/n: ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        Permissions += ePermissions::deleteClientP;
    }

    cout << "\nUpdate Client? y/n: ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        Permissions += ePermissions::updateClientP;
    }

    cout << "\nFind Client? y/n: ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        Permissions += ePermissions::findClientP;
    }

    cout << "\nTransactions? y/n: ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        Permissions += ePermissions::transactionP;
    }

    cout << "\nManage Users? y/n: ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        Permissions += ePermissions::manageUsersP;
    }

    return Permissions;
}

string ConvertUserInfoToString(sUserInfo userInfo)
{
    return userInfo.username + "#//#" + userInfo.password + "#//#" + to_string(userInfo.Permissions);
}

sUserInfo ConvertUserInfoLineToUserInfo(string userInfoLine)
{
    sUserInfo UserInfo;

    vector<string> vUserInfo;
    vUserInfo = Split(userInfoLine);

    UserInfo.username = vUserInfo[0];
    UserInfo.password = vUserInfo[1];
    UserInfo.Permissions = stoi(vUserInfo[2]);

    return UserInfo;
}

vector<sUserInfo> LoadUsersFromFile(string UserFileName)
{
    vector<sUserInfo> vUsersInfo;

    fstream UserFile;
    UserFile.open(UserFileName, ios::in);

    if (UserFile.is_open())
    {
        string line;
        while (getline(UserFile, line))
        {
            vUsersInfo.push_back(ConvertUserInfoLineToUserInfo(line));
        }
        UserFile.close();
    }
    return vUsersInfo;
}

bool isUserExist(string username, string userFileName)
{
    vector<sUserInfo> vUsers = LoadUsersFromFile(UserFileName);
    for (sUserInfo user : vUsers)
    {
        if (user.username == username)
        {
            return true;
        }
    }
    return false;
}

bool isUserExist(string username, string password, string userFileName)
{
    vector<sUserInfo> vUsers = LoadUsersFromFile(UserFileName);
    for (sUserInfo user : vUsers)
    {
        if (user.username == username && user.password == password)
        {
            return true;
        }
    }
    return false;
}

sUserInfo ReadUserInfo()
{
    sUserInfo UserInfo;

    cout << "\nAdd user:\n";
    cout << "Enter username: ";
    cin >> UserInfo.username;

    while (isUserExist(UserInfo.username, UserFileName))
    {
        cout << "User with username " << UserInfo.username << " is already exist.\n";
        cout << "Enter username: ";
        cin >> UserInfo.username;
    }

    cout << "Enter password: ";
    cin >> UserInfo.password;
    UserInfo.Permissions = ReadPermissions();

    return UserInfo;
}

void AddUserToFile(sUserInfo userInfo)
{
    fstream UserFile;
    UserFile.open(UserFileName, ios::out | ios::app);
    if (UserFile.is_open())
    {
        UserFile << ConvertUserInfoToString(userInfo) << "\n";
        UserFile.close();
    }
}

void GoBackToManageUsersScreen()
{
    cout << "Press any key to go back to manage users menu..." << flush;
    system("bash -c 'read'");
    ManageUsersScreen();
}

void ListUserScreen()
{
    vector<sUserInfo> vUsers = LoadUsersFromFile(UserFileName);

    cout << "\n----------------------------------------\n";
    cout << "\tUsers List (" << vUsers.size() << ") User" << ((vUsers.size() > 1) ? "s" : "");
    cout << "\n----------------------------------------\n";

    cout << "----------------------------------------\n";
    cout << "|" << left << setw(15) << "User Name" << "|" << left << setw(10) << "Password" << "|" << left << setw(12) << "Permissions";
    cout << "\n----------------------------------------\n";
    if (vUsers.size() == 0)
    {
        cout << "\tThere no data available...";
    }
    else
    {
        for (int i = 0; i < vUsers.size(); i++)
        {
            cout << "|" << setw(15) << vUsers[i].username << "|" << setw(10) << vUsers[i].password << "|" << setw(12) << vUsers[i].Permissions << "\n";
        }
    }

    cout << "----------------------------------------\n";
}

sUserInfo FindUser(string username, string filename)
{
    vector<sUserInfo> vUsers = LoadUsersFromFile(UserFileName);
    sUserInfo UserInfo;

    if (isUserExist(username, filename))
    {
        for (sUserInfo user : vUsers)
        {
            if (user.username == username)
            {
                return user;
            }
        }
    }
    cout << "----------------------------------------\n";
    cout << "|" << left << setw(15) << "User Name" << "|" << left << setw(10) << "Password" << "|" << left << setw(12) << "Permissions";
    cout << "\n----------------------------------------\n";
    if (vUsers.size() == 0)
    {
        cout << "\tThere no data available...";
    }
    else
    {
        for (int i = 0; i < vUsers.size(); i++)
        {
            cout << "|" << setw(15) << vUsers[i].username << "|" << setw(10) << vUsers[i].password << "|" << setw(12) << vUsers[i].Permissions << "\n";
        }
    }

    cout << "----------------------------------------\n";
    return UserInfo;
}

void PrintUserInfo(sUserInfo UserInfo)
{
    cout << "\nThe following are the user details:";
    cout << "\n------------------------\n";
    cout << "Username   : " << UserInfo.username << "\n";
    cout << "Password   : " << UserInfo.password << "\n";
    cout << "Permissions: " << UserInfo.Permissions << "\n";
    cout << "------------------------\n";
}

void FindUserScreen()
{
    cout << "====================================\n";
    cout << "\tFind User Screen\n";
    cout << "====================================\n";

    sUserInfo User;
    string username = ReadUserName();

    if (isUserExist(username, UserFileName))
    {
        cout << "User exist.\n";
        User = FindUser(username, UserFileName);
        PrintUserInfo(User);
    }
    else
        cout << "User does not exixt.\n";
}

void AddUserScreen()
{
    cout << "====================================\n";
    cout << "\tAdd User Screen\n";
    cout << "====================================\n";

    char answer = 'y';
    while (answer == 'y' || answer == 'Y')
    {
        sUserInfo UserInfo = ReadUserInfo();
        AddUserToFile(UserInfo);
        cout << "\nClient Added Successfuly, do you want to add more users? y/n:";
        cin >> answer;
    }
}

void DeletUserFromFile(vector<sUserInfo> vUsers, sUserInfo User)
{
    ClearFile(UserFileName);

    for (int i = 0; i < vUsers.size(); i++)
    {
        if (vUsers[i].username != User.username)
        {
            AddUserToFile(vUsers[i]);
        }
    }
}

void DeleteUserScreen()
{
    cout << "====================================\n";
    cout << "\tDelete User Screen\n";
    cout << "====================================\n";

    char answer;

    vector<sUserInfo> vUsers = LoadUsersFromFile(UserFileName);
    string username = ReadUserName();

    while (!isUserExist(username, UserFileName))
    {
        cout << "User with username '" << username << "' does not exist.\n";
        cout << "You want to delete another user: y/n: ";
        cin >> answer;

        if (tolower(answer) == 'y')
            username = ReadUserName();
        else
            return;
    }

    sUserInfo User = FindUser(username, UserFileName);
    PrintUserInfo(User);

    cout << "Are you sure you want to delete " << User.username << " : y/n : ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
    {
        if (User.username == "admin")
        {
            cout << "Can't delete admin\n";
            return;
        }

        DeletUserFromFile(vUsers, User);
        cout << "User deleted successfully.\n";
    }
}

void SaveUsersToFile(vector<sUserInfo> vUsers)
{
    ClearFile(UserFileName);
    for (sUserInfo &User : vUsers)
    {
        AddUserToFile(User);
    }
}

sUserInfo ReadUpdateUserInfo(sUserInfo &User)
{
    cout << "Enter password: ";
    cin >> User.password;
    User.Permissions = ReadPermissions();
    return User;
}

void UpdateUserInfo(vector<sUserInfo> &vUsers, sUserInfo User)
{
    for (sUserInfo &U : vUsers)
    {
        if (U.username == User.username)
        {
            U = User;
            break;
        }
    }
}

void UpdateUserScreen()
{
    cout << "====================================\n";
    cout << "\tUpdate User Screen\n";
    cout << "====================================\n";

    char updateAgain = 'y';

    while (tolower(updateAgain) == 'y')
    {
        vector<sUserInfo> vUsers = LoadUsersFromFile(UserFileName);
        string username = ReadUserName();
        char answer;

        while (!isUserExist(username, UserFileName))
        {
            cout << "User with username '" << username << "' does not exist.\n";
            cout << "You want to update another user's info: y/n: ";
            cin >> answer;

            if (tolower(answer) == 'y')
                username = ReadUserName();
            else
                return;
        }

        sUserInfo User = FindUser(username, UserFileName);
        PrintUserInfo(User);

        cout << "Are you sure you want to Update " << User.username << "'s Info : y / n : ";
        cin >> answer;

        if ((answer == 'y' || answer == 'Y') && User.username != "admin")
        {
            ReadUpdateUserInfo(User);
            UpdateUserInfo(vUsers, User);
            SaveUsersToFile(vUsers);
            cout << "\nUser Updated successfully.\n\n";
            PrintUserInfo(User);
        }

        cout << "\nDo you want to update another user: ";
        cin >> updateAgain;
    }
}

void ExitManageUserScreen()
{
    ShowMainMenu();
}

eManageUsersOptions ReadManageUsersOption()
{
    cout << "Choose what yo want to do? [1 -> 6]: ";
    short option;
    cin >> option;
    return (eManageUsersOptions)option;
}

void PerformManageUsers(eManageUsersOptions option)
{
    switch (option)
    {
    case eManageUsersOptions::eListUsers:
        system("clear");
        ListUserScreen();
        GoBackToManageUsersScreen();
        break;

    case eManageUsersOptions::eAddUser:
        system("clear");
        AddUserScreen();
        GoBackToManageUsersScreen();
        break;

    case eManageUsersOptions::eDeleteUser:
        system("clear");
        DeleteUserScreen();
        GoBackToManageUsersScreen();
        break;

    case eManageUsersOptions::eUpdateUser:
        system("clear");
        UpdateUserScreen();
        GoBackToManageUsersScreen();
        break;

    case eManageUsersOptions::eFindUser:
        system("clear");
        FindUserScreen();
        GoBackToManageUsersScreen();
        break;

    case eManageUsersOptions::eExitManageUser:
        system("clear");
        ExitManageUserScreen();
        break;

    default:
        break;
    }
}

void ManageUsersScreen()
{
    cout << "====================================\n";
    cout << "\tManage Users Screen\n";
    cout << "====================================\n";

    if (!(CheckPermission(CurrentUsser.Permissions, ePermissions::manageUsersP)))
    {
        cout << "Access Denied!\n";
        cout << "----------------------\n";
        ReturnToMainMenu();
    }
    else
    {
        cout << "[1] List Users.\n";
        cout << "[2] Add User.\n";
        cout << "[3] Delete USer.\n";
        cout << "[4] Update User.\n";
        cout << "[5] Find User.\n";
        cout << "[6] Main Menu.\n";
        cout << "====================================\n";
        PerformManageUsers(ReadManageUsersOption());
    }
}

void ReturnToMainMenu()
{
    cout << "Press any key to go back to main menu..." << flush;
    system("bash -c 'read'");
    ShowMainMenu();
}

MainMenuOptions ReadMainMenuOption()
{
    cout << "Choose What do yo want to do? [1 to 8] : ";
    short Option;
    cin >> Option;
    return MainMenuOptions(Option);
}

void Logout()
{
    system("clear");
    LoginScreen();
};

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
        break;

    case MainMenuOptions::eManageUsers:
        system("clear");
        ManageUsersScreen();
        break;

    default:
        Logout();
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
    cout << "[7] Manage Users.\n";
    cout << "[8] Logout.\n";
    cout << "====================================\n";
    PerformMainMenuOption(ReadMainMenuOption());
}

void LoginScreen()
{
    bool loginFailed = false;
    string username, password;

    do
    {
        system("clear");
        cout << "====================================\n";
        cout << "\t   Login Screen.\n";
        cout << "====================================\n";

        if (loginFailed)
        {
            cout << "Username/password incorrect\n\n";
        }

        username = ReadUserName();
        cout << "Enter password: ";
        cin >> password;
        loginFailed = !isUserExist(username, password, UserFileName);

    } while (loginFailed);

    CurrentUsser = FindUser(username, UserFileName);
    ShowMainMenu();
}

int main()
{
    LoginScreen();
    cout << endl;
    return 0;
}

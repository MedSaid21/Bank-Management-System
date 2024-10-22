#pragma once
#include <iostream>
#include <string>
#include "clsPerson.h"
#include "clsString.h"
#include <vector>
#include "clsUser.h";
#include "clsDate.h"
#include "Global.h"
#include <fstream> 

class clsBankClient : public clsPerson
{

private:

    enum enMode { EmptyMode = 0, UpdateMode = 1 , AddNewMode=2 }; 
    enMode _Mode;
    string _AccountNumber;
    string _PinCode;
    float _AccountBalance;
    bool _MarkForDelete = false; 

    static clsBankClient _ConvertLinetoClientObject(string Line, string Seperator = "#//#")
    {
        vector<string> vClientData;
        vClientData = clsString::Split(Line, Seperator);

        return clsBankClient(enMode::UpdateMode, vClientData[0], vClientData[1], vClientData[2],
            vClientData[3], vClientData[4], vClientData[5], stod(vClientData[6]));

    }
    static string _ConvertClientObjectToLine(clsBankClient Client,string Seperator = "#//#") {
        string DataLine = "";
        DataLine += Client.FirstName + Seperator;
        DataLine += Client.LastName + Seperator;
        DataLine += Client.Email + Seperator;
        DataLine += Client.Phone + Seperator;
        DataLine += Client.AccountNumber() + Seperator;
        DataLine += Client.PinCode + Seperator;
        DataLine += to_string(Client.AccountBalance);
        return DataLine;  
    }
    static clsBankClient _GetEmptyClientObject()
    {
        return clsBankClient(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }
    static vector<clsBankClient> _LoadClientDataFromFile() {

        vector<clsBankClient> vClients; 
        fstream MyFile;
        MyFile.open("Clients.txt", ios::in);
        if (MyFile.is_open()) {
            string Line;
            while (getline(MyFile, Line)) {
                clsBankClient Client = _ConvertLinetoClientObject(Line); 
                vClients.push_back(Client); 
            }
            MyFile.close(); 
        }

        return vClients; 
            
    }
    void _SaveClientsDataToFile(vector<clsBankClient> vClients) {
        fstream MyFile;
        MyFile.open("Clients.txt", ios::out);
        string Line;
        if (MyFile.is_open()) {
            for (clsBankClient& C : vClients) {
                if (C._MarkForDelete == false) {
                    Line = _ConvertClientObjectToLine(C); 
                    MyFile << Line << endl; 
                } 
                
            }
            MyFile.close();  
        }
    }

    void _Update() {
        vector<clsBankClient> _vClients;
        _vClients = _LoadClientDataFromFile();

        for (clsBankClient& C : _vClients) {
            if (C.AccountNumber() == AccountNumber()) {
                C = *this;
                break;
            }
        }

        _SaveClientsDataToFile(_vClients); 
    }
    void _AddDataLineToFile(string DataLine) {
        fstream MyFile;
        MyFile.open("Clients.txt", ios::out|ios::app); 
        if (MyFile.is_open()) {
            MyFile << DataLine << endl; 

            MyFile.close();
        }
    }
    void _AddNew() {
        _AddDataLineToFile(_ConvertClientObjectToLine(*this));
    }

    string _PrepareTransferLogRecord(float Amount, clsBankClient DestinationClient,
        string UserName, string Seperator = "#//#")
    {
        string TransferLogRecord = "";
        TransferLogRecord += clsDate::GetSystemDateTimeString() + Seperator;
        TransferLogRecord += AccountNumber() + Seperator;
        TransferLogRecord += DestinationClient.AccountNumber() + Seperator;
        TransferLogRecord += to_string(Amount) + Seperator;
        TransferLogRecord += to_string(AccountBalance) + Seperator;
        TransferLogRecord += to_string(DestinationClient.AccountBalance) + Seperator;
        TransferLogRecord += UserName;
        return TransferLogRecord;
    }

    void _RegisterTransferLog(float Amount, clsBankClient DestinationClient, string UserName)
    {

        string stDataLine = _PrepareTransferLogRecord(Amount, DestinationClient, UserName);

        fstream MyFile;
        MyFile.open("TransferLog.txt", ios::out | ios::app); 

        if (MyFile.is_open())
        {

            MyFile << stDataLine << endl; 

            MyFile.close(); 
        }

    }

    struct stTrnsferLogRecord;

    static stTrnsferLogRecord _ConvertTransferLogLineToRecord(string Line, string Seperator = "#//#")
    {
        stTrnsferLogRecord TrnsferLogRecord;

        vector <string> vTrnsferLogRecordLine = clsString::Split(Line, Seperator);
        TrnsferLogRecord.DateTime = vTrnsferLogRecordLine[0];
        TrnsferLogRecord.SourceAccountNumber = vTrnsferLogRecordLine[1];
        TrnsferLogRecord.DestinationAccountNumber = vTrnsferLogRecordLine[2];
        TrnsferLogRecord.Amount = stod(vTrnsferLogRecordLine[3]);
        TrnsferLogRecord.srcBalanceAfter = stod(vTrnsferLogRecordLine[4]);
        TrnsferLogRecord.destBalanceAfter = stod(vTrnsferLogRecordLine[5]);
        TrnsferLogRecord.UserName = vTrnsferLogRecordLine[6];

        return TrnsferLogRecord;

    }


public:


    clsBankClient(enMode Mode, string FirstName, string LastName,string Email, string Phone, string AccountNumber, string PinCode,float AccountBalance)
        :
        clsPerson(FirstName, LastName, Email, Phone)

    {
        _Mode = Mode;
        _AccountNumber = AccountNumber;
        _PinCode = PinCode;
        _AccountBalance = AccountBalance;

    }

    struct stTrnsferLogRecord
    {
        string DateTime;
        string SourceAccountNumber;
        string DestinationAccountNumber;
        float Amount;
        float srcBalanceAfter;
        float destBalanceAfter;
        string UserName;

    };



    bool IsEmpty()
    {
        return (_Mode == enMode::EmptyMode);
    }


    string AccountNumber()
    {
        return _AccountNumber;
    }

    void SetPinCode(string PinCode)
    {
        _PinCode = PinCode;
    }

    string GetPinCode()
    {
        return _PinCode;
    }
    __declspec(property(get = GetPinCode, put = SetPinCode)) string PinCode;

    void SetAccountBalance(float AccountBalance)
    {
        _AccountBalance = AccountBalance;
    }

    float GetAccountBalance()
    {
        return _AccountBalance;
    }
    __declspec(property(get = GetAccountBalance, put = SetAccountBalance)) float AccountBalance;

    /*void Print()
    {
        cout << "\nClient Card:";
        cout << "\n___________________";
        cout << "\nFirstName   : " << FirstName;
        cout << "\nLastName    : " << LastName;
        cout << "\nFull Name   : " << FullName();
        cout << "\nEmail       : " << Email;
        cout << "\nPhone       : " << Phone;
        cout << "\nAcc. Number : " << _AccountNumber;
        cout << "\nPassword    : " << _PinCode;
        cout << "\nBalance     : " << _AccountBalance;
        cout << "\n___________________\n";

    }*/

    static clsBankClient Find(string AccountNumber)
    {
        vector <clsBankClient> vClients;

        fstream MyFile;
        MyFile.open("Clients.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsBankClient Client = _ConvertLinetoClientObject(Line);
                if (Client.AccountNumber() == AccountNumber)
                {
                    MyFile.close();
                    return Client;
                }
                vClients.push_back(Client);
            }

            MyFile.close();

        }

        return _GetEmptyClientObject();
    }

    static clsBankClient Find(string AccountNumber, string PinCode)
    {
        vector <clsBankClient> vClients;


        fstream MyFile;
        MyFile.open("Clients.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsBankClient Client = _ConvertLinetoClientObject(Line);
                if (Client.AccountNumber() == AccountNumber && Client.PinCode == PinCode)
                {
                    MyFile.close();
                    return Client;
                }
                vClients.push_back(Client);
            }

            MyFile.close();

        }
        return _GetEmptyClientObject();
    }

 
    static bool IsClientExist(string AccountNumber) {
       clsBankClient Client = clsBankClient::Find(AccountNumber);
       return (!Client.IsEmpty());
    }

    enum enSaveResults{svFailEmptyObject=0,svSucceeced=1,svFaildAccountNumberExists=2};

    enSaveResults Save() {
        switch (_Mode)
        {
        case clsBankClient::EmptyMode:
            return enSaveResults::svFailEmptyObject;
            break;
        case clsBankClient::UpdateMode:

            _Update();
            return enSaveResults::svSucceeced;
            break;
        case clsBankClient::AddNewMode:
            if (clsBankClient::IsClientExist(_AccountNumber)) {
                return enSaveResults::svFaildAccountNumberExists;
            }
            else {
                _AddNew();
                _Mode = enMode::UpdateMode;
                return enSaveResults::svSucceeced; 
            }

        }
    }

    static clsBankClient GetAddNewClientObject(string AccountNumber) {
        return clsBankClient(enMode::AddNewMode, "", "", "", "", AccountNumber, "", 0);
    }

    bool Delete() {
        vector<clsBankClient> vClients;
        vClients = _LoadClientDataFromFile();

        for (clsBankClient& C : vClients) {
            if (C.AccountNumber() == AccountNumber()) {
                C._MarkForDelete = true;  
                break;
            }
        }
        _SaveClientsDataToFile(vClients);
        *this = _GetEmptyClientObject(); 
        return true; 
    }

    static vector<clsBankClient> GetClientsList() {
        return _LoadClientDataFromFile();
    }

     static double GetTotalBalances() {
         vector<clsBankClient> vClients = clsBankClient::GetClientsList();
         double TotalBalances = 0;
         for (clsBankClient& Client : vClients) {
             TotalBalances += Client.AccountBalance; 
         }

         return TotalBalances;  
    }

     void Deposit(double Amount) {
         _AccountBalance += Amount;
         Save();
     }

     bool Withdraw(double Amount) {
         if (Amount > _AccountBalance) {
             return false;
         }
         else {
             _AccountBalance -= Amount; 
             Save(); 
         }
         
     }

     bool Transfer(float Amount, clsBankClient& DestinationClient , string UserName) {
         if (Amount > AccountBalance) {
             return false;
         }
         Withdraw(Amount);
         DestinationClient.Deposit(Amount);
         _RegisterTransferLog(Amount, DestinationClient, UserName);
         return true;
     }

     static  vector <stTrnsferLogRecord> GetTransfersLogList()
     {
         vector <stTrnsferLogRecord> vTransferLogRecord;

         fstream MyFile;
         MyFile.open("TransferLog.txt", ios::in);//read Mode

         if (MyFile.is_open())
         {

             string Line;

             stTrnsferLogRecord TransferRecord;

             while (getline(MyFile, Line))
             {

                 TransferRecord = _ConvertTransferLogLineToRecord(Line);

                 vTransferLogRecord.push_back(TransferRecord);

             }

             MyFile.close();

         }

         return vTransferLogRecord;

     }


};

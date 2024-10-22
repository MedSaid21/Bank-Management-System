#pragma once
#include <iostream>
#include "clsScreen.h"
#include "clsPerson.h"
#include "clsBankClient.h"
#include "clsInputValidate.h"
#include "clsUser.h"
class clsDeleteClientScreen : protected clsScreen
{
private:
    static void _PrintClient(clsBankClient Client)
    {
        cout << "\nClient Card:";
        cout << "\n___________________";
        cout << "\nFirstName   : " << Client.FirstName;
        cout << "\nLastName    : " << Client.LastName;
        cout << "\nFull Name   : " << Client.FullName();
        cout << "\nEmail       : " << Client.Email;
        cout << "\nPhone       : " << Client.Phone;
        cout << "\nAcc. Number : " << Client.AccountNumber();
        cout << "\nPassword    : " << Client.PinCode;
        cout << "\nBalance     : " << Client.AccountBalance;
        cout << "\n___________________\n";

    }
public :
    static void ShowDeleteClientScreen() {
        if (!_CheckAccessRight(clsUser::enPermissions::pDeleteClient)) {
            return; 
        }

        _DrawScreenHeader("\t Delete Client Screen ");
        cout << "\nPlease Enter Account Number: ";
        string AccountNumber = ""; 
        AccountNumber = clsInputValidate::ReadString(); 
        while (!clsBankClient::IsClientExist(AccountNumber)) {
            cout << "\nAccount number is not found, choose another one: ";
            AccountNumber = clsInputValidate::ReadString(); 
        }

        clsBankClient Client1 = clsBankClient::Find(AccountNumber); 
        _PrintClient(Client1); 

        cout << "\nAre you sure you want to delete this client y/n? ";
        char Answer = 'n';
        cin >> Answer; 
        if (Answer == 'Y' || Answer == 'y') {
            if (Client1.Delete()) {
                cout << "\nClient Deleted Successfully :-)\n";
                _PrintClient(Client1); 
            }
            else {
                cout << "\nError Client Was not Deleted\n"; 
            }

        }

    }
};


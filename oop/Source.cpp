#include<iostream>
#include <string>
#include "clsPerson.h";
#include <vector>
#include "clsString.h";
#include"clsBankClient.h";
#include"clsPerson.h";
#include "clsUtil.h";
#include "clsInputValidate.h"; 
#include <iomanip> 
//#include"clsMainScreen.h";
#include"clsLoginScreen.h"
#include "Global.h"
using namespace std; 

int main() {
	clsMainScreen::ShowMainMenue(); 

	while (true) { 
		if (!clsLoginScreen::ShowLoginScreen()) {
			break; 
		}
		clsLoginScreen::ShowLoginScreen();      

	}

	
	return 0;  
}
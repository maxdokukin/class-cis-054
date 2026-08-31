//
//  main.cpp
//  ATM Simulator
//
//  Created by Max on 15/05/2021.
//
// ATM.cpp : This file contains all of the code for the ATM simulato
//           Code for deposits needs to be completed
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include "ATM.hpp"

using namespace std;

#include <unistd.h>
void delay(int amount)  {   usleep(amount * 1000);  }

void initializeAtmDrawing();
int drawATM(string options[6], int cursorStart, bool allowCursorMove, bool cashOut);
void printAtmBalance(streamoff customerIndex, char accountType);

int main()
{
    initializeAtmDrawing(); //fill atm draw array
    
    streamoff customerIndex;
    char anotherRequest = ' ';

    while (true) {  // infinite loop - typical for embedded systems
//        cout << "\n\n===========================\n";
//        cout <<     "== Welcome to Phony Bank ==\n";
//        cout <<     "===========================\n\n";

        customerIndex = searchForCustomer();
        do {
            if (customerIndex == -1) {
                return -1;  // can't open the customer file. Exit program
            }
            else if (customerIndex == -2) {
                
                //instead of cout
                string options[6] = {"Account not found", "", "", "", "", ""};
                drawATM(options, 0, false, false);
                
                delay(2000);
                //cout << "Account number not found" << endl;
                continue;
            }
            else if (customerIndex == -3) {
                
                //instead of cout
                string options[6] = {"  Incorrect PIN", "", "", "", "", ""};
                drawATM(options, 0, false, false);
            
                delay(2000);
                //cout << "Incorrect PIN" << endl;
                continue;
            }
            else if (customerIndex == -4)  // the customer file was displayed
                continue;
            
            char checkingOrSavings = selectAccount(customerIndex); // 'C' = checking, 'S' = savings, 'X' = cancel
            
            if (checkingOrSavings == 'X')
                break;  // done with this customer
            
            char transaction = selectTransaction();
            
            switch (transaction) {
                    
                case 'B':  // balance
                    //cout << showpoint << fixed << setprecision(2);
                    //cout << "$" << getBalance(customerIndex, checkingOrSavings) << endl;
                    printAtmBalance(customerIndex, checkingOrSavings);
                    break;
                        
                case 'D':  // deposit
                    deposit(customerIndex, checkingOrSavings);
                    break;
                    
                case 'W':  // withdraw
                    withdraw(customerIndex, checkingOrSavings);
                    break;
                    
                case 'X':  // cancel
                    break;
            }
            // Do you want another requst for the same customer?
            do { // get a 'Y' or 'N' response
                                     //Avaliable balance
                string options7[6] = {"  Continue for", "", "  this account?", "", "Yes", "No"};
                
                switch (drawATM(options7, 4, true, false)){
                        
                    case 4:
                        anotherRequest = 'Y';
                        break;
                        
                    case 5:
                        anotherRequest = 'N';
                        break;
                }
                
                //cout << "Do you want another transaction for the same customer? (Y/N)? ";
                //anotherRequest = toupper(getChar());
            } while (anotherRequest != 'Y' && anotherRequest != 'N');
        } while (anotherRequest == 'Y');
    } // end of while (true)
    return 0;   // end of main( )
}

////////////////////////////////////////////////////////////
// searchForCustomer()
//    return:
//      >= 0 index into the file for the selected customer
//      -1 = unable to open customer file
//      -2 = customer not found in the fileIN
//      -3 = PIN does not match customer selection
//      -4 = customer file displayed to stdout
streamoff searchForCustomer() {
    int accountNo;
    int pin = 0;
    streamoff customerIndex=0;      // -2=customer not found,  -3=PIN does not match
    ATM customer;                   // customer record (acctNo, PIN, chk, sav)

    
    //cout << "Enter a customer account number," << endl;
    //cout << "[Enter] key to  display the customer data file" << endl;
    //cout << "Account? ";
    string options1[6] = {" Enter account #", "", "", "", "", ""};
    drawATM(options1, 0, false, false);
    
    accountNo = getInt();

    if (accountNo == 0) { // display file
        displayFile();
        return -4;  // entire customer file was displayed
    }
    if (accountNo >= 0) {
        
   //cout << "Enter PIN? ";
        string options2[6] = {"    Enter PIN", "", "", "", "", ""};
        drawATM(options2, 0, false, false);

        pin = getInt();
    }
    
    // open the customer data file
    ifstream ATM_file(ATMfilename, ios::binary);
    if (ATM_file.fail()) {
        cout << "Unable to open ATMaccounts " << endl;
        return -1;
    }
    ATM_file.read((char*)&customer, sizeof(customer));  // read first record
    for (int i = 0; !ATM_file.eof(); i++) {
        if (accountNo == customer.getAcctNo()) { // found the customer in the file
            if (pin == customer.getPIN()) customerIndex = i;  // customer an PIN match
            else  customerIndex = -3; // PIN does not match return code
            break;  // customer and PIN match the request. Exit loop
        }
        ATM_file.read((char*)&customer, sizeof(customer));  // next customer
    }
    if (ATM_file.eof())   // reached EOF and didn't find the customer
        customerIndex = -2;  // customer not found
    ATM_file.close();
    

//    if (customerIndex >= 0) {  // customer has been found. Display balances
//        //cout << fixed << showpoint << setprecision(2);
//        //cout << endl << "Checking $" << getBalance(customerIndex, 'C')
//        //     << "  Savings $" << getBalance(customerIndex, 'S') << endl << endl;
//    }
    return customerIndex;
}

////////////////////////////////////////////////////////////
// displayFile()
//   Displays all of the customer records in the file
//     return:
//      0 = success
//     -1 = error displaying file
int displayFile() {
    ifstream ATM_file(ATMfilename, ios::binary);
    if (ATM_file.fail()) {
        cout << "Unable to open ATMaccounts " << endl;
        return -1;
    }
    ATM customer;    // customer record (acctNo, PIN, chk, sav)
    // display header information
    cout << fixed << showpoint << setprecision(2);  // format the output
    cout << setw(-7) << " Acct #" << "     "
        << setw(-4) << "PIN" << "   "
        << setw(-8) << "Checking" << "    "
        << setw(-8) << "Savings" << endl;

    ATM_file.read((char*)&customer, sizeof(customer));  // read first customer
    for (int i = 0; !ATM_file.eof(); i++) {
        cout << setw(7) << customer.getAcctNo() << "    "
             << setw(4) << customer.getPIN() << "   "
             << setw(8) << customer.getChecking() << "   "
             << setw(8) << customer.getSavings() << endl;
        ATM_file.read((char*)&customer, sizeof(customer));  // read next customer
    }
    ATM_file.close();
    return 0;
}

////////////////////////////////////////////////////////////
// selectAccount()
//    return:  'C  or  'S'   for checking or savings
char selectAccount(streamoff customerIndex) {
//    cout << "Select account:" << endl;
//    cout << "  C = checking" << endl;
//    cout << "  S = savings" << endl;
//    cout << "  X = cancel" << endl;
    
    //added to get savings and checking balances
    ifstream ATM_file(ATMfilename, ios::binary);
    if (ATM_file.fail()) {
        cout << "Unable to open ATMaccounts " << endl;
        return 0;
    }
    // seek to the selected customer
    
    ATM customer;    // customer record (acctNo, PIN, chk, sav)
    // seek to the selected customer
    streamoff customerPositionInFile = customerIndex * ATM::ATMsize;
    ATM_file.seekg(customerPositionInFile, ATM_file.beg);
    ATM_file.read((char*)&customer, sizeof(customer));
    
    //cout << customer.getSavings() << "     " << customer.getChecking() << endl;

    char accountType = ' ';  // C=checking,  S=savings
    
    //create array of options
    string options3[6] = {"Checking", "", "Savings", "", "Exit", ""};
    
    //add balances
    options3[1] = to_string(customer.getChecking());
    options3[3] = to_string(customer.getSavings());
    
    //make 2 numbers decimals after dot
    options3[1] = "$" + options3[1].substr(0, options3[1].find('.') + 3);
    options3[3] = "$" + options3[3].substr(0, options3[3].find('.') + 3);

    while(accountType == ' '){
                    
        switch (drawATM(options3, 0, true, false)) { //will return selected option
                
            case 0:
                accountType = 'C';
                break;
                
            case 2:
                accountType = 'S';
                break;
                
            case 4:
                accountType = 'X';
                break;
                
            default:
                cout << "  Illegal selection. Try again." << endl;

                break;
        }
    }

    ATM_file.close();

    return accountType;
}

////////////////////////////////////////////////////////////
// getBalance(int customerIndex, char accountType)
//    where:
//      customerIndex = customer number in the file
//      accountType     'C'=checking,  'S'=savings
//    return:
//       of either customer's checking or savings account
double getBalance(streamoff customerIndex, char accountType) {
    
    double balance = 0.0;
    // search the file for the customer number
    ifstream ATM_file(ATMfilename, ios::binary);
    if (ATM_file.fail()) {
        cout << "Unable to open ATMaccounts " << endl;
        return balance;
    }
    ATM customer;    // customer record (acctNo, PIN, chk, sav)
    // seek to the selected customer
    streamoff customerPositionInFile = customerIndex * ATM::ATMsize;
    ATM_file.seekg(customerPositionInFile, ATM_file.beg);
    ATM_file.read((char*)&customer, sizeof(customer));
    if (accountType == 'C')
        balance = customer.getChecking();
    else if (accountType == 'S')
        balance = customer.getSavings();
    ATM_file.close();
    return balance;
}

////////////////////////////////////////////////////////////
// char selectTransaction()
//    return:  'B', 'D', 'W' or 'X'
char selectTransaction() { // D=deposit  W=withdraw  X=cancel
//    cout << "Select transaction:" << endl;
//    cout << "  B = Balance" << endl;
//    cout << "  D = deposit" << endl;
//    cout << "  W = withdraw" << endl;
//    cout << "  X = cancel" << endl;
    
    char transactionType = ' ';

    //array of options
    string options4[6] = {"Select operation", "", "Balance", "Deposit", "Withdraw", "Cancel"};
    
    while (transactionType == ' ') {

        switch (drawATM(options4, 2, true, false)) { //will return selected option
            case 2:
                transactionType = 'B';
                break;
                
            case 3:
                transactionType = 'D';
                break;
                
            case 4:
                transactionType = 'W';
                break;
                
            case 5:
                transactionType = 'C';
                break;
                
            default:
                cout << "  Illegal selection. Try again." << endl;
                break;
        }
    }

    return transactionType;
}

////////////////////////////////////////////////////////////
// void deposit(int customerIndex, char accountType)
//    where:
//      customerIndex = customer within the file
//      accountType:  'C'=checking   'S'=savings
//    The function requests the amount to deposit, then
//      validates that the amount is greater than zero
//      reads the customer record and adds the deposit
//    ` updates the customer data file
void deposit(streamoff customerIndex, char accountType) {

    double depositAmount = 0;
    double newBalance = 0.0;
    
    string options[6] = {"  Enter amount", "", "", "", "", ""}; //array of strings for displaying options on atm screen
    drawATM(options, 4, 0, 0); //draws atm in console
    
    depositAmount = getDouble();  // check for > 0 after getting current balance

    //update deposit amount on atm screen when entered
    options[4] = "$" + to_string(depositAmount).substr(0, to_string(depositAmount).find(".") + 3);
    drawATM(options, 4, 0, 1);
    delay(1000);

    // open in binary mode for both reading and writing
    fstream ATM_file(ATMfilename, ios::binary | ios::in | ios::out);
    if (ATM_file.fail()) {
        cout << "Unable to open ATMaccounts data file" << endl;
        return;
    }
    ATM currentCustomer;
    // seek to the selected customer from beginning of the file and update the selected balance
    streamoff customerPositionInFile = customerIndex * ATM::ATMsize;
    ATM_file.seekg(customerPositionInFile, ATM_file.beg);
    ATM_file.read((char*)&currentCustomer, sizeof(ATM));
    if (accountType == 'C') {   // update the customer's checking balance
        newBalance = currentCustomer.getChecking() + depositAmount;
        currentCustomer.setChecking(newBalance);
    }
    else if (accountType == 'S') { // update customer's savings balance
        newBalance = currentCustomer.getSavings() + depositAmount;
        currentCustomer.setSavings(newBalance);
    }
    // seek back to the same record and write the updated record back to disk
    ATM_file.seekg(customerPositionInFile, ATM_file.beg);
    ATM_file.write((char*)&currentCustomer, sizeof(ATM));

    ATM_file.close(); // close the file

    //print atm with balance screen
    printAtmBalance(customerIndex, accountType);
}

////////////////////////////////////////////////////////////
// void withdraw(int customerIndex, char accountType)
//    where:
//      customerIndex = customer within the file
//      accountType:  'C'=checking   'S'=savings
//    The function requests the amount to withdraw, then
//      validates that the amount is an even multiple of $20.00
//      maximum withdrawal is $500.00
//      reads the customer record and validates sufficient funds
//      deducts the amount and updates the customer data file
void withdraw(streamoff customerIndex, char accountType) {
    
    double withdrawAmount;
    double newBalance = 0.0;

    //cout << "Enter the amount of the withdrawal in increments of $20 up to $500: ";
    
    string options[6] = {"  Enter amount", "", "", "", "", ""};
    drawATM(options, 4, 0, 0);
    
    withdrawAmount = getDouble();  // check for > 0 after getting current balance

    // compute amount of withdrawal in pennies used to check for increments of $20.00
    int intWithdrawX100 = (int)(withdrawAmount * 100);  // convert to pennies

    if (withdrawAmount <= 0.00){
        
        //cout << "Withdrawal must be greater than zero" << endl << endl;
        options[0] = " Withdrawal must";
        options[2] = " be greater";
        options[4] = " than zero!";

        drawATM(options, 4, 0, 0);
        delay(5000);
    }
    else if (withdrawAmount > 500.00){
        
        //cout << "Withdrawal must be greater than zero" << endl << endl;
        options[0] = " Withdrawal must";
        options[2] = " not exceed";
        options[4] = " $500.00";

        drawATM(options, 4, 0, 0);
        delay(5000);
    }
    else if (intWithdrawX100 % 2000 != 0){
        
        //cout << "Withdrawal must be greater than zero" << endl << endl;
        options[0] = " Withdrawal must";
        options[2] = " be in increments";
        options[4] = " of $20.00";
        drawATM(options, 4, 0, 0);
        delay(5000);
    }  // 2000 = $20 in pennies,
    else // proceed with withdrawal
    {
        options[4] = "$" + to_string(withdrawAmount).substr(0, to_string(withdrawAmount).find(".") + 3);
        drawATM(options, 4, 0, 1);
        delay(1000);
        
        // open in binary mode for both reading and writing
        fstream ATM_file(ATMfilename, ios::binary | ios::in | ios::out);
        if (ATM_file.fail()) {
            cout << "Unable to open ATMaccounts data file" << endl;
            return;
        }
        ATM currentCustomer;
        // seek to the selected customer from beginning of the file and update the selected balance
        streamoff customerPositionInFile = customerIndex * ATM::ATMsize;
        ATM_file.seekg(customerPositionInFile, ATM_file.beg);
        ATM_file.read((char*)&currentCustomer, sizeof(ATM));
        if (accountType == 'C') {   // update the customer's checking balance
            newBalance = currentCustomer.getChecking() - withdrawAmount;
            currentCustomer.setChecking(newBalance);
        }
        else if (accountType == 'S') { // update customer's savings balance
            newBalance = currentCustomer.getSavings() - withdrawAmount;
            currentCustomer.setSavings(newBalance);
        }
        // seek back to the same record and write the updated record back to disk
        ATM_file.seekg(customerPositionInFile, ATM_file.beg);
        ATM_file.write((char*)&currentCustomer, sizeof(ATM));

        ATM_file.close(); // close the file

        // display the updated balance
        //cout << fixed << showpoint << setprecision(2);  // display 2 digits past the decimal
        //cout << "Your balance is $" << newBalance << endl;
        
        printAtmBalance(customerIndex, accountType);

    } // end of a processed withdrawal
}




////////////////////////////////////////////////////////////
//// The following methods read from the keyboard and
////   verify that valid data was entered
////////////////////////////////////////////////////////////
// char getChar()
//  reads a line of text from standard input (keyboard)
//  returns the first character that was input
//  the rest of the line is discarded
char getChar() {
    char buff[100];
    cin.getline(buff, 100);
    return(buff[0]);
}

// int getInt()
//  reads an integer from standard input (keyboard)
//  returns the first character that was input
//  displays an error message and tries again if a non-integer is input
int getInt() {
    int result;
    bool tryAgain;
    do {
        tryAgain = false;
        try {
            char buff[100];
            cin.getline(buff, 100);
            result = atoi(buff);
            if (cin.fail())
                throw 1; // different values for the throw to display different conditions in the catch
        }
        catch (...) {
            cout << "*** Illegal entry. Enter an integer. Try again" << endl << "? ";
            cin.clear(); // clear the input buffer
            cin.ignore(1000, '\n'); // and ignore anything else before a new cin
            tryAgain = true;
        }
    } while (tryAgain);
    return result;
}

// double getDouble()
//  reads a real number from standard input (keyboard)
//  returns the first character that was input
//  displays an error message and tries again if an error is detected
double getDouble() {
    double result;
    bool tryAgain;

    do {
        tryAgain = false;
        try {
            char buff[100];
            cin.getline(buff, 100);
            result = atof(buff);
            if (cin.fail())
                throw 1; // different values for the throw to display different conditions in the catch
        }
        catch (...) {
            cout << "*** Illegal entry. Enter an number. Try again" << endl << "? ";
            tryAgain = true;
        }
    } while (tryAgain);
    return result;
}

//Graphics functions start here//

int cursorPos = 0; //cursor position on the atm display

string * atmDrawing; //pointer for atm drawing array of strings


//adds cursor to the final atm drawing
void addCursor(){
    
    string controls = "oooooo";
    
    if(cursorPos % 2)
        controls = controls.replace(cursorPos, 1, "<");
    else
        controls = controls.replace(cursorPos, 1, ">");

    
    atmDrawing[4] = atmDrawing[4].replace(2, 1, controls.substr(0, 1));
    atmDrawing[4] = atmDrawing[4].replace(22, 1, controls.substr(1, 1));
    atmDrawing[6] = atmDrawing[6].replace(2, 1, controls.substr(2, 1));
    atmDrawing[6] = atmDrawing[6].replace(22, 1, controls.substr(3, 1));
    atmDrawing[8] = atmDrawing[8].replace(2, 1, controls.substr(4, 1));
    atmDrawing[8] = atmDrawing[8].replace(22, 1, controls.substr(5, 1));
}


//prints atm with the screen of balance of choosen account
void printAtmBalance(streamoff customerIndex, char accountType) {
    
    string options[6] = {"Avaliable balance", "", "", "", "Continue", ""};
    options[2] = "$" + to_string(getBalance(customerIndex, accountType)).substr(0, to_string(getBalance(customerIndex, accountType)).find(".") + 3);
    
    while(drawATM(options, 4, 1, 0) != 4);
}


//prints atm drawing to console
void printAtm(bool allowCursorMove){
    
    if(allowCursorMove)
        addCursor();

    //print every line
    for(int i = 0; i < 20; i++)
        cout << atmDrawing[i] << endl;
}


//moves cursor on the atm screen
void moveCursor(){
    
    //cout << "WASD to move cursor" << endl;
    
    char curChar = '\n';
    
    while(1){
        
        char newChar = cin.get();
        
        if(newChar == '\n' && curChar == '\n')
           break;
           
        curChar = newChar;
        
        if(curChar != '\n'){
            
            switch (toupper(curChar)) {
                    
                case 'W':
                    if(cursorPos >= 2)
                        cursorPos -= 2;
                    break;
                    
                case 'A':
                    if(cursorPos % 2 == 1)
                        cursorPos --;
                    break;
                                        
                case 'S':
                    if(cursorPos <= 3)
                        cursorPos += 2;
                    break;
                    
                case 'D':
                    if(cursorPos % 2 == 0)
                        cursorPos++;
                    break;
            }
            
            printAtm(true);
        }
    }
}


//generates and prints atm drawing
int drawATM(string options[6], int cursorStart, bool allowCursorMove, bool cashOut){
    
    cursorPos = cursorStart;
    
    //add options to atm drawing
    atmDrawing[4] = atmDrawing[4].replace(4, options[0].length(), options[0]);
    atmDrawing[4] = atmDrawing[4].replace(21 - options[1].length(), options[1].length(), options[1]);
    atmDrawing[6] = atmDrawing[6].replace(4, options[2].length(), options[2]);
    atmDrawing[6] = atmDrawing[6].replace(21 - options[3].length(), options[3].length(), options[3]);
    atmDrawing[8] = atmDrawing[8].replace(4, options[4].length(), options[4]);
    atmDrawing[8] = atmDrawing[8].replace(21 - options[5].length(), options[5].length(), options[5]);

    
    //if cashtray is open, make an animation
    if(cashOut){
        
        atmDrawing[12] = atmDrawing[12].replace(4, 5, "$$$$$"); //open
        printAtm(allowCursorMove);
        delay(2000);
        atmDrawing[12] = atmDrawing[12].replace(4, 5, "XXXXX"); //close
    }

    printAtm(allowCursorMove);
    
    //add cursor
    if(allowCursorMove)
        moveCursor();

    
    //return atm drawing array to initial state without text
    
    //remove option0 text
    string filler = "";
    for(int i = 0; i < options[0].length(); i++)
        filler += " ";
    atmDrawing[4] = atmDrawing[4].replace(4, options[0].length(), filler);
    
    //remove option1 text
    filler  = "";
    for(int i = 0; i < options[1].length(); i++)
        filler += " ";
    atmDrawing[4] = atmDrawing[4].replace(21 - options[1].length(), options[1].length(), filler);

    //remove option2 text
    filler  = "";
    for(int i = 0; i < options[2].length(); i++)
        filler += " ";
    atmDrawing[6] = atmDrawing[6].replace(4, options[2].length(), filler);
    
    //remove option3 text
    filler  = "";
    for(int i = 0; i < options[3].length(); i++)
        filler += " ";
    atmDrawing[6] = atmDrawing[6].replace(21 - options[3].length(), options[3].length(), filler);

    //remove option4 text
    filler  = "";
    for(int i = 0; i < options[4].length(); i++)
        filler += " ";
    atmDrawing[8] = atmDrawing[6].replace(4, options[4].length(), filler);
    
    //remove option5 text
    filler  = "";
    for(int i = 0; i < options[5].length(); i++)
        filler += " ";
    atmDrawing[8] = atmDrawing[6].replace(21 - options[5].length(), options[5].length(), filler);
    
    return cursorPos;
}


void initializeAtmDrawing(){
    
    atmDrawing = new string[20];
    
    atmDrawing[0] =     "  _____________________  ";
    atmDrawing[1] =     " /=========ATM=========\\";
    atmDrawing[2] =     "/=======================\\";
    atmDrawing[3] =     "|  ___________________  |";
    atmDrawing[4] =     "| o|                 |o |";
    atmDrawing[5] =     "|  |                 |  |";
    atmDrawing[6] =     "| o|                 |o |";
    atmDrawing[7] =     "|  |                 |  |";
    atmDrawing[8] =     "| o|                 |o |";
    atmDrawing[9] =     "|  -------------------  |";
    atmDrawing[10] =    "|                 ______|";
    atmDrawing[11] =    "|  _______       |1|2|3||";
    atmDrawing[12] =    "|  |XXXXX|       |4|5|6||";
    atmDrawing[13] =    "|  =======       |7|8|9||";
    atmDrawing[14] =    "|                | |0| ||";
    atmDrawing[15] =    "|-----------------------|";
    atmDrawing[16] =    "|                       |";
    atmDrawing[17] =    "|                       |";
    atmDrawing[18] =    "|                       |";
    atmDrawing[19] =    "+-----------------------+";
}

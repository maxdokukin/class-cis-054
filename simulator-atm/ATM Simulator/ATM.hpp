// ATM.hpp  -- header file for the C++ language ATM.cpp program

#pragma once
using namespace std;

// Location of the ATM data file on the disk
//  1) Choose ATM_accounts.dat   if you are on a Windows/DOS system, or 
//            ATM_accounts.bin   if you are on a Mac or Linux system
//  2) Change this definition to the location of where you placed the ATMaccounts.dat file
static const char ATMfilename[] = "/Users/xewe/Documents/Programming/CIS-054/ATM Simulator/ATM Simulator/ATM_accounts.dat";

class ATM {
private:
    int acctNo;
    int pin;
    double checking;
    double savings;

public:
    static const int ATMsize = sizeof(int) * 2 + sizeof(double) * 2;
    ATM(int acct=0, int p=0, double chk=0, double sav=0) {
        acctNo = acct;
        pin = p;
        checking = chk;
        savings = sav;
    }
// setters
    int setAcctNo(int acctNo) { return this->acctNo = acctNo; }
    int setPIN(int pin) { return this->pin = pin; }
    double setChecking(double checking) { return this->checking = checking; }
    double setSavings(double savings) { return this->savings = savings; }
//getters
    int getAcctNo() { return acctNo; }
    int getPIN() { return pin; }
    double getChecking() { return checking; }
    double getSavings() { return savings; }
};

// function prototypes
char getChar();
int  getInt();
double getDouble();
streamoff searchForCustomer(); // position in the file, or neg-number if not found
int displayFile();
char selectAccount(streamoff customerIndex);
double getBalance(streamoff customerIndex, char accountType);
char selectTransaction();
void deposit(streamoff customerIndex, char accountType);
void withdraw(streamoff customerIndex, char accountType);

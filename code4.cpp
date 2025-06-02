#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Transaction {
public:
    string type;
    double amount;

    Transaction(string type, double amount) {
        this->type = type;
        this->amount = amount;
    }

    void display() const {
        cout << type << ": Rs. " << amount << endl;
    }
};

class Account {
private:
    static int nextAccNum;
public:
    int accountNumber;
    double balance;
    vector<Transaction> transactions;

    Account() {
        accountNumber = ++nextAccNum;
        balance = 0.0;
    }

    void deposit(double amount) {
        balance += amount;
        transactions.push_back(Transaction("Deposit", amount));
    }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            transactions.push_back(Transaction("Withdrawal", amount));
            return true;
        }
        return false;
    }

    bool transfer(Account &to, double amount) {
        if (withdraw(amount)) {
            to.deposit(amount);
            transactions.push_back(Transaction("Transfer to Acc#" + to.getAccountNumberString(), amount));
            to.transactions.push_back(Transaction("Received from Acc#" + getAccountNumberString(), amount));
            return true;
        }
        return false;
    }

    void displayTransactions() const {
        cout << "Transaction history for Account #" << accountNumber << ":\n";
        for (auto &t : transactions) {
            t.display();
        }
    }

    void displayBalance() const {
        cout << "Account #" << accountNumber << " Balance: Rs. " << balance << endl;
    }

    string getAccountNumberString() const {
        return to_string(accountNumber);
    }
};

int Account::nextAccNum = 1000;

class Customer {
public:
    string name;
    int customerID;
    Account account;

    Customer(string name, int id) {
        this->name = name;
        this->customerID = id;
    }

    void displayCustomerInfo() {
        cout << "\nCustomer ID: " << customerID << "\nName: " << name << endl;
        account.displayBalance();
        account.displayTransactions();
    }
};

int main() {
    vector<Customer> customers;
    int numCustomers;

    cout << "Enter number of customers: ";
    cin >> numCustomers;

    for (int i = 0; i < numCustomers; ++i) {
        string name;
        int id;
        cout << "\nEnter name for customer " << i + 1 << ": ";
        cin.ignore();  // Clear input buffer
        getline(cin, name);
        cout << "Enter customer ID for " << name << ": ";
        cin >> id;
        customers.push_back(Customer(name, id));
    }

    int choice;
    do {
        cout << "\n--- Banking Menu ---\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Transfer\n";
        cout << "4. Display Customer Info\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        int cid, target_cid;
        double amount;

        switch (choice) {
            case 1:
                cout << "Enter customer ID to deposit into: ";
                cin >> cid;
                cout << "Enter amount to deposit: ";
                cin >> amount;
                for (auto &cust : customers) {
                    if (cust.customerID == cid) {
                        cust.account.deposit(amount);
                        cout << "Deposit successful.\n";
                        break;
                    }
                }
                break;

            case 2:
                cout << "Enter customer ID to withdraw from: ";
                cin >> cid;
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                for (auto &cust : customers) {
                    if (cust.customerID == cid) {
                        if (cust.account.withdraw(amount))
                            cout << "Withdrawal successful.\n";
                        else
                            cout << "Insufficient balance.\n";
                        break;
                    }
                }
                break;

            case 3:
                cout << "Enter sender customer ID: ";
                cin >> cid;
                cout << "Enter receiver customer ID: ";
                cin >> target_cid;
                cout << "Enter amount to transfer: ";
                cin >> amount;
                {
                    Customer *from = nullptr, *to = nullptr;
                    for (auto &cust : customers) {
                        if (cust.customerID == cid) from = &cust;
                        if (cust.customerID == target_cid) to = &cust;
                    }
                    if (from && to) {
                        if (from->account.transfer(to->account, amount))
                            cout << "Transfer successful.\n";
                        else
                            cout << "Transfer failed. Insufficient funds.\n";
                    } else {
                        cout << "Invalid customer IDs.\n";
                    }
                }
                break;

            case 4:
                cout << "Enter customer ID to display info: ";
                cin >> cid;
                for (auto &cust : customers) {
                    if (cust.customerID == cid) {
                        cust.displayCustomerInfo();
                        break;
                    }
                }
                break;

            case 5:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    return 0;
}


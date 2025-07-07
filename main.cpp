/*
Project Group ID: MPRJ_CPP_SY_A_07
Project Name: Restaurant Management System

Description: Implement a system for managing orders at a restaurant. Users should be
able to view the menu, place an order, and receive a bill. The program should keep track
of orders and stock.
Learning Concepts: Arrays/vectors, functions, basic input/output, file handling.
*/

#include <iostream>      // For input and output operations.
#include <fstream>       // For file handling operations.
#include <vector>        // For using the dynamic array to store data.
#include <string>        // For handling and manipulating text data.
#include <iomanip>       // For formatting output.
#include <limits>        // For managing numeric limits and resetting input streams.
#include <map>           // For storing key-value pairs.
#include <unordered_set> // For storing unique elements with efficient lookup.
#include <chrono>        // For working with time intervals and measuring durations.
#include <ctime>         // For manipulating date and time.
#include <sstream>       // For formatting and manipulating strings.
#include <regex>         // For validating phone number
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Restaurant class
class Restaurant
{

    // Menu and Stock management
    // Data members
    vector<pair<string, float>> menu;       // Stores item name and price
    vector<int> stock;                      // Stores the stock quantity for each item
    vector<pair<string, int>> currentOrder; // Stores item name and stock quantity
    float currentBill = 0;

    map<int, vector<pair<string, float>>> customerOrders; // Customer ID -> list of orders (item + price)
    map<int, string> customers;                           // Customer ID -> Name
    map<int, string> customerPhones;                      // Customer ID -> Phone number
    map<int, pair<string, float>> employees;              // Employee ID -> Name and Salary
    map<int, string> employeeWork;                        // Employee ID -> Work Type
    map<int, string> employeePhones;                      // Employee ID -> Phone numbers by ID
                                                          // Read Customer File to Map Customer ID -> Phone Number
    unordered_map<int, string> customerPhoneMap;

    // Daily Sales Data (Date -> Sales Amount)
    map<string, float> dailySales; // Date -> Total sales of the day
    map<string, int> dailyCustomers;

public:
    int customerIdCounter = 1; // Start customer IDs from 1
    int employeeIdCounter = 1; // Start employee IDs from 1

    //-----------Menu Related Functions---------//
    // Function to load menu in program =>
    void loadMenuFromFile()
    {
        ifstream file("menu.txt");
        // if file not opened then show an massage and return
        if (!file)
        {
            cout << "Error: Could not open menu file." << endl;
            return;
        }
        string itemName;
        float price;
        int quantity;
        menu.clear(); // Clear existing data of vectors before loading the new data
        stock.clear();

        // This while loop reads from file until it reaches the end of the file or encounters invalid data.
        while (file >> itemName >> price >> quantity)
        {
            // Basic validation because price and quantity must be positive
            if (price >= 0 && quantity >= 0)
            {
                // push the data into vectors
                menu.push_back({itemName, price});
                stock.push_back(quantity);
            }
        }
        // close the file after loading data into vectors
        file.close();
    }

    // Function to save menu in file =>
    void saveMenuToFile()
    {
        ofstream file("menu.txt");

        // if file not opened then show an massage and return
        if (!file)
        {
            cout << "Error: Could not save menu file." << endl;
            return;
        }
        // loop to store the data of vectors in menu file
        for (int i = 0; i < menu.size(); i++)
        {
            file << menu[i].first << " " << fixed << setprecision(2) << menu[i].second << " " << stock[i] << endl;
        }
        // close the menu file after storing the data of vectors
        file.close();
    }

    // Function to display menu =>
    void displayMenu()
    {
        // if menu is empty then show massege and return
        if (menu.empty())
        {
            cout << "\nMenu is empty!" << endl;
            return;
        }

        /*
        setw is used from the <iomanip> library to set a fixed width for each column,
        ensuring the text aligns in columns regardless of content length.
        */
        cout << "\n------------------- Menu -------------------\n"
             << endl;
        cout << setw(4) << "No." << setw(20) << "Item" << setw(10) << "Price" << setw(15) << "Stock" << endl;
        cout << string(49, '-') << endl;

        // display the menu
        for (int i = 0; i < menu.size(); i++)
        {
            cout << setw(4) << (i + 1) << setw(20) << menu[i].first
                 << setw(10) << fixed << setprecision(2) << menu[i].second
                 << setw(15) << stock[i] << endl;
        }
        // for horizontal line of string containing only '-'
        cout << string(49, '-') << endl;
    }

    // Function to add menu item by Owner =>
    void addMenuItem()
    {
        string itemName;
        float price;
        int quantity;

        cout << "Enter new item name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, itemName);

        // basic validation of price using Exception handling
        while (true)
        {
            try
            {
                cout << "\nEnter price: ";
                if (!(cin >> price) || price < 0)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw "\nInvalid price. Please enter a valid price.";
                }
                break;
            }
            catch (const char *msg)
            {
                cout << msg << endl; // Catch and display the error message
            }
        }

        // basic validation of quantity using Exception handling
        while (true)
        {
            try
            {
                cout << "\nEnter quantity in stock: ";
                if (!(cin >> quantity) || quantity < 0)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw "\nInvalid quantity. Please enter a valid quantity.";
                }
                break;
            }
            catch (const char *msg)
            {
                cout << msg << endl; // Catch and display the error message
            }
        }

        // push the entered data into the vectors
        menu.push_back({itemName, price});
        stock.push_back(quantity);

        // save all the menu to file
        saveMenuToFile();
        cout << "\nItem added to menu successfully." << endl;
    }

    // Function to delete Menu item by owner =>
    void deleteMenuItem()
    {
        // basic check for menu if menu is empty then owner cant delete the item
        if (menu.empty())
        {
            cout << "Menu is empty. Nothing to delete." << endl;
            return;
        }

        displayMenu();
        int choice;

        // Basic validation of index using Exception handling
        while (true)
        {
            try
            {
                cout << "\nEnter index of item to delete: ";
                if (!(cin >> choice))
                {
                    cin.clear();                                          // Clear the error flag on cin
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore all remaining input until newline
                    throw "\nInvalid input. Please enter a valid index."; // Throw an error message
                }
                // If input is valid, break the loop
                break;
            }
            catch (const char *msg)
            {
                cout << msg << endl; // Catch and display the error message
            }
        }

        // if choice is in the range of menu then delete the item
        if (choice > 0 && choice <= menu.size())
        {
            string deletedItem = menu[choice - 1].first;

            // menu.begin() + choice - 1 calculates an iterator pointing to the item that the user chose to delete.
            menu.erase(menu.begin() + choice - 1);
            stock.erase(stock.begin() + choice - 1);

            // after deleting the item from menu upadate the menu file
            saveMenuToFile();
            cout << "Item '" << deletedItem << "' deleted from menu." << endl;
        }
        else
        {
            cout << "Invalid choice." << endl;
        }
    }

    // Function to update stock quantity =>
    void updateStockQuantity()
    {
        // if the menu is empty then owner cant update the quantity
        if (menu.empty())
        {
            cout << "Menu is empty. No stock to update." << endl;
            return;
        }

        displayMenu();
        int choice, newQuantity;

        // Basic validation of choice using Exception handling
        while (true)
        {
            try
            {
                cout << "\nEnter index of item to update stock quantity: ";
                if (!(cin >> choice))
                {
                    cin.clear();                                          // Clear the error flag on cin
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore all remaining input until newline
                    throw "\nInvalid input. Please enter a valid index."; // Throw an error message
                }
                // If input is valid, break the loop
                break;
            }
            catch (const char *msg)
            {
                cout << msg << endl; // Catch and display the error message
            }
        }

        // if the choice ia in range of menu then upadate the quantity of item in stock vector
        if (choice > 0 && choice <= stock.size())
        {
            // Basic validation of stock quantity using Exception handling
            while (true)
            {
                try
                {
                    cout << "\nEnter the stock quantity to be added: ";
                    if (!(cin >> newQuantity) || newQuantity < 0)
                    {
                        cin.clear();                                                // Clear the error flag on cin
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');        // Ignore all remaining input until newline
                        throw "\nInvalid quantity. Please enter a Valid quantity."; // Throw an error message
                    }
                    // If input is valid, break the loop
                    break;
                }
                catch (const char *msg)
                {
                    cout << msg << endl; // Catch and display the error message
                }
            }

            // Update stock quantity in vector
            stock[choice - 1] += newQuantity;

            // save the changes to file
            saveMenuToFile();
            cout << "Stock quantity updated successfully." << endl;
        }
        else
        {
            cout << "Invalid choice." << endl;
        }
    }

    // Function to check the OWNER mode =>
    bool isOwner()
    {
        ifstream file("owner.txt");
        // if file not opened then show an massage and return false
        if (!file)
        {
            cout << "Error: Could not open owner file." << endl;
            return false;
        }
        string savedPassword, inputPassword;

        // read password from file and store it in variable
        file >> savedPassword;
        file.close();

        // Clear the input buffer after reading an password
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter password: ";
        getline(cin, inputPassword);

        // if inputPassword is equal to enteredPassword then it will return true
        return inputPassword == savedPassword;
    }

    //-------Customer Related Functions--------------//
    // Function to place orders by customer =>
    void placeOrder()
    {
        try
        {
            if (menu.empty())
            {
                throw runtime_error("Sorry, no items available to order.");
            }

            int customerId;
            cout << "Enter customer ID (0 for new customer): ";
            cin >> customerId;

            // Handle new customer registration
            if (customerId == 0)
            {
                registerCustomer();
                customerId = customerIdCounter - 1;
            }
            // Handle invalid customer ID
            else if (customers.find(customerId) == customers.end())
            {
                throw invalid_argument("Customer ID not found!");
            }

            cout << "Welcome, " << customers[customerId] << "!" << endl;

            int choice, quantity;
            currentBill = 0;
            currentOrder.clear();
            displayMenu();

            vector<pair<string, float>> orderDetails;

            do
            {
                cout << "\nEnter item index to order (0 to finish): ";
                cin >> choice;

                // Validate the choice
                if (choice > 0 && choice <= menu.size())
                {
                    cout << "\nEnter quantity: ";
                    cin >> quantity;

                    // Validate stock availability
                    if (stock[choice - 1] >= quantity)
                    {
                        string itemName = menu[choice - 1].first;
                        float price = menu[choice - 1].second * quantity;

                        currentOrder.push_back({itemName, quantity});
                        orderDetails.push_back({itemName, price});
                        currentBill += price;
                        stock[choice - 1] -= quantity;

                        cout << "\nAdded " << quantity << " x " << itemName
                             << " - Rs." << fixed << setprecision(2) << price
                             << " to your order.\n"
                             << endl;
                    }
                    else
                    {
                        cout << "Insufficient stock for " << menu[choice - 1].first << "." << endl;
                    }
                }
                else if (choice != 0)
                {
                    cout << "Invalid choice." << endl;
                }
            } while (choice != 0);

            // Check if any items were ordered
            if (!currentOrder.empty())
            {
                // Update the orders and save
                customerOrders[customerId] = orderDetails;
                saveOrderToFile(customerId);
                saveMenuToFile();

                // Print the total bill
                cout << "\n===== Order Summary =====" << endl;
                for (const auto &item : orderDetails)
                {
                    cout << item.first << ": Rs." << fixed << setprecision(2) << item.second << endl;
                }
                cout << "------------------------" << endl;
                cout << "Total Bill: Rs." << fixed << setprecision(2) << currentBill << endl;
            }
            else
            {
                throw logic_error("No items were selected to order.");
            }
        }
        catch (const runtime_error &e)
        {
            cout << "Error: " << e.what() << endl;
        }
        catch (const invalid_argument &e)
        {
            cout << "Error: " << e.what() << endl;
        }
        catch (const logic_error &e)
        {
            cout << "Error: " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "Unexpected error: " << e.what() << endl;
        }
    }

    // Function to register a new customer
    void registerCustomer()
    {
        try
        {
            string name, phone;
            cout << "Enter customer name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);

            if (name.empty())
            {
                throw invalid_argument("Customer name cannot be empty.");
            }

            // Loop until a valid phone number is entered
            while (true)
            {
                cout << "Enter customer phone number: ";
                getline(cin, phone);

                if (phone.empty())
                {
                    cout << "Phone number cannot be empty. Please enter a valid phone number." << endl;
                    continue; // Ask for phone number again
                }

                // Validate phone number (must be exactly 10 digits)
                if (!regex_match(phone, regex("\\d{10}")))
                {
                    cout << "Phone number must be exactly 10 digits. Please enter a valid phone number." << endl;
                    continue; // Ask for phone number again
                }

                break; // Exit loop if phone number is valid
            }

            int newId = customerIdCounter++;
            customers[newId] = name;
            customerPhones[newId] = phone;

            // Saving customer data to a file
            ofstream outFile("customers.txt", ios::app);
            if (!outFile)
            {
                throw runtime_error("Error opening the file to save customer data!");
            }

            outFile << newId << "," << name << "," << phone << endl;
            outFile.close();

            cout << "Customer registered successfully!" << endl;
            cout << "Customer ID: " << newId << endl;
        }
        catch (const invalid_argument &e)
        {
            cout << "Input Error: " << e.what() << endl;
        }
        catch (const runtime_error &e)
        {
            cout << "File Error: " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "Unexpected Error: " << e.what() << endl;
        }
    }

    // Function to load customer data =>
    void loadCustomerData()
    {
        ifstream inFile("customers.txt");
        if (!inFile)
        {
            cout << "No existing customer data found. Starting fresh." << endl;
            customerIdCounter = 1; // Start fresh with ID 1
            return;
        }

        string line;
        int maxId = 0;

        while (getline(inFile, line))
        {
            stringstream ss(line);
            string token;
            vector<string> tokens;

            // Split the line by ',' to extract ID and name
            while (getline(ss, token, ','))
            {
                tokens.push_back(token);
            }

            if (tokens.size() >= 2)
            {
                int id = stoi(tokens[0]);  // Extract ID
                customers[id] = tokens[1]; // Store in the map
                if (id > maxId)
                    maxId = id; // Track the largest ID
            }
        }

        customerIdCounter = maxId + 1; // Set the next ID as max ID + 1
        inFile.close();
    }

    // Function to save order to file =>
    void saveOrderToFile(int customerId)
    {
        ofstream file("orders.txt", ios::app);
        if (!file)
        {
            cout << "Error: Could not save order to file." << endl;
            return;
        }

        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);

        file << "\nOrder Details - " << ctime(&currentTime);
        file << "Customer ID: " << customerId << endl;
        file << "Customer Name: " << customers[customerId] << endl;

        for (const auto &item : currentOrder)
        {
            file << setw(20) << item.first << " x" << item.second << endl; // Assuming item.second is quantity here
        }

        file << "Total Bill: Rs." << fixed << setprecision(2) << currentBill << endl;
        file << string(40, '-') << endl;
        file.close();
    }

    // Function to view Customer Details =>
    void viewCustomerDetails()
    {
        try
        {
            int customerId;
            cout << "Enter customer ID: ";
            cin >> customerId;

            // Validate input
            if (cin.fail() || customerId < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid customer ID entered.");
            }

            // Check if the customer ID exists in the map
            if (customers.find(customerId) == customers.end())
            {
                throw runtime_error("Customer not found in the File.");
            }

            cout << string(40, '-') << endl;
            cout << "Order History:" << endl;

            ifstream file("orders.txt");
            if (!file.is_open())
            {
                throw runtime_error("Error: Could not open 'orders.txt'.");
            }

            // Open customers.txt to get the phone number
            ifstream customersFile("customers.txt");
            if (!customersFile.is_open())
            {
                throw runtime_error("Error: Could not open 'customers.txt'.");
            }

            string line;
            string phoneNumber;
            bool foundCustomer = false;
            while (getline(customersFile, line))
            {
                stringstream ss(line);
                int id;
                string name;
                string phone;
                char delimiter;
                ss >> id >> delimiter;
                getline(ss, name, ',');
                ss >> phone;

                if (id == customerId)
                {
                    phoneNumber = phone;
                    foundCustomer = true;
                    break;
                }
            }

            if (!foundCustomer)
            {
                throw runtime_error("Customer ID not found in 'customers.txt'.");
            }

            // Display basic customer info
            cout << string(40, '-') << endl;
            cout << "Customer Name: " << customers[customerId] << endl;
            cout << "Phone Number: " << phoneNumber << endl;

            bool foundOrder = false;
            string searchKey = "Customer ID: " + to_string(customerId); // Exact match key

            while (getline(file, line))
            {
                // Check if the current line contains the exact customer ID
                if (line == searchKey) // Compare the entire line
                {
                    foundOrder = true;
                    cout << line << endl;

                    // Read all subsequent lines related to this customer's orders
                    while (getline(file, line) && !line.empty())
                    {
                        cout << "  " << line << endl; // Indented for clarity
                    }
                    cout << endl;
                }
            }

            if (!foundOrder)
            {
                cout << "No order history found for this customer." << endl;
            }

            file.close();
        }
        catch (const runtime_error &e)
        {
            cout << "Error: " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "Unexpected error: " << e.what() << endl;
        }
        catch (...)
        {
            cout << "An unknown error occurred." << endl;
        }
    }

    //-----------Employee Related Functions-------------//
    // Function to load employee data and initialize employeeIdCounter
    void loadEmployeeData()
    {
        ifstream inFile("employees.txt");
        if (!inFile)
        {
            cout << "No existing employee data found. Starting fresh." << endl;
            return;
        }

        string line;
        int maxId = 0;

        while (getline(inFile, line))
        {
            stringstream ss(line);
            string token;
            int employeeId;
            string name, work;
            float salary;
            int fieldIndex = 0;

            // Parse each line
            while (getline(ss, token, ','))
            {
                if (fieldIndex == 0)
                    employeeId = stoi(token);
                else if (fieldIndex == 1)
                    name = token;
                else if (fieldIndex == 2)
                    work = token;
                else if (fieldIndex == 3)
                    salary = stof(token);

                fieldIndex++;
            }

            // Update in-memory data structures
            employees[employeeId] = {name, salary}; // Store name and salary with employeeId
            employeeWork[employeeId] = work;        // Store work type using employeeId

            // Track the maximum ID
            if (employeeId > maxId)
            {
                maxId = employeeId;
            }
        }

        // Set the next available ID
        employeeIdCounter = maxId + 1;

        inFile.close();
    }

    // Function to add employee details
    void addEmployee()
    {
        try
        {
            string name, work, phone;
            float salary;

            cout << "Enter employee name: ";
            cin.ignore();
            getline(cin, name);

            // Input validation for employee name
            if (name.empty())
            {
                throw invalid_argument("Employee name cannot be empty.");
            }

            cout << "Enter employee work type (e.g., Chef, Waiter, etc.): ";
            getline(cin, work);

            // Input validation for employee work type
            if (work.empty())
            {
                throw invalid_argument("Work type cannot be empty.");
            }

            cout << "Enter employee salary: ";
            cin >> salary;

            // Input validation for salary
            if (salary <= 0)
            {
                throw invalid_argument("Salary must be a positive number.");
            }

            // Loop until a valid phone number is entered
            while (true)
            {
                cout << "Enter employee phone number: ";

                // Clear any leftover newline character from previous inputs
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // Take input directly
                getline(cin, phone);

                if (phone.empty())
                {
                    cout << "Phone number cannot be empty. Please enter a valid phone number." << endl;
                    continue; // Ask for phone number again
                }

                // Validate phone number (must be exactly 10 digits)
                if (!regex_match(phone, regex("\\d{10}")))
                {
                    cout << "Phone number must be exactly 10 digits. Please enter a valid phone number." << endl;
                    continue; // Ask for phone number again
                }

                break; // Exit loop if phone number is valid
            }

            int employeeId = employeeIdCounter++;
            employees[employeeId] = {name, salary};
            employeeWork[employeeId] = work;    // Store work type by employee ID
            employeePhones[employeeId] = phone; // Store phone number by employee ID

            // Save to file with exception handling for file opening
            ofstream outFile("employees.txt", ios::app);
            if (!outFile)
            {
                throw runtime_error("Error opening file for saving employee data.");
            }
            outFile << employeeId << "," << name << "," << work << "," << salary << "," << phone << endl;
            outFile.close();

            cout << "Employee added successfully with ID " << employeeId << endl;
        }
        catch (const invalid_argument &e)
        {
            cout << "Invalid input: " << e.what() << endl; // Handle invalid arguments such as empty name or non-positive salary
        }
        catch (const runtime_error &e)
        {
            cout << "Runtime error: " << e.what() << endl; // Handle errors related to file handling
        }
        catch (const exception &e)
        {
            cout << "Unexpected error: " << e.what() << endl; // Catch any other standard exceptions
        }
        catch (...)
        {
            cout << "An unknown error occurred." << endl; // Catch any non-standard exceptions
        }
    }

    // Function to view employee details by ID
    void viewEmployeeDetails()
    {
        try
        {
            int employeeId;
            cout << "Enter employee ID: ";
            cin >> employeeId;

            // Check for invalid input type (non-integer)
            if (cin.fail())
            {
                throw invalid_argument("Invalid employee ID. Please enter a valid integer.");
            }

            // Check if employee exists in the map
            if (employees.find(employeeId) != employees.end())
            {
                auto employee = employees[employeeId];
                cout << "Employee Name: " << employee.first << endl;
                cout << "Salary: Rs." << employee.second << endl;

                // Fetch the correct work type using employeeId
                if (employeeWork.find(employeeId) != employeeWork.end())
                {
                    cout << "Work Type: " << employeeWork[employeeId] << endl;
                }
                else
                {
                    cout << "Work Type: Not assigned." << endl;
                }

                // Fetch the phone number using employeeId
                if (employeePhones.find(employeeId) != employeePhones.end())
                {
                    cout << "Phone Number: " << employeePhones[employeeId] << endl;
                }
                else
                {
                    cout << "Phone Number: Not available." << endl;
                }
            }
            else
            {
                throw runtime_error("Employee not found.");
            }
        }
        catch (const invalid_argument &e)
        {
            cout << "Input Error: " << e.what() << endl; // Handle invalid input (non-integer employee ID)
        }
        catch (const runtime_error &e)
        {
            cout << "Runtime Error: " << e.what() << endl; // Handle employee not found or other runtime errors
        }
        catch (const exception &e)
        {
            cout << "Unexpected Error: " << e.what() << endl; // Catch any other standard exceptions
        }
        catch (...)
        {
            cout << "An unknown error occurred." << endl; // Catch any non-standard exceptions
        }
    }

    // Function to delete Employee Details =>
    void deleteEmployee()
    {
        try
        {
            int employeeId;
            cout << "Enter employee ID to delete: ";
            cin >> employeeId;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Invalid employee ID. Please enter a valid integer.");
            }

            if (employees.find(employeeId) == employees.end())
            {
                throw runtime_error("Employee not found in memory.");
            }

            employees.erase(employeeId);
            employeeWork.erase(employeeId);

            ifstream inFile("employees.txt");
            ofstream tempFile("temp.txt");

            if (!inFile)
            {
                throw runtime_error("Error opening employee data file for reading.");
            }
            if (!tempFile)
            {
                throw runtime_error("Error opening temporary file for writing.");
            }

            string line;
            bool employeeDeleted = false;

            while (getline(inFile, line))
            {
                stringstream ss(line);
                string tempEmployeeId, tempSalary, fileName, fileWorkType;

                if (!getline(ss, tempEmployeeId, ',') || !getline(ss, fileName, ',') ||
                    !getline(ss, fileWorkType, ',') || !getline(ss, tempSalary))
                {
                    cerr << "Warning: Malformed line skipped: " << line << endl;
                    continue; // Skip malformed lines
                }

                int fileEmployeeId = stoi(tempEmployeeId);
                float fileSalary = stof(tempSalary);

                if (fileEmployeeId != employeeId)
                {
                    tempFile << fileEmployeeId << "," << fileName << "," << fileWorkType << "," << fileSalary << endl;
                }
                else
                {
                    employeeDeleted = true;
                }
            }

            inFile.close();
            tempFile.close();

            if (remove("employees.txt") != 0)
            {
                throw runtime_error("Error deleting the old employee data file.");
            }
            if (rename("temp.txt", "employees.txt") != 0)
            {
                throw runtime_error("Error renaming the temporary file.");
            }

            if (employeeDeleted)
            {
                cout << "Employee with ID " << employeeId << " has been deleted." << endl;
            }
            else
            {
                throw runtime_error("Employee not found in file (unexpected).");
            }
        }
        catch (const invalid_argument &e)
        {
            cout << "Input Error: " << e.what() << endl;
        }
        catch (const runtime_error &e)
        {
            cout << "Runtime Error: " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "Unexpected Error: " << e.what() << endl;
        }
        catch (...)
        {
            cout << "An unknown error occurred." << endl;
        }
    }

    //--------------Employee Details----------//
    // Function to check employee details =>
    void employeeDetails()
    {
        int choice;
        do
        {
            try
            {
                cout << "\n====== Employee Details ======" << endl;
                cout << "1. Add Employee" << endl;
                cout << "2. View Employee Details" << endl;
                cout << "3. Delete Employee Details" << endl;
                cout << "4. Exit" << endl;
                cout << "======================" << endl;
                cout << "Enter Choice: ";
                cin >> choice;

                // Check for invalid input
                if (cin.fail())
                {
                    throw invalid_argument("Invalid input! Please enter a valid choice.");
                }

                switch (choice)
                {
                case 1:
                    addEmployee();
                    break;
                case 2:
                    viewEmployeeDetails();
                    break;
                case 3:
                    deleteEmployee();
                    break;
                case 4:
                    cout << "Exiting from Employee Details..." << endl;
                    break;
                default:
                    throw out_of_range("Invalid choice. Please select a valid option from the menu.");
                }
            }
            catch (const invalid_argument &e)
            {
                cout << "Error: " << e.what() << endl;
                // Clear the error state and ignore the invalid input
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            catch (const out_of_range &e)
            {
                cout << "Error: " << e.what() << endl;
            }
            catch (const ios_base::failure &e)
            {
                cout << "File handling error: " << e.what() << endl;
            }
            catch (const exception &e)
            {
                // Catch any other exceptions
                cout << "An unexpected error occurred: " << e.what() << endl;
            }

        } while (choice != 4);
    }

    //------------Restaurant Dashboard----------//
    // function to display restaurant dashboard
    void displayDashboard()
    {
        int choice;
        cout << "Welcome to the Restaurant Dashboard!\n"
             << endl;
        do
        {
            cout << "\n====== Restaurant Dashboard ======" << endl;
            cout << "1. Check Today's Details\n";
            cout << "2. View Last Week's Summary\n";
            cout << "3. View Last Month's Summary\n";
            cout << "4. Custom Date Details\n";
            cout << "5. Custom Date Range Analysis\n";
            cout << "6. Exit\n";
            cout << string(35, '=') << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                showTodaysDetails();
                break;
            case 2:
                showLastWeeksSummary();
                break;
            case 3:
                showLastMonthsSummary();
                break;
            case 4:
                showSingleDayDetails();
                break;
            case 5:
                showCustomDateDetails();
                break;
            case 6:
                cout << "Exiting Restaurant Dashboard.\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 6);
    }

    // function to display todays details
    void showTodaysDetails()
    {
        // Current Date
        time_t now = time(0);
        tm *currentTime = localtime(&now);
        int currentDay = currentTime->tm_mday;
        int currentMonth = currentTime->tm_mon + 1;    // Months are 0-indexed
        int currentYear = currentTime->tm_year + 1900; // Year since 1900

        unordered_map<int, string> customerPhoneMap;

        ifstream customerFile("customers.txt");
        if (customerFile)
        {
            string line;
            while (getline(customerFile, line))
            {
                stringstream ss(line);
                int customerId;
                string customerName, phoneNumber;

                // Parse customer details
                if (getline(ss, line, ','))
                    customerId = stoi(line);
                if (getline(ss, customerName, ',') && getline(ss, phoneNumber))
                    customerPhoneMap[customerId] = phoneNumber;
            }
            customerFile.close();
        }

        // Open Orders File
        ifstream inFile("orders.txt");
        if (!inFile)
        {
            cout << "Error opening orders file!" << endl;
            return;
        }

        string line;
        float totalBill = 0;
        int totalCustomers = 0, totalOrders = 0, totalItemsSold = 0; // Added totalItemsSold

        cout << "\nOrders for Today (" << currentDay << "/" << currentMonth << "/" << currentYear << "):\n";
        cout << left << setw(15) << "Customer ID"
             << setw(25) << "Customer Name"
             << setw(15) << "Phone Number"
             << setw(20) << "Total Items"
             << setw(15) << "Total Bill" << endl;
        cout << string(95, '-') << endl;

        string orderDate, customerIdStr, customerName, totalBillStr;

        while (getline(inFile, line))
        {
            // Look for the date in the "Order Details -" line
            if (line.rfind("Order Details -", 0) == 0)
            {
                // Extract the date part
                string dateStr = line.substr(15); // Skip "Order Details -"
                tm orderTime = {};
                istringstream dateStream(dateStr);
                dateStream >> get_time(&orderTime, "%a %b %d %H:%M:%S %Y");

                // Check if the order date matches today's date
                if (orderTime.tm_mday == currentDay &&
                    (orderTime.tm_mon + 1) == currentMonth &&
                    (orderTime.tm_year + 1900) == currentYear)
                {
                    // Read and extract relevant details from subsequent lines
                    getline(inFile, line); // Customer ID
                    int customerId = stoi(line.substr(line.find(":") + 2));

                    getline(inFile, line); // Customer Name
                    customerName = line.substr(line.find(":") + 2);

                    int totalItems = 0;
                    while (getline(inFile, line) && !line.empty() && line.find("Total Bill:") == string::npos)
                    {
                        // Count the items from item lines
                        size_t xPos = line.find("x");
                        if (xPos != string::npos)
                        {
                            int itemQuantity = stoi(line.substr(xPos + 1));
                            totalItems += itemQuantity;
                        }
                    }

                    if (line.find("Total Bill:") != string::npos)
                    {
                        totalBillStr = line.substr(line.find("Rs.") + 3);
                        float bill = stof(totalBillStr);

                        // Add to totals
                        totalBill += bill;
                        totalCustomers++;
                        totalOrders++;
                        totalItemsSold += totalItems; // Accumulate total items sold

                        // Get the phone number from the map
                        string phoneNumber = customerPhoneMap.count(customerId) ? customerPhoneMap[customerId] : "N/A";

                        // Display the order
                        cout << left << setw(15) << customerId
                             << setw(25) << customerName
                             << setw(15) << phoneNumber
                             << setw(20) << totalItems
                             << setw(15) << "Rs." + totalBillStr << endl;
                    }
                }
            }
        }
        cout << string(95, '-') << endl;

        // Summary for today
        cout << "\nSummary for Today (" << currentDay << "/" << currentMonth << "/" << currentYear << "):\n";
        cout << string(30, '=') << endl;
        cout << "Total Orders: " << totalOrders << endl;
        cout << "Total Customers: " << totalCustomers << endl;
        cout << "Total Bill: Rs." << totalBill << endl;
        cout << "Total Items Sold: " << totalItemsSold << endl; // Display total items sold
        cout << string(30, '=');

        inFile.close();
    }

    // Function to display custom day details
    void showSingleDayDetails()
    {
        // Input date
        string inputDateStr;
        cout << "Enter date (format: DD/MM/YYYY): ";
        cin >> inputDateStr;

        // Convert input string to time_t for comparison
        tm inputDate = {};
        istringstream inputStream(inputDateStr);
        inputStream >> get_time(&inputDate, "%d/%m/%Y");

        // Normalize the time structure to time_t
        time_t inputEpoch = mktime(&inputDate);

        // Read Customer File to Map Customer ID -> Phone Number
        unordered_map<int, string> customerPhoneMap;
        ifstream customerFile("customers.txt");
        if (customerFile)
        {
            string line;
            while (getline(customerFile, line))
            {
                stringstream ss(line);
                int customerId;
                string customerName, phoneNumber;

                // Parse customer details
                if (getline(ss, line, ','))
                {
                    try
                    {
                        customerId = stoi(line);
                    }
                    catch (const invalid_argument &e)
                    {
                        continue; // Skip this line if invalid
                    }
                }
                if (getline(ss, customerName, ',') && getline(ss, phoneNumber))
                    customerPhoneMap[customerId] = phoneNumber;
            }
            customerFile.close();
        }

        // Open Orders File
        ifstream inFile("orders.txt");
        if (!inFile)
        {
            cout << "Error opening orders file!" << endl;
            return;
        }

        string line;
        float totalBill = 0;
        int totalCustomers = 0, totalOrders = 0, totalItemsSold = 0; // New variable for total items sold

        cout << "\nOrders for " << inputDateStr << ":\n";
        cout << left << setw(15) << "Customer ID"
             << setw(25) << "Customer Name"
             << setw(15) << "Phone Number"
             << setw(20) << "Total Items"
             << setw(15) << "Total Bill"
             << setw(15) << "Date" << endl;
        cout << string(95, '-') << endl;

        string orderDate, customerIdStr, customerName, totalBillStr;

        while (getline(inFile, line))
        {
            // Look for the date in the "Order Details -" line
            if (line.rfind("Order Details -", 0) == 0)
            {
                // Extract the date part
                string dateStr = line.substr(15); // Skip "Order Details -"
                tm orderTime = {};
                istringstream dateStream(dateStr);
                dateStream >> get_time(&orderTime, "%a %b %d %H:%M:%S %Y");

                // Check if the order date matches the input date
                if (orderTime.tm_year == inputDate.tm_year &&
                    orderTime.tm_mon == inputDate.tm_mon &&
                    orderTime.tm_mday == inputDate.tm_mday)
                {
                    // Read and extract relevant details from subsequent lines
                    getline(inFile, line); // Customer ID
                    int customerId = -1;
                    try
                    {
                        customerId = stoi(line.substr(line.find(":") + 2));
                    }
                    catch (const invalid_argument &e)
                    {
                        continue; // Skip this order if invalid
                    }

                    getline(inFile, line); // Customer Name
                    customerName = line.substr(line.find(":") + 2);

                    int totalItems = 0;
                    while (getline(inFile, line) && !line.empty() && line.find("Total Bill:") == string::npos)
                    {
                        // Count the items from item lines
                        size_t xPos = line.find("x");
                        if (xPos != string::npos)
                        {
                            try
                            {
                                int itemQuantity = stoi(line.substr(xPos + 1));
                                totalItems += itemQuantity;
                            }
                            catch (const invalid_argument &e)
                            {
                                continue; // Ignore invalid item lines
                            }
                        }
                    }

                    if (line.find("Total Bill:") != string::npos)
                    {
                        totalBillStr = line.substr(line.find("Rs.") + 3);
                        float bill = 0.0f;
                        try
                        {
                            bill = stof(totalBillStr);
                        }
                        catch (const invalid_argument &e)
                        {
                            continue; // Skip this order if invalid
                        }

                        // Add to totals
                        totalBill += bill;
                        totalCustomers++;
                        totalOrders++;
                        totalItemsSold += totalItems; // Accumulate total items sold

                        // Get the phone number from the map
                        string phoneNumber = customerPhoneMap.count(customerId) ? customerPhoneMap[customerId] : "N/A";

                        // Format date as DD/MM/YYYY
                        stringstream formattedDate;
                        formattedDate << orderTime.tm_mday << "/"
                                      << orderTime.tm_mon + 1 << "/"
                                      << orderTime.tm_year + 1900;

                        // Display the order
                        cout << left << setw(15) << customerId
                             << setw(25) << customerName
                             << setw(15) << phoneNumber
                             << setw(20) << totalItems
                             << setw(15) << "Rs." + totalBillStr
                             << setw(15) << formattedDate.str() << endl;
                    }
                }
            }
        }
        cout << string(95, '-') << endl;

        // Summary for the day
        cout << "\nSummary for " << inputDateStr << ":\n";
        cout << string(30, '=') << endl;
        cout << "Total Orders: " << totalOrders << endl;
        cout << "Total Customers: " << totalCustomers << endl;
        cout << "Total Bill: Rs." << totalBill << endl;
        cout << "Total Items Sold: " << totalItemsSold << endl; // Display total items sold
        cout << string(30, '=') << endl;

        inFile.close();
    }

    // Function to display last 7 days details
    void showLastWeeksSummary()
    {
        // Current Date
        time_t now = time(0);
        tm *currentTime = localtime(&now);

        // Calculate last week's start date
        tm lastWeekStartTime = *currentTime;
        lastWeekStartTime.tm_mday -= 7; // Subtract 7 days
        mktime(&lastWeekStartTime);     // Normalize the date

        // Read Customer File to Map Customer ID -> Phone Number
        unordered_map<int, string> customerPhoneMap;
        ifstream customerFile("customers.txt");
        if (customerFile)
        {
            string line;
            while (getline(customerFile, line))
            {
                stringstream ss(line);
                int customerId;
                string customerName, phoneNumber;

                // Parse customer details
                if (getline(ss, line, ','))
                {
                    try
                    {
                        customerId = stoi(line);
                    }
                    catch (const invalid_argument &e)
                    {
                        continue; // Skip this line if invalid
                    }
                }
                if (getline(ss, customerName, ',') && getline(ss, phoneNumber))
                    customerPhoneMap[customerId] = phoneNumber;
            }
            customerFile.close();
        }

        // Open Orders File
        ifstream inFile("orders.txt");
        if (!inFile)
        {
            cout << "Error opening orders file!" << endl;
            return;
        }

        string line;
        float totalBill = 0;
        int totalCustomers = 0, totalOrders = 0, totalItemsSold = 0; // Added totalItemsSold

        cout << "\nOrders for Last Week ("
             << lastWeekStartTime.tm_mday << "/" << lastWeekStartTime.tm_mon + 1 << "/" << lastWeekStartTime.tm_year + 1900
             << " - " << currentTime->tm_mday << "/" << currentTime->tm_mon + 1 << "/" << currentTime->tm_year + 1900 << "):\n";
        cout << left << setw(15) << "Customer ID"
             << setw(25) << "Customer Name"
             << setw(15) << "Phone Number"
             << setw(20) << "Total Items"
             << setw(15) << "Total Bill"
             << setw(15) << "Date" << endl;
        cout << string(95, '-') << endl;
        string orderDate, customerIdStr, customerName, totalBillStr;

        while (getline(inFile, line))
        {
            // Look for the date in the "Order Details -" line
            if (line.rfind("Order Details -", 0) == 0)
            {
                // Extract the date part
                string dateStr = line.substr(15); // Skip "Order Details -"
                tm orderTime = {};
                istringstream dateStream(dateStr);
                dateStream >> get_time(&orderTime, "%a %b %d %H:%M:%S %Y");

                // Check if the order date falls within the last week's range
                time_t orderTimeEpoch = mktime(&orderTime);
                time_t lastWeekStartEpoch = mktime(&lastWeekStartTime);
                if (difftime(orderTimeEpoch, lastWeekStartEpoch) >= 0 && difftime(now, orderTimeEpoch) >= 0)
                {
                    // Read and extract relevant details from subsequent lines
                    getline(inFile, line); // Customer ID
                    int customerId = -1;
                    try
                    {
                        customerId = stoi(line.substr(line.find(":") + 2));
                    }
                    catch (const invalid_argument &e)
                    {
                        continue; // Skip this order if invalid
                    }

                    getline(inFile, line); // Customer Name
                    customerName = line.substr(line.find(":") + 2);

                    int totalItems = 0;
                    while (getline(inFile, line) && !line.empty() && line.find("Total Bill:") == string::npos)
                    {
                        // Count the items from item lines
                        size_t xPos = line.find("x");
                        if (xPos != string::npos)
                        {
                            try
                            {
                                int itemQuantity = stoi(line.substr(xPos + 1));
                                totalItems += itemQuantity;
                            }
                            catch (const invalid_argument &e)
                            {
                                continue; // Ignore invalid item lines
                            }
                        }
                    }

                    if (line.find("Total Bill:") != string::npos)
                    {
                        totalBillStr = line.substr(line.find("Rs.") + 3);
                        float bill = stof(totalBillStr);

                        // Add to totals
                        totalBill += bill;
                        totalCustomers++;
                        totalOrders++;
                        totalItemsSold += totalItems; // Accumulate total items sold

                        // Get the phone number from the map
                        string phoneNumber = customerPhoneMap.count(customerId) ? customerPhoneMap[customerId] : "N/A";

                        // Format date as DD/MM/YYYY
                        stringstream formattedDate;
                        formattedDate << orderTime.tm_mday << "/"
                                      << orderTime.tm_mon + 1 << "/"
                                      << orderTime.tm_year + 1900;

                        // Display the order
                        cout << left << setw(15) << customerId
                             << setw(25) << customerName
                             << setw(15) << phoneNumber
                             << setw(20) << totalItems
                             << setw(15) << "Rs." + totalBillStr
                             << setw(15) << formattedDate.str() << endl;
                    }
                }
            }
        }
        cout << string(95, '-') << endl;

        // Summary for last week
        cout << "\nSummary for Last Week ("
             << lastWeekStartTime.tm_mday << "/" << lastWeekStartTime.tm_mon + 1 << "/" << lastWeekStartTime.tm_year + 1900
             << " - " << currentTime->tm_mday << "/" << currentTime->tm_mon + 1 << "/" << currentTime->tm_year + 1900
             << "):\n";
        cout << string(30, '=') << endl;
        cout << "Total Orders: " << totalOrders << endl;
        cout << "Total Customers: " << totalCustomers << endl;
        cout << "Total Bill: Rs." << totalBill << endl;
        cout << "Total Items Sold: " << totalItemsSold << endl; // Display total items sold
        cout << string(30, '=') << endl;

        inFile.close();
    }

    // Function to display Last 30 days details
    void showLastMonthsSummary()
    {
        // Current Date
        time_t now = time(0);
        tm *currentTime = localtime(&now);

        // Calculate last month's start date (30 days ago)
        tm pastMonthStartTime = *currentTime;
        pastMonthStartTime.tm_mday -= 30; // Subtract 30 days
        mktime(&pastMonthStartTime);      // Normalize the date

        // Read Customer File to Map Customer ID -> Phone Number
        unordered_map<int, string> customerPhoneMap;
        ifstream customerFile("customers.txt");
        if (customerFile)
        {
            string line;
            while (getline(customerFile, line))
            {
                stringstream ss(line);
                int customerId;
                string customerName, phoneNumber;

                // Parse customer details
                if (getline(ss, line, ','))
                {
                    try
                    {
                        customerId = stoi(line);
                    }
                    catch (const invalid_argument &e)
                    {
                        continue; // Skip this line if invalid
                    }
                }
                if (getline(ss, customerName, ',') && getline(ss, phoneNumber))
                    customerPhoneMap[customerId] = phoneNumber;
            }
            customerFile.close();
        }

        // Open Orders File
        ifstream inFile("orders.txt");
        if (!inFile)
        {
            cout << "Error opening orders file!" << endl;
            return;
        }

        string line;
        float totalBill = 0;
        int totalCustomers = 0, totalOrders = 0, totalItemsSold = 0; // Added totalItemsSold

        cout << "\nOrders for Last 30 Days (From " << pastMonthStartTime.tm_mday << "/" << pastMonthStartTime.tm_mon + 1 << "/" << pastMonthStartTime.tm_year + 1900
             << " to " << currentTime->tm_mday << "/" << currentTime->tm_mon + 1 << "/" << currentTime->tm_year + 1900 << "):\n";
        cout << left << setw(15) << "Customer ID"
             << setw(25) << "Customer Name"
             << setw(15) << "Phone Number"
             << setw(20) << "Total Items"
             << setw(15) << "Total Bill"
             << setw(15) << "Date" << endl;
        cout << string(95, '-') << endl;

        string orderDate, customerIdStr, customerName, totalBillStr;

        while (getline(inFile, line))
        {
            // Look for the date in the "Order Details -" line
            if (line.rfind("Order Details -", 0) == 0)
            {
                // Extract the date part
                string dateStr = line.substr(15); // Skip "Order Details -"
                tm orderTime = {};
                istringstream dateStream(dateStr);
                dateStream >> get_time(&orderTime, "%a %b %d %H:%M:%S %Y");

                // Check if the order date falls within the last 30 days range
                time_t orderTimeEpoch = mktime(&orderTime);
                time_t pastMonthStartEpoch = mktime(&pastMonthStartTime);
                if (difftime(orderTimeEpoch, pastMonthStartEpoch) >= 0 && difftime(now, orderTimeEpoch) >= 0)
                {
                    // Read and extract relevant details from subsequent lines
                    getline(inFile, line); // Customer ID
                    int customerId = -1;
                    try
                    {
                        customerId = stoi(line.substr(line.find(":") + 2));
                    }
                    catch (const invalid_argument &e)
                    {
                        continue; // Skip this order if invalid
                    }

                    getline(inFile, line); // Customer Name
                    customerName = line.substr(line.find(":") + 2);

                    int totalItems = 0;
                    while (getline(inFile, line) && !line.empty() && line.find("Total Bill:") == string::npos)
                    {
                        // Count the items from item lines
                        size_t xPos = line.find("x");
                        if (xPos != string::npos)
                        {
                            try
                            {
                                int itemQuantity = stoi(line.substr(xPos + 1));
                                totalItems += itemQuantity;
                            }
                            catch (const invalid_argument &e)
                            {
                                continue; // Ignore invalid item lines
                            }
                        }
                    }

                    if (line.find("Total Bill:") != string::npos)
                    {
                        totalBillStr = line.substr(line.find("Rs.") + 3);
                        float bill = stof(totalBillStr);

                        // Add to totals
                        totalBill += bill;
                        totalCustomers++;
                        totalOrders++;
                        totalItemsSold += totalItems; // Accumulate total items sold

                        // Get the phone number from the map
                        string phoneNumber = customerPhoneMap.count(customerId) ? customerPhoneMap[customerId] : "N/A";

                        // Format date as DD/MM/YYYY
                        stringstream formattedDate;
                        formattedDate << orderTime.tm_mday << "/"
                                      << orderTime.tm_mon + 1 << "/"
                                      << orderTime.tm_year + 1900;

                        // Display the order
                        cout << left << setw(15) << customerId
                             << setw(25) << customerName
                             << setw(15) << phoneNumber
                             << setw(20) << totalItems
                             << setw(15) << "Rs." + totalBillStr
                             << setw(15) << formattedDate.str() << endl;
                    }
                }
            }
        }
        cout << string(95, '-') << endl;

        if (totalOrders == 0)
        {
            cout << "No orders found for the past 30 days.\n";
        }
        else
        {
            cout << "\nSummary for Last 30 Days:\n";
            cout << string(30, '=') << endl;
            cout << "Total Orders: " << totalOrders << endl;
            cout << "Total Customers: " << totalCustomers << endl;
            cout << "Total Bill: Rs." << totalBill << endl;
            cout << "Total Items Sold: " << totalItemsSold << endl; // Display total items sold
            cout << string(30, '=') << endl;
        }

        inFile.close();
    }

    // Function to display details using custom date range
    void showCustomDateDetails()
    {
        // Input start date and end date
        string startDateStr, endDateStr;
        cout << "Enter start date (format: DD/MM/YYYY): ";
        cin >> startDateStr;
        cout << "Enter end date (format: DD/MM/YYYY): ";
        cin >> endDateStr;

        // Convert input string to time_t for comparison
        tm startDate = {}, endDate = {};
        istringstream startStream(startDateStr);
        istringstream endStream(endDateStr);
        startStream >> get_time(&startDate, "%d/%m/%Y");
        endStream >> get_time(&endDate, "%d/%m/%Y");

        // Set the end time to the last moment of the end date (23:59:59)
        endDate.tm_hour = 23;
        endDate.tm_min = 59;
        endDate.tm_sec = 59;
        mktime(&endDate); // Normalize the end date time to include the whole day

        // Normalize the start date
        time_t startEpoch = mktime(&startDate);
        time_t endEpoch = mktime(&endDate);

        // Read Customer File to Map Customer ID -> Phone Number
        unordered_map<int, string> customerPhoneMap;
        ifstream customerFile("customers.txt");
        if (customerFile)
        {
            string line;
            while (getline(customerFile, line))
            {
                stringstream ss(line);
                int customerId;
                string customerName, phoneNumber;

                // Parse customer details
                if (getline(ss, line, ','))
                {
                    try
                    {
                        customerId = stoi(line);
                    }
                    catch (const invalid_argument &e)
                    {
                        continue; // Skip this line if invalid
                    }
                }
                if (getline(ss, customerName, ',') && getline(ss, phoneNumber))
                    customerPhoneMap[customerId] = phoneNumber;
            }
            customerFile.close();
        }

        // Open Orders File
        ifstream inFile("orders.txt");
        if (!inFile)
        {
            cout << "Error opening orders file!" << endl;
            return;
        }

        string line;
        float totalBill = 0;
        int totalCustomers = 0, totalOrders = 0, totalItemsSold = 0; // Added totalItemsSold

        cout << "\nOrders from " << startDateStr << " to " << endDateStr << ":\n";
        cout << left << setw(15) << "Customer ID"
             << setw(25) << "Customer Name"
             << setw(15) << "Phone Number"
             << setw(20) << "Total Items"
             << setw(15) << "Total Bill"
             << setw(15) << "Date" << endl;
        cout << string(95, '-') << endl;

        string orderDate, customerIdStr, customerName, totalBillStr;

        while (getline(inFile, line))
        {
            // Look for the date in the "Order Details -" line
            if (line.rfind("Order Details -", 0) == 0)
            {
                // Extract the date part
                string dateStr = line.substr(15); // Skip "Order Details -"
                tm orderTime = {};
                istringstream dateStream(dateStr);
                dateStream >> get_time(&orderTime, "%a %b %d %H:%M:%S %Y");

                // Check if the order date falls within the custom date range, including the end date
                time_t orderTimeEpoch = mktime(&orderTime);
                if (difftime(orderTimeEpoch, startEpoch) >= 0 && difftime(orderTimeEpoch, endEpoch) <= 0)
                {
                    // Read and extract relevant details from subsequent lines
                    getline(inFile, line); // Customer ID
                    int customerId = -1;
                    try
                    {
                        customerId = stoi(line.substr(line.find(":") + 2));
                    }
                    catch (const invalid_argument &e)
                    {
                        continue; // Skip this order if invalid
                    }

                    getline(inFile, line); // Customer Name
                    customerName = line.substr(line.find(":") + 2);

                    int totalItems = 0;
                    while (getline(inFile, line) && !line.empty() && line.find("Total Bill:") == string::npos)
                    {
                        // Count the items from item lines
                        size_t xPos = line.find("x");
                        if (xPos != string::npos)
                        {
                            try
                            {
                                int itemQuantity = stoi(line.substr(xPos + 1));
                                totalItems += itemQuantity;
                            }
                            catch (const invalid_argument &e)
                            {
                                continue; // Ignore invalid item lines
                            }
                        }
                    }

                    if (line.find("Total Bill:") != string::npos)
                    {
                        totalBillStr = line.substr(line.find("Rs.") + 3);
                        float bill = stof(totalBillStr);

                        // Add to totals
                        totalBill += bill;
                        totalCustomers++;
                        totalOrders++;
                        totalItemsSold += totalItems; // Accumulate total items sold

                        // Get the phone number from the map
                        string phoneNumber = customerPhoneMap.count(customerId) ? customerPhoneMap[customerId] : "N/A";

                        // Format date as DD/MM/YYYY
                        stringstream formattedDate;
                        formattedDate << orderTime.tm_mday << "/"
                                      << orderTime.tm_mon + 1 << "/"
                                      << orderTime.tm_year + 1900;

                        // Display the order
                        cout << left << setw(15) << customerId
                             << setw(25) << customerName
                             << setw(15) << phoneNumber
                             << setw(20) << totalItems
                             << setw(15) << "Rs." + totalBillStr
                             << setw(15) << formattedDate.str() << endl;
                    }
                }
            }
        }
        cout << string(95, '-') << endl;

        cout << "\nSummary for Custom Date Range:\n";
        cout << string(30, '=') << endl;
        cout << "Total Orders: " << totalOrders << endl;
        cout << "Total Customers: " << totalCustomers << endl;
        cout << "Total Bill: Rs." << totalBill << endl;
        cout << "Total Items Sold: " << totalItemsSold << endl; // Display total items sold
        cout << string(30, '=') << endl;

        inFile.close();
    }

    //----------------Owner Menu----------------//
    // Function for owner mode =>
    void ownerMenu()
    {
        // validation of owner
        if (!isOwner())
        {
            cout << "Access denied. Incorrect password." << endl;
            return;
        }
        int choice;
        // displays owner menu and take choices
        do
        {
            cout << "\n====== Owner Menu ======" << endl;
            cout << "1. Menu Details" << endl;
            cout << "2. Employee Details" << endl;
            cout << "3. Costomer Details" << endl;
            cout << "4. Restaurant Dashboard" << endl;
            cout << "5. Exit" << endl;
            cout << "======================" << endl;

            // basic validation of choice using Exception handling
            while (true)
            {
                try
                {
                    cout << "\nEnter your choice: ";
                    if (!(cin >> choice))
                    {
                        cin.clear();                                           // Clear the error flag on cin
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');   // Ignore all remaining input until newline
                        throw "\nInvalid input. Please enter a valid choice."; // Throw an error message
                    }
                    // If input is valid, break the loop
                    break;
                }
                catch (const char *msg)
                {
                    cout << msg << endl; // Catch and display the error message
                }
            }

            switch (choice)
            {
            case 1:
                menuDetails();
                break;
            case 2:
                employeeDetails();
                break;
            case 3:
            {
                // view costimer details by id function
                viewCustomerDetails();
                break;
            }
            case 4:
                displayDashboard();
                break;
            case 5:
                cout << "Exiting owner menu." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
            }
        } while (choice != 5);
    }

    //--------------Menu Details-----------//
    // Function To check menu details by Owner =>
    void menuDetails()
    {
        int choice;
        // displays owner menu and take choices
        do
        {
            cout << "\n====== Menu Details ======" << endl;
            cout << "1. Add New Menu Item" << endl;
            cout << "2. Delete Menu Item" << endl;
            cout << "3. Check Menu" << endl;
            cout << "4. Update Stock Quantity" << endl;
            cout << "5. Exit" << endl;
            cout << "======================" << endl;

            // basic validation of choice using Exception handling
            while (true)
            {
                try
                {
                    cout << "\nEnter your choice: ";
                    if (!(cin >> choice))
                    {
                        cin.clear();                                           // Clear the error flag on cin
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');   // Ignore all remaining input until newline
                        throw "\nInvalid input. Please enter a valid choice."; // Throw an error message
                    }
                    // If input is valid, break the loop
                    break;
                }
                catch (const char *msg)
                {
                    cout << msg << endl; // Catch and display the error message
                }
            }

            switch (choice)
            {
            case 1:
                addMenuItem();
                break;
            case 2:
                deleteMenuItem();
                break;
            case 3:
                displayMenu();
                break;
            case 4:
                updateStockQuantity();
                break;
            case 5:
                cout << "Exiting from Menu Details." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
            }
        } while (choice != 5);
    }

    //-----------------Customer Menu-----------------//
    // Function for customer mode =>
    void customerMenu()
    {
        int choice;
        // displays costomer menu and take choices from customer
        do
        {
            cout << "\n====== Customer Menu ======" << endl;
            cout << "1. View Menu" << endl;
            cout << "2. Place Order" << endl;
            cout << "3. Exit" << endl;
            cout << "=========================" << endl;

            // Basic validation of customer choice using Exception handling
            while (true)
            {
                try
                {
                    cout << "\nEnter your choice: ";
                    if (!(cin >> choice))
                    {
                        cin.clear();                                           // Clear the error flag on cin
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');   // Ignore all remaining input until newline
                        throw "\nInvalid input. Please enter a valid choice."; // Throw an error message
                    }
                    // If input is valid, break the loop
                    break;
                }
                catch (const char *msg)
                {
                    cout << msg << endl; // Catch and display the error message
                }
            }

            switch (choice)
            {
            case 1:
                displayMenu();
                break;
            case 2:
                placeOrder();
                break;
            case 3:
                cout << "Thank you for visiting!" << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
            }
        } while (choice != 3);
    }
};

//------------Main Function------------//
int main()
{
    // Object of restaurent class
    Restaurant restaurant;
    // load employee data into the program
    restaurant.loadEmployeeData();

    // load Customer data into the program
    restaurant.loadCustomerData();

    // load the menu vector from file
    restaurant.loadMenuFromFile();
    int mode;

    // displays owner and customer mode and take choices
    do
    {
        cout << "\n=== Restaurant Management System ===" << endl;
        cout << "1. Customer Mode" << endl;
        cout << "2. Owner Mode" << endl;
        cout << "3. Exit" << endl;
        cout << "=================================" << endl;

        // Basic validation of choice using Exception handling
        while (true)
        {
            try
            {
                cout << "\nSelect Mode: ";
                if (!(cin >> mode))
                {
                    cin.clear();                                         // Clear error flag of cin
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore all remaining input until newline
                    throw "\nInvalid input. Please enter a valid Mode."; // Throw an error message
                }
                break;
            }
            catch (const char *msg)
            {
                cout << msg << endl; // Catch the error
            }
        }

        switch (mode)
        {
        case 1:
            restaurant.customerMenu();
            break;
        case 2:
            restaurant.ownerMenu();
            break;
        case 3:
            cout << "\nThank you for using our system!" << endl;
            break;
        default:
            cout << "\nInvalid selection." << endl;
        }
    } while (mode != 3);

    return 0;
}
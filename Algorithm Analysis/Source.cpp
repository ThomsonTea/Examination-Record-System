#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <conio.h>
#include <tabulate/table.hpp>
using namespace tabulate;
using namespace std;

void bubbleSort(string** data, int rowCount, int& swapCount);
void merge(string** data, int left, int mid, int right);
void mergeSort(string** data, int left, int right);
void printData(string** data, int rowCount);
void linearSearch(string** data, int rowCount, string* searchNames, int searchCount);
void binarySearch(string** data, int rowCount, string* searchNames, int searchCount);
void loadData(string**& data, int& rowCount);
void loadSearchNames(string*& searchNames, int& searchCount);
void tableFormat(Table table);
string getCurrentTime();
void showReport(string** data, int rowCount);
string getGrade(string mark);
string getGPA(string mark);

#define MAX_ROWS 10000
#define MAX_COLUMNS 8
#define MAX_SEARCH_NAMES 100

int main() {
    int swapCount = 0;
    int choice, searchChoice;
    string** data = new string * [MAX_ROWS];
    int rowCount = 0;
    string* searchNames = nullptr;
    int searchCount = 0;

    // Load the data initially
    loadData(data, rowCount);

    while (true) {
        system("cls");
        cout << "Menu:\n";
        cout << "1. Sort Data Ascending\n";
        cout << "2. Search Data\n";
        cout << "3. View CGPA\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: { // Sort Menu
            system ("cls");
            bool goBackToSortMenu = true;
            while (goBackToSortMenu) {
                loadData(data, rowCount);
                int sortChoice;
                cout << "Sort Menu:\n";
                cout << "1. Bubble Sort\n";
                cout << "2. Merge Sort\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> sortChoice;
                if (sortChoice == 0) {
                    goBackToSortMenu = false; // Exit Sort Menu
                    break;
                }
                string startTime = getCurrentTime();
                auto start = chrono::high_resolution_clock::now(); // Start time

                switch (sortChoice) {
                case 1:
                    system("cls");
                    bubbleSort(data, rowCount, swapCount);
                    cout << swapCount << " swaps made.\n";
                    printData(data, rowCount);
                    break;
                case 2:
                    system("cls");
                    mergeSort(data, 0, rowCount - 1);
                    printData(data, rowCount);
                    break;
                default:
                    cout << "Invalid choice. Returning to Sort Menu.\n";
                    break;
                }

                auto end = chrono::high_resolution_clock::now(); // End time
                string endTime = getCurrentTime();
                auto duration = chrono::duration_cast<chrono::milliseconds>(end - start); // Calculate duration
                cout << "Start Time: " << startTime << endl;
                cout << "End Time: " << endTime << endl;
                cout << "Duration: " << duration.count() << " milliseconds.\n";

                cout << "Press any key to go back to the Sort Menu.\n";
                cin.ignore();
                _getch(); // Wait for user input
                system("cls");
            }
            break;
        }
        case 2: {
            bool searchMenuActive = true; // Flag to control the search submenu loop
            while (searchMenuActive) {
                loadData(data, rowCount);
                loadSearchNames(searchNames, searchCount);

                string sortStartTime = "", sortEndTime = "";
                chrono::high_resolution_clock::time_point sortStart, sortEnd;

                system("cls");
                cout << "Search Menu:\n";
                cout << "1. Linear Search\n";
                cout << "2. Binary Search\n";
                cout << "0. Go Back to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> searchChoice;

                switch (searchChoice) {
                case 1:
                    cout << "Performing Linear Search...\n";
                    linearSearch(data, rowCount, searchNames, searchCount);
                    break;
                case 2:
                    cout << "Sorting data for Binary Search...\n";

                    sortStartTime = getCurrentTime();
                    sortStart = chrono::high_resolution_clock::now();
                    mergeSort(data, 0, rowCount - 1);
                    sortEnd = chrono::high_resolution_clock::now();
                    sortEndTime = getCurrentTime();

                    auto sortDuration = chrono::duration_cast<chrono::milliseconds>(sortEnd - sortStart);
                    cout << "Sort Start Time: " << sortStartTime << endl;
                    cout << "Sort End Time: " << sortEndTime << endl;
                    cout << "Sorting Duration: " << sortDuration.count() << " milliseconds.\n";

                    cout << "Performing Binary Search...\n";
                    binarySearch(data, rowCount, searchNames, searchCount);
                    break;
                case 0:
                    searchMenuActive = false; // Exit the search submenu loop
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
                }

                if (searchMenuActive) {
                    cout << "Press any key to return to the Search Menu.\n";
                    char temp = _getch(); // Handle user input
                }
            }
            break;
        }
        case 3:
            system("cls");
            showReport(data, rowCount);
            break;
        case 0:
            cout << "Exiting program.\n";
            for (int i = 0; i < MAX_ROWS; ++i) {
                delete[] data[i];
            }
            delete[] data;
            delete[] searchNames;
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
            cin.ignore();
            cin.get();
            break;
        }
    }

    return 0;
}

string getCurrentTime() {
    auto now = chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = chrono::duration_cast<chrono::milliseconds>(duration).count();
    time_t now_c = chrono::system_clock::to_time_t(now);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now_c);

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);

    // Extract milliseconds from duration
    long ms = millis % 1000;

    // Append milliseconds to formatted time
    stringstream ss;
    ss << buffer << "." << setfill('0') << setw(3) << ms;
    return ss.str();
}



void loadData(string**& data, int& rowCount) {
    ifstream inputFile("../csv/ExamRecords.csv");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file." << endl;
        exit(1);
    }

    // Clean up existing data
    if (data != nullptr) {
        for (int i = 0; i < rowCount; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }

    // Allocate memory for new data
    data = new string * [MAX_ROWS];
    for (int i = 0; i < MAX_ROWS; ++i) {
        data[i] = new string[MAX_COLUMNS];
    }

    // Skip the column title line
    string line;
    if (!getline(inputFile, line)) {
        cerr << "Error: File is empty or has no data rows." << endl;
        inputFile.close();
        return;
    }

    // Read the CSV file
    rowCount = 0;
    while (getline(inputFile, line) && rowCount < MAX_ROWS) {
        stringstream ss(line);
        string cell;
        int colCount = 0;
        while (getline(ss, cell, ',') && colCount < MAX_COLUMNS) {
            data[rowCount][colCount++] = cell;
        }
        rowCount++;
    }

    inputFile.close();
}


// Function to load the names to search from an external file
void loadSearchNames(string*& searchNames, int& searchCount) {
    ifstream inputFile("../csv/RandomNames.csv");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open search names file." << endl;
        exit(1);
    }

    searchNames = new string[MAX_SEARCH_NAMES];
    searchCount = 0;
    string name;
    while (getline(inputFile, name) && searchCount < MAX_SEARCH_NAMES) {
        searchNames[searchCount++] = name;
    }

    inputFile.close();
}

// Sorting and printing functions

void bubbleSort(string** data, int rowCount, int& swapCount) {
    swapCount = 0;
    for (int i = 0; i < rowCount - 1; ++i) {
        for (int j = 0; j < rowCount - i - 1; ++j) {
            if (data[j][1] > data[j + 1][1]) {  // Compare by Full Name (column 1)
                for (int k = 0; k < MAX_COLUMNS; ++k) {
                    swap(data[j][k], data[j + 1][k]);
                }
                ++swapCount;
            }
        }
    }
}

void merge(string** data, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    string** leftArray = new string * [n1];
    string** rightArray = new string * [n2];

    for (int i = 0; i < n1; ++i) {
        leftArray[i] = new string[MAX_COLUMNS];
        for (int j = 0; j < MAX_COLUMNS; ++j) {
            leftArray[i][j] = data[left + i][j];
        }
    }

    for (int i = 0; i < n2; ++i) {
        rightArray[i] = new string[MAX_COLUMNS];
        for (int j = 0; j < MAX_COLUMNS; ++j) {
            rightArray[i][j] = data[mid + 1 + i][j];
        }
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArray[i][1] <= rightArray[j][1]) {
            for (int l = 0; l < MAX_COLUMNS; ++l) {
                data[k][l] = leftArray[i][l];
            }
            i++;
        }
        else {
            for (int l = 0; l < MAX_COLUMNS; ++l) {
                data[k][l] = rightArray[j][l];
            }
            j++;
        }
        k++;
    }

    while (i < n1) {
        for (int l = 0; l < MAX_COLUMNS; ++l) {
            data[k][l] = leftArray[i][l];
        }
        i++;
        k++;
    }

    while (j < n2) {
        for (int l = 0; l < MAX_COLUMNS; ++l) {
            data[k][l] = rightArray[j][l];
        }
        j++;
        k++;
    }

    for (int i = 0; i < n1; ++i) {
        delete[] leftArray[i];
    }
    delete[] leftArray;

    for (int i = 0; i < n2; ++i) {
        delete[] rightArray[i];
    }
    delete[] rightArray;
}

void mergeSort(string** data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(data, left, mid);
        mergeSort(data, mid + 1, right);
        merge(data, left, mid, right);
    }
}

void printData(string** data, int rowCount) {
    Table table;

    // Adding headers
    table.add_row({ "Paper ID", "Student Name","Age", "Class", "Phone Number", "Subject", "Score", "Exam Date"});

    // Adding rows (limiting to the first 100 rows for display)
    for (int i = 0; i < min(rowCount, 100); ++i) {
        table.add_row({ data[i][0], data[i][1], data[i][2], data[i][3], data[i][4], data[i][5], data[i][6], data[i][7] });
    }

    tableFormat(table);
    // Print the table
    std::cout << table << std::endl;
}

void tableFormat(Table table) {
    // Start of table format
    // Iterate over rows in the table
    size_t index = 0;

    table[0].format()
        .font_color(Color::grey)
        .font_background_color(Color::yellow)
        .padding_top(0.5)
        .padding_bottom(0.5);

    for (auto& row : table) {
        row.format()
            .font_style({ FontStyle::bold })
            .font_align(FontAlign::center)
            .border_top(" ")
            .border_bottom(" ")
            .border_left(" ")
            .border_right(" ")
            .corner(" ");

        // Set blue background color for alternate rows
        if (index > 0 && index % 2 == 0) {
            for (auto& cell : row) {
                cell.format()
                    .padding_top(0)  // Ensure no extra height
                    .padding_bottom(0)
                    .font_background_color(Color::blue);
            }
        }
        index += 1;
    }
    // End of table format
}


void linearSearch(string** data, int rowCount, string* searchNames, int searchCount) {
    string startTime = getCurrentTime();
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < searchCount; ++i) {
        bool found = false;
        for (int j = 0; j < rowCount; ++j) {
            if (data[j][1] == searchNames[i]) {
                cout << "\033[32m" << "Name: '" << searchNames[i] << "' ---------> NAME FOUND\n" << "\033[0m";
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "\033[31m" << "Name: '" << searchNames[i] << "' ---------> NAME NOT FOUND\n" << "\033[0m";
        }
    }
    auto end = chrono::high_resolution_clock::now();
    string endTime = getCurrentTime();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nStart Time: " << startTime << endl;
    cout << "End Time: " << endTime << endl;
    cout << "Duration: " << duration.count() << " milliseconds." << endl;
}

void binarySearch(string** data, int rowCount, string* searchNames, int searchCount) {
    string startTime = getCurrentTime();
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < searchCount; ++i) {
        int left = 0, right = rowCount - 1;
        bool found = false;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (data[mid][1] == searchNames[i]) {
                cout << "\033[32m" << "Name: '" << searchNames[i] << "' ---------> NAME FOUND\n" << "\033[0m";
                found = true;
                break;
            }
            else if (data[mid][1] < searchNames[i]) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }

        if (!found) {
            cout << "\033[31m" << "Name: '" << searchNames[i] << "' ---------> NAME NOT FOUND\n" << "\033[0m";
        }
    }
    auto end = chrono::high_resolution_clock::now();
    string endTime = getCurrentTime();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nStart Time: " << startTime << endl;
    cout << "End Time: " << endTime << endl;
    cout << "Duration: " << duration.count() << " milliseconds." << endl;
}

void showReport(string** data, int rowCount)
{
    Table table;

    // Adding headers
    table.add_row({ "Paper ID", "Student Name","Age", "Class", "Phone Number", "Subject", "Score", "Exam Date", "Grade", "GPA"});

    // Adding rows (limiting to the first 100 rows for display)
    for (int i = 0; i < min(rowCount, 100); ++i) {
        table.add_row({ data[i][0], data[i][1], data[i][2], data[i][3], data[i][4], data[i][5], data[i][6], data[i][7], getGrade(data[i][6]), getGPA(getGrade(data[i][6]))});
    }

    tableFormat(table);
    // Print the table

    std::cout << table << std::endl;
    cout << "Press any key to go back to the Menu.\n";
    cin.ignore();
    _getch(); // Wait for user input
    system("cls");
}

string getGrade(string mark)
{
    int i_mark = stoi(mark);

    if (i_mark >= 80) {
        if (i_mark >= 75) {
            return "A-";   // A- for 75 to 79
        }
        else {
            return "A";    // A for 80 to 100
        }
    }
    else if (i_mark >= 70) {
        return "B+";       // B+ for 70 to 74
    }
    else if (i_mark >= 65) {
        return "B";        // B for 65 to 69
    }
    else if (i_mark >= 60) {
        return "B-";       // B- for 60 to 64
    }
    else if (i_mark >= 55) {
        return "C+";       // C+ for 55 to 59
    }
    else if (i_mark >= 50) {
        return "C";        // C for 50 to 54
    }
    else if (i_mark >= 47) {
        return "C-";       // C- for 47 to 49
    }
    else if (i_mark >= 44) {
        return "D+";       // D+ for 44 to 46
    }
    else if (i_mark >= 40) {
        return "D";        // D for 40 to 43
    }
    else {
        return "E";        // E for 00 to 39
    }
}

string getGPA(string grade)
{
    if (grade == "A") {
        return "4.0";
    }
    else if (grade == "A-") {
        return "3.7";
    }
    else if (grade == "B+") {
        return "3.3";
    }
    else if (grade == "B") {
        return "3.0";
    }
    else if (grade == "B-") {
        return "2.7";
    }
    else if (grade == "C+") {
        return "2.3";
    }
    else if (grade == "C") {
        return "2.0";
    }
    else if (grade == "C-") {
        return "1.7";
    }
    else if (grade == "D+") {
        return "1.3";
    }
    else if (grade == "D") {
        return "1.0";
    }
    else if (grade == "E") {
        return "0.0";
    }
    else {
        return "Invalid grade";  // Return an error message if grade is not recognized
    }
}
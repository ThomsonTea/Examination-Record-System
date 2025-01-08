#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <conio.h>
#include <unordered_set>
#include <tabulate/table.hpp>
#include <unordered_map>
#include <exception>


using namespace tabulate;
using namespace std;

void bubbleSort(string** data, int rowCount, int& swapCount);
void printData(string** data, int rowCount);
void linearSearch(string** data, int rowCount, string* searchNames, int searchCount);
void binarySearch(string** data, int rowCount, string* searchNames, int searchCount);
void loadData(string**& data, int& rowCount);
void loadSearchNames(string*& searchNames, int& searchCount);
void tableFormat(Table table);
string getCurrentTime();
void generateReport(string** data, int rowCount);
string getGrade(string mark);
string getGPA(string mark);
string getScore(string marks);
void outputRetakeCandidates(string** data, int rowCount);
void analyzeSubject(string** data, int rowCount);
void quickSort(string** data, int low, int high, int columnIndex, int& swapCount);
int partition(string** data, int low, int high, int columnIndex, int& swapCount);
void swapRows(string** data, int row1, int row2, int& swapCount);
void improvedlinearSearch(string** data, int rowCount, string* searchNames, int searchCount);

#define MAX_ROWS 10000
#define MAX_COLUMNS 8
#define MAX_SEARCH_NAMES 100

int main() {
    auto programStart = chrono::high_resolution_clock::now(); // Program start time
    string programStartTime = getCurrentTime();

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
        cout << "1. Sort Student Name Ascending\n";
        cout << "2. Search Data\n";
        cout << "3. Generate Full Report\n";
        cout << "4. Generate Retake Report\n";
        cout << "5. Subject Summary\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: { // Sort Menu
            system("cls");
            bool goBackToSortMenu = true;
            while (goBackToSortMenu) {
                loadData(data, rowCount);
                int sortChoice;
                cout << "Sort Menu:\n";
                cout << "1. Bubble Sort\n";
                cout << "2. Quick Sort\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> sortChoice;
                if (sortChoice == 0) {
                    goBackToSortMenu = false; // Exit Sort Menu
                    break;
                }

                // Reset swapCount before sorting
                swapCount = 0;
                string startTime = getCurrentTime();
                auto start = chrono::high_resolution_clock::now(); // Start time

                switch (sortChoice) {
                case 1:
                    system("cls");
                    bubbleSort(data, rowCount, swapCount);
                    printData(data, rowCount);
                    cout << swapCount << " swaps made.\n";
                    break;
                case 2:
                    system("cls");
                    quickSort(data, 0, rowCount - 1, 1, swapCount); // Pass columnIndex and swapCount
                    printData(data, rowCount);
                    cout << swapCount << " swaps made.\n";
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
                cout << "2. Improved Linear Search\n";
                cout << "3. Binary Search\n";
                cout << "0. Go Back to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> searchChoice;

                switch (searchChoice) {
                case 1:
                    cout << "Performing Linear Search...\n";
                    linearSearch(data, rowCount, searchNames, searchCount);
                    break;
                case 2:
                    cout << "Performing Linear Search...\n";
                    improvedlinearSearch(data, rowCount, searchNames, searchCount);
                    break;
                case 3:
                    cout << "Sorting data for Binary Search...\n";

                    sortStartTime = getCurrentTime();
                    sortStart = chrono::high_resolution_clock::now();
                    quickSort(data, 0, rowCount - 1, 1, swapCount);
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
            generateReport(data, rowCount);
            break;
        case 4:
            system("cls");
            outputRetakeCandidates(data, rowCount);
            break;
        case 5:
            system("cls");
            analyzeSubject(data, rowCount);
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


void bubbleSort(string** data, int rowCount, int& swapCount) {
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


void printData(string** data, int rowCount) {
    Table table;

    // Adding headers
    table.add_row({ "Paper ID", "Student Name","Age", "Class", "Phone Number", "Subject", "Correct Marks", "Exam Date" });

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
        index += 1;
    }
}


void binarySearch(string** data, int rowCount, string* searchNames, int searchCount) {
    string startTime = getCurrentTime();
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < searchCount; ++i) {
        int left = 0, right = rowCount - 1;
        bool found = false;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (data[mid][1] == searchNames[i]) 
            {
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

void improvedlinearSearch(string** data, int rowCount, string* searchNames, int searchCount) {
    string startTime = getCurrentTime();
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < searchCount; ++i) {
        bool found = false;
        for (int j = 0; j < rowCount; ++j) {
            if (data[j][1] == searchNames[i]) {
                cout << "\033[32mName: '" + searchNames[i] + "'\033[0m" << " ---------> " << "\033[32mNAME FOUND\033[0m\n";
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "\033[31mName: '" + searchNames[i] + "'\033[0m" << " ---------> " << "\033[31mNAME NOT FOUND\033[0m\n";
        }
    }

    auto end = chrono::high_resolution_clock::now();
    string endTime = getCurrentTime();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nStart Time: " << startTime << endl;
    cout << "End Time: " << endTime << endl;
    cout << "Duration: " << duration.count() << " milliseconds." << endl;
}

void linearSearch(string** data, int rowCount, string* searchNames, int searchCount) {
    string startTime = getCurrentTime();
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < searchCount; ++i) {
        bool found = false;
        for (int j = 0; j < rowCount; ++j) {
            if (data[j][1] == searchNames[i]) {
                cout << "\033[32mName: '" + searchNames[i] + "'\033[0m" << " ---------> " << "\033[32mNAME FOUND\033[0m\n";
                found = true;
            }
        }
        if (!found) {
            cout << "\033[31mName: '" + searchNames[i] + "'\033[0m" << " ---------> " << "\033[31mNAME NOT FOUND\033[0m\n";
        }
    }

    auto end = chrono::high_resolution_clock::now();
    string endTime = getCurrentTime();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nStart Time: " << startTime << endl;
    cout << "End Time: " << endTime << endl;
    cout << "Duration: " << duration.count() << " milliseconds." << endl;
}

void generateReport(string** data, int rowCount)
{
    Table table;

    // Open a CSV file for writing the report
    ofstream outFile("../reportList/Full_Report.csv", ios::out | ios::trunc);
    if (!outFile.is_open())
    {
        cerr << "Error: Could not create output file." << endl;
        return;
    }

    // Add table headers
    table.add_row({ "Paper ID", "Student Name", "Age", "Class", "Phone Number", "Subject", "Correct Marks", "Exam Date", "Score (%)", "Grade", "GPA" });

    // Write CSV headers
    outFile << "Paper ID,Student Name,Age,Class,Phone Number,Subject,Correct Marks,Exam Date,Score (%),Grade,GPA\n";

    int sampleCount = 0; // Counter for displayed sample rows

    // Process all rows
    for (int i = 0; i < rowCount; ++i)
    {
        string score = getScore(data[i][6]);
        string grade = getGrade(score);
        string gpa = getGPA(grade);

        // Write row to the CSV file
        outFile << data[i][0] << "," << data[i][1] << "," << data[i][2] << "," << data[i][3] << ","
            << data[i][4] << "," << data[i][5] << "," << data[i][6] << "," << data[i][7] << ","
            << score << "," << grade << "," << gpa << "\n";

        // Add row to the sample data table (limit to 100 rows for display)
        if (sampleCount < 10)
        {
            ++sampleCount; // Increment sample counter
            table.add_row({ data[i][0], data[i][1], data[i][2], data[i][3], data[i][4], data[i][5], data[i][6], data[i][7], score, grade, gpa });
        }
    }

    // Close the CSV file
    outFile.close();

    tableFormat(table);

    // Print the sample data table
    std::cout << "\nSample Data (First 10 Rows):" << std::endl;
    std::cout << table << std::endl;

    // Inform the user about the exported file
    cout << "\nThe full report has been successfully exported to 'Full_Report.csv'." << endl;

    cout << "Press any key to go back to the Menu.\n";
    cin.ignore();
    _getch(); // Wait for user input
    system("cls");
}


string getGrade(string score)
{
    int i_mark = stoi(score);

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

string getScore(string marks)
{
    double score = (stoi(marks) / 40.0) * 100;
    string s_score = to_string(score);

    stringstream ss;
    ss << fixed << setprecision(2) << score;
    return ss.str();
}

void outputRetakeCandidates(string** data, int rowCount) {
    Table table;

    // Open a CSV file for writing the retake candidates
    ofstream outFile("../retakeList/Retake_Candidates.csv", ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cerr << "Error: Could not create output file." << endl;
        return;
    }

    cout << "\nSample Data (First 100 Rows with Score < 40):" << endl;
    table.add_row({ "Paper ID", "Student Name", "Age", "Class", "Phone Number", "Subject", "Correct Marks", "Exam Date", "Score (%)" });

    // Write CSV headers
    outFile << "Paper ID,Student Name,Age,Class,Phone Number,Subject,Correct Marks,Exam Date,Score (%)\n";

    unordered_set<string> uniquePIDs;    // To store unique PIDs
    unordered_map<string, int> classDist; // Class-wise distribution
    unordered_map<string, int> subjectDist; // Subject-wise distribution
    unordered_map<string, int> ageDist; // Age-wise distribution
    int retakeCount = 0;                // Counter for students needing a retake
    int sampleCount = 0;                // Counter for rows in the sample data

    // Arrays for storing scores and lowest scores
    int scores[1000]; // Assuming a max of 1000 students
    int scoreCount = 0; // To track the number of scores
    int lowestScores[5] = { 100, 100, 100, 100, 100 }; // To store lowest 5 scores

    // Iterate through data to find rows where the score is below 40
    for (int i = 0; i < rowCount; ++i) {
        if (!data[i][6].empty()) { // Ensure the score field is not empty
            try {
                int score = stoi(getScore(data[i][6])); // Convert score to integer
                if (score < 40) {
                    ++retakeCount; // Increment the retake counter
                    scores[scoreCount++] = score; // Store score
                    if (scoreCount <= 5) {
                        // For the first 5 scores, store directly in lowestScores
                        lowestScores[scoreCount - 1] = score;
                    }
                    else {
                        // For subsequent scores, update the lowestScores array
                        for (int j = 0; j < 5; ++j) {
                            if (score < lowestScores[j]) {
                                for (int k = 4; k > j; --k) {
                                    lowestScores[k] = lowestScores[k - 1];
                                }
                                lowestScores[j] = score;
                                break;
                            }
                        }
                    }

                    // Always write the row to the CSV file
                    outFile << data[i][0] << "," << data[i][1] << "," << data[i][2] << "," << data[i][3] << ","
                        << data[i][4] << "," << data[i][5] << "," << data[i][6] << "," << data[i][7] << ","
                        << score << "\n";

                    // Add to the sample data table if less than 100 rows displayed
                    if (sampleCount < 100) {
                        ++sampleCount; // Increment the sample counter
                        table.add_row({ data[i][0], data[i][1], data[i][2], data[i][3], data[i][4],
                                        data[i][5], data[i][6], data[i][7], to_string(score) });
                        uniquePIDs.insert(data[i][0]); // Add PID to the set
                    }

                    // Update class, subject, and age distributions
                    ++classDist[data[i][3]];
                    ++subjectDist[data[i][5]];
                    ++ageDist[data[i][2]];
                }
            }
            catch (exception& e) {
                cerr << "Error processing score for row " << i + 1 << ": " << e.what() << endl;
            }
        }
    }

    // Close the CSV file
    outFile.close();
    tableFormat(table);

    // Display the sample data table
    cout << table << endl;

    // Calculate percentage of students needing a retake
    double retakePercentage = (rowCount > 0) ? (retakeCount * 100.0 / rowCount) : 0.0;

    // Find class and subject with the most retake candidates
    string topClass, topSubject;
    int maxClassCount = 0, maxSubjectCount = 0;
    for (const auto& entry : classDist) {
        if (entry.second > maxClassCount) {
            maxClassCount = entry.second;
            topClass = entry.first;
        }
    }
    for (const auto& entry : subjectDist) {
        if (entry.second > maxSubjectCount) {
            maxSubjectCount = entry.second;
            topSubject = entry.first;
        }
    }

    // Display the total number of students needing a retake
    cout << "\nThe retake candidates list has been successfully exported to 'Retake_Candidates.csv'." << endl;
    cout << "\nTotal number of students needing a retake (score < 40%): " << retakeCount << endl;
    cout << "Percentage of students needing a retake: " << fixed << setprecision(2) << retakePercentage << "%" << endl;

    // Display the count of unique PIDs
    cout << "Total unique Paper IDs displayed: " << uniquePIDs.size() << endl;

    // Display class-wise distribution
    cout << "\nClass-Wise Distribution:" << endl;
    for (const auto& entry : classDist) {
        cout << entry.first << ": " << entry.second << " students" << endl;
    }

    // Display subject-wise distribution
    cout << "\nSubject-Wise Distribution:" << endl;
    for (const auto& entry : subjectDist) {
        cout << "Subject " << entry.first << ": " << entry.second << " students" << endl;
    }

    // Display age-wise distribution
    cout << "\nAge-Wise Distribution:" << endl;
    for (const auto& entry : ageDist) {
        cout << "Age " << entry.first << ": " << entry.second << " students" << endl;
    }

    // Display retake recommendation summary
    cout << "\nRetake Summary:" << endl;
    cout << "Class with most retake candidates: " << topClass << endl;
    cout << "Subject with most retake candidates: " << topSubject << endl;

    cout << "\n\nPress any key to continue" << endl;
    _getch();
}

void analyzeSubject(string** data, int rowCount)
{
    while (true)
    {
        // Display subject choices
        cout << "Choose a subject:\n";
        cout << "1. Biology\n";
        cout << "2. Chemistry\n";
        cout << "3. English\n";
        cout << "4. Geography\n";
        cout << "5. History\n";
        cout << "6. Math\n";
        cout << "7. Physics\n";
        cout << "8. Science\n";
        cout << "0. Back to Menu\n";
        cout << "\nEnter the number corresponding to your subject choice: ";

        int choice;
        cin >> choice;

        // Handle the "back to menu" option
        if (choice == 0)
        {
            system("cls");
            return;
        }

        // Determine the subject using switch-case
        string subject;
        switch (choice)
        {
        case 1: subject = "Biology"; break;
        case 2: subject = "Chemistry"; break;
        case 3: subject = "English"; break;
        case 4: subject = "Geography"; break;
        case 5: subject = "History"; break;
        case 6: subject = "Math"; break;
        case 7: subject = "Physics"; break;
        case 8: subject = "Science"; break;
        default:
            cout << "Invalid choice. Please try again.\n";
            cout << "Press any key to continue.\n";
            _getch();
            system("cls");
            continue;
        }

        cout << "\nYou selected: " << subject << endl;

        // Create a temporary array to hold filtered rows
        const int MAXIMUM_ROWS = 2000;
        string* filteredData[MAXIMUM_ROWS];
        int filteredCount = 0;

        // Filter data by the selected subject
        int totalMarks = 0; // For average calculation
        for (int i = 0; i < rowCount; ++i)
        {
            if (data[i][5] == subject) // Column 5 is the subject
            {
                filteredData[filteredCount++] = data[i];
                totalMarks += stoi(data[i][6]); // Accumulate marks
            }
        }

        if (filteredCount == 0)
        {
            cout << "No records found for the subject: " << subject << endl;
            cout << "Press any key to continue.\n";
            _getch();
            system("cls");
            continue;
        }

        double averageMark = static_cast<double>(totalMarks) / filteredCount;

        // Sort filtered data by marks in descending order
        for (int i = 0; i < filteredCount - 1; ++i)
        {
            for (int j = i + 1; j < filteredCount; ++j)
            {
                if (stoi(filteredData[i][6]) < stoi(filteredData[j][6])) // Compare marks
                {
                    swap(filteredData[i], filteredData[j]);
                }
            }
        }

        // Find highest and lowest marks
        int highestMark = stoi(filteredData[0][6]);
        int lowestMark = stoi(filteredData[filteredCount - 1][6]);

        // Arrays to collect students with highest and lowest marks
        const int MAX_STUDENTS = 2000;
        string* highestScorers[MAX_STUDENTS];
        string* lowestScorers[MAX_STUDENTS];
        int highestCount = 0, lowestCount = 0;

        for (int i = 0; i < filteredCount; ++i)
        {
            int mark = stoi(filteredData[i][6]);
            if (mark == highestMark)
                highestScorers[highestCount++] = filteredData[i];
            if (mark == lowestMark)
                lowestScorers[lowestCount++] = filteredData[i];
        }

        // Display options
        cout << "Choose an option:\n";
        cout << "1. Display students with the highest marks\n";
        cout << "2. Display students with the lowest marks\n";
        cout << "3. Display both highest and lowest marks\n";
        cout << "0. Back to Menu\n";
        cout << "\nEnter your choice: ";

        int option;
        cin >> option;
        system("cls");

        if (option == 0)
        {
            system("cls");
            return;
        }

        Table table;

        if (option == 1 || option == 3)
        {
            cout << "Students with the Highest Mark (" << highestMark << ") [Count: " << highestCount << "]:\n";
            table.add_row({ "Rank", "Paper ID", "Student Name", "Subject", "Marks", "Exam Date" });
            for (int i = 0; i < highestCount; ++i)
            {
                table.add_row({
                    to_string(i + 1),
                    highestScorers[i][0],
                    highestScorers[i][1],
                    highestScorers[i][5],
                    highestScorers[i][6],
                    highestScorers[i][7]
                    });
            }
        }

        if (option == 2 || option == 3)
        {
            cout << "Students with the Lowest Mark (" << lowestMark << ") [Count: " << lowestCount << "]:\n";
            table.add_row({ "Rank", "Paper ID", "Student Name", "Subject", "Marks", "Exam Date" });
            for (int i = 0; i < lowestCount; ++i)
            {
                table.add_row({
                    to_string(i + 1),
                    lowestScorers[i][0],
                    lowestScorers[i][1],
                    lowestScorers[i][5],
                    lowestScorers[i][6],
                    lowestScorers[i][7]
                    });
            }
        }


        cout << table << endl;


        // Calculate average score as a percentage
        double averageScore = (averageMark * 100.0) / 40.0;  // Assuming 40 is the max score

        // Use std::round to properly round the average score to 2 decimal places
        averageScore = std::round(averageScore * 100.0) / 100.0;

        cout << "Sum of the total Marks: " << totalMarks << "\n";
        cout << "Total Students: " << filteredCount << "\n";
        cout << "\nAverage Mark of whole students: " << fixed << setprecision(2) << averageMark << "\n";
        cout << "\nAverage Score of whole students: " << fixed << setprecision(2) << averageScore << "%\n";

        cout << "Press any key to continue or 0 to go back to the menu.\n";
        char userInput = _getch();
        if (userInput == '0')
        {
            system("cls");
            return;
        }
        system("cls");
    }
}

void quickSort(string** data, int low, int high, int columnIndex, int& swapCount) {
    if (low < high) {
        // Partition the array and get the pivot index
        int pivotIndex = partition(data, low, high, columnIndex, swapCount);
        // Recursively sort the left and right subarrays
        quickSort(data, low, pivotIndex - 1, columnIndex, swapCount);
        quickSort(data, pivotIndex + 1, high, columnIndex, swapCount);
    }
}

int partition(string** data, int low, int high, int columnIndex, int& swapCount) {
    string pivot = data[high][columnIndex]; // Choose the last element in the range as the pivot
    int i = low - 1; // Initialize the index for smaller elements

    for (int j = low; j < high; j++) {
        // Compare current element with the pivot
        if (data[j][columnIndex] <= pivot) {
            i++;
            // Swap rows at indices i and j
            swapRows(data, i, j, swapCount);
        }
    }

    // Swap the pivot element into its correct position
    swapRows(data, i + 1, high, swapCount);
    return i + 1; // Return the pivot index
}

void swapRows(string** data, int row1, int row2, int& swapCount) {
    if (row1 == row2) return; // Skip swapping if both rows are the same

    for (int col = 0; col < MAX_COLUMNS; col++) {
        // Swap elements column by column
        swap(data[row1][col], data[row2][col]);
    }
    swapCount++; // Increment the swap count after each row swap
}







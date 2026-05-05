/*
 * ============================================================
 *
 *  Apexcify Technologys Internship - Task 1
 *  Made by : Hadi Ahmed Alvi
 *
 *  ============================================================
*/

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>

using namespace std;

// Structure for a course
struct Course {
    string name;
    string grade;
    float gradePoint;
    int creditHours;
};

// Convert a alphabet grade to grade points
float getGradePoints(string grade) {
    transform(grade.begin(), grade.end(), grade.begin(), ::toupper);

    if (grade == "A+" || grade == "A") return 4.0;
    if (grade == "A-") return 3.7;
    if (grade == "B+") return 3.3;
    if (grade == "B") return 3.0;
    if (grade == "B-") return 2.7;
    if (grade == "C+") return 2.3;
    if (grade == "C") return 2.0;
    if (grade == "C-") return 1.7;
    if (grade == "D+") return 1.3;
    if (grade == "D") return 1.0;
    if (grade == "F") return 0.0;

    return -1; // For Invalid grade
}

// Print grade reference table
void printGradeTable() {
    cout << "\n  Grade Reference:\n";
    cout << "  -------------------------------------\n";
    cout << "    A+/A=4.0  A-=3.7  B+=3.3  B=3.0   \n";
    cout << "    B-=2.7    C+=2.3  C=2.0   C-=1.7  \n";
    cout << "    D+=1.3    D=1.0   F=0.0            \n";
    cout << "  -------------------------------------\n\n";
}

// Input a single course
Course inputCourse(int courseNum) {
    Course c;

    cout << "\n  Course " << courseNum << ":\n";

    // Course name
    cout << "    Name: ";
    cin.ignore();
    getline(cin, c.name);
    if (c.name.empty()) c.name = "Course " + to_string(courseNum);

    // Grade input with Boundary cases
    while (true) {
        cout << "    Grade (A+, A, A-, B+, B, B-, C+, C, C-, D+, D, F): ";
        cin >> c.grade;

        c.gradePoint = getGradePoints(c.grade);

        if (c.gradePoint != -1)
            break;

        cout << "    Invalid grade! Use A+, A, A-, B+, B, B-, C+, C, C-, D+, D, or F\n";
    }

    // Credit hours input with boundary cases
    while (true) {
        cout << "    Credit hours (1-4): ";
        cin >> c.creditHours;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "    Please enter a number!\n";
            continue;
        }

        if (c.creditHours >= 1 && c.creditHours <= 4)
            break;

        cout << "    Credit hours must be between 1 and 4\n";
    }

    return c;
}

// Display all courses with their details
void displayCourses(const vector<Course>& courses) {
    cout << "\n  ==============================================================\n";
    cout << "     COURSE DETAILS\n";
    cout << "  ==============================================================\n";

    cout << left
        << "     " << setw(25) << "Course Name"
        << setw(10) << "Grade"
        << setw(10) << "Credits"
        << "Points\n";

    cout << "     ------------------------------------------------------------\n";

    for (const auto& c : courses) {
        cout << "     " << setw(25) << c.name
            << setw(10) << c.grade
            << setw(10) << c.creditHours
            << fixed << setprecision(1) << c.gradePoint << "\n";
    }
}

// Get academic standing based on CGPA
string getAcademicStanding(double cgpa) {
    if (cgpa >= 3.7)
        return "Excellent";
    else if (cgpa >= 3.3)
        return "Very Good";
    else if (cgpa >= 3.0)
        return "Good";
    else if (cgpa >= 2.0)
        return "Satisfactory";
    else
        return "Needs Improvement";
}

int main() {
    cout << "\n  ==============================================\n";
    cout << "               CGPA CALCULATOR \n";
    cout << "  ==============================================\n";

    // Previous academic record
    float prevCGPA = 0;
    int prevCredits = 0;
    bool hasPrevious = false;

    char hasPrev;
    cout << "\n  Do you have previous CGPA? (y/n): ";
    cin >> hasPrev;

    if (hasPrev == 'y' || hasPrev == 'Y') {
        cout << "  Enter your previous CGPA: ";
        cin >> prevCGPA;

        cout << "  Enter total credit hours completed: ";
        cin >> prevCredits;
        hasPrevious = true;
    }

    // Current semester input
    int numCourses;
    cout << "\n  Enter number of courses this semester (max 6): ";
    cin >> numCourses;

    while (numCourses < 1 || numCourses > 6) {
        cout << "  Please enter between 1 and 6 courses: ";
        cin >> numCourses;
    }

    // Show grade reference
    printGradeTable();

    // Input courses
    vector<Course> courses;
    float totalPoints = 0;
    int totalCredits = 0;

    for (int i = 0; i < numCourses; i++) {
        Course c = inputCourse(i + 1);

        totalPoints += c.gradePoint * c.creditHours;
        totalCredits += c.creditHours;

        courses.push_back(c);
    }

    // Calculate the semester GPA
    float semesterGPA = totalPoints / totalCredits;

    // Calculate the overall CGPA
    float overallCGPA;
    if (hasPrevious) {
        overallCGPA = ((prevCGPA * prevCredits) + totalPoints) / (prevCredits + totalCredits);
    }
    else {
        overallCGPA = semesterGPA;
    }

    // Displaying the results
    displayCourses(courses);

    cout << "\n  ==============================================================\n";
    cout << "     RESULTS SUMMARY\n";
    cout << "  ==============================================================\n";

    cout << fixed << setprecision(2);

    if (hasPrevious) {
        cout << "     Previous CGPA        : " << prevCGPA << endl;
        cout << "     Previous Credits     : " << prevCredits << endl;
        cout << "     ------------------------------------------------------------\n";
    }

    cout << "     Current Credits      : " << totalCredits << endl;
    cout << "     Total Grade Points   : " << totalPoints << endl;
    cout << "     Semester GPA         : " << semesterGPA << endl;

    cout << "     ------------------------------------------------------------\n";
    cout << "     OVERALL CGPA         : " << overallCGPA << " / 4.00\n";

    // Academic standing
    string standing = getAcademicStanding(overallCGPA);
    cout << "     Academic Standing    : " << standing << "\n";

    cout << "  ==============================================================\n";
    cout << "\n  Thank you for using the CGPA Calculator!\n\n\n";
    system("pause");

    return 0;
}

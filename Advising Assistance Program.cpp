//============================================================================

//Robert Heavner
//2/19/2024
//SNHU Project Two

//============================================================================



#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//============================================================================
// Data structure definitions
//============================================================================

// Define a structure to hold course information
struct Course {
    string courseNumber;
    string name;
    vector<Course*> prerequisites;
};

// Define a node in the tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// Define the Tree class to hold courses
class Tree {
private:
    Node* root;

    void inOrderPrint(Node* node);
    Node* insert(Node* node, Course course);
    Node* find(Node* node, string courseNumber);
    void clear(Node* node);

public:
    Tree();
    ~Tree();
    void insertCourse(Course course);
    void printAllCourses();
    Course* findCourse(string courseNumber);
};

Tree::Tree() : root(nullptr) {}

Tree::~Tree() {
    clear(root);
}

void Tree::clear(Node* node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

void Tree::insertCourse(Course course) {
    root = insert(root, course);
}

Node* Tree::insert(Node* node, Course course) {
    if (node == nullptr) {
        return new Node(course);
    }
    if (course.courseNumber < node->course.courseNumber) {
        node->left = insert(node->left, course);
    }
    else if (course.courseNumber > node->course.courseNumber) {
        node->right = insert(node->right, course);
    }
    return node;
}

void Tree::printAllCourses() {
    inOrderPrint(root);
}

void Tree::inOrderPrint(Node* node) {
    if (node != nullptr) {
        inOrderPrint(node->left);
        cout << node->course.courseNumber << ", " << node->course.name;
        if (!node->course.prerequisites.empty()) {
            cout << ", Prerequisites: ";
            for (const auto& prereq : node->course.prerequisites) {
                cout << prereq->courseNumber << " ";
            }
        }
        cout << endl;
        inOrderPrint(node->right);
    }
}


Course* Tree::findCourse(string courseNumber) {
    Node* node = find(root, courseNumber);
    if (node == nullptr) {
        return nullptr;
    }
    return &(node->course);
}

Node* Tree::find(Node* node, string courseNumber) {
    if (node == nullptr || node->course.courseNumber == courseNumber) {
        return node;
    }
    if (courseNumber < node->course.courseNumber) {
        return find(node->left, courseNumber);
    }
    return find(node->right, courseNumber);
}

//============================================================================
// Functions
//============================================================================

void loadCoursesFromFile(string filename, Tree& tree) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber, name, requirement;
        getline(ss, courseNumber, ',');
        getline(ss, name, ',');

        Course course;
        course.courseNumber = courseNumber;
        course.name = name;

        tree.insertCourse(course);

        while (getline(ss, requirement, ',')) {
            Course* prereqCourse = tree.findCourse(requirement);
            if (prereqCourse != nullptr) {
                tree.findCourse(courseNumber)->prerequisites.push_back(prereqCourse);
            }
        }
    }
    file.close();
}

void printCourseInformation(Tree& tree, string courseNumber) {
    Course* course = tree.findCourse(courseNumber);
    if (course != nullptr) {
        cout << course->courseNumber << ": " << course->name << endl;
        if (!course->prerequisites.empty()) {
            cout << "Requirements: ";
            for (Course* prereq : course->prerequisites) {
                cout << prereq->courseNumber << " ";
            }
            cout << endl;
        }
        else {
            cout << "No requirements" << endl;
        }
    }
    else {
        cout << "Course not found" << endl;
    }
}



//============================================================================
// Main program
//============================================================================

// ... (Rest of the code remains unchanged)

int main(int argc, char* argv[]) {
    Tree coursesTree;
    string csvPath;

    // Check for command line arguments and process accordingly
    if (argc > 1) {
        csvPath = argv[1];
    }
    else {
        // Default CSV file path
        csvPath = "ABCU_Advising_Program_Input.csv";
    }

    int option = 0;
    cout << "Welcome to the course planner." << endl;

    while (true) {
        cout << "1. Load Data Structure.(You must load in data FIRST)" << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? (Choose number | E=Enroll N=New Class choice) ";
        cin >> option;

        // Check for invalid input
        if (cin.fail()) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip to the next newline
            cout << "Invalid input. Please enter a number." << endl;
            continue; // Continue to the next iteration of the loop
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the newline left in the input stream

        if (option == 9) {
            cout << "Thank you for using the course planner!" << endl;
            break; // Break out of the loop to exit
        }

        switch (option) {
        case 1: {
            cout << "Loading courses from file: " << csvPath << endl;
            clock_t ticks = clock();
            loadCoursesFromFile(csvPath, coursesTree);
            ticks = clock() - ticks;
            cout << "Data loaded successfully in " << static_cast<float>(ticks) / CLOCKS_PER_SEC << " seconds." << endl;
            break;
        }
        case 2:
            cout << "Course List:" << endl;
            coursesTree.printAllCourses();
            break;
        case 3: {
            cout << "Enter course number: ";
            string courseNumber;
            getline(cin, courseNumber);
            printCourseInformation(coursesTree, courseNumber);
            break;
        }
        default:
            cout << "Invalid option. Please try again." << endl;
            break;
        }
    }
    return 0;
}



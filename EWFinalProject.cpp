/**
 * @file EWFinalProject.cpp
 * @brief This file contains the implementation of a course management system using a hash table.
 * 
 * The program allows users to load course data from a file, validate the data, search for specific courses,
 * and print all courses in the Computer Science department. The course data includes course numbers, titles,
 * and prerequisites.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

/**
 * @struct Course
 * @brief Represents a course with a number, title, and prerequisites.
 */
struct Course {
    std::string number; ///< The course number
    std::string title; ///< The course title
    std::vector<std::string> prerequisites; ///< List of prerequisite course numbers
};

typedef std::unordered_map<std::string, Course> HashTable;

/**
 * @brief Loads course data from a file into a hash table.
 * 
 * @param filePath The path to the file containing course data.
 * @return HashTable A hash table containing the loaded courses.
 */
HashTable loadCoursesToHashTable(const std::string &filePath) {
    std::ifstream file(filePath);
    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return HashTable();
    }

    // Create a hash table to store the courses
    HashTable coursesHashTable;
    std::string lineData;

    // Read each line from the file
    while (std::getline(file, lineData)) {
        std::vector<std::string> tokens;
        std::stringstream ss(lineData);
        std::string token;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 2) {
            std::cerr << "Error: Line has less than 2 parameters." << std::endl;
            continue;
        }

        // Extract course data from tokens
        std::string courseNumber = tokens[0];
        std::string courseTitle = tokens[1];
        std::vector<std::string> prerequisites(tokens.begin() + 2, tokens.end());

        // Create a course object
        Course course;
        course.number = courseNumber;
        course.title = courseTitle;
        course.prerequisites = prerequisites;

        coursesHashTable[courseNumber] = course;
    }
    // Close the file
    file.close();
    
    //return the hash table
    return coursesHashTable;
}

/**
 * @brief Validates the hash table to ensure all prerequisites exist as courses.
 * 
 * @param coursesHashTable The hash table containing the courses.
 * @return true If all prerequisites exist as courses.
 * @return false If any prerequisite does not exist as a course.
 */
bool validateHashTable(const HashTable &coursesHashTable) {
    std::unordered_set<std::string> courseNumbers;
    // Collect all course numbers
    for (const auto &pair : coursesHashTable) {
        courseNumbers.insert(pair.first);
    }

    // Check each course's prerequisites
    for (const auto &pair : coursesHashTable) {
        const auto &course = pair.second;
        for (const auto &prerequisite : course.prerequisites) {
            if (courseNumbers.find(prerequisite) == courseNumbers.end()) {
                std::cerr << "Error: Prerequisite " << prerequisite << " does not exist as a course." << std::endl;
                return false;
            }
        }
    }

    return true;
}

/**
 * @brief Searches for a course in the hash table and prints its details.
 * 
 * @param coursesHashTable The hash table containing the courses.
 * @param courseNumber The course number to search for.
 */
void searchCourseInHashTable(const HashTable &coursesHashTable, const std::string &courseNumber) {
    auto it = coursesHashTable.find(courseNumber);
    // Check if the course exists in the hash table
    if (it != coursesHashTable.end()) {
        const auto &course = it->second;
        std::cout << "Course Number: " << course.number << std::endl;
        std::cout << "Course Title: " << course.title << std::endl;
        std::cout << "Prerequisites: ";

        if (course.prerequisites.empty()) {
            std::cout << "None" << std::endl;
        } else {
            for (const auto &prerequisite : course.prerequisites) {
                std::cout << prerequisite << " ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cerr << "Error: Course " << courseNumber << " not found." << std::endl;
    }
}

/**
 * @brief Prints all courses in the hash table in alphanumeric order.
 * 
 * @param coursesHashTable The hash table containing the courses.
 */
void PrintAllCourses(HashTable coursesHashTable) {
    std::vector<std::string> courseNumbers;

    // Collect all course numbers
    for (const auto& entry : coursesHashTable) {
        courseNumbers.push_back(entry.first);
    }

    // Sort the course numbers alphanumerically
    std::sort(courseNumbers.begin(), courseNumbers.end());

    // Print the sorted course numbers and titles
    std::cout << "Courses in the Computer Science department:\n";
    for (const auto& courseNumber : courseNumbers) {
        const auto& course = coursesHashTable[courseNumber];
        std::cout << course.number << ": " << course.title << std::endl;
    }
}

/**
 * @brief The main function that runs the course management system.
 * 
 * @return int Exit status of the program.
 */
int main() {
    //initialize variables
    std::string filePath;
    std::string courseNumber;
    HashTable coursesHashTable = loadCoursesToHashTable(filePath);

    // Main menu loop
    while (true) {
        // Display the menu options
        int userChoice = NULL;
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Load file" << std::endl;
        std::cout << "2. Print List" << std::endl;
        std::cout << "3. Search for Course" << std::endl;
        std::cout << "9. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> userChoice;

        // Check if the input is valid
        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        }

        // Process the user's choice
        // Load file
        if(userChoice == 1) {
            std::cout << "Enter filepath to load: ";
            std::cin.ignore();
            std::getline(std::cin, filePath);
            std::cout << filePath;
            coursesHashTable = loadCoursesToHashTable(filePath);
        }
        // Print list
        else if(userChoice == 2) {
            // Print list
            PrintAllCourses(coursesHashTable); 
        }
        // Search for course
        else if(userChoice == 3) {
            // Search for course
            std::cout << "Enter course number to search: ";
            std::cin >> courseNumber;
            searchCourseInHashTable(coursesHashTable, courseNumber);

        }
        // Exit
        else if(userChoice == 9) {
            std::cout << "Goodbye!" << std::endl;
            return 0;
        }
        // Invalid choice
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;

        }

    }
    return 0;
}

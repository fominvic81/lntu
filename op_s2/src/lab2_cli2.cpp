#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <iomanip>
#include <Windows.h>
#include <sstream>

using namespace std;

struct Student {
	string name;
	vector<int> points;
};

vector<Student> students;

enum Page {
	Main,
	Add,
	Show,
	Select,
	AddPoints,
	Save,
};

Page page = Main;
int selectedItem = 0;
int selectedStudent = 0;


void gotoPage(Page value) {
	page = value;
	selectedItem = 0;
}

void displayMenuItem(string text, int index) {
	cout << (index == selectedItem ? "> " : "  ") << text << (index == selectedItem ? " <" : "  ") << endl;
}

string serialize() {
	stringstream ss;

	for (auto& student : students) {
		int pointsSum = 0;
		int pointsCount = 0;

		ss << "Name: " << student.name << endl;
		ss << "Points: ";
		for (int value : student.points) {
			pointsSum += value;
			++pointsCount;
			ss << value << " ";
		}
		ss << endl;
		ss << "Average: " << setprecision(1) << fixed << (double)pointsSum / max(1, pointsCount) << endl;
		if (&student != &*(students.end() - 1)) ss << string(25, '=') << endl;
	}
	return ss.str();
}

void mainMenu() {
	displayMenuItem("Add student", 0);
	displayMenuItem("Display students", 1);
	displayMenuItem("Add points", 2);
	displayMenuItem("Save", 3);

	char action = _getch();

	if (action == 72) { // Down
		selectedItem = (selectedItem - 1 + 4) % 4;
	} else if (action == 80) { // Up
		selectedItem = (selectedItem + 1) % 4;
	} else if (action == 13) { // Enter
		if (selectedItem == 0) gotoPage(Add);
		if (selectedItem == 1) gotoPage(Show);
		if (selectedItem == 2) gotoPage(Select);
		if (selectedItem == 3) gotoPage(Save);
	} else if (action == 27) { // Escape
		exit(0);
	}
}

void addStudent() {
	cout << "Add student\nEnter name: ";
	Student student;
	string str;
	do {
		cin >> str;
		student.name += str + " ";
	} while (cin.peek() != '\n');

	if (student.name != "") {
		student.name.pop_back();
		students.push_back(student);
	}
	gotoPage(Main);
}

void showStudents() {
	if (students.size() == 0) {
		cout << "There are no students" << endl;
	} else {
		cout << serialize();
	}


	char action = _getch();
	gotoPage(Main);
}

void selectStudent() {
	if (students.size() == 0) {
		cout << "There are no students" << endl;
		char action = _getch();
		gotoPage(Main);
		return;
	}
	cout << "Select student" << endl;
	for (int i = 0; i < students.size(); ++i) displayMenuItem(students[i].name, i);

	char action = _getch();

	if (action == 72) { // Down
		selectedItem = (selectedItem - 1 + students.size()) % students.size();
	} else if (action == 80) { // Up
		selectedItem = (selectedItem + 1) % students.size();
	} else if (action == 13) { // Enter
		selectedStudent = selectedItem;
		gotoPage(AddPoints);
	} else if (action == 27) { // Escape
		gotoPage(Main);
	}
}

void AddPoint() {
	cout << "Enter values: ";

	int value;
	do {
		if (!(cin >> value)) {
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}
		students[selectedStudent].points.push_back(value);
	} while (cin.peek() != '\n');
	gotoPage(Main);
}

void SaveStudents() {
	if (students.size() == 0) {
		cout << "There are no students" << endl;
	} else {
		ofstream file("save.txt", fstream::app | fstream::out);


		file << serialize();
		file << "\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n";

		cout << "Saved";
	}
	char action = _getch();
	gotoPage(Main);
}

int main() {

	int w = 120, h = 40;
	char* filler = new char[(w + 2) * h + 1];
	for (int j = 0; j < h; ++j) {
		for (int i = 0; i < w; ++i) {
			filler[j * (w + 2) + i] = ' ';
		}
		filler[(j + 1) * (w + 2) - 1] = '\n';
		filler[(j + 1) * (w + 2) - 2] = '\r';
	}
	filler[(w + 2) * h] = '\0';

	while (true) {

		//system("CLS");
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
		cout << filler;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });


		switch (page) {
		case Main:
			mainMenu();
			break;
		case Add:
			addStudent();
			break;
		case Show:
			showStudents();
			break;
		case Select:
			selectStudent();
			break;
		case AddPoints:
			AddPoint();
			break;
		case Save:
			SaveStudents();
			break;
		default:
			mainMenu();
			break;
		}
	}

	return 0;
}
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Book {
public:
	string name;
	string author;
	double price;

	Book(string name, string author, double price) {
		this->name = name;
		this->author = author;
		this->price = price;
	}

	Book(string name, string author, int price) {
		this->name = name;
		this->author = author;
		this->price = price;
	}

	~Book() {
		cout << "Book destructor" << endl;
	}

	void print() {
		cout << "Name: " << name << " Author: " << author << " Price: " << price << endl;
	}

	void print(int index) {
		cout << index << ". Name: " << name << " Author: " << author << " Price: " << price << endl;
	}
};

class BookShop {
private:
	vector<Book> books;

public:

	~BookShop() {
		cout << "BookShop destructor" << endl;
	}

	void addBook(Book& b) {
		books.push_back(b);
	}

	void addBook(string name, string author, double price) {
		books.emplace_back(name, author, price);
	}

	void print() {
		for (int i = 0; i < books.size(); ++i) books[i].print(i + 1);
	}

	int getPrice() {
		int sum = 0;
		for (Book& book : books) sum += book.price;
		return sum;
	}
};

int main() {

	Book book1("Name1", "Author1", 10);
	Book book2("Name2", "Author2", 20.0);
	BookShop shop;

	shop.addBook(book1);
	shop.addBook(book2);


	cout << "Book:" << endl;
	book1.print();
	book2.print();

	cout << "shop:" << endl;

	shop.print();
	cout << shop.getPrice() << endl;

	return 0;
}

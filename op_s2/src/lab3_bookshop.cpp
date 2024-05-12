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

	void print() {
		cout << "Name: " << name << " Author: " << author << " Price: " << price << endl;
	}
};

class BookShop {
private:
	vector<Book> books;

public:
	void addBook(Book b) {
		books.push_back(b);
	}

	void print() {
		for (Book& book : books) book.print();
	}

	int getPrice() {
		int sum = 0;
		for (Book& book : books) sum += book.price;
		return sum;
	}
};

int main() {
		
	Book book1("Name1", "Author1", 10);
	Book book2("Name2", "Author2", 20);
	BookShop shop;

	shop.addBook(book1);
	shop.addBook(book2);


	cout << "Book:" << endl;
	book1.print();
	book2.print();

	cout << "Shop:" << endl;
	shop.print();
	cout << shop.getPrice() << endl;

	return 0;
}
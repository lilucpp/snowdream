#include "Book.h"
#include <iostream>

class Book::BookImpl {
public:
  void print();

private:
  std::string contents_;
  std::string title_;
};

Book::Book() : impl_(new BookImpl()) {}

Book::~Book() {
  delete impl_;
}

void Book::print() { impl_->print(); }

void Book::BookImpl::print() {
  std::cout << "print from BookImpl" << std::endl;
}
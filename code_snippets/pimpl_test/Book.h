#ifndef BOOK_H
#define BOOK_H
#include <string>

class Book {
public:
  Book();
  ~Book();
  void print();

private:
  class BookImpl;
  BookImpl *impl_;
};

#endif
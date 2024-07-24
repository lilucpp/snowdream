#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  vector<string> names{"a/", "b/", "c/", "d/", "e/", "f/", "g/", "h/", "i/", "j/", "k/", "l/",
                       "m/", "n/", "o/", "p/", "q/", "r/", "s/", "t/", "u/", "v/", "w/", "x/",
                       "y/", "z/", "0/", "1/", "2/", "3/", "4/", "5/", "6/", "7/", "8/", "9/"};
  for (auto l1 : names) {
    for (auto l2 : names)
      for (auto l3 : names)
        for (auto l4 : names) boost::filesystem::create_directories("./" + l1 + l2 + l3 + l4);
  }

  return 0;
}

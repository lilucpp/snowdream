#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>

using namespace std;

void usage_help() { cout << "Usage: a.out [-r] dir." << endl; }

template <typename T>
void dir_itr(boost::filesystem::path &p) {
  boost::system::error_code ec;
  T end_itr;
  // ++itr:exception if no permission
  for (T itr = T(p, ec); itr != end_itr; itr = itr.increment(ec)) {
    if (ec) cout << ec.message() << endl;

    cout << itr->path() << endl;
  }
  if (ec) cout << "construct directory_itr fail: " << ec.message() << endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    usage_help();
    return 0;
  }
  namespace fs = boost::filesystem;

  fs::path p;
  bool isRecursive = false;
  if (boost::iequals("-r", argv[1])) {
    if (argc < 3) {
      usage_help();
      return 0;
    }

    isRecursive = true;
    p = argv[2];
  } else {
    p = argv[1];
  }

  boost::system::error_code ec;
  if (!fs::is_directory(p, ec)) {
    if (ec) cout << "is directory error: " << ec.message() << endl;

    cout << "file " << p << " is not dir!" << endl;
    return 0;
  }

  if (isRecursive) {
    dir_itr<fs::recursive_directory_iterator>(p);
  } else {
    dir_itr<fs::directory_iterator>(p);
  }

  return 0;
}
#include <iostream>
#include "boost/filesystem.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " filename" << endl;
    return 0;
  }

  cout << argv[1] << " ";

  boost::system::error_code ec;
  if (!boost::filesystem::exists(argv[1], ec)) {
    cout << "not exit!" << endl;
    if (ec) {
      cout << "error: " << ec.message() << endl;
    }
    return 0;
  }

  // symlink_status(path): Pathname resolution terminates if p names a symbolic
  // link.
  if (boost::filesystem::is_symlink(argv[1])) {
    cout << "is symlink file!" << endl;
    return 0;
  }

  // status(path)
  if (boost::filesystem::is_regular_file(argv[1])) {
    cout << "is regular file!" << endl;
    return 0;
  }

  if (boost::filesystem::is_directory(argv[1])) {
    cout << "is directory!" << endl;
    return 0;
  }
  cout << "unknow type!" << endl;

  return 0;
}

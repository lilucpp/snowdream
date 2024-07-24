#pragma once
#include <string>
#include <fstream>

static std::string GetFileContent(const std::string &filename) {
  std::ifstream ifs(filename);
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                     (std::istreambuf_iterator<char>()    ) );

  return content;
}
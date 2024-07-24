// Base - A C++ base library
// Copyright (c) 2023, Lu Li.  All rights reserved.
// https://github.com/lilucpp/base
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//   * Neither the name of Shuo Chen nor the names of other contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace Helper {

class KVConfFile {
 public:
  KVConfFile() : fieldSep_('='), commentPrefixes_(1, std::string(1, '#')) {}

  KVConfFile(const std::string &filename, const char fieldSep = '=',
             const char comment = '#')
      : fieldSep_(fieldSep), commentPrefixes_(1, std::string(1, comment)) {
    load(filename);
  }

  void setFieldSep(const char sep) { fieldSep_ = sep; }

  void setCommentChar(const char comment) {
    commentPrefixes_ = {std::string(1, comment)};
  }

  void setCommentPrefixes(const std::vector<std::string> &commentPrefixes) {
    commentPrefixes_ = commentPrefixes;
  }

  void load(const std::string &fileName) {
    std::ifstream is(fileName.c_str());
    decode(is);
  }

  void loadStr(const std::string &content) { decode(content); }

  void decode(const std::string &content) {
    std::istringstream ss(content);
    decode(ss);
  }

  void decode(std::istream &is) {
    int lineNo = 0;
    // iterate file by line
    while (!is.eof() && !is.fail()) {
      std::string line;
      std::getline(is, line, '\n');
      eraseComments(&line);
      trim(&line);
      ++lineNo;

      // skip if line is empty
      if (line.size() == 0) continue;

      // find key value separator
      std::size_t pos = line.find(fieldSep_);
      if (pos == std::string::npos) {
        std::stringstream ss;
        ss << "l" << lineNo << ": parsing failed, no '" << fieldSep_
           << "' found";
        std::cerr << ss.str() << std::endl;
        continue;
      }
      // retrieve field name and value
      std::string name = line.substr(0, pos);
      trim(&name);
      std::string value = line.substr(pos + 1, std::string::npos);
      trim(&value);
      kvs_[name] = value;
    }
  }
  void encode(std::ostream &os) const {
    // iterate through all sections in this file
    for (const auto &item : kvs_) {
      os << item.first << fieldSep_ << item.second << std::endl;
    }
  }

  void save(const std::string &fileName) const {
    std::ofstream os(fileName.c_str());
    encode(os);
  }

  std::string get(const std::string &key) {
    if (kvs_.find(key) != kvs_.end()) {
      return kvs_[key];
    } else {
      std::cout << "not fount key: " << key << std::endl;
      return "";
    }
  }

  void set(const std::string &key, const std::string &value) {
    kvs_[key] = value;
  }

 private:
  void trim(std::string *out) {
    size_t startpos = out->find_first_not_of(" \t");
    if (std::string::npos != startpos) {
      size_t endpos = out->find_last_not_of(" \t");
      *out = out->substr(startpos, endpos - startpos + 1);
    } else {
      *out = "";
    }
  }

  void eraseComment(const std::string &commentPrefix, std::string *out,
                    std::string::size_type startpos = 0) {
    size_t prefixpos = out->find(commentPrefix, startpos);
    if (std::string::npos == prefixpos) return;
    // Found a comment prefix, is it escaped?
    if (0 != prefixpos && '\\' == (*out)[prefixpos - 1]) {
      // The comment prefix is escaped, so just delete the escape char
      // and keep erasing after the comment prefix
      out->erase(prefixpos - 1, 1);
      eraseComment(commentPrefix, out, prefixpos - 1 + commentPrefix.size());
    } else {
      out->erase(prefixpos);
    }
  }

  void eraseComments(std::string *out) {
    for (auto &commentPrefix : commentPrefixes_)
      eraseComment(commentPrefix, out);
  }

 private:
  char fieldSep_;
  std::vector<std::string> commentPrefixes_;
  std::map<std::string, std::string> kvs_;
};

}  // namespace Helper

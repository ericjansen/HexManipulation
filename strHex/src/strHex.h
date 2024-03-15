/*
 * This license is mainly applied to libraries. You may copy, distribute and modify the software 
 * provided that modifications are described and licensed for free under LGPL. Derivatives works 
 * (including modifications or anything statically linked to the library) can only be 
 * redistributed under LGPL, but applications that use the library don't have to be.
 *
 *  Copyright (C) 2024 Eric Jansen
 */
#pragma once;

#include <vector>
#include <algorithm>
#include <optional>
#include <sstream>
#include <memory>

namespace jansen {
  enum MODE {
    ONELINE = 0,
    NEWLINE = 1
  };
 
class strHex {
  public:
    strHex(std::stringstream& ss) {
      std::string s;
      while (ss >> s)
        m_vHex.push_back(s);
    }
 
    strHex(const uint32_t& argc, char** argv) {
      if (argc == 2) {
        std::string s(argv[1]);
        std::stringstream ss(s);
        while (ss >> s)
          m_vHex.push_back(s);
      } else if (argc > 2 && argc != 2) {
        for (int i=1; i<argc; ++i)
          m_vHex.push_back(argv[i]);
      } else {
        std::cout << "------------------------------------------------------------------\n";
        std::cout << " program name : strHex\n";
        std::cout << "------------------------------------------------------------------\n";
        std::cout << "      (usage): replace spaces with '\\x' from data\n";
        std::cout << "               which in hex\n";
        std::cout << " (example):\n";
        std::cout << " INPUT:\n";
        std::cout << "        $ ./strHex 4f 17 9a 7b 00 00 00 18 c2\n";
        std::cout << " OUTPUT:\n";
        std::cout << "        $ \\x4f\\x17\\x9a\\x7b\\x00\\x00\\x00\\x18\\xc2\n";
        std::cout << "------------------------------------------------------------------\n";
        std::cout << "    Free to use and modify: https://github.com/ericjansen\n";
        std::cout << "------------------------------------------------------------------\n";
      }
    }
 
    ~strHex() = default;
 
    std::optional<std::string> operator()(const uint16_t &row = 15, const MODE &m = MODE::NEWLINE) {
      std::string strOutput;
 
      if (m_vHex.empty()) {
        strOutput = "\n";
        return std::make_optional<std::string>(strOutput);
      }
      int8_t rc = validateInput();
      std::cout << "rc = " << rc << '\n';
 
      switch (rc) {
        case -1:
          strOutput = "Error! Incorrect input (length > 2) !!\n";
          break;
        case -2:
          strOutput = "Error! Incorrect input (not hex data)!!\n";
          break;
        default:
        case 0: {
          switch (m) {
            case MODE::ONELINE: {
                strOutput = \\x;
                for (int i=0; i<m_vHex.size(); ++i) {
                  (i < m_vHex.size() - 1) ?
                    strOutput += (m_vHex[i] + \\x) :
                    strOutput += m_vHex[i];
                }
                strOutput += ("\tTotal: " + std::to_string(m_vHex.size()) + '\n');
             } break;
            default:
            case MODE::NEWLINE: {
                int j=0;                 
                for (int i=0; i<m_vHex.size(); i+=row) {
                  strOutput += \\x;
                  for (j=i; j<row+i && j<m_vHex.size(); j++) {
                    (j >= i && j < (row+i-1) && j != m_vHex.size()-1) ?
                      strOutput += (m_vHex[j] + \\x) :
                      strOutput += m_vHex[j];
                  }
                  (j == m_vHex.size()) ?
                    strOutput += '\n' :
                    strOutput += \\\n;
                }
                strOutput += ("Total: " + std::to_string(m_vHex.size()) + '\n');
              } break;
          }
        } break;
      }
 
      return std::make_optional<std::string>(strOutput);
    }
 
  private:
    std::vector<std::string> m_vHex;
 
   int8_t validateInput() {
      int8_t rc;
      std::for_each(m_vHex.begin(), m_vHex.end(),
          [&rc](const std::string& str){
            uint32_t count;
            if (str.length() != 2) rc = -1;
            else if (str.length() == 2) {
              assert(str.length() == 2);
              count = std::count_if(str.begin(),str.end(),
                  [](const uint8_t& c) {
                  return (!std::isxdigit(c));
                  }
              );
              if (count != 0) rc = -2;
            } else rc = 0;
          });
      return rc;
    }
};
}

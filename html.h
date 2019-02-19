#pragma once

#ifndef HTML_H
#define HTML_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <cstring>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <utility>
#include <iterator>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

namespace html
{
class Html
{
public:
     Html( std::string& html, std::string& src );
     ~Html();
     void TrimTag( std::string str );
     void TrimEnter( std::string str );
     void TrimSpace( std::string str );
     void TrimSep( std::string str );
     void ToLower( std::vector<std::string>& strVec );
     void Print();
     void DelTrash( std::vector<std::string>& strVec );
     bool ParseKeyW();
     bool ParseContent();
     void Comp();
     std::vector<std::string> GetTh();
     std::vector<std::pair<std::string, int>> GetThem();
     std::vector<std::pair<std::string, int>> GetWord();
     std::string GetSource();

private:
     std::string html_;
     std::vector<std::string> themes_;
     std::vector<std::pair<std::string, int>> pThem;
     std::vector<std::string> content_;
     std::vector<std::pair<std::string, int>> pCont;
     std::string source_;
     bool level_;
};

} //namespace html

#endif // HTML_H

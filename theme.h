#pragma once

#ifndef THEME_H
#define THEME_H

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

typedef std::vector<std::pair<int, double>> allocation;

namespace theme
{

allocation make_alloc(int var, double p);

class Theme
{
public:
     Theme( std::string& name, std::string& source, std::vector<std::pair<std::string, allocation>>& words, double& a );
     Theme( std::string& name, std::string& source, std::vector<std::pair<std::string, allocation>>& words, std::vector<std::pair<int, double>>& a );
     ~Theme();
     void Set( std::string& source, std::vector<std::pair<std::string, allocation>>& words, double& a );
     std::string GetName();
     std::vector<std::string> GetSources();
     std::vector<std::pair<std::string, allocation>> GetWords();
     std::vector<std::pair<int, double>> GetAlloc();
     void Print();

private:
     std::string name_;
     std::vector<std::string> source_;
     std::vector<std::pair<std::string, allocation>> words_;
     std::vector<std::pair<int, double>> a_;
};

} //namespace html

#endif // THEME_H

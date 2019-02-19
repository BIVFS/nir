#include "html.h"
#include "theme.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <cstring>
#include <fstream>
#include <boost/asio.hpp>
#include <vector>
#include <utility>
#include <iterator>
#include <iomanip>
#include <ctime>
#include <chrono>

using boost::asio::ip::tcp;
using namespace html;
using namespace theme;

std::vector<Theme> themes;
std::vector<Html> htmls;

// Give us output from linux command
std::string exec(const char* cmd)
{
     printf( "%s\n\n", cmd );
     std::array<char, 128> buffer;
     std::string result;
     std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
     if (!pipe)
     {
          throw std::runtime_error("popen() failed!");
     }
     while (!feof(pipe.get()))
     {
          if ( fgets( buffer.data(), 128, pipe.get() ) != nullptr ) result += buffer.data();
     }
     return result;
}

void ReadIni( std::vector<Theme>& themes )
{
     exec( "ls -1 themes/ > tmp.txt" );
     std::ifstream fin( "tmp.txt" );
     if( fin.good() )
     {
          while( true )
          {
               if( fin.eof() )
               {
                    break;
               }
               std::string file = "";
               std::string name = "";
               std::getline( fin, name );
               file = "themes/" + name;
               name = name.substr( 0, name.find( '.' ) );
               std::ifstream fl( file );
               std::string src = "";
               std::getline( fl, src );
               if( src.find( "Sources" ) != std::string::npos )
               {
                    src = "";
                    std::getline( fl, src );
               }
               std::string word = "";
               std::vector<std::pair<std::string, allocation>> words;
               std::getline( fl, word );
               if( word.find( "Words" ) != std::string::npos )
               {
                    while( true )
                    {
                         word = "";
                         allocation alloc;
                         std::string var = "";
                         std::string p = "";
                         std::getline( fl, word );
                         if( word.find( "Alloc" ) != std::string::npos )
                         {
                              break;
                         }
                         std::getline( fl, var );
                         std::getline( fl, p );
                         size_t lposV = 0;
                         size_t lposP = 0;
                         while( true )
                         {
                              size_t posV = var.find( '|', lposV );
                              size_t posP = p.find( '|', lposP );
                              if( ( posV != std::string::npos ) && ( posP != std::string::npos ) )
                              {
                                   alloc.push_back( std::make_pair( atoi( var.substr( lposV , posV - lposV ).c_str() ), atof( p.substr( lposP , posP - lposP ).c_str() ) ) );
                                   lposV = posV + 1;
                                   lposP = posP + 1;
                              }
                              else
                              {
                                   break;
                              }
                         }
                         words.push_back( std::make_pair( word, alloc ) );
                    }
               }
               std::string var = "";
               std::string p = "";
               std::getline( fl, var );
               std::getline( fl, p );
               size_t lposV = 0;
               size_t lposP = 0;
               allocation alloc;
               while( true )
               {
                    size_t posV = var.find( '|', lposV );
                    size_t posP = p.find( '|', lposP );
                    if( ( posV != std::string::npos ) && ( posP != std::string::npos ) )
                    {
                         alloc.push_back( std::make_pair( atoi( var.substr( lposV , posV - lposV ).c_str() ), atof( p.substr( lposP , posP - lposP ).c_str() ) ) );
                         lposV = posV + 1;
                         lposP = posP + 1;
                    }
                    else
                    {
                         break;
                    }
               }
               if( fin.eof() )
               {
                    break;
               }
               themes.push_back( Theme( name, src, words, alloc ) );
          }
     }
     fin.close();
     exec( "sudo rm ./tmp.txt" );
}

double Clsfr( std::vector<Theme>& themes, Html& html )
     {
          double p = 0;
          std::vector<std::pair<std::string, int>> thms = html.GetThem();
          std::vector<std::pair<std::string, int>> wrds = html.GetWord();
          int summW = 0;
          for( size_t i = 0; i < wrds.size(); ++i )
          {
               summW += wrds[i].second;
          }
          int nTh = 0;
          int nP = 0;
          for( size_t i = 0; i < themes.size(); ++i )
          {
               for( size_t j = 0; j < thms.size(); ++j )
               {
                    nTh++;
                    if( themes[i].GetName().find( thms[j].first.substr( 0, thms[j].first.length() - 2 ) ) != std::string::npos )
                    {
                         double pT = 0;
                         std::vector<std::pair<int, double>> a = themes[i].GetAlloc();
                         double vMax = 0;
                         for( size_t n = 0; n < a.size(); ++n )
                         {
                              if( thms[j].second >= a[n].first )
                              {
                                   vMax = a[n].second;
                              }
                         }
                         pT = vMax;
                         vMax = 0;
                         double pW = 0;
                         std::vector<std::pair<std::string, allocation>> w = themes[i].GetWords();
                         for( size_t n = 0; n < w.size(); ++n )
                         {
                              for( size_t m = 0; m < wrds.size(); ++m )
                              {
                                   if( w[n].first.find( wrds[m].first.substr( 0, wrds[m].first.length() - 2 ) ) != std::string::npos )
                                   {
                                        for( size_t k = 0; k < w[n].second.size(); ++k )
                                        {
                                             if( wrds[m].second >= w[n].second[k].first )
                                             {
                                                  vMax = w[n].second[k].second;
                                             }
                                        }
                                        pW = ( vMax / wrds[m].second * static_cast<double>( summW ) );
                                        if( pW > 0 )
                                        {
                                             nP++;
                                             p += 1 / ( pW * pT );
                                        }
                                   }
                              }
                         }
                    }
               }
          }
          if( !nTh )
          {
               return 0;
          }
          return ( p / nTh / nP );
     }

void PrintF()
{
     for( size_t i = 0; i < themes.size(); ++i )
     {
          std::string file = "themes/" + themes[i].GetName() + ".txt";
          std::ofstream fl( file );
          fl << "Sources:\n";
          std::vector<std::string> src = themes[i].GetSources();
          std::copy( src.begin(), src.end(), std::ostream_iterator<std::string>( fl, " " ) );
          fl << "\nWords:\n";
          std::vector<std::pair<std::string, allocation>> words = themes[i].GetWords();
          for( size_t j = 0; j < words.size(); ++j )
          {
               fl << words[j].first << std::endl;
               for( size_t k = 0; k < words[j].second.size(); ++k )
               {
                    fl << words[j].second[k].first << "|";
               }
               fl << std::endl;
               for( size_t k = 0; k < words[j].second.size(); ++k )
               {
                    fl << words[j].second[k].second << "|";
               }
               fl << std::endl;
          }
          fl << "Alloc:\n";
          std::vector<std::pair<int, double>> a = themes[i].GetAlloc();
          for( size_t j = 0; j < a.size(); ++j )
          {
               fl << a[j].first << "|";
          }
          fl << std::endl;
          for( size_t j = 0; j < a.size(); ++j )
          {
               fl << a[j].second << "|";
          }
          fl << std::endl << std::endl;
          fl.close();
     }
}

void Print()
{
     for( size_t i = 0; i < themes.size(); ++i )
     {
          std::cout << "Theme: " << themes[i].GetName() << std::endl;
          std::cout << "Sources:\n";
          std::vector<std::string> src = themes[i].GetSources();
          std::copy( src.begin(), src.end(), std::ostream_iterator<std::string>( std::cout, " " ) );
          std::cout << std::endl << "Words:\n";
          std::vector<std::pair<std::string, allocation>> words = themes[i].GetWords();
          for( size_t j = 0; j < words.size(); ++j )
          {
               std::cout << words[j].first << std::endl;
               for( size_t k = 0; k < words[j].second.size(); ++k )
               {
                    std::cout << words[j].second[k].first << "\t";
               }
               std::cout << std::endl;
               for( size_t k = 0; k < words[j].second.size(); ++k )
               {
                    std::cout << words[j].second[k].second << "\t";
               }
               std::cout << std::endl << std::endl;
          }
          std::cout << std::endl << "Alloc:\n";
          std::vector<std::pair<int, double>> a = themes[i].GetAlloc();
          for( size_t j = 0; j < a.size(); ++j )
          {
               std::cout << a[j].first << "\t";
          }
          std::cout << std::endl;
          for( size_t j = 0; j < a.size(); ++j )
          {
               std::cout << a[j].second << "\t";
          }
          std::cout << std::endl << std::endl;
     }
     std::cout << std::endl << std::endl;
}

int Find( std::vector<Theme>& themes, std::pair<std::string, int> rawThs )
{
     for( size_t i = 0; i < themes.size(); ++i )
     {
          if( themes[i].GetName().find( rawThs.first.substr( 0, rawThs.first.length() - 2 ) ) != std::string::npos )
          {
               return i;
          }
     }
     return 0;
}

bool FindSrc( std::vector<Theme>& themes, std::string& src )
{
     for( size_t i = 0; i < themes.size(); ++i )
     {
          std::vector<std::string> srcs = themes[i].GetSources();
          for( size_t j = 0; j < srcs.size(); ++j )
          {
               if( srcs[j].find( src.substr( 0, src.length() - 2 ) ) != std::string::npos )
               {
                    return true;
               }
          }
     }
     return false;
}

void MkT( std::vector<Html>& html, std::vector<Theme>& themes )
{
     for( size_t i = 0; i < html.size(); ++i )
     {
          std::string src = html[i].GetSource();
          if( !FindSrc( themes, src ) )
          {
               std::vector<std::pair<std::string, int>> rawThs = html[i].GetThem();
               std::vector<std::pair<std::string, int>> rawWs = html[i].GetWord();
               std::vector<std::pair<std::string, std::vector<std::pair<int, double>>>> words;
               //ищем сумму вхождения тем в документ
               int sumTh = 0;
               for( size_t j = 0; j < rawThs.size(); ++j )
               {
                    sumTh += rawThs[j].second;
               }
               //ищем сумму вхождения слов в тему
               int sumRw = 0;
               for( size_t j = 0; j < rawWs.size(); ++j )
               {
                    sumRw += rawWs[j].second;
               }
               //создаем вектор пар(слово, его вероятность), исключая пары, слова которых 
               //встречаются лишь 1 раз
               for( size_t j = 0; j < rawWs.size(); ++j )
               {
                    if( rawWs[j].second > 1 )
                    {
                         words.push_back( make_pair( rawWs[j].first, make_alloc( rawWs[j].second,  static_cast<double> ( rawWs[j].second ) / sumRw ) ) );
                    }
               }
               //ищем сумму вероятностей оставшихся слов
               double sumW = 0;
               for( size_t j = 0; j < words.size(); ++j )
               {
                    if( sumW < words[j].second[0].second)
                    {
                         sumW = words[j].second[0].second;
                    }
               }

               //находим градацию по вероятности темы
               std::vector<double> grad;
               for( size_t j = 0; j < rawThs.size(); ++j )
               {
                    if( rawThs[j].second > 1 )
                    {
                         grad.reserve( grad.size() + 1 );
                         grad.push_back( static_cast<double>( rawThs[j].second ) / sumTh );
                    }
                    else
                    {
                         rawThs.erase( rawThs.begin() + j );
                         j--;
                    }
               }
               //отделяем темы и отправляем их в вектор тем
               while( true )
               {
                    double maxGrad = 0;
                    size_t nGrad = 0;
                    //находим максимальное значение в градации

                    for( size_t j = 0; j < grad.size(); ++j )
                    {
                         if( grad[j] > maxGrad )
                         {
                              maxGrad = grad[j];
                              nGrad = j;
                         }
                              if( j == ( grad.size() - 1 ) )
                              {
                                   grad[nGrad] = 0;
                              }
                    }

                    //это значит что прошли все градации, а следовательно и темы
                    if( maxGrad == 0 )
                    {
                         break;
                    }
                    std::vector<std::pair<std::string, allocation>> wordsBuf;
                    //так как вероятности вхождения слов не велики, то оцениваем их принадлежность
                    //к теме не по вероятности слова, а по пропорции его вероятности(отношению
                    //вероятности к сумме вероятностей)
                    //оцениваем принадлежность слов к теме и добавляем по временный вектор слов
                    for( size_t k = 0; k < words.size(); ++k)
                    {
                         if( ( words[k].second[0].second / sumW ) >= maxGrad )
                         {
                              wordsBuf.push_back( words[k] );
                         }
                    }
                    int pos = Find( themes, rawThs[nGrad] );
                    if( !pos )
                    {
                         //создаем временный объект темы и добавляем его в вектор тем
                         std::string srcBuff = html[i].GetSource();
                         Theme tBuff( rawThs[nGrad].first, srcBuff, wordsBuf, maxGrad );
                         themes.push_back( tBuff );
                    }
                    else
                    {
                         std::string srcBuff = html[i].GetSource();
                         themes[pos].Set( srcBuff, wordsBuf, maxGrad );
                    }
                         maxGrad = 0;
                         nGrad = 0;
               }
          }
     }
}

int main(int argc, char* argv[])
{
     std::cout.precision( 4 );
     std::string urlStr = "";
     double p = 0.7;
     std::cout << "Learning(0) or Classifier(1)?\n";
     std::string ans = "";
     std::getline( std::cin, ans );
     if( ans == "0" )
     {
     std::ifstream srcs( "./sources.txt" );
     while( true )
     {
          if( !srcs.good() )
          {
               break;
          }
          std::getline( srcs, urlStr );
          if( srcs.eof() )
          {
               break;
          }
          exec( "sudo rm ./src_tmp.txt" );
          std::string cmd = "sudo curl " + urlStr + " > ./src_tmp.txt";
          exec( cmd.c_str() );
          std::ifstream fin( "./src_tmp.txt" );
          std::stringstream ss;
          ss << fin.rdbuf();
          std::string s = ss.str();
          fin.close();
          Html obj( s, urlStr );
          htmls.push_back( obj );
     }
     srcs.close();
     exec( "sudo rm ./src_tmp.txt" );
     MkT( htmls, themes );
     PrintF();
     }
     else if( ans == "1" )
     {
          ReadIni( themes );
     while( true )
     {
          std::cout << "Enter 0 to stop\n";
          std::getline( std::cin, urlStr );
          if( urlStr == "0" )
          {
               break;
          }
          std::string cmd = "sudo curl " + urlStr + " > ./src_tmp.txt";
          exec( cmd.c_str() );
          std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
          std::ifstream fin( "./src_tmp.txt" );
          std::stringstream ss;
          ss << fin.rdbuf();
          std::string s = ss.str();
          fin.close();
          Html obj( s, urlStr );
          exec( "sudo rm ./src_tmp.txt" );
          double resVer = Clsfr( themes, obj );
          if( resVer >= p )
          {
               std::cout << "This webpage isn't spam\n";
          }
          else
          {
               std::cout << "This webpage is spam\n";
          }
          std::chrono::system_clock::time_point now2 = std::chrono::system_clock::now();
          int milliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>( now2 - now ).count();
          std::cout << "Html time: " << milliSeconds << "ms\n";
     }
     }

     return 0;
}

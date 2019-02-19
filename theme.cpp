#include "theme.h"

namespace theme
{

allocation make_alloc(int var, double p)
{
     allocation alloc;
     alloc.push_back( std::make_pair( var, p ) );
     return alloc;
}

Theme::Theme( std::string& name, std::string& source, std::vector<std::pair<std::string, allocation>>& words, double& a )
: name_( name )
{
     words_ = words;
     source_.push_back( source );
     if( a_.size() )
     {
          a_.push_back( std::make_pair( a_.size() + 1, a_[a_.size() - 1].second * a ) );
     }
     else
     {
          a_.push_back( std::make_pair( a_.size() + 1, a ) );
     }
}

Theme::Theme( std::string& name, std::string& source, std::vector<std::pair<std::string, allocation>>& words, std::vector<std::pair<int, double>>& a )
: name_( name )
{
     words_ = words;
     a_ = a;
     source_.push_back( source );
}

void Theme::Set( std::string& source, std::vector<std::pair<std::string, allocation>>& words, double& a )
{
    // try{
     source_.push_back( source );
     if( words_.size() )
     {
          for( size_t i = 0; i < words.size(); ++i )
          {
               for( size_t j = 0; j < words_.size(); ++j )
               {
                    if( words[i].first.find( words_[j].first.substr( 0, words_[j].first.length() - 2 ) ) != std::string::npos )
                    {
                         for( size_t n = 0; n < words[i].second.size(); ++n )
                         {
                              for( size_t m = 0; m < words_[j].second.size(); ++m )
                              {
                                   if( words[i].second[n].first == words_[j].second[m].first )
                                   {
                                        words_[j].second[m].second = ( words_[j].second[m].second + words[i].second[n].second ) / 2;
                                   }
                                   else
                                   {
                                        if( m == words_[j].second.size() - 1 )
                                        {
                                             words_[j].second.push_back( words[i].second[n] );
                                        }
                                   }
                              }
                         }
                    }
                    else
                    {
                         if( j == words_.size() - 1 )
                         {
                              words_.push_back( words[i] );
                         }
                    }
               }
          }
     }
     else
     {
          words_ = words;
     }

     if( a_.size() )
     {
          a_.push_back( std::make_pair( a_.size() + 1, a_[a_.size() - 1].second * a ) );
     }
     else
     {
          a_.push_back( std::make_pair( a_.size() + 1, a ) );
     }
 //    }
 //    catch( std::exception& e )
 //    {
 //         std::cout << e.what() << std::endl;
 //    }
}

Theme::~Theme()
{
}

std::string Theme::GetName()
{
     return name_;
}

std::vector<std::string> Theme::GetSources()
{
     return source_;
}

std::vector<std::pair<std::string, allocation>> Theme::GetWords()
{
     return words_;
}

std::vector<std::pair<int, double>> Theme::GetAlloc()
{
     return a_;
}

void Theme::Print()
{
     std::cout << name_ << std::endl;
}

} //namespace theme

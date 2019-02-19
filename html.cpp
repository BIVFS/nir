#include "html.h"

namespace html
{

size_t minLenW = 8;

Html::Html( std::string& html, std::string& src )
: html_( html )
, source_( src )
, level_( true )
{
     ParseKeyW();
     ParseContent();
     ToLower( themes_ );
     DelTrash( themes_ );
     ToLower( content_ );
     DelTrash( content_ );
     Comp();
}

Html::~Html()
{
}

void Html::DelTrash( std::vector<std::string>& strVec )
{
     std::vector<std::string> str { "тогд", "котор", "всяк", "другой", "другая", "таков", "стольк", "скольк", "никто",
          "ничто", "какой", "какая", "какое", "такой", "такая", "это", "все", "что", "нужн", "почем", "потом", "есл" };
     for ( size_t i = 0; i < strVec.size(); ++i )
     {
          for( size_t j = 0; j < str.size(); ++j )
          {
               bool flag = false;
               if ( strVec[i].find( str[j] ) != std::string::npos )
               {
                    strVec.erase( strVec.begin() + i );
                    flag = true;
               }
               if( flag )
               {
                    i--;
                    break;
               }
          }
     }
}

void Html::ToLower( std::vector<std::string>& strVec )
{
     for( size_t i = 0; i < strVec.size(); ++i )
     {
          std::string cyrillicAlf = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
          std::string cyrillicAlf2 = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
          for( size_t j = 0; j < cyrillicAlf.length(); j = j + 2 )
          {
               size_t pos = strVec[i].find( cyrillicAlf.substr( j, 2 ) );
               if( pos != std::string::npos )
               {
                    strVec[i][pos] = cyrillicAlf2[j];
                    strVec[i][pos + 1] = cyrillicAlf2[j + 1];
               }
          }
          std::transform( strVec[i].begin(), strVec[i].end(), strVec[i].begin(), ::tolower);
     }
}

void Html::Print()
{
     std::cout << "THEMES\n\n" << themes_.size() << std::endl;
     std::copy( themes_.begin(), themes_.end(), std::ostream_iterator<std::string>( std::cout, "\n" ) );
     std::cout << std::endl << "\n\n\nCONTENT\n\n";
     std::copy( content_.begin(), content_.end(), std::ostream_iterator<std::string>( std::cout, "\n" ) );
     std::cout << std::endl;
}

void Html::TrimTag( std::string str )
{
     std::string tag = "<";
     std::string tagEnd = ">";
     while( true )
     {
          if( str.length() < minLenW )
          {
               break;
          }
          size_t begin = 0;
          size_t end = 0;
          begin = str.find( tag );
          if( begin == std::string::npos )
          {
               if( !str.empty() )
               {
                    TrimEnter( str );
               }
               break;
          }
          else
          {
               end = str.find( tagEnd, begin );
               str.erase( begin, end - begin + 1 );
          }
     }
}

void Html::TrimEnter( std::string str )
{
     size_t begin = 0;
     std::string enter = "\n";
     while( true )
     {
          if( str.length() < minLenW )
          {
               break;
          }
          size_t posEnter = str.find( enter, begin );
          if( posEnter == std::string::npos )
          {
               if( str.substr( begin, str.length() - begin ).length() >= minLenW )
               {
                    TrimSpace( str.substr( begin, str.length() - begin ) );
               }
               break;
          }
          else
          {
               if( str.substr( begin, posEnter - begin ).length() >= minLenW )
               {
                    TrimSpace( str.substr( begin, posEnter - begin ) );
               }
               begin = posEnter + enter.length();
          }
     }
}

void Html::TrimSpace( std::string str )
{
     if( str.length() < minLenW )
     {
          return;
     }
     size_t begin = 0;
     std::string space = " ";
     while( ( str[0] == ' ' ) || ( str[0] == '\t' ) )
     {
          str.erase( 0, 1 );
     }
     if( str.length() < minLenW )
     {
          return;
     }
     while( true )
     {
          size_t posSpace = str.find( space, begin );
          if( posSpace == std::string::npos )
          {
               if( ( str.substr( begin, str.length() - begin ).length() >= minLenW ) && ( str.substr( begin, str.length() - begin ) != space ) )
               {
                    TrimSep( str.substr( begin, str.length() - begin ) );
               }
               break;
          }
          else
          {
               if( ( str.substr( begin, posSpace - begin ).length() >= minLenW ) && ( str.substr( begin, posSpace - begin ) != space ) )
               {
                    TrimSep( str.substr( begin, posSpace - begin ) );
               }
               begin = posSpace + space.length();
          }
     }
}

void Html::TrimSep( std::string str )
{
     if( str.length() < minLenW )
     {
          return;
     }
     std::string sep = ".,-:;\"'&~`<>!?+=|/\\$#%^*_()[]{}1234567890\t";//–
     std::vector<std::string> sep2 { "–", "—", " ", "nbsp", "№", "«", "»", "🔥", "^M" };
     for ( size_t i = 0; i < sep.length(); ++i )
     {
          size_t posSep = 0;
          while ( true )
          {
               if( str.length() < minLenW )
               {
                    break;
               }
               posSep = str.find( sep[i] );
               if ( posSep != std::string::npos )
               {
                    str.erase( posSep, 1 );
               }
               else
               {
                    break;
               }
          }
     }

     for ( size_t i = 0; i < sep2.size(); ++i )
     {
          size_t posSep = 0;
          while ( true )
          {
               if( str.length() < minLenW )
               {
                    break;
               }
               posSep = str.find( sep2[i] );
               if ( posSep != std::string::npos )
               {
                    str.erase( posSep, sep2[i].length() );
               }
               else
               {
                    break;
               }
          }
     }

     if( str.length() >= minLenW )
     {
          if( level_ == true )
          {
               themes_.push_back( str );
          }
          if( level_ == false )
          {
               content_.push_back( str );
          }
     }
}

bool Html::ParseKeyW()
{
     std::string h = "<h1";
     std::string hEnd = "</h";
     std::string html = html_;
     std::string substr = "<meta name=\"keywords\" content=\"";
     std::string sep = ", ";
     size_t begin = html.find( substr );
     size_t end = 0;
     if( begin != std::string::npos )
     {
     begin += substr.length();
     end = html.find ( "\"", begin );
     while ( begin < end )
     {
          size_t posSep = html.find( sep, begin );
          if( ( posSep > end ) || ( posSep == std::string::npos ) )
          {
               std::string s = html.substr( begin, end - begin );
               themes_.push_back( s );
          }
          else
          {
               std::string s = html.substr( begin, posSep - begin );
               themes_.push_back( s );
          }
          begin = posSep + sep.length();
     }
     }
     begin = 0;
     end = 0;
     if( !themes_.size() )
     {
          level_ = true;
          for( size_t i = 0; i < 6; ++i )
          {
               if( themes_.size() )
               {
                    break;
               }
          while( true )
          {
               begin = html.find( h, begin );
               if( begin == std::string::npos )
               {
                    break;
               }
               bool flag = true;
               while( ( html[begin + h.length()] != ' ' ) && ( html[begin + h.length()] != '>' ) )
               {
                    begin++;
                    begin = html.find( h, begin );
                    if( begin == std::string::npos )
                    {
                         flag = false;
                         break;
                    }
               }
               if( !flag )
               {
                    break;
               }
               begin = html.find( '>', begin );
               if( begin == std::string::npos )
               {
                    break;
               }
               begin++;
               end = html.find( hEnd, begin );
               TrimTag( html.substr( begin, end - begin ) );

               html.erase( begin, end - begin );
          }
          h.erase( h.length() - 1, 1);
          char buff = ( i + 2 ) + '0';
          h += buff;
          }
     }
     level_ = false;
     return true;
}

bool Html::ParseContent()
{
     std::string html = html_;
     size_t begin = 0;
     size_t end = 0;
     std::string space = " ";
     std::string li = "<li";
     std::string liEnd = "</li>";
     std::string br = "<br";
     std::string brEnd = br;

     std::string p = "<p";
     std::string pEnd = "</p>";
     std::string h = "<h1";
     std::string hEnd = "</h";
     while( true )
     {
          begin = html.find( h, begin );
          if( begin == std::string::npos )
          {
               break;
          }
          bool flag = true;
          while( ( html[begin + h.length()] != ' ' ) && ( html[begin + h.length()] != '>' ) )
          {
               begin++;
               begin = html.find( h, begin );
               if( begin == std::string::npos )
               {
                    flag = false;
                    break;
               }

          }
          if( !flag )
          {
               break;
          }
          begin = html.find( '>', begin );
          if( begin == std::string::npos )
          {
               break;
          }
          begin++;
          end = html.find( hEnd, begin );
          TrimTag( html.substr( begin, end - begin ) );

          html.erase( begin, end - begin );
     }

     begin = 0;
     end = 0;
     while( true )
     {
          begin = html.find( li, begin );
          if( begin == std::string::npos )
          {
               break;
          }

          bool flag = true;
          while( ( html[begin + li.length()] != ' ' ) && ( html[begin + li.length()] != '>' ) )
          {
               begin++;
               begin = html.find( li, begin );
               if( begin == std::string::npos )
               {
                    flag = false;
                    break;
               }

          }
          if( !flag )
          {
               break;
          }
          begin = html.find( '>', begin );
          if( begin == std::string::npos )
          {
               break;
          }
          begin++;
          end = html.find( liEnd, begin );
          TrimTag( html.substr( begin, end - begin ) );
          html.erase( begin, end - begin );
     }
     begin = 0;
     end = 0;
     while( true )
     {
          begin = html.find( p, begin );
          if( begin == std::string::npos )
          {
               break;
          }
          bool flag = true;
          while( ( html[begin + p.length()] != ' ' ) && ( html[begin + p.length()] != '>' ) )
          {
               begin++;
               begin = html.find( p, begin );
               if( begin == std::string::npos )
               {
                    flag = false;
                    break;
               }

          }
          if( !flag )
          {
               break;
          }
          begin = html.find( '>', begin );
          if( begin == std::string::npos )
          {
               break;
          }
          begin++;
          end = html.find( pEnd, begin );
          TrimTag( html.substr( begin, end - begin ) );

          html.erase( begin, end - begin );
     }
     return true;
}

void Html::Comp()
{
     for( size_t i = 0; i < themes_.size(); ++i )
     {
          int counter = 0;
          for( size_t j = 0; j < content_.size(); ++j )
          {
               if( content_[j].find( themes_[i].substr( 0, themes_[i].length() - 2 ) ) != std::string::npos )
               {
                    counter++;
               }
          }
          pThem.push_back( std::make_pair( themes_[i], counter ) );
     }

     for( size_t i = 0; i < content_.size(); ++i )
     {
          bool flag = false;
          for( size_t j = 0; j < pCont.size(); ++j )
          {
               if( pCont[j].first.find( content_[i].substr( 0, content_[i].length() - 2 ) ) != std::string::npos )
               {
                    flag = true;
                    break;
               }
          }
          if( !flag )
          {
          int counter = 0;
          for( size_t j = 0; j < content_.size(); ++j )
          {
               if( content_[j].find( content_[i].substr( 0, content_[i].length() - 2 ) ) != std::string::npos )
               {
                    counter++;
               }
          }


          pCont.push_back( std::make_pair( content_[i], counter ) );
          }
     }
}

std::vector<std::pair<std::string, int>> Html::GetThem()
{
     return pThem;
}
std::vector<std::pair<std::string, int>> Html::GetWord()
{
     return pCont;
}

std::string Html::GetSource()
{
     return source_;
}

std::vector<std::string> Html::GetTh()
{
     return themes_;
}

} //namespace html

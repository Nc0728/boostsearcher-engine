#include<iostream>
#include<string>
#include<fstream>

namespace nm_util
{
    class file_util
    {
        public:
        static bool  read_file(const std::string& file,std::string &result)
        {
          std::ifstream in(file,std::ios::in);
          std::string now_file;
          if(!in.is_open())
          {
              std::cerr<<"file open"<<file<<"error"<<std::endl;
          }
          while(std::getline(in,now_file))
          {
              result+=now_file;
          }
          in.close();
          return true;
        }
    };
}

#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<boost/algorithm/string.hpp>
#include"cppjieba/Jieba.hpp" 
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
              return false;
          }
          while(std::getline(in,now_file))
          {
              result+=now_file;
              std::cout<<std::endl;
          }
          in.close();
          return true;
        }
    };
    class string_util
    {
        //boost;;split可以对string进行切以any_of'char'
        public:
        static void CutString(const std::string&input,std::vector<std::string>&output,const std::string s)
        {
            boost::split(output,input,boost::is_any_of(s));
        }
    };
      // 1. 路径常量
      const char* JIEBA_PATH = "./dict/jieba.dict.utf8";
      const char* HMM_PATH = "./dict/hmm_model.utf8";
      const char* USER_PATH = "./dict/user.dict.utf8";
      const char* IDF_PATH = "./dict/idf.utf8";
      const char* WORDS_PATH = "./dict/stop_words.utf8";

    class jieba_util
    {
       static cppjieba::Jieba jieba;//创建静态结巴，避免jieba被大量创建浪费内存
     public: 
       static void CutSearch(const std::string&doc,std::vector<std::string>&word)
       {
           jieba.CutForSearch(doc,word);
       }
    };cppjieba::Jieba jieba_util::jieba(JIEBA_PATH, HMM_PATH, USER_PATH, IDF_PATH, WORDS_PATH);
// 顺序：主词典, HMM, 用户词典, IDF, 停用词
}

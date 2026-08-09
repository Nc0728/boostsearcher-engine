#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include"util.hpp"

#define '\3' GEP// 分隔符

namespace DX
{
   struct ForwordIndex
   {
       std::string title;
       std::string content;
       std::string url;
       uint64_t id;
   };
  
   typedef  struct InvertedIndex
   {
       std::string word;
       uint64_t id;
       uint64_t  weight;
   }Invert;
   class index
   {
    private:
     typedef std:vector<Invert>InvertedList;
     typedef std::unordered_map<std::string,std::vector<Invert> >invert;//构建倒排拉链,通过word找到对应倒排id
     std::vector<std::string>FoIndex;//建立索引数组,下标就是id
    public:
       index(){}
       ~index(){}
       ForwordIndex*GetFIndex(const std::string&file);
       bool GetInvIndex(ForwordIndex&nowFD);
       ForwordIndex* FIndex(uint64_t id)
       {
           if(id>=invert.size())
           {
               std::cerr<<id":"<<"not exits!"<<std::endl;
               return nullptr;
           }
           return &invert[id];
       }
       Invert* InvIndex(const std::string& word)
       {
           auto iter=invert.find(word);
           if(iter==invert.end())
           {
               cerr<<"sorry:"<<word<<"is not in here"<<std::endl;
               return false;
           }
           return &(iter->second);
       }
       //建立索引桥梁,根据文档内容  src_path=boostsearcher/data/output/output.txt
       bool Index(const std::string&input)
       {
            ifstream in(input,std::ios::in|std::ios::binary);
            std::string line;
            if(in.is_open())
            {
                std::cerr<<"File:"input<<"open fail"<<std::endl;
            }
            while(getline(in,line)
            {
             //建立索引id
             invert*GF=GetFIndex(line);
              if(GF==nullptr)
                 {
                     std::cerr<<line<<"made error"<<std::endl;
                     continue;
                 }
              //建立倒排索引
             GetInvIndex(*GF):
                
            }                 
             return true;
       }
       private:
              ForwordIndex*GetFIndex(const std::string&file)
              {
                  //1.将引入的文件切割
                  std::vector<std::string>doc;
                  const std::string gap="GAP";
                  nm_util::string_util:: CutString(file,doc,gap);
                  //2.将文件填充到正排数组
                  ForwordIndex result;
                  result.title=doc.title;
                  result.content=doc.content;
                  result.url=doc.url;
                  result.id=FoIndex.size();

                  FoIndex.push_back(std::move(result));
                  return &FoIndex.back();
              }
              struct weight
              {
                  size_t title_weight;
                  size_t content_weight;
                  weight():title_weight(0),content_weight(0){}
              };
              std::unordered_map<std::string,weight>force;//统计权重
              bool GetInvIndex(ForwordIndex&nowFD)
              {
                  std::vector<std::string>t_w;
                  std::vector<std::string>c_w;
                   //分词
                   un_util::cppjieba::CutSearch(nowFD.title,t_w);
                   for(auto now_word:t_w)//词频统计,不用引用是不想修改原来的词语
                   {
                       boost::to_lower(now_word);
                       force[now_word].title_weight++;
                   }
                   un_util::cppjieba::CutSearch(nowFD.content,c_w);
                    
                   for(auto now_word:c_w)
                   {
                       boost::to_lower(now_word);
                       force[now_word].content_weight++;
                   }
#define TITLE_WEIGHT 10
#define CONTENT_WEIGHT 1
                   //写入倒排
                   for(auto &now_word:force)
                   {
                      Invert iv;
                      iv.id=nowFD.id;
                      iv.weight=now_word.title_weight*TITLE_WEIGHT+now_word.content_weight*CONTENT_WEIGHT; 
                      iv.word=now_word.first;
                      InvertedList&invert_list=invert[now_word.first];//统计不同文件中当前word的Invert
                      invert_list.push_back(std::move(iv)); //相同的word插入一个vector
                   }     
                   return true; 
              }

   };
};   




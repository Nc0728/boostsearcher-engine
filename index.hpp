#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include<mutex>
#include"util.hpp"

#define GAP "\3"   // 分隔符

namespace ns_index 
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
     public:
     typedef std::vector<Invert> InvertedList;
     private:
     std::unordered_map<std::string,InvertedList>invert;//构建倒排拉链,通过word找到对应倒排id
     std::vector<ForwordIndex>FoIndex;//建立索引数组,下标就是id
       index(){}
       index(const index&)=delete;//禁止拷贝
       index& operator=(const index&)=delete;//禁止赋值
    public: 
       ~index(){}
       //建立索引单例
       static index*example;
       static index*GetExample()
       {
	   if(example==nullptr)
	   {
           static std::mutex mtx;//防止多人同时索引污染数据
	       if(example==nullptr)
               {
                   mtx.lock();
                   if(example==nullptr)
                   {
                       example=new index();
                   }
	          }
	       mtx.unlock();
	   }
	   return example;
       }
       ForwordIndex* FIndex(uint64_t id)
       {
           if(id>=FoIndex.size())
           {
               std::cerr<<id<<"not exits!"<<std::endl;
               return nullptr;
           }
           return &FoIndex[id];
       }
       InvertedList* InvIndex(const std::string& word)
       {
           auto iter=invert.find(word);
           if(iter==invert.end())
           {
               std::cerr<<"sorry:"<<word<<"is not in here"<<std::endl;
               return nullptr;
           }
           return &(iter->second);
       }
       //建立索引桥梁,根据文档内容  src_path=boostsearcher/data/output/output.txt
       bool Index(const std::string&input)
       {
           std::ifstream in(input,std::ios::in);
            std::string line;
            if(!in.is_open())
            {
                std::cerr<<input<<"open fail"<<std::endl;
                return false;
            }
            int cunt=0;
            while(getline(in,line))
            {
             //建立索引id
             ForwordIndex*GF=GetFIndex(line);
             
              if(GF==nullptr)
                 {
                     std::cerr<<line<<"made error"<<std::endl;
                     continue;
                 }
              //建立倒排索引

             GetInvIndex(*GF);
              if(cunt%50==0)
              {
                  std::cout<<"现在的索引文档："<<cunt<<std::endl;
              }
              cunt++;
                
            }                 
             return true;
       }
       private:
              ForwordIndex*GetFIndex(const std::string&file)
              {
                  //1.将引入的文件切割
                  std::vector<std::string>doc;
                  const std::string gap=GAP;
                  nm_util::string_util::CutString(file,doc,gap);
                  if(doc.size()!=3)
                  {
                      return nullptr;
                  }
                  ForwordIndex result;
                  //2.将文件填充到正排数组
                  result.title=doc[0];
                  result.content=doc[1];
                  result.url=doc[2];
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
              bool GetInvIndex(ForwordIndex&nowFD)
              {

              std::unordered_map<std::string,weight>force;//统计权重
                  std::vector<std::string>t_w;
                  std::vector<std::string>c_w;
                   //分词
                  nm_util::jieba_util::CutSearch(nowFD.title,t_w);
                   for(auto now_word:t_w)//词频统计,不用引用是不想修改原来的词语
                   {
                       boost::to_lower(now_word);
                       force[now_word].title_weight++;
                   }
                   nm_util::jieba_util::CutSearch(nowFD.content,c_w);
                    
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
                      iv.weight=now_word.second.title_weight*TITLE_WEIGHT+now_word.second.content_weight*CONTENT_WEIGHT; 
                      iv.word=now_word.first;
                      InvertedList&invert_list=invert[now_word.first];//统计不同文件中当前word的Invert
                      invert_list.push_back(std::move(iv)); //相同的word插入一个vector
                   }     
                   return true; 
              }
   };       index*index::example=nullptr;

};   




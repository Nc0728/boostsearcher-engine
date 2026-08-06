#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>

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
     std::unordered_map<std::string,std::vector<Invert> >invert;//构建倒排拉链,通过word找到对应倒排id
     std::vector<std::string>FoIndex;//建立索引数组,下标就是id
    public:
       ForwordIndex* FIndex(uint64_t id)
       {
           return nullptr;
       }
       Invert* GetInvIndex(const std::string& word)
       {
           return nullptr;
       }
       //建立索引桥梁,根据文档内容  src_path=boostsearcher/data/output/output.txt
       bool Index(const std::string&input)
       {
           return true;
       }
   };
};   




#include"index.hpp"
#include<algorithm>
#include"util.hpp"
#include<json/json.h>
namespace ns_searcher
{
    class searcher
   {
    private:
          ns_index::index*S_Example;
    public:
         searcher(){}
        ~searcher(){}

         //初始化searcher,传入的文档加载到内存以供搜索
        void Inisearcher(const std::string&path)
        {
           //创建index对象
           S_Example=ns_index::index::GetExample();
           std::cout<<"索引对象创建成功"<<std::endl;
           //建立index索引
           S_Example->Index(path);
           std::cout<<"索引建立成功"<<std::endl;
        }
        //摘要
        std::string GetDesc(const std::string&Content,const std::string&Word)
        {
            //1.找到content对应位置
            //size_t pos=Content.find(Word);//find区分大小写

            auto iter =std::search(Content.begin(),Content.end(),Word.begin(),Word.end(),[](char x,char y){return std::tolower(x)==std::tolower(y);});
            int pos=std::distance(Content.begin(),iter);
            if(iter==Content.end())
            {
                return "Zone1";
            }
            int start=0;
            int end=Content.size()-1;
            //2.更新start end
            if(start>=end)return "Zone2";

            if(pos-start>50)start=pos-50;
            if(end-pos>50)end=pos+50;
            //返回摘要
            return Content.substr(start,end-start+1);
        }
	
       void  search(const std::string input,std::string& json_string)
        {
           //将输入的word分词
            std::vector<std::string>words;
	    nm_util::jieba_util::CutSearch(input,words);
	   //找到所有对应的拉链并且存储
	   ns_index::index::InvertedList all_index_List;
       //提示词转小写
	   for(auto& word:words)
           {
	       //word->id->文档
               boost::to_lower(word);
               ns_index::index::InvertedList*Now_Inv_index_List=S_Example->InvIndex(word);
              if(Now_Inv_index_List==nullptr)
              {
                  continue;
              }
	      all_index_List.insert(all_index_List.end(),Now_Inv_index_List->begin(),Now_Inv_index_List->end());
           }
	   
           //将List快排
	   std::sort(all_index_List.begin(),all_index_List.end(),[](const ns_index::Invert&e1,const ns_index::Invert&e2){return e1.weight>e2.weight;});//快排不接受自定义，用lamada
       Json::Value root;
       for(auto &item:all_index_List)
       { 
         Json::Value elem;
         ns_index::ForwordIndex*now_FIndex=S_Example->FIndex(item.id);
         if(now_FIndex==nullptr)
         continue;
                 
         elem["title"]=now_FIndex->title;   
         elem["desc"]=GetDesc(now_FIndex->content,item.word);
         elem["url"]=now_FIndex->url;
         //elem["id"]=item.id;
         //elem["weight"]=item.weight;
         root.append(elem);

       }
        //Json::StyledWriter writer;
        Json::StreamWriterBuilder builder;
        json_string=Json::writeString(builder,root);
       }
        
    };
}

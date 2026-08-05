#include<iostream>
#include<vector>
#include<string>
#include<boost/filesystem.hpp>
#include"util.cpp"
const std::string src_root="data/input/";
const std::string output="data/output/output.txt";

typedef struct data_infor
{
    //标题
    std::string title;
    //内容 
    std::string content;
    //网址
    std::string url;
}data_infor;

bool ENUM_FILE(const std::string&root, std::vector<std::string>&file_list);
bool PARSER_FILE(const std::vector<std::string>&file,std::vector<data_infor>&outdata);
bool SAVE_FILE(const std::vector<data_infor>&outdata,const std::string&out);


int main()
{
     //将文件遍历，拿取所有文件名
     std::vector<std::string>file_list;
     if(!(ENUM_FILE(src_root,file_list)))
     {
         std::cerr<<"root error!"<<std::endl;
         return 1;
     }
     //将文件解析为标题，内容，网址的形式
     std::vector<data_infor>results;
     if(!(PARSER_FILE(file_list,results)))
     {
         std::cerr<<"error content"<<std::endl;
         return 2;
     }
     //将解析好的内容output,并以\3分割
     if(!SAVE_FILE(results,output))
     {
         std::cerr<<"praser error!"<<std::endl;
         return 3;
     }
     return 0;
}
bool ENUM_FILE(const std::string&root, std::vector<std::string>&file_list)
{
    namespace fs=boost::filesystem;
    fs::path root_path=root;
     if(!(exists(root_path)))
     {
        std::cerr<<"not exists"<<std::endl;
        return false;
     }
     //遍历子目录
     fs::recursive_directory_iterator end;
     for(fs::recursive_directory_iterator iter(root_path);iter!=end;iter++)
     {
         //判断是否为普通文件
         if(!(is_regular_file(*iter)))
         {
             continue;
         }
         //判断是否为html
         if(iter->path().extension()!=".html")
         {
             continue;
         }
         //std::cout<<"debug"<<iter->path().string()<<std::endl;
         //目标文件
         file_list.push_back(iter->path().string());
     }
     return true;
}
static bool parser_title(const std::string&now_file,std::string&now_title);
static bool parser_content(const std::string&now_file,std::string&now_content);
static bool parser_url(const std::string&file_path,std::string&now_url);

//debug
void DeBug_out_data(data_infor &data)
{
    std::cout<<"title: "<<data.title<<std::endl;
    std::cout<<"content: "<<data.content<<std::endl;
    std::cout<<"data: "<<data.url<<std::endl;
}
bool PARSER_FILE(const std::vector<std::string>&file,std::vector<data_infor>&outdata)
{
    for(const std::string FILE:file)
   {
       //1.读取文件
       data_infor doc;
       std::string all_file;
       if(!nm_util::file_util::read_file(FILE,all_file))
       {
           continue;
       }
       //解析文件
       if(!parser_title(all_file,doc.title))
       {
           continue;
       }
       if(!parser_content(all_file,doc.content))
       {
           continue;
       }
       if(!parser_url(FILE,doc.url))
       {
           continue;
       }
       //保存
    //   outdata.push_back(doc);       
        outdata.push_back(std::move(doc));
       //for debug
//       DeBug_out_data(doc);
   }
    return true;
}
static bool parser_title(const std::string&now_file,std::string&now_title)
{
   std::size_t begin=now_file.find("<title>");
   if(begin==std::string::npos)
    {
        return false;
    }
    std::size_t end=now_file.find("</title>");
    if(end==std::string::npos)
    {
        return false;
    }
    if(begin>end)
    {
        return false;
    }
    now_title=now_file.substr(std::string("<title>").size()+begin,end-begin);
    return true;
}
 static bool parser_content(const std::string&now_file,std::string&now_content)
 {
     //定义状态机去标签
     enum status
     {
         lables,
         content
     };
     enum status a=lables;//开关
     for(char s:now_file)
     {
         switch(a)
         {
            case lables:
            if(s=='>')
            {
                a=content;
            }
            break;
            case content:
               //填入数据
               if(s=='<')
               {
                   a=lables;
               }
               else
               {
               if(s=='\n')
                   s=' ' ;// "\n"作为分隔符
               now_content.push_back(s);
               }
               break;
           default:
           break; 
         }
     }return true;
 }
static bool parser_url(const std::string&file_path,std::string&url)
{
   std::string url_head="https://www.boost.org/doc/libs/1_84_0/doc/html/";
   std::string url_tail=file_path.substr(src_root.size());
   url=url_head+url_tail;
   return true;
}
#define GAP '\3'
#define NEXT '\n'
bool SAVE_FILE(const std::vector<data_infor>&outdata,const std::string&output)
{
    std::ofstream out(output,std::ios::binary|std::ios::out);//二进制防止字符被修改
    if(!out.is_open())
     {
         std::cerr<<"open fail:"<<output<<std::endl;
         return false;
     }
    //写入文件
    for(auto &gout:outdata)
     {
         std::string sout;
         sout+=gout.title;
         sout+=GAP;
         sout+=gout.content;
         sout+=GAP;
         sout+=gout.url;
         sout+=NEXT;
         out.write(sout.data(),sout.size());
     }
     out.close();
    return true;
}



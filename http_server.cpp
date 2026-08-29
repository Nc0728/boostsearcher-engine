#include"searcher.hpp"
#include"cpp-httplib/httplib.h"
const std::string root_path="./www.root";
const std::string PATH="./data/output/output.txt";
int main()
{
    httplib::Server svr;
    svr.set_base_dir(root_path.c_str());
    ns_searcher::searcher Search;
    Search.Inisearcher(PATH); 
    svr.Get("/s",[&Search](const httplib::Request&rst,httplib::Response&rps){
            if(!rst.has_param("word"))
            {
               rps.set_content("请输入搜索提示词!","text/plain;charset=utf8");
               return;
            }
            const std::string word=rst.get_param_value("word");
            //rps.set_content("word\n","text/plain;charset=utf8");});
            std::string Json_string;
            Search.search(word,Json_string);
            rps.set_content(Json_string,"application/json;charset=utf8");
});
            svr.listen("0.0.0.0",8081);
    return 0;
}

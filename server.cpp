#include"searcher.hpp"

const std::string PATH="./data/output/output.txt";
int main()
{
    ns_searcher::searcher *S1=new ns_searcher::searcher();
    S1->Inisearcher(PATH);
    while(true)
    {
        std::cout<<"Enter Your Words"<<std::endl;
        std::string words;
        std::cin>>words;
        std::string Json_string;
        S1->search(words,Json_string);
        std::cout<<Json_string<<std::endl;
    }
      
    return 0;
}

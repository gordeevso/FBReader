#ifndef OPDSSIMPLEPARSER_HPP
#define OPDSSIMPLEPARSER_HPP

#include <string>
#include <vector> 
#include <utility> 

class OPDSSimpleParser{
public:
    explicit OPDSSimpleParser(const std::string &OPDSFile);
    ~OPDSSimpleParser(){};
    void parse();
    std::string get_book_type(const size_t &idx, const size_t &href_num);
    std::vector <std::string> OPDS_Title_nodes;
    std::vector <std::vector <std::pair<std::string, std::string> > > OPDS_tree_href;
private:
	std::string OPDSFile;
};

#endif  /* OPDSSIMPLEPARSER_HPP */
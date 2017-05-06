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
    void print_OPDS_tree();
    std::string parse_user_input(const size_t &idx, const size_t &href_num);
    std::string get_book_type(const size_t &idx, const size_t &href_num);
    std::vector <std::string> OPDS_Title_nodes;
    std::vector <std::vector <std::pair<std::string, std::string> > > OPDS_tree_href;
private:
	std::string OPDSFile;
    bool is_book_link(const size_t &idx, const size_t &href_num);
    void saveBook(const size_t &idx, const size_t &href_num);
};

#endif  /* OPDSSIMPLEPARSER_HPP */
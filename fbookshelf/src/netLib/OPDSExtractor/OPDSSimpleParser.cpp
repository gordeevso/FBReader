#include "OPDSSimpleParser.h"
#include "OPDSDownloader.h"
#include <iostream>
#include <fstream>

static const std::string TAG_OPEN_TITLE = "<title>";
static const std::string TAG_CLOSE_TITLE = "</title>";
static const std::string TAG_HREF = "<link href='";
static const std::string TAG_Entry = "<entry>";
static const std::string TAG_Entry_CLOSE = "</entry>";
static const std::string TAG_TYPE = "type='";
static const std::string APPLICATION_ATOM ="application/atom+xml";
static const std::string APPLICATION = "application";
static const std::string PDF = "pdf";
static const std::string FB2 = "fb2";

OPDSSimpleParser::OPDSSimpleParser(const std::string &OPDSFile) : OPDSFile(OPDSFile){}

typedef std::vector <std::pair<std::string, std::string> > href_tree_type;

void OPDSSimpleParser::print_OPDS_tree(){
	if (OPDS_Title_nodes.size() == 0){
		std::cout << "Nothing to be done here" << std::endl;
		return;
	}
    for (size_t i = 0; i < OPDS_Title_nodes.size(); i++){
        std::cout << i + 1 << " " << OPDS_Title_nodes[i] << std::endl;
        for (size_t j = 0; j < OPDS_tree_href[i].size(); j++){
        	std::cout << "   " << j + 1 << " " << OPDS_tree_href[i][j].first << std::endl;
        }
    }
}

std::string OPDSSimpleParser::parse_user_input(const size_t &idx, const size_t &href_num){
	if (idx > OPDS_Title_nodes.size() - 1 || idx == 0) return "/opds";
	if (href_num > OPDS_tree_href[idx].size()) return "/opds";
	if (is_book_link(idx, href_num)){
		saveBook(idx, href_num);
		return "/opds";
	}
	return OPDS_tree_href[idx][href_num].second;
}

bool OPDSSimpleParser::is_book_link(const size_t &idx, const size_t &href_num){
	std::string type_name = OPDS_tree_href[idx][href_num].first;
	if (type_name.find(APPLICATION) != std::string::npos &&
		type_name.find(APPLICATION_ATOM) == std::string::npos){
		return true;
	}
	return false;
}

std::string OPDSSimpleParser::get_book_type(const size_t &idx, const size_t &href_num){
	std::string type_name = OPDS_tree_href[idx][href_num].first;
	std::string book_type;

	if (type_name.find(PDF) != std::string::npos){
		book_type = PDF;
	}
	else if(type_name.find(FB2) != std::string::npos){
		book_type = FB2;
	}

	return book_type;
}

void OPDSSimpleParser::saveBook(const size_t &idx, const size_t &href_num){
	std::string type_name = OPDS_tree_href[idx][href_num].first;
	std::string book_type;

	if (type_name.find(PDF) != std::string::npos){
		book_type = PDF;
	}
	else if(type_name.find(FB2) != std::string::npos){
		book_type = FB2;
	}

	std::string book_name = OPDS_Title_nodes[idx] + "." + book_type;
	std::ofstream write_book(book_name.c_str());

	std::string url = mainDomain + OPDS_tree_href[idx][href_num].second;
	OPDSDownloader downloader;
	std::string content = downloader.download(url);
	write_book << content;

	return;
}

void OPDSSimpleParser::parse(){
	size_t last = 0;
	last = OPDSFile.find(TAG_Entry, last);
	while (last != std::string::npos){
		last = OPDSFile.find(TAG_OPEN_TITLE, last);
		if (last != std::string::npos){
			last += TAG_OPEN_TITLE.length();
			size_t length_of_name = OPDSFile.find(TAG_CLOSE_TITLE, last + 1) - last;
			std::string title = OPDSFile.substr(last, length_of_name);
			last += length_of_name + TAG_CLOSE_TITLE.length();
			if (OPDS_tree_href.size() >= 0){
				size_t closing_tile = OPDSFile.find(TAG_Entry_CLOSE, last);
				if (closing_tile == std::string::npos){
					closing_tile = OPDSFile.length() - 1;
				}
				OPDS_Title_nodes.push_back(title);
				href_tree_type href_arr;
				while(true){
					size_t newHref = OPDSFile.find(TAG_HREF, last);
					if (newHref == std::string::npos || newHref > closing_tile){
						break;
					}
					newHref += TAG_HREF.length();
					last = newHref;
					size_t length_of_href = OPDSFile.find("'", last + 1) - last;
					std::string href = OPDSFile.substr(last, length_of_href);
					last += length_of_href;
					last = OPDSFile.find(TAG_TYPE, last) + TAG_TYPE.length();
					size_t length_of_type = OPDSFile.find("'", last + 1) - last;
					std::string type = OPDSFile.substr(last, length_of_type);
					href_arr.push_back({type, href});
					last += length_of_type;
				}
				OPDS_tree_href.push_back(href_arr);
			}
			else{
				OPDS_Title_nodes.push_back(title);
				href_tree_type tmp;
				OPDS_tree_href.push_back(tmp);
			}
		}
	}
}
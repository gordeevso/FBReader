/*
 * Copyright (C) 2004-2010 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "NetworkFBReaderActions.h"
#include "../OPDSExtractor/OPDSDownloader.h"
#include "../OPDSExtractor/OPDSSimpleParser.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

std::string NetworkFBReaderActions::downloadBook(shared_ptr<Book> book){
    OPDSDownloader downloader;
    std::string book_name = book->title() + "." + book->encoding();
    std::string book_path = downloader.getHomeDir() + "/FBookshelfNet/";
    std::ofstream write_book((book_path + book_name).c_str());
    std::string url = mainDomain + book->file().physicalFilePath();
    std::string content = downloader.download(url);
    write_book << content;
    return (book_path + book_name);
}

std::vector<BookModelFill> NetworkFBReaderActions::getNetworkLibrary(){
    std::string url = "https://books.fbreader.org/opds/by_title";
    OPDSDownloader downloader;
    std::string content = downloader.download(url);
    OPDSSimpleParser parser(content);
    parser.parse();
    std::string baseString = "";
    std::vector < std::pair<std::string, shared_ptr<Book> > > booksmap;
    for (size_t i = 0; i < parser.OPDS_Title_nodes.size(); ++i){
        std::string title =  parser.OPDS_Title_nodes[i];
        std::string path = parser.OPDS_tree_href[i][2].second;
        std::string type = parser.get_book_type(i, 2);
        shared_ptr<Book> bookptr = Book::createBook(
            ZLFile(path), i,
            type,
            "English",
            title
        );
        bookptr->addAuthor("author");
        baseString += "a";
        booksmap.push_back(std::make_pair(path,bookptr));
    }
    return booksmap;
}

void NetworkFBReaderActions::logOut(){
	std::system("cookiebrowser");
}
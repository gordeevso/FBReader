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
#include <ZLibrary.h>
#include <ZLFile.h>
#include <ZLFileImage.h>
#include "NetworkFBReaderActions.h"
#include "OPDSExtractor/OPDSDownloader.h"
#include "OPDSExtractor/OPDSSimpleParser.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

std::string NetworkFBReaderActions::deleteSpaces(std::string name){
	std::string newName = "";
	for(int i = 0; i < name.length(); i++){
		if (name[i] != ' ') newName += name[i];
	}
	return newName;
}

bool doesFileExist(const std::string &fileName){
    std::ifstream f(fileName.c_str());
    return f.good();
}

std::string NetworkFBReaderActions::downloadBook(shared_ptr<Book> book){
    std::string bookName = getBookName(book->title(), book->extension());
    if (!doesFileExist(bookName)){
        std::ofstream write_book(bookName.c_str());
        std::string url = mainDomain + book->url();
        std::string content = downloader.download(url);
        write_book << content;
    }
    return (bookName);
}

std::string NetworkFBReaderActions::getBookName(std::string title, std::string extension){
	std::string book_name = deleteSpaces(ZLFile::replaceIllegalCharacters(title, '_')) + "." + extension;
    std::string book_path = downloader.getHomeDir() + "/FBookshelfNet/";
    return (book_path + book_name);
}

std::string NetworkFBReaderActions::getImageName(std::string title){
	std::string myFileName = ZLFile::replaceIllegalCharacters(title, '_');
	myFileName = downloader.getHomeDir() + ZLibrary::FileNameDelimiter + "/FBookshelfNet/"+ myFileName + ".JPEG";
	return myFileName;
}


void NetworkFBReaderActions::downloadImage(std::string url, std::string name){
    if (!doesFileExist(name)){
    	std::ofstream write_image(name.c_str());
        std::string downloadUrl = mainDomain + url;
        std::string content = downloader.download(downloadUrl);
        write_image << content;
    }
}


std::vector<BookModelFill> NetworkFBReaderActions::getNetworkLibrary(){
    std::string url = "https://books.fbreader.org/opds/by_title";
    std::string content = downloader.download(url);
    OPDSSimpleParser parser(content);
    parser.parse();
    std::vector < std::pair<std::string, shared_ptr<Book> > > booksmap;
    for (size_t i = 0; i < parser.OPDS_Title_nodes.size(); ++i){
        std::string title =  parser.OPDS_Title_nodes[i];
        std::string path = parser.OPDS_tree_href[i][2].second;
        std::string imagePath = parser.OPDS_tree_href[i][1].second;
        std::string imageName = getImageName(title);
        downloadImage(imagePath, imageName);
        ZLFile imageFile(imageName);
        shared_ptr<ZLFileImage> myTitleImage = new ZLFileImage(imageFile, 0);
        std::string type = parser.get_book_type(i, 2);
        bool isDownloaded = doesFileExist(getBookName(title, type));
        shared_ptr<Book> bookptr = Book::createNetBook(
            myTitleImage,
            path,
            title,
            type,
            isDownloaded
        );
        bookptr->addAuthor("author");
        booksmap.push_back(std::make_pair(path,bookptr));
    }
    return booksmap;
}

void NetworkFBReaderActions::logOut(){
	std::system("cookiebrowser");
}
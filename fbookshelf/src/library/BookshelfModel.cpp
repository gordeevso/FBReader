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

#include <queue>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#include <ZLibrary.h>
#include <ZLStringUtil.h>
#include <ZLFile.h>
#include <ZLDir.h>
#include <ZLDialogManager.h>

#include "BookshelfModel.h"
#include "Book.h"
#include "Author.h"
#include "Tag.h"

#include "../formats/FormatPlugin.h"

#include "../database/booksdb/BooksDBUtil.h"
#include "../database/booksdb/BooksDB.h"

shared_ptr<BookshelfModel> BookshelfModel::ourInstance;

BookshelfModel &BookshelfModel::Instance() {
	if (ourInstance.isNull()) {
		ourInstance = new BookshelfModel();
	}
        ourInstance->loadShelvesFromDB();
	return *ourInstance;
}

BookshelfModel::BookshelfModel()
{
    BooksDBUtil::getRecentBooks(myRecentBooks);
}

BooksMap &BookshelfModel::getLibrary()
{
    return myLibrary;
}

bool author_comp(const shared_ptr<Book> & book1, const shared_ptr<Book> & book2)
{
    return book1->authors().back()->name() < book2->authors().back()->name();
}

bool title_comp(const shared_ptr<Book> & book1, const shared_ptr<Book> & book2)
{
    return book1->title() < book2->title();
}

bool bookId_comp(const shared_ptr<Book> & book1, const shared_ptr<Book> & book2)
{
    return book1->bookId() < book2->bookId();
}

void BookshelfModel::buildVecLibrary(BookshelfModel::SortType sort_type)
{   std::map<std::string, shared_ptr<Book> > &booksMap = BookshelfModel::Instance().getLibrary();
    BooksMap::const_iterator it_begin = booksMap.begin();
    BooksMap::const_iterator it_end = booksMap.end();
    if(sort_type == SORT_BY_AUTHOR)
    {
        for(BooksMap::const_iterator it = it_begin; it != it_end; ++it)
        {
            myVecLibrarySortedByAuthors.push_back((*it).second);
            std::sort(myVecLibrarySortedByAuthors.begin(), myVecLibrarySortedByAuthors.end(), author_comp);
        }   
    }
    if(sort_type == SORT_BY_TITLE)
    {
        for(BooksMap::const_iterator it = it_begin; it != it_end; ++it)
        {
            myVecLibrarySortedByTitles.push_back((*it).second);
            std::sort(myVecLibrarySortedByTitles.begin(), myVecLibrarySortedByTitles.end(), title_comp);
        }   
    }
    if(sort_type == SORT_BY_ID)
    {
        for(BooksMap::const_iterator it = it_begin; it != it_end; ++it)
        {
            myVecLibrarySortedByIds.push_back((*it).second);
            std::sort(myVecLibrarySortedByIds.begin(), myVecLibrarySortedByIds.end(), bookId_comp);
        }   
    }
}

std::vector<shared_ptr<Book> > & BookshelfModel::getLibrary(BookshelfModel::SortType sort_type)
{
    if(sort_type == SORT_BY_AUTHOR)
    {
        if(myVecLibrarySortedByAuthors.empty()) { buildVecLibrary(sort_type); }
        return myVecLibrarySortedByAuthors;
    }
    
    if(sort_type == SORT_BY_TITLE)
    {
        if(myVecLibrarySortedByTitles.empty()) { buildVecLibrary(sort_type); }
        return myVecLibrarySortedByTitles;
    }

    if(sort_type == SORT_BY_ID)
    {
        if(myVecLibrarySortedByIds.empty()) { buildVecLibrary(sort_type); }
        return myVecLibrarySortedByIds;
    }

    return myVecLibrarySortedByIds;
}


bool BookshelfModel::hasBooks(const std::string &shelf) const
{
    const BooksByShelf::const_iterator it = myBooksByShelf.find(shelf);
    return it != myBooksByShelf.end() && !it->second.empty();
}

const BookList &BookshelfModel::getBooks(const std::string &shelf) const
{
    return myBooksByShelf[shelf];
}

const ShelfList &BookshelfModel::getShelves() const
{
    return myShelves;
}

int BookshelfModel::loadShelvesFromDB()
{
    myShelves.clear();
    myBooksByShelf.clear();
    
    std::ifstream fin(getPathToShelfDB().c_str());
    if (!fin.is_open())
    {
        saveShelvesFromModelToDB();
    }
    size_t nShelves = 0;
    fin >> nShelves;
    for (size_t i = 0; i < nShelves; ++i)
    {
        std::string shelf;
        fin >> shelf;
        myShelves.push_back(shelf);
        size_t nBooksOnShelf = 0;
        fin >> nBooksOnShelf;
        for (size_t j = 0; j < nBooksOnShelf; ++j)
        {
            int bookId = 0;
            fin >> bookId;
            BooksMap::const_iterator it = myLibrary.begin();
            BooksMap::const_iterator itEnd = myLibrary.end();
            for (; it != itEnd; ++it) {
                if ((*it).second->bookId() == bookId)
                {
                    myBooksByShelf[shelf].push_back((*it).second);
                    (*it).second->addShelf(shelf);
                    break;
                }
            }
        }
    }
    return 0;
}

int BookshelfModel::saveShelvesFromModelToDB()
{
    std::ofstream fout(getPathToShelfDB().c_str());
    if(!fout.is_open()) {
        std::cout << "Can't open file" << std::endl;
        return OPEN_FILE_ERROR;
    }
    fout << myBooksByShelf.size() << std::endl;
    BooksByShelf::const_iterator it = myBooksByShelf.begin();
    BooksByShelf::const_iterator itEnd = myBooksByShelf.end();
    for (; it != itEnd; ++it)
    {
        fout << (*it).first << " " << (*it).second.size() << std::endl;
        for (size_t i = 0; i != (*it).second.size(); ++i)
        {
            fout << (*it).second[i]->bookId() << " ";
        }
        fout << std::endl;
    }
    return 0;
}

void BookshelfModel::removeShelf(const std::string &shelf)
{
    const BooksByShelf::iterator it = myBooksByShelf.find(shelf);
    if (it != myBooksByShelf.end())
    {
        myBooksByShelf.erase(it);
    }
    ShelfList::iterator jt = myShelves.begin();
    ShelfList::iterator jtEnd = myShelves.end();
    for (; jt != jtEnd; ++jt)
    {
        if ((*jt) == shelf)
        {
            myShelves.erase(jt);
            break;
        }
    }
    saveShelvesFromModelToDB();
 }

int BookshelfModel::renameShelf(const std::string &from, const std::string &to)
{
    BooksByShelf::iterator from_it = myBooksByShelf.find(from);
    if (from_it == myBooksByShelf.end())
    {
        return CAN_NOT_FIND_SHELF;
    }
    BooksByShelf::iterator to_it = myBooksByShelf.find(to);
    if (to_it != myBooksByShelf.end())
    {
        return SHELF_ALDREADY_EXISTS;
    }
    ShelfList::iterator it = std::find(myShelves.begin(), myShelves.end(), from);
    myShelves.erase(it);
    myShelves.push_back(to);
    BookList bookList = myBooksByShelf[from];
    myBooksByShelf.insert(std::make_pair(to, bookList));
    myBooksByShelf.erase(from);
    saveShelvesFromModelToDB();
    return 0;
}

void BookshelfModel::addBookToShelf(const std::string &shelf, shared_ptr<Book> book)
{   
    const BooksByShelf::iterator it = myBooksByShelf.find(shelf);
    if (it != myBooksByShelf.end())
    {
        BookList::iterator jt = std::find((*it).second.begin(), (*it).second.end(), book);
        if (jt == (*it).second.end()) 
        {
            (*it).second.push_back(book);
        }
    } else
    {
        createShelf(shelf);
        myBooksByShelf[shelf].push_back(book);
    }
    book->addShelf(shelf);
    saveShelvesFromModelToDB();
}

void BookshelfModel::createShelf(const std::string &shelf)
{
    ShelfList::iterator it = std::find(myShelves.begin(), myShelves.end(), shelf);
    if (it == myShelves.end())
    {
        myShelves.push_back(shelf);
    }
    BooksByShelf::iterator jt = myBooksByShelf.find(shelf);
    if (jt == myBooksByShelf.end())
    {
        BookList emptyList(0);
        myBooksByShelf.insert(std::make_pair(shelf, emptyList));
    }
    saveShelvesFromModelToDB();
}

void BookshelfModel::removeBookFromShelf(const std::string &shelf, shared_ptr<Book> book) 
{
    const ShelfList::iterator kt = std::find(myShelves.begin(), myShelves.end(), shelf);
    if (kt == myShelves.end())
    {
        myShelves.erase(kt);
    }
    BooksByShelf::iterator jt = myBooksByShelf.find(shelf);
    if (jt != myBooksByShelf.end())
    {
        BookList::iterator it = std::find((*jt).second.begin(), (*jt).second.end(), book);
        if (it != (*jt).second.end()) 
        {
            (*jt).second.erase(it);
        }
    }
    book->removeShelf(shelf);
    saveShelvesFromModelToDB();
}

std::string BookshelfModel::getPathToShelfDB(){
    int myuid;
    passwd *mypasswd;
    std::string homeDir;
    myuid = getuid();
    mypasswd = getpwuid(myuid);
    homeDir = mypasswd->pw_dir;
    std::string pathToShelfDB = homeDir + "/.FBookshelf/shelfDB.txt";
    return pathToShelfDB;
}
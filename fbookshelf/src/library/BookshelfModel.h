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

#ifndef __BOOKSHELFMODEL_H__
#define __BOOKSHELFMODEL_H__

#include <map>
#include <set>
#include <shared_ptr.h>
#include <string>
#include <vector>
#include <ZLOptions.h>

#include "Author.h"
#include "Book.h"
#include "Tag.h"
#include "Lists.h"
#include "BookshelfBaseModel.h"

typedef std::map<std::string, shared_ptr<Book> > BooksMap;

class BookshelfModel : BookshelfBaseModel{

public:
    
    static BookshelfModel &Instance();
    
    enum SortType {
        SORT_BY_ID,
        SORT_BY_AUTHOR,
        SORT_BY_TITLE
    };
    
    enum ShelfDBError {
        OPEN_FILE_ERROR,
        CAN_NOT_FIND_SHELF,
        SHELF_ALDREADY_EXISTS
    };
    
private:
    static shared_ptr<BookshelfModel> ourInstance;
        
private:
    BookshelfModel();

public:
    
    BooksMap &getLibrary();
    std::vector<shared_ptr<Book> > & getLibrary(SortType);
    const ShelfList &getShelves() const;
    const BookList &getBooks(const std::string &shelf) const;
    const BooksByShelf &getBooksByShelf() const;
    bool hasBooks(const std::string &shelf) const;
    void removeShelf(const std::string &shelf);
    int renameShelf(const std::string &from, const std::string &to);
    void addBookToShelf(const std::string &shelf, shared_ptr<Book> book, size_t slotIndex);
    void addBookToShelf(const std::string &shelf, shared_ptr<Book> book);
    void replaceBookToShelfSlot(const std::string &shelfFrom, const std::string &shelfTo, size_t slotNumberTo, shared_ptr<Book> book);
    void removeBookFromShelf(const std::string &shelf, shared_ptr<Book> book);
    void createShelf(const std::string &shelf);

private:
    
    BooksMap myLibrary;
    std::vector<shared_ptr<Book> > myVecLibrarySortedByAuthors;
    std::vector<shared_ptr<Book> > myVecLibrarySortedByTitles;
    std::vector<shared_ptr<Book> > myVecLibrarySortedByIds;
    mutable BookList myRecentBooks;
    mutable BookList myUnplacedBooks;
    mutable ShelfList myShelves;
    mutable BooksByShelf myBooksByShelf;
    int saveShelvesFromModelToDB();
    std::string getPathToShelfDB();
    void buildVecLibrary(SortType);
    int loadShelvesFromDB();
};

bool author_comp(const shared_ptr<Book> & book1, const shared_ptr<Book> & book2);
bool title_comp(const shared_ptr<Book> & book1, const shared_ptr<Book> & book2);
bool bookId_comp(const shared_ptr<Book> & book1, const shared_ptr<Book> & book2);

#endif /* __BOOKSHELFMODEL_H__ */
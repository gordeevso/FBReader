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

#include <string>
#include <vector>
#include <set>
#include <map>

#include <shared_ptr.h>

#include <ZLOptions.h>

#include "Book.h"
#include "Author.h"
#include "Tag.h"
#include "Lists.h"

typedef std::map<std::string, shared_ptr<Book> > BooksMap;

class BookshelfModel {

public:
	static BookshelfModel &Instance();

    enum SortType {
        SORT_BY_ID,
        SORT_BY_AUTHOR,
        SORT_BY_TITLE
    };

private:
	static shared_ptr<BookshelfModel> ourInstance;
        
private:
	BookshelfModel();

public:
  
        BooksMap &getLibrary();        
        void buildVecLibrary(SortType);
        std::vector<shared_ptr<Book> > & getLibrary(SortType);

private:
        BooksMap myLibrary;
        std::vector<shared_ptr<Book> > myVecLibrarySortedByAuthors;
        std::vector<shared_ptr<Book> > myVecLibrarySortedByTitles;
        std::vector<shared_ptr<Book> > myVecLibrarySortedByIds;
	mutable BookList myRecentBooks;
};

#endif /* __BOOKSHELFMODEL_H__ */

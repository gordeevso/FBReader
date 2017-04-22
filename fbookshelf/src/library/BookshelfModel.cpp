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
{   BooksMap booksMap = BookshelfModel::Instance().getLibrary();
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
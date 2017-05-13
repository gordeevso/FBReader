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

#ifndef __BOOKSHELFNETFBREADERMODEL_H__
#define __BOOKSHELFNETFBREADERMODEL_H__

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

class BookshelfNetFBReaderModel : public BookshelfBaseModel{

public:
    
    static BookshelfNetFBReaderModel &Instance();
    
    
private:
    static shared_ptr<BookshelfNetFBReaderModel> ourInstance;
        
private:
    BookshelfNetFBReaderModel();

public:
    
    BooksMap &getLibrary();
    std::vector<shared_ptr<Book> > & getLibrary(SortType);

private:
    mutable BookList myRecentBooks;
    void buildVecLibrary(SortType);
};

#endif /* __BOOKSHELFNETFBREADERMODEL_H__ */

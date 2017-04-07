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

#ifndef __FBOOKSHELF_H__
#define __FBOOKSHELF_H__

#include <string>
#include <map>

#include <ZLView.h>
#include <ZLApplication.h>
#include <ZLKeyBindings.h>
#include <ZLOptions.h>

#include "BookshelfView.h"

#include "../library/Library.h"
#include "../bookmodel/BookModel.h"


class ZLFile;
class ZLImageData;
class Book;

typedef std::map<std::string, shared_ptr<Book> > BooksMap;

class Fbookshelf : public ZLApplication {

public:
    static Fbookshelf &Instance();

    static bool createBook(const ZLFile &bookFile, shared_ptr<Book> &book);


public:
    Fbookshelf(const std::string &bookToOpen);
    ~Fbookshelf();

    BookshelfView & getBookshelfView();

    bool addBook(const std::string & pathToBook);
    const BooksMap &getLibrary() const;

    void refreshWindow();

public:
    ZLBooleanOption EnableTapScrollingOption;
private:
    void initWindow();
    std::string helpFileName(const std::string &language) const;

    shared_ptr<ZLKeyBindings> myBindings0;
    shared_ptr<ZLKeyBindings> myBindings90;
    shared_ptr<ZLKeyBindings> myBindings180;
    shared_ptr<ZLKeyBindings> myBindings270;

    BooksMap mLibrary;

    shared_ptr<ZLView> mBookshelfView;
    std::string mBookToOpen;

};

#endif /* __FBOOKSHELF_H__ */

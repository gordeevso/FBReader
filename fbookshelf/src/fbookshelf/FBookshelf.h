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

#include "GridView.h"

#include "../library/BookshelfModel.h"
#include "../bookmodel/BookModel.h"


class ZLFile;
class ZLImageData;
class Book;


class Fbookshelf : public ZLApplication {

public:
    static Fbookshelf &Instance();

    static bool createBook(const ZLFile &bookFile, shared_ptr<Book> &book);

    enum ViewMode {
        UNDEFINED_MODE = 0,
        GRID_MODE = 1 << 0,
        BOOKSHELF_MODE = 1 << 1,
        ALL_MODES = 0xFF
    };

public:
    Fbookshelf(const std::string &bookToOpen);
    ~Fbookshelf();

    void setMode(ViewMode mode);
    ViewMode mode() const;

    GridView & getGridView();
    shared_ptr<ZLKeyBindings> keyBindings();

    void refreshWindow();

private:
    void initWindow();
    std::string helpFileName(const std::string &language) const;

    shared_ptr<ZLKeyBindings> myBindings0;
    shared_ptr<ZLView> myBookshelfView;
    ViewMode myViewMode;

};

#endif /* __FBOOKSHELF_H__ */

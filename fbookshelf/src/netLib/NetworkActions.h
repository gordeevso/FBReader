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

#ifndef __NETWORKACTIONS_H__
#define __NETWORKACTIONS_H__
#include <iostream>
#include <utility>
#include <functional>
#include <shared_ptr.h>
#include <string>
#include <vector>
#include "../library/Book.h"

typedef std::pair<std::string, shared_ptr<Book> > BookModelFill;

class NetworkActions{
public:
	NetworkActions() {}
	// returns the path to which the book was saved
	virtual std::string downloadBook(shared_ptr<Book> book) = 0;
	// returns such a vector with which can easily fill the BookshelfModel
	virtual std::vector<BookModelFill> getNetworkLibrary() = 0;
	virtual void logOut() = 0;
};

#endif /* __NETWORKACTIONS_H__ */
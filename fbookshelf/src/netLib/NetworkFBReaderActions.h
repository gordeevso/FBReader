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

#ifndef __NETWORKFBREADERACTIONS_H__
#define __NETWORKFBREADERACTIONS_H__
#include "OPDSExtractor/OPDSDownloader.h"
#include "NetworkActions.h"


class NetworkFBReaderActions : NetworkActions{
public:
	std::string downloadBook(shared_ptr<Book> book);
	std::vector<BookModelFill> getNetworkLibrary();
	void logOut();
	std::string getBookName(std::string title, std::string extension);
private:
	void downloadImage(std::string url, std::string name);
	std::string getImageName(std::string title);
	std::string deleteSpaces(std::string name);
	OPDSDownloader downloader;
};

#endif /* NETWORKFBREADERACTIONS_H */
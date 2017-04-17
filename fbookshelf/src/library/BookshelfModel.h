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
typedef std::map<shared_ptr<Author>, shared_ptr<Book>, AuthorComparator> BooksMapByAuthor;
typedef std::map<std::string, shared_ptr<Book> > BooksMapByTitle;
typedef std::map<int, shared_ptr<Book> > BooksMapByBookId;

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
//	static const size_t MaxRecentListSize;

//public:
//	ZLStringOption PathOption;
//	ZLBooleanOption ScanSubdirsOption;
//	ZLBooleanOption CollectAllBooksOption;

private:
	BookshelfModel();

public:
//	const AuthorList &authors() const;
//	const TagList &tags() const;
//	const BookList &books(shared_ptr<Author> author) const;
//	const BookList &books(shared_ptr<Tag> tag) const;
//	const BookList &recentBooks() const;

//	enum RemoveType {
//		REMOVE_DONT_REMOVE = 0,
//		REMOVE_FROM_LIBRARY = 1,
//		REMOVE_FROM_DISK = 2,
//		REMOVE_FROM_LIBRARY_AND_DISK = REMOVE_FROM_LIBRARY | REMOVE_FROM_DISK
//	};
		
//	RemoveType canRemove(shared_ptr<Book> book) const;

//	void collectSeriesTitles(shared_ptr<Author> author, std::set<std::string> &titles) const;

//	size_t revision() const;

//	void addBook(shared_ptr<Book> book);
//	void removeBook(shared_ptr<Book> book);
//	void updateBook(shared_ptr<Book> book);
//	void addBookToRecentList(shared_ptr<Book> book);

//	void replaceAuthor(shared_ptr<Author> from, shared_ptr<Author> to);

//	bool hasBooks(shared_ptr<Tag> tag) const;
//	bool hasSubtags(shared_ptr<Tag> tag) const;
//	void removeTag(shared_ptr<Tag> tag, bool includeSubTags);
//	void renameTag(shared_ptr<Tag> from, shared_ptr<Tag> to, bool includeSubTags);
//	void cloneTag(shared_ptr<Tag> from, shared_ptr<Tag> to, bool includeSubTags);    
        BooksMap &getLibrary();
        BooksMapByAuthor &getLibrarySortedByAuthor();
        BooksMapByTitle &getLibrarySortedByTitle();
        BooksMapByBookId &getLibrarySortedByBookId();
        
        std::vector<shared_ptr<Book> > & getLibrary(SortType);

        void loadLibrarySortedByAuthor();
        void loadLibrarySortedByTitle();
        void loadLibrarySortedByBookId();
        void collectDirNames(std::string path, bool myScanSubdirs, std::set<std::string> &nameSet);
        void collectBookFileNames(std::string path, bool myScanSubdirs, std::set<std::string> &bookFileNames);

//private:

//	void synchronize() const;

//	void rebuildBookSet() const;
//	void rebuildMaps() const;

//	void insertIntoBookSet(shared_ptr<Book> book) const;

private:
        BooksMap myLibrary;
        BooksMapByAuthor myLibrarySortedByAuthor;
        BooksMapByTitle myLibrarySortedByTitle;
        BooksMapByBookId myLibrarySortedByBookId;
        std::vector<shared_ptr<Book> > myVecLibrary;
//	mutable BookSet myBooks;
//	mutable BookSet myExternalBooks;

//	mutable AuthorList myAuthors;
//	mutable TagList myTags;
//	typedef std::map<shared_ptr<Author>,BookList,AuthorComparator> BooksByAuthor;
//	mutable BooksByAuthor myBooksByAuthor;
//	typedef std::map<shared_ptr<Tag>,BookList,TagComparator> BooksByTag;
//	mutable BooksByTag myBooksByTag;
	mutable BookList myRecentBooks;

//	mutable std::string myPath;
//	mutable bool myScanSubdirs;

//	enum BuildMode {
//		BUILD_NOTHING = 0,
//		BUILD_UPDATE_BOOKS_INFO = 1 << 0,
//		BUILD_COLLECT_FILES_INFO = 1 << 1,
//		BUILD_ALL = 0x03
//	};
//	mutable BuildMode myBuildMode;
//	mutable size_t myRevision;

//friend class LibrarySynchronizer;
};

#endif /* __BOOKSHELFMODEL_H__ */

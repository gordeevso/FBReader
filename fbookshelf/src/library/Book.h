/*
 * Copyright (C) 2009-2010 Geometer Plus <contact@geometerplus.com>
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

#ifndef __BOOK_H__
#define __BOOK_H__

#include <string>

#include <shared_ptr.h>

#include <ZLFile.h>
#include <ZLFileImage.h>

#include "Lists.h"


class Author;
class Tag;

class Book {

public:
	static const std::string AutoEncoding;

public:
	static shared_ptr<Book> createBook(
		const ZLFile &file,
		int id,
		const std::string &encoding,
		const std::string &language,
		const std::string &title
	);

    static shared_ptr<Book> createNetBook(
        shared_ptr<ZLFileImage> fileimage,
        const std::string &url,
        const std::string &title,
        const std::string &extension,
        const bool &isLocal = false
    );

	static shared_ptr<Book> loadFromFile(const ZLFile &file);

	// this method is used in Migration only
	static shared_ptr<Book> loadFromBookInfo(const ZLFile &file);

private:
    Book(const ZLFile &file, int id, bool is_net_book);

public:
	~Book();

public: // unmodifiable book methods
	const std::string &title() const;
	const ZLFile &file() const;
	const std::string &language() const;
	const std::string &encoding() const;
	const std::string &seriesTitle() const;
	const bool &isLocal() const;
	int indexInSeries() const;

	const TagList &tags() const;
	const AuthorList &authors() const;
    const ShelfList &shelves() const;

    //webview
    const std::string &url() const;
    const std::string &extension() const;
    const shared_ptr<ZLFileImage> image() const;

public: // modifiable book methods
	void setTitle(const std::string &title);
	void setLanguage(const std::string &language);
	void setEncoding(const std::string &encoding);
	void setSeries(const std::string &title, int index);
	void setLocal(const bool &isLocal);
    //webview
    void setUrl(const std::string &url);
    void setExtension(const std::string &extension);
    void setImage(shared_ptr<ZLFileImage> fileimage);

public:
	bool addTag(shared_ptr<Tag> tag);
	bool addTag(const std::string &fullName);
	bool removeTag(shared_ptr<Tag> tag, bool includeSubTags);
	bool renameTag(shared_ptr<Tag> from, shared_ptr<Tag> to, bool includeSubTags);
	bool cloneTag(shared_ptr<Tag> from, shared_ptr<Tag> to, bool includeSubTags);
	void removeAllTags();

	void addAuthor(shared_ptr<Author> author);
	void addAuthor(const std::string &displayName, const std::string &sortKey = std::string());
	bool replaceAuthor(shared_ptr<Author> from, shared_ptr<Author> to);
	void removeAllAuthors();
        
        void addShelf(const std::string &shelf);
        void removeShelf(const std::string &shelf);

public:
	int bookId() const;
	void setBookId(int bookId);

private:
	int myBookId;

	const ZLFile myFile;
	std::string myTitle;
	std::string myLanguage;
	std::string myEncoding;
	std::string mySeriesTitle;
	bool myIsLocal;
	int myIndexInSeries;
	TagList myTags;
	AuthorList myAuthors;

    ShelfList myShelves;
    bool myIsNetBook;
    shared_ptr<ZLFileImage> myImage;
    std::string myUrl;
    std::string myExtension;

private: // disable copying
	Book(const Book &);
	const Book &operator = (const Book &);
};

class BookComparator {

public:
	bool operator () (
		const shared_ptr<Book> book0,
		const shared_ptr<Book> book1
	) const;
};

class BookByFileNameComparator {

public:
	bool operator () (
		const shared_ptr<Book> book0,
		const shared_ptr<Book> book1
	) const;
};

inline const std::string &Book::title() const { return myTitle; }
inline const bool &Book::isLocal() const { return myIsLocal; }
inline const ZLFile &Book::file() const { return myFile; }
inline const std::string &Book::language() const { return myLanguage; }
inline const std::string &Book::encoding() const { return myEncoding; }
inline const std::string &Book::seriesTitle() const { return mySeriesTitle; }
inline int Book::indexInSeries() const { return myIndexInSeries; }

inline const TagList &Book::tags() const { return myTags; }
inline const AuthorList &Book::authors() const { return myAuthors; }
inline const ShelfList &Book::shelves() const { return myShelves; }

inline int Book::bookId() const { return myBookId; }
inline void Book::setBookId(int bookId) { myBookId = bookId; }

inline const std::string & Book::url() const { return myUrl; }
inline const std::string & Book::extension() const { return myExtension; }
inline const shared_ptr<ZLFileImage> Book::image() const { return myImage; }
#endif /* __BOOK_H__ */

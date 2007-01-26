/*
 * Geometric Calculator -- interactive geometry program
 * Copyright (C) 2003-2007 Nikolay Pultsin <geometer@mawhrin.net>
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

#ifndef __GCSAVESCENEHANDLER_H__
#define __GCSAVESCENEHANDLER_H__

#include <map>
#include <set>

#include <ZLSelectionDialog.h>

#include "GCSceneHandler.h"

class GCSaveSceneHandler : public ZLTreeSaveHandler, private GCSceneHandler {

public:
	GCSaveSceneHandler(const std::string &sceneName);

	const std::string sceneName() const;
	const std::string fileName() const;

private:
	bool accept(const std::string &state);
	void processNode(const ZLTreeNode &node);

	const std::string stateDisplayName() const;
	void changeFolder(const ZLTreeNode &node);
	const std::vector<ZLTreeNodePtr> &subnodes() const;
	int selectedIndex() const;

private:
	mutable std::string mySceneName;

	mutable std::map<std::string,std::string> myFileNameBySceneName;
	mutable std::set<std::string> myFileNames;

	mutable bool myIsUpToDate;
};

#endif /* __GCSAVESCENEHANDLER_H__ */
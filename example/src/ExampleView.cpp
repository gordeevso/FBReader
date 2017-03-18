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

#include <cmath>
#include <algorithm>

#include <ZLUnicodeUtil.h>
#include <ZLTimeManager.h>
#include <ZLTextSelectionModel.h>

#include "ExampleView.h"
#include "Example.h"

ExampleView::ExampleView(ZLPaintContext &context) : ZLView(context) {
}


const std::string &ExampleView::caption() const {
	static const std::string cap = "Example";
	return cap;
}

ZLColor ExampleView::backgroundColor() const {
	return ZLColor(255, 255, 255);
}

void ExampleView::paint() {
	context().clear(ZLColor(255, 255, 255));
	context().setColor(ZLColor(255, 0, 0));
	context().drawFilledCircle(100, 100, 50);
	// ZLView::paint();

}

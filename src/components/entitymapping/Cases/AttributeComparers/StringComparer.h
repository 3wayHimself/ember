//
// C++ Interface: StringComparer
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSSTRINGCOMPARER_H
#define EMBEROGRE_MODEL_MAPPING_CASES_ATTRIBUTECOMPARERSSTRINGCOMPARER_H

#include <string>

namespace Ember {



namespace EntityMapping {

namespace Cases {

namespace AttributeComparers {

/**
	Tests for string equality. This is case sensitive.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class StringComparer
{
public:
	explicit StringComparer(std::string value);

	/**
	Returns true if the supplied value equals the stored value. The comparison is case sensitive.
	@param value
	*/
	virtual bool test(const std::string& value);

protected:
	std::string mValue;
};
}

}

}

}

#endif

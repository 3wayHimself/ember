/*
	ConsoleObjectImpl.h by Miguel Guzman (Aglanor)
	Copyright (C) 2002 Miguel Guzman & The Worldforge Project

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __EmberOgre_ConsoleObjectImpl_H__
#define __EmberOgre_ConsoleObjectImpl_H__

#include "framework/ConsoleObject.h"
#include "framework/Singleton.h"

namespace Ember {
namespace OgreView {

/**
@brief The Ogre specific implementation of Ember::ConsoleObject.

This allows a central location to place console commands that belongs to EmberOgre but have no other obvious place in the class hierarcy.

*/
class ConsoleObjectImpl: public Ember::ConsoleObject, public Ember::Singleton<ConsoleObjectImpl>
{
	public:

	ConsoleObjectImpl(void);
	virtual ~ConsoleObjectImpl();

	/**
	 * Receive commands from console
	 */
	void runCommand(const std::string &command, const std::string &args);

	private:


};

}

}

#endif

//
// C++ Implementation: AdapterBase
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AdapterBase.h"

#include "framework/LoggingInstance.h"

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

AdapterSelfUpdateContext::AdapterSelfUpdateContext(AdapterBase& adapter)
: mAdapter(adapter)
{
	mAdapter.mSelfUpdate = true;
}

AdapterSelfUpdateContext::~AdapterSelfUpdateContext()
{
	mAdapter.mSelfUpdate = false;
}


AdapterBase::AdapterBase(const ::Atlas::Message::Element& element)
: mOriginalElement(element), mEditedElement(element), mSelfUpdate(false), mRemoved(false)
{
}


AdapterBase::~AdapterBase()
{
	disconnectAllGuiEventConnections();
}

void AdapterBase::setValue(::Atlas::Message::Element& element)
{
	updateGui(element);
	EventValueChanged.emit();
}

::Atlas::Message::Element& AdapterBase::getValue()
{
	fillElementFromGui();
	return mEditedElement;
}

const ::Atlas::Message::Element& AdapterBase::getOriginalValue() const
{
	return mOriginalElement;
}

bool AdapterBase::hasChanges()
{
	if (mRemoved) {
		return true;
	}
	try {
		return _hasChanges();
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when checking for changes." << ex);
		return false;
	}
}

::Atlas::Message::Element AdapterBase::getChangedElement()
{
	if (mRemoved) {
		return ::Atlas::Message::Element();
	}
	return _getChangedElement();
}

::Atlas::Message::Element AdapterBase::_getChangedElement()
{
	return getValue();
}

void AdapterBase::remove()
{
	mRemoved = true;
}

bool AdapterBase::isRemoved() const
{
	return mRemoved;
}

void AdapterBase::addSuggestion(const std::string& suggestedValue)
{
}


void AdapterBase::addGuiEventConnection(CEGUI::Event::Connection connection)
{
	mGuiEventConnections.push_back(connection);
}

void AdapterBase::disconnectAllGuiEventConnections()
{
	for (std::vector<CEGUI::Event::Connection>::iterator I = mGuiEventConnections.begin(); I != mGuiEventConnections.end(); ++I) {
		(*I)->disconnect();
	}
	mGuiEventConnections.clear();
}



}

}

}

}
}

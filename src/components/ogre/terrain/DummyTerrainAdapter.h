/*
 Copyright (C) 2013 Samuel Kogler

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

#ifndef DUMMYSCENEMANAGERADAPTER_H_
#define DUMMYSCENEMANAGERADAPTER_H_

#include <string>

#include "ITerrainAdapter.h"


namespace Ember {
namespace OgreView {
namespace Terrain {

class TerrainManager;
class ITerrainObserver;


/**
 * @brief 
 */
class DummyTerrainAdapter: public Ember::OgreView::Terrain::ITerrainAdapter
{
public:
	DummyTerrainAdapter(Ogre::SceneManager& sceneManager);
	virtual ~DummyTerrainAdapter();

	virtual int getPageSize() ;

	virtual Ogre::Real getHeightAt(const Ogre::Real x, const Ogre::Real z) ;

	virtual void setWorldPagesDimensions(int numberOfPagesWidth, int numberOfPagesHeight, int widthOffsetInPages, int heightOffsetInPages) ;


	virtual void setCamera(Ogre::Camera* camera) ;


	virtual void setResourceGroupName(const std::string& groupName) ;


	virtual void loadOptions(const std::string& filePath) ;


	virtual void resize(Ogre::AxisAlignedBox newSize, int levels) ;


	virtual void loadScene() ;


	virtual void reset() ;


	virtual void setOption(const std::string& strKey, const void* pValue) ;


	virtual void getOption(const std::string& strKey, void* pDestValue) ;


	virtual void setUninitializedHeight(float height) ;


	virtual void reloadAllPages() ;


	virtual void reloadPage(unsigned int x, unsigned int z) ;

	virtual void loadFirstPage() ;


	virtual std::string getDebugInfo() ;


	virtual ITerrainObserver* createObserver() ;


	virtual void destroyObserver(ITerrainObserver* observer) ;


	virtual std::pair<bool, Ogre::Vector3> rayIntersects(const Ogre::Ray& ray) const;

private:
	Ogre::SceneManager& mSceneManager;
};
}
}
}

#endif /* DUMMYSCENEMANAGERADAPTER_H_ */

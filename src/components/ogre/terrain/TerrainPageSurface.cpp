//
// C++ Implementation: TerrainPageSurface
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainPageSurface.h"
#include "TerrainPageShadow.h"
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurfaceCompiler.h"
#include "TerrainPageGeometry.h"
#include "TerrainLayerDefinition.h"
#include "../Convert.h"
#include <OgreMaterialManager.h>
#include <OgreRoot.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

TerrainPageSurface::TerrainPageSurface(const TerrainPage& terrainPage, ICompilerTechniqueProvider& compilerTechniqueProvider) :
	mTerrainPage(terrainPage), mSurfaceCompiler(new TerrainPageSurfaceCompiler(compilerTechniqueProvider)), mShadow(new TerrainPageShadow(terrainPage))
{
	//create a name for out material
	// 	S_LOG_INFO("Creating a material for the terrain.");
	std::stringstream materialNameSS;
	materialNameSS << "EmberTerrain_Segment";
	materialNameSS << "_" << terrainPage.getWFPosition().x() << "_" << terrainPage.getWFPosition().y();
	mMaterialName = materialNameSS.str();

	mMaterial = Ogre::MaterialManager::getSingleton().create(mMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mMaterialComposite = Ogre::MaterialManager::getSingleton().create(mMaterialName+ "/CompositeMap", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

}

TerrainPageSurface::~TerrainPageSurface()
{
	for (auto& entry : mLayers) {
		delete entry.second;
	}
	Ogre::MaterialManager::getSingleton().remove(mMaterial);
	Ogre::MaterialManager::getSingleton().remove(mMaterialComposite);
	delete mShadow;
}

const TerrainPageSurface::TerrainPageSurfaceLayerStore& TerrainPageSurface::getLayers() const
{
	return mLayers;
}

TerrainPageSurfaceLayer* TerrainPageSurface::updateLayer(TerrainPageGeometry& geometry, int layerIndex, bool repopulate)
{
	auto I = mLayers.find(layerIndex);
	if (I != mLayers.end()) {
		if (repopulate) {
			I->second->populate(geometry);
		}
		//		I->second->updateCoverageImage(geometry);
		return I->second;
	}
	return 0;
}

const TerrainPosition& TerrainPageSurface::getWFPosition() const
{
	return mTerrainPage.getWFPosition();
}

int TerrainPageSurface::getNumberOfSegmentsPerAxis() const
{
	return mTerrainPage.getNumberOfSegmentsPerAxis();
}

unsigned int TerrainPageSurface::getPixelWidth() const
{
	return mTerrainPage.getBlendMapSize();
}

const Ogre::MaterialPtr TerrainPageSurface::getMaterial() const
{
	return mMaterial;
}

const Ogre::MaterialPtr TerrainPageSurface::getCompositeMapMaterial() const
{
	return mMaterialComposite;
}

TerrainPageSurfaceCompilationInstance* TerrainPageSurface::createSurfaceCompilationInstance(const TerrainPageGeometryPtr& geometry) const
{
	//The compiler only works with const surfaces, so we need to create such a copy of our surface map.
	SurfaceLayerStore constLayers;
	for (auto entry : mLayers) {
		constLayers.insert(SurfaceLayerStore::value_type(entry.first, entry.second));
	}
	return mSurfaceCompiler->createCompilationInstance(geometry, constLayers, mShadow);
}

TerrainPageSurfaceLayer* TerrainPageSurface::createSurfaceLayer(const TerrainLayerDefinition& definition, int surfaceIndex, const Mercator::Shader& shader)
{
	TerrainPageSurfaceLayer* terrainSurface = new TerrainPageSurfaceLayer(*this, definition, surfaceIndex, shader);
	mLayers.insert(TerrainPageSurfaceLayerStore::value_type(surfaceIndex, terrainSurface));
	return terrainSurface;
}

TerrainPageShadow* TerrainPageSurface::getShadow() const
{
	return mShadow;
}


}

}
}

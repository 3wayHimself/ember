/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "TerrainModChangeTask.h"
#include "TerrainHandler.h"
#include "TerrainMod.h"

#include "framework/LoggingInstance.h"

#include <Mercator/TerrainMod.h>
#include <Mercator/Terrain.h>
#include <Eris/TerrainMod.h>
#include <Eris/Entity.h>
#include <wfmath/axisbox.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

TerrainModChangeTask::TerrainModChangeTask(Mercator::Terrain& terrain, const Mercator::TerrainMod& terrainMod, const std::string& entityId, TerrainHandler& handler, TerrainModMap& terrainMods) :
	TerrainModTaskBase::TerrainModTaskBase(terrain, 0, entityId, handler, terrainMods), mNewTerrainMod(terrainMod.clone())
{

}

TerrainModChangeTask::~TerrainModChangeTask()
{
	delete mNewTerrainMod;
}

void TerrainModChangeTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	TerrainModMap::iterator I = mTerrainMods.find(mEntityId);
	if (I != mTerrainMods.end()) {
		mManagerLocalTerrainMod = I->second;
		mTerrainMods.erase(I);
		mUpdatedAreas.push_back(mManagerLocalTerrainMod->bbox());
		// Use the pointer returned from addMod() to remove it
		mTerrain.removeMod(mManagerLocalTerrainMod);
		delete mManagerLocalTerrainMod;

		mManagerLocalTerrainMod = mTerrain.addMod(*mNewTerrainMod);
		if (mManagerLocalTerrainMod) {
			mUpdatedAreas.push_back(mManagerLocalTerrainMod->bbox());
			mTerrainMods.insert(TerrainModMap::value_type(mEntityId, mManagerLocalTerrainMod));
		}
	} else {
		S_LOG_WARNING("Got a change signal for a terrain mod which isn't registered with the terrain handler. This shouldn't happen.");
	}
}

void TerrainModChangeTask::executeTaskInMainThread()
{
	mHandler.reloadTerrain(mUpdatedAreas);
}
}

}
}

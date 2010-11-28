//
// C++ Interface: CaelumEnvironment
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifndef EMBEROGRE_ENVIRONMENTCAELUMENVIRONMENT_H
#define EMBEROGRE_ENVIRONMENTCAELUMENVIRONMENT_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "Environment.h"
#include "framework/ConsoleObject.h"

namespace Caelum
{
	class CaelumSystem;
	class SkyDome;
	class BaseSkyLight;
}

namespace Ember {
namespace OgreView {

namespace Environment {

class CaelumSky;
class CaelumSun;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class CaelumEnvironment : public IEnvironmentProvider, public Ember::ConsoleObject
{
public:
    CaelumEnvironment(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera& camera);

    ~CaelumEnvironment();

	virtual void createEnvironment();

	virtual ISun* getSun();
	virtual ISky* getSky();
	virtual IFog* getFog();
	virtual IWater* getWater();
	
	Caelum::CaelumSystem* getCaelumSystem() const;

	const Ember::ConsoleCommandWrapper SetCaelumTime;
    
    virtual void setTime(int hour, int minute, int second = 0);
	virtual void setTime(int seconds);

	/**
	 * @brief Sets the position of the world.
	 * @param longitudeDegrees The longitude, as degrees.
	 * @param latitudeDegrees The latitude, as degrees.
	 */
	virtual void setWorldPosition(float longitudeDegrees, float latitudeDegrees);
	
	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);


private:

	
	/**
	 *    Creates and initializes the Caelum system.
	 * @param root 
	 * @param sceneMgr 
	 * @param window 
	 * @param camera 
	 */
	void setupCaelum(Ogre::Root *root, Ogre::SceneManager *sceneMgr, Ogre::RenderWindow* window, Ogre::Camera& camera);
	
	/**
	 *    Creates a water plane.
	 */
	void setupWater();
	
	// Caelum system
	Caelum::CaelumSystem *mCaelumSystem;
	
	Ogre::SceneManager *mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::Camera& mCamera;
	
	CaelumSky* mSky;
	CaelumSun* mSun;
	IWater* mWater;

};
	
inline Caelum::CaelumSystem* CaelumEnvironment::getCaelumSystem() const
{
	return	mCaelumSystem;
}	

class CaelumEnvironmentComponent
{
protected:
	CaelumEnvironmentComponent(CaelumEnvironment& environment) : mEnvironment(environment), mCaelumSystem(environment.getCaelumSystem()) {}
	CaelumEnvironment& mEnvironment;
	
	/// Caelum system
	Caelum::CaelumSystem *mCaelumSystem;
	
};

}

}

}

#endif

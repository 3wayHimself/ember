//
// C++ Interface: Environment
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
#ifndef EMBER_OGRE_ENVIRONMENTENVIRONMENT_H
#define EMBER_OGRE_ENVIRONMENTENVIRONMENT_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "components/ogre/ILightning.h"
#include "framework/ConsoleObject.h"
#include <sigc++/signal.h>

namespace Ember {
namespace OgreView {
class ICollisionDetector;
namespace Terrain
{
class TerrainManager;
}

/**
 * @brief Namespace for environemnt related classes and activities.
 * With "environment" we refer to things like the weather, the ocean, the foliage etc.
 */
namespace Environment {

class Forest;

class ISun : public ILightning
{
public:
	virtual ~ISun() {}
	virtual void setAmbientLight(const Ogre::ColourValue& colour) = 0;
	virtual Ogre::Vector3 getSunDirection() const = 0;

};

class ISky
{
public:
};

class IFog
{
public:
	virtual ~IFog() {}
	virtual void setDensity(float density) = 0;
	virtual float getDensity() const = 0;
};


/**
@brief Interface for all water techniques. Call isSupported() first to see if the water can be created, and then initialize() to set it all up.
@author Erik Hjortsberg <erik@worldforge.org>
*/
class IWater
{
public:
	virtual ~IWater() {}

	/**
	 * @brief Performs checks to see whether this technique is supported on the current hardware.
	 * @return True if the water technique is supported.
	 */
	virtual bool isSupported() const = 0;
	/**
	 * @brief Initializes the water. You must call this in order for the water to show up.
	 * @return True if the water technique could be setup, else false.
	 */
	virtual bool initialize() = 0;

	/**
	 * @brief Sets the level of the water.
	 * @param height The height of the water level, in world units.
	 */
	virtual void setLevel(float height) = 0;

	/**
	 * @brief Creates a collision detector for the water.
	 * @return A collision detector for the water.
	 */
	virtual ICollisionDetector* createCollisionDetector() = 0;

	/**
	 * @brief Attaches a user object, if possible, to the water.
	 * @param The user object to attach.
	 * @note If the object could be attached, ownership is transferred to the water entity. If however it couldn't be attached, it's responsibility of the calling method to make sure that the object is destroyed.
	 * @return True if the user object was successfully attached. This means that ownership transfers to the water instance.
	 */
	virtual bool setUserAny(const Ogre::Any &anything) = 0;
};

/**
 * @brief An interface for environment providers.
 *
 * An implementation of this is expected to be able to supply the instances which make up the environment.
 */
class IEnvironmentProvider
{
public:

	virtual ~IEnvironmentProvider() {}

	virtual void createEnvironment() = 0;

	virtual ISun* getSun() = 0;
	virtual ISky* getSky() = 0;
	virtual IFog* getFog() = 0;
	virtual IWater* getWater() = 0;

	/**
	 * @brief Sets the current time.
	 * @param hour The hour.
	 * @param minute The minute.
	 * @param second The second.
	 */
	virtual void setTime(int hour, int minute, int second = 0) = 0;

    /**
     * @brief Sets the current time.
     * @param seconds Seconds since midnight.
     */
	virtual void setTime(int seconds) = 0;

	/**
	 * @brief Sets the time multiplier, i.e. how much the time of the environment will progress per real time second.
	 * @param multiplier The time multiplier.
	 */
	virtual void setTimeMultiplier(float multiplier) = 0;

	/**
	 * @brief Gets the time multiplier, i.e. how much the time of the environment will progress per real time second.
	 * @return The time multiplier.
	 */
	virtual float getTimeMultiplier() const = 0;

	/**
	 * @brief Sets the position of the world.
	 * @param longitudeDegrees The longitude, as degrees.
	 * @param latitudeDegrees The latitude, as degrees.
	 */
	virtual void setWorldPosition(float longitudeDegrees, float latitudeDegrees) = 0;

};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Represents the environment.
 *
 * The environment is made up of a sun, a sky, water, a forest and foliage handler and fog.
 *
 * Note that the actual instances used are provided by an instance of IEnvironmentProvider.
 */
class Environment : public ConsoleObject
{
public:

	/**
	 * @brief Ctor.
	 * @param terrainManager The main terrain manager.
	 * @param provider Main environment provider.
	 * @param fallbackProvider A fallback provider which is used if the main provider for some reason fails to create the environment (if for instance the hardware doesn't support it).
	 */
    Environment(Terrain::TerrainManager& terrainManager, IEnvironmentProvider* provider, IEnvironmentProvider* fallbackProvider = 0);

    ~Environment();

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);

 	const ConsoleCommandWrapper SetTime;
	const ConsoleCommandWrapper SetFogDensity;
	const ConsoleCommandWrapper SetAmbientLight;

	ISun* getSun();
	ISky* getSky();
	IFog* getFog();
	IWater* getWater();
	Forest* getForest();


	/**
	 * @brief Sets the current time.
	 * @param hour The hour.
	 * @param minute The minute.
	 * @param second The second.
	 */
    void setTime(int hour, int minute, int second = 0);

    /**
     * @brief Sets the current time.
     * @param seconds Seconds since midnight.
     */
	void setTime(int seconds);

	/**
	 * @brief Sets the time multiplier, i.e. how much the time of the environment will progress per real time second.
	 * @param multiplier The time multiplier.
	 */
	void setTimeMultiplier(float multiplier);

	/**
	 * @brief Gets the time multiplier, i.e. how much the time of the environment will progress per real time second.
	 * @return The time multiplier.
	 */
	float getTimeMultiplier() const;

    void initialize();

	/**
	 *    changes the ambient light
	 * @param colour
	 */
	void setAmbientLight(const Ogre::ColourValue& colour);

	/**
	 * @brief Sets the position of the world.
	 * @param longitudeDegrees The longitude, as degrees.
	 * @param latitudeDegrees The latitude, as degrees.
	 */
	void setWorldPosition(float longitudeDegrees, float latitudeDegrees);


   	/**
	* @brief Emitted when the world ambient light is changed.
	* The first param is the new colour value of the ambient light.
	*/
	sigc::signal<void, const Ogre::ColourValue&> EventUpdatedAmbientLight;

private:

	IEnvironmentProvider* mProvider, *mFallbackProvider;
	Forest* mForest;
};

inline ISun* Environment::getSun()
{
	return mProvider->getSun();
}

inline ISky* Environment::getSky()
{
	return mProvider->getSky();
}

inline IFog* Environment::getFog()
{
	return mProvider->getFog();
}

inline IWater* Environment::getWater()
{
	return mProvider->getWater();
}

inline Forest* Environment::getForest()
{
	return mForest;
}

}

}

}

#endif

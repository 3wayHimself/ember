/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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


#include "World.h"

#include "Avatar.h"
#include "MovementController.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "authoring/EntityMoveManager.h"
#include "Scene.h"
#include "EntityWorldPickListener.h"
#include "EmberEntity.h"
#include "EmberOgreSignals.h"
#include "components/ogre/AvatarCameraMotionHandler.h"
#include "components/ogre/authoring/AuthoringManager.h"
#include "components/ogre/authoring/AuthoringMoverConnector.h"

#include "camera/MainCamera.h"
#include "camera/ThirdPersonCameraMount.h"

#include <Eris/Avatar.h>
#include <Eris/View.h>
#include <Eris/Connection.h>

#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreRoot.h>

namespace Ember
{
namespace OgreView
{

World::World(Eris::View& view, Ogre::RenderWindow& renderWindow, EmberOgreSignals& signals, Input& input) :
	mView(view), mRenderWindow(renderWindow), mSignals(signals), mScene(new Scene()), mViewport(renderWindow.addViewport(&mScene->getMainCamera())), mAvatar(0), mMovementController(0), mMainCamera(new Camera::MainCamera(mScene->getSceneManager(), mRenderWindow, input, mScene->getMainCamera())), mMoveManager(new Authoring::EntityMoveManager(*this)), mEmberEntityFactory(new EmberEntityFactory(view, *mScene)), mMotionManager(new MotionManager()), mAvatarCameraMotionHandler(0), mEntityWorldPickListener(0), mAuthoringManager(new Authoring::AuthoringManager(*this)), mAuthoringMoverConnector(new Authoring::AuthoringMoverConnector(*mAuthoringManager, *mMoveManager))
{
	///set the background colour to black
	mViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mScene->getMainCamera().setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

	signals.EventMotionManagerCreated.emit(*mMotionManager);
	Ogre::Root::getSingleton().addFrameListener(mMotionManager);

	//When calling Eris::View::registerFactory ownership is transferred
	view.registerFactory(mEmberEntityFactory);
	signals.EventCreatedEmberEntityFactory.emit(*mEmberEntityFactory);

	view.getAvatar()->GotCharacterEntity.connect(sigc::mem_fun(*this, &World::View_gotAvatarCharacter));

	mEntityWorldPickListener = new EntityWorldPickListener(*mEmberEntityFactory, *mScene);
	mMainCamera->pushWorldPickListener(mEntityWorldPickListener);

}

World::~World()
{
	delete mAuthoringMoverConnector;
	delete mAuthoringManager;

	delete mMainCamera;
	delete mMoveManager;

	///The factory will be deleted by the mWorldView when that is deleted later on, so we shall not delete it here
	// 	delete mEmberEntityFactory;
	delete mMovementController;
	delete mAvatar;
	delete mAvatarCameraMotionHandler;

	Ogre::Root::getSingleton().removeFrameListener(mMotionManager);
	delete mMotionManager;
	mSignals.EventMotionManagerDestroyed();

	delete mScene;
}

Eris::View& World::getView() const
{
	return mView;
}

Scene& World::getScene() const
{
	return *mScene;
}

Ogre::SceneManager& World::getSceneManager() const
{
	return mScene->getSceneManager();
}

Avatar* World::getAvatar() const
{
	return mAvatar;
}

MotionManager& World::getMotionManager() const
{
	return *mMotionManager;
}

Camera::MainCamera& World::getMainCamera() const
{
	return *mMainCamera;
}

EmberEntityFactory& World::getEntityFactory() const
{
	return *mEmberEntityFactory;
}

MovementController* World::getMovementController() const
{
	return mMovementController;
}

EmberEntity* World::getEmberEntity(const std::string & eid) const
{
	return static_cast<EmberEntity*> (mView.getEntity(eid));
}

EntityWorldPickListener& World::getEntityPickListener() const
{
	return *mEntityWorldPickListener;
}


Authoring::AuthoringManager& World::getAuthoringManager() const
{
	//This can never be null.
	return *mAuthoringManager;
}

void World::View_gotAvatarCharacter(Eris::Entity* entity)
{
	if (entity) {
		EmberEntity& emberEntity = static_cast<EmberEntity&> (*entity);
		//Set up the third person avatar camera and switch to it.
		mAvatar = new Avatar(emberEntity, *mScene, mMainCamera->getCameraSettings());
		mAvatarCameraMotionHandler = new AvatarCameraMotionHandler(*mAvatar);
		mAvatar->getCameraMount().setMotionHandler(mAvatarCameraMotionHandler);
		mMovementController = new MovementController(*mAvatar, *mMainCamera);
		mMainCamera->setMovementProvider(mMovementController);
		mMainCamera->attachToMount(&mAvatar->getCameraMount());

		emberEntity.BeingDeleted.connect(sigc::mem_fun(*this, &World::avatarEntity_BeingDeleted));

		mSignals.EventMovementControllerCreated.emit();
		mSignals.EventCreatedAvatarEntity.emit(emberEntity);
	} else {
		S_LOG_CRITICAL("Somehow got a null avatar entity.");
	}
}

void World::avatarEntity_BeingDeleted()
{
	mMainCamera->attachToMount(0);
	mMainCamera->setMovementProvider(0);
	delete mMovementController;
	mMovementController = 0;
	mSignals.EventMovementControllerDestroyed.emit();
	delete mAvatarCameraMotionHandler;
	mAvatarCameraMotionHandler = 0;
	delete mAvatar;
	mAvatar = 0;
}

}
}

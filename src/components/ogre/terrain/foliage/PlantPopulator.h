/*
 * PlantPopulator.h
 *
 *  Created on: 8 mar 2010
 *      Author: Erik Ogenvik
 */

#ifndef PLANTPOPULATOR_H_
#define PLANTPOPULATOR_H_

#include "components/ogre/terrain/Types.h"

namespace WFMath
{
class MTRand;
}

namespace Ogre {
class Vector2;
}

namespace Ember
{
namespace OgreView
{

namespace Terrain
{
class PlantAreaQueryResult;

namespace Foliage
{

class IScaler
{
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~IScaler() = default;

	virtual void scale(WFMath::MTRand& rnd, const WFMath::Point<2>& pos, Ogre::Vector2& scale) = 0;
};

class UniformScaler : public IScaler
{
public:
	UniformScaler(float min, float max);

	void scale(WFMath::MTRand& rnd, const WFMath::Point<2>& pos, Ogre::Vector2& scale) override;
private:
	float mMin;
	float mRange;
};

class Scaler : public IScaler
{
public:
	Scaler(float xMin, float xMax, float yMin, float yMax);

	void scale(WFMath::MTRand& rnd, const WFMath::Point<2>& pos, Ogre::Vector2& scale) override;
private:
	float mXMin;
	float mXRange;
	float mYMin;
	float mYRange;
};

class PlantPopulator
{
public:

	PlantPopulator(unsigned int layerIndex, IScaler* scaler, size_t plantIndex);
	virtual ~PlantPopulator();

	virtual void populate(PlantAreaQueryResult& result, SegmentRefPtr segmentRef) = 0;

protected:

	int mLayerIndex;
	IScaler* mScaler;
	size_t mPlantIndex;

};

}

}

}

}

#endif /* PLANTPOPULATOR_H_ */

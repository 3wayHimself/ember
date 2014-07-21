#ifndef SMARTBODYSKELETONMAP_H
#define SMARTBODYSKELETONMAP_H

#include <fstream>
#include <string>
#include <vector>


namespace SmartBody
{

class SBAssetManager;
class SBJointMapManager;

}

namespace Ember
{

class SmartBodyManager;

/**
 @brief Represents a joint map for a skeleton.

 It loads and parse a map file, which must be formatted like this :

			OgreBoneName1->SBBoneName1
			OgreBoneName2->SBBoneName2
			...
			OgreBoneNameN->SBBoneNameN

 @author Céline NOEL <celine.noel.7294@gmail.com>

 */
class SmartBodySkeletonMap
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodySkeletonMap(const std::string& skeleton);

	/**
	 * @brief Dtor.
	 */
	~SmartBodySkeletonMap();


	/**
	 * @brief Create the joint map and assign it to the skeleton.
	 */
	void setMap(SmartBody::SBAssetManager& assetMng, SmartBody::SBJointMapManager& mapMng) const;

	/**
	 * @brief Check that the map exists (namely that the map file exists, and that it is correctly written).
	 */
	bool exists() const;

private:

	/**
	 * @brief Name of the skeleton (and by the way, the corresponding joint map).
	 */
	std::string mName;

	/**
	 * @brief Contains the names of the bones : {OgreBoneName, SBJointName}.
	 */
	std::vector<std::string>mBones;

	/**
	 * @brief States that the map exists.
	 */	
	bool mHasMap; 


	/**
	 * @brief Parse the file.
	 */	
	bool parseMapFile(std::ifstream& mapFile);
	
};

}

#endif
//
// C++ Implementation: MeshCollisionDetector
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
// This code is taked from http://www.ogre3d.org/wiki/index.php/Raycasting_to_the_polygon_level (2008-01-13), where it's released as Public Domain, and now relicensed to GPL. Author is unknown.
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

#include "MeshCollisionDetector.h"

#include "model/Model.h"
#include "model/SubModel.h"
#include <OgreSceneNode.h>
#include <OgreRay.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

namespace Ember {
namespace OgreView {

MeshCollisionDetector::MeshCollisionDetector(Model::Model* model)
:mModel(model)
{
}


void MeshCollisionDetector::reload()
{
}

void MeshCollisionDetector::refit()
{
}

void MeshCollisionDetector::setVisualize(bool visualize)
{
}
bool MeshCollisionDetector::getVisualize() const
{
	return false;
}

void MeshCollisionDetector::testCollision(Ogre::Ray& ray, CollisionResult& result)
{

	// at this point we have raycast to a series of different objects bounding boxes.
	// we need to test these different objects to see which is the first polygon hit.
	// there are some minor optimizations (distance based) that mean we wont have to
	// check all of the objects most of the time, but the worst case scenario is that
	// we need to test every triangle of every object.
	Ogre::Real closest_distance = -1.0f;
	Ogre::Vector3 closest_result = Ogre::Vector3::ZERO;
	if (mModel->getNodeProvider() && mModel->getNodeProvider()->getNode()) {
		auto* node = mModel->getNodeProvider()->getNode();
		const auto& submodels = mModel->getSubmodels();
		for (auto& submodel : submodels) {
			Ogre::Entity* pentity = submodel->getEntity();
			if (pentity->isVisible()) {
				// mesh data to retrieve
				size_t vertex_count;
				size_t index_count;

				// get the mesh information
				getMeshInformation(pentity->getMesh(), vertex_count, mVertices, index_count, mIndices,
								   node->_getDerivedPosition(),
								   node->_getDerivedOrientation(),
								   node->getScale());

				// test for hitting individual triangles on the mesh
				bool new_closest_found = false;
				for (int i = 0; i < static_cast<int>(index_count); i += 3) {
					// check for a hit against this triangle
					std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, mVertices[mIndices[i]],
																			 mVertices[mIndices[i + 1]], mVertices[mIndices[i + 2]], true, false);

					// if it was a hit check if its the closest
					if (hit.first) {
						if ((closest_distance < 0.0f) ||
							(hit.second < closest_distance)) {
							// this is the closest so far, save it off
							closest_distance = hit.second;
							new_closest_found = true;
						}
					}
				}

				//16000 seems like a good number for most meshes
				mVertices.resize(16000);
				mVertices.shrink_to_fit();
				mIndices.resize(16000);
				mIndices.shrink_to_fit();

				// if we found a new closest raycast for this object, update the
				// closest_result before moving on to the next object.
				if (new_closest_found) {
					closest_result = ray.getPoint(closest_distance);
				}
			}
		}
	}


	// return the result
	if (closest_distance >= 0.0f)
	{
		// raycast success
		result.collided = true;
		result.position = closest_result;
		result.distance = closest_distance;
	}
	else
	{
		// raycast failed
		result.collided = false;
	}
}


// Get the mesh information for the given mesh.
// Code found on this forum link: http://www.ogre3d.org/wiki/index.php/RetrieveVertexData
void MeshCollisionDetector::getMeshInformation(const Ogre::MeshPtr& mesh,
                                size_t &vertex_count,
                                std::vector<Ogre::Vector3> &vertices,
                                size_t &index_count,
                                std::vector<unsigned long> &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );

		// We only need to add the shared vertices once
		if(submesh->useSharedVertices)
		{
			if( !added_shared )
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}

		// Add the indices
		index_count += submesh->indexData->indexCount;
	}


	// Allocate space for the vertices and indices
	vertices.reserve(vertex_count);
	indices.reserve(index_count);

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
		{
			if(submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf =
				vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex =
				static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
			//  as second argument. So make it float, to avoid trouble when Ogre::Real will
			//  be comiled/typedefed as double:
			//      Ogre::Real* pReal;
			float* pReal;

			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		auto pLong = static_cast<Ogre::uint32*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		auto pShort = reinterpret_cast<Ogre::uint16*>(pLong);


		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

		if ( use32bitindexes )
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				indices[index_offset++] = pLong[k] + offset;
			}
		}
		else
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				indices[index_offset++] = pShort[k] + offset;
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
}

}
}

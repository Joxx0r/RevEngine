
#include "stdafx.h"
#include "RevWorldLoader.h"
#include "RevWorld.h"
#include "MathLib/RevMath.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "RevGameState.h"
#include "RevObject.h"
#include "RevPaths.h"
#include "rapidxml/rapidxml.hpp"
#include "Shlwapi.h"

using namespace rapidxml;
using namespace std;

int GBaseSet = 0; 
int GBaseSetB = 0;

void RevWorldLoader::Initialize()
{
	std::string path = RevPaths::AddLevelPath("GameObjects.xml");
	xml_document<> doc;
	xml_node<> * root_node;
	// Read the xml file into a vector
	ifstream theFile(path);
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);
	// Find our root node
	root_node = doc.first_node("game-objects-types");

	// Iterate over the brewerys
	for (xml_node<> * brewery_node = root_node->first_node("object-type"); brewery_node; brewery_node = brewery_node->next_sibling())
	{
		RevGameObjectType data = {};
		data.m_name = brewery_node->first_attribute("id")->value();
		data.m_instancePath = RevPaths::AddContentPath(brewery_node->first_attribute("instance-path")->value());
		if (PathFileExists(data.m_instancePath.c_str()))
		{
			m_objectTypes.push_back(data);
		}
	}
}

void LoadLights(RevWorld* world, const char* baseFileName)
{
	xml_document<> doc;
	xml_node<> * root_node;
	std::string filePathModified = baseFileName;
	filePathModified.append("_light.xml");
	ifstream theFile(filePathModified.c_str());
	if(theFile.is_open())
	{
		vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("light-instances");
		REV_ASSERT(root_node != nullptr);
		
		for (xml_node<> * instanceNode = root_node->first_node("light-instance"); instanceNode; instanceNode = instanceNode->next_sibling())
		{
			float x, y, z;
	
			x = (float)atof(instanceNode->first_attribute("pos-x")->value());
			y = (float)atof(instanceNode->first_attribute("pos-y")->value());
			z = (float)atof(instanceNode->first_attribute("pos-z")->value());

			float r, g, b;
			r = (float)atof(instanceNode->first_attribute("color-r")->value());
			g = (float)atof(instanceNode->first_attribute("color-g")->value());
			b = (float)atof(instanceNode->first_attribute("color-b")->value());

			float radius;
			radius = (float)atof(instanceNode->first_attribute("radius")->value());
			RevLightType lightType = (RevLightType)(atoi(instanceNode->first_attribute("type")->value()));
			RevLight* newLight = new RevLight();
			newLight->m_color = RevVector3(r, g, b);
			newLight->m_location = RevVector3(x, y, z);
			newLight->m_radius = radius;
			newLight->m_type = lightType;
			world->m_lights.push_back(newLight);
		}
	}
	else
	{
		//todo johlander log
	}
}

RevWorld* RevWorldLoader::LoadWorld(const char* fileName)
{
	RevWorld* world = new RevWorld();
	world->Initialize();
	xml_document<> doc;
	xml_node<> * root_node;
	// Read the xml file into a vector
	std::string filePathModified = fileName;
	filePathModified.append(".xml");
	ifstream theFile(filePathModified.c_str());
	if(theFile.is_open())
	{
		vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("object-instances");
		REV_ASSERT(root_node != nullptr);

		// Iterate over the brewerys
		for (xml_node<> * instanceNode = root_node->first_node("object-instance"); instanceNode; instanceNode = instanceNode->next_sibling())
		{
			float x, y, z;
			float rotx, roty, rotz;
			x = (float)atof(instanceNode->first_attribute("pos-x")->value());
			y = (float)atof(instanceNode->first_attribute("pos-y")->value());
			z = (float)atof(instanceNode->first_attribute("pos-z")->value());
			rotx = (float)atof(instanceNode->first_attribute("rot-x")->value());
			roty = (float)atof(instanceNode->first_attribute("rot-y")->value());
			rotz = (float)atof(instanceNode->first_attribute("rot-z")->value());

			const char* name = instanceNode->first_attribute("type")->value();
			RevGameObjectType* type = nullptr;
			for (INT32 index =0; index < m_objectTypes.size(); index++)
			{
				const RevGameObjectType& typeInstance = m_objectTypes[index];
				if (strcmp(typeInstance.m_name.c_str(), name) == 0)
				{
					type = &m_objectTypes[index];
					break;
				}
			}

			if (type != nullptr)
			{
				RevObject* revObject = new RevObject();
				RevMatrix m;
				m.Identity();
		
				RevMatrix mRotX, mRotY, mRotZ;

				mRotX = RevCreateRotationFromX(rotx * REV_RADIAN_TO_ANGLE);
				mRotY = RevCreateRotationFromY(roty * REV_RADIAN_TO_ANGLE);
				mRotZ = RevCreateRotationFromZ(rotz * REV_RADIAN_TO_ANGLE);

				m = mRotX * mRotY * mRotZ;
				m.SetLocation(RevVector(x, y, z));
				revObject->Initialize(world, m, (UINT)world->m_revGameState->m_gameObjects.size(), type->m_instancePath.c_str());
				world->m_revGameState->m_gameObjects.push_back(revObject);
			}
		}
	}
	else
	{
		//todo johlander log
	}
	
	//still load lights even though file might not been open
	LoadLights(world, fileName);
	
	return world;
}

void RevWorldLoader::SpawnInstanceToWorld(class RevWorld* world, const char* name)
{
	RevGameObjectType* type = nullptr;
	for (INT32 index = 0; index < m_objectTypes.size(); index++)
	{
		const RevGameObjectType& typeInstance = m_objectTypes[index];
		if (strcmp(typeInstance.m_name.c_str(), name) == 0)
		{
			type = &m_objectTypes[index];
			break;
		}
	}

	if (type)
	{
		RevObject* instance = new RevObject();
		RevMatrix m;
		m.Identity();
		instance->Initialize(world, m, (UINT)world->m_revGameState->m_gameObjects.size(), type->m_instancePath.c_str());
		world->m_revGameState->ReplaceObject(0, instance);
	}
}


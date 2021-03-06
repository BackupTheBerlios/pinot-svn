/*
 *  Copyright 2007,2008 Fabrice Colin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <iostream>

#include "DBusIndex.h"
#ifdef HAVE_GOOGLEAPI
#include "GoogleAPIEngine.h"
#endif
#include "PluginWebEngine.h"
#include "ModuleFactory.h"
#if 0
#include "XapianDatabaseFactory.h"
#include "XapianIndex.h"
#include "XapianEngine.h"
#endif

#ifdef __CYGWIN__
#define DLOPEN_FLAGS RTLD_NOW
#else
#define DLOPEN_FLAGS (RTLD_NOW|RTLD_LOCAL)
#endif

#define GETMODULETYPEFUNC	"getModuleType"
#define OPENORCREATEINDEXFUNC	"openOrCreateIndex"
#define MERGEINDEXESFUNC	"mergeIndexes"
#define GETINDEXFUNC		"getIndex"
#define GETSEARCHENGINEFUNC	"getSearchEngine"
#define CLOSEALLFUNC		"closeAll"

typedef string (getModuleTypeFunc)(void);
typedef bool (openOrCreateIndexFunc)(const string &, bool &, bool, bool);
typedef bool (mergeIndexesFunc)(const string &, const string &, const string &);
typedef IndexInterface *(getIndexFunc)(const string &);
typedef SearchEngineInterface *(getSearchEngineFunc)(const string &);
typedef void (closeAllFunc)(void);

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::map;
using std::set;
using std::pair;

LoadableModule::LoadableModule(const string &type, const string &location,
	void *pHandle) :
	m_type(type),
	m_location(location),
	m_canSearch(false),
	m_canIndex(false),
	m_pHandle(pHandle)
{
}

LoadableModule::LoadableModule(const LoadableModule &other) :
	m_type(other.m_type),
	m_location(other.m_location),
	m_canSearch(other.m_canSearch),
	m_canIndex(other.m_canIndex),
	m_pHandle(other.m_pHandle)
{
}

LoadableModule::~LoadableModule()
{
}

LoadableModule &LoadableModule::operator=(const LoadableModule &other)
{
	m_type = other.m_type;
	m_location = other.m_location;
	m_canSearch = other.m_canSearch;
	m_canIndex = other.m_canIndex;
	m_pHandle = other.m_pHandle;

	return *this;
}

map<string, LoadableModule> ModuleFactory::m_types;

ModuleFactory::ModuleFactory()
{
}

ModuleFactory::~ModuleFactory()
{
}

IndexInterface *ModuleFactory::getLibraryIndex(const string &type, const string &option)
{
	map<string, LoadableModule>::iterator typeIter = m_types.find(type);
	if ((typeIter == m_types.end()) ||
		(typeIter->second.m_canIndex == false))
	{
		// We don't know about this type, or doesn't support indexes
		return false;
	}

	void *pHandle = typeIter->second.m_pHandle;
	if (pHandle == NULL)
	{
		return NULL;
	}

	getIndexFunc *pFunc = (getIndexFunc *)dlsym(pHandle,
		GETINDEXFUNC);
	if (pFunc != NULL)
	{
		return (*pFunc)(option);
	}
#ifdef DEBUG
	cout << "ModuleFactory::getLibraryIndex: couldn't find export getIndex" << endl;
#endif

	return NULL;
}

SearchEngineInterface *ModuleFactory::getLibrarySearchEngine(const string &type, const string &option)
{
	map<string, LoadableModule>::iterator typeIter = m_types.find(type);
	if (typeIter == m_types.end())
	{
		// We don't know about this type
		return NULL;
	}

	void *pHandle = typeIter->second.m_pHandle;
	if (pHandle == NULL)
	{
		return NULL;
	}

	getSearchEngineFunc *pFunc = (getSearchEngineFunc *)dlsym(pHandle,
		GETSEARCHENGINEFUNC);
	if (pFunc != NULL)
	{
		return (*pFunc)(option);
	}
#ifdef DEBUG
	cout << "ModuleFactory::getLibrarySearchEngine: couldn't find export getSearchEngine" << endl;
#endif

	return NULL;
}

unsigned int ModuleFactory::loadModules(const string &directory)
{
	struct stat fileStat;
	unsigned int count = 0;

	if (directory.empty() == true)
	{
		return 0;
	}

	// Is it a directory ?
	if ((stat(directory.c_str(), &fileStat) == -1) ||
		(!S_ISDIR(fileStat.st_mode)))
	{
		cerr << "ModuleFactory::loadModules: " << directory << " is not a directory" << endl;
		return 0;
	}

	// Scan it
	DIR *pDir = opendir(directory.c_str());
	if (pDir == NULL)
	{
		return 0;
	}

	// Iterate through this directory's entries
	struct dirent *pDirEntry = readdir(pDir);
	while (pDirEntry != NULL)
	{
		char *pEntryName = pDirEntry->d_name;
		if (pEntryName != NULL)
		{
			string fileName = pEntryName;
			string::size_type extPos = fileName.find_last_of(".");

			if ((extPos == string::npos) ||
				(fileName.substr(extPos) != ".so"))
			{
				// Next entry
				pDirEntry = readdir(pDir);
				continue;
			}

			fileName = directory;
			fileName += "/";
			fileName += pEntryName;

			// Check this entry
			if ((stat(fileName.c_str(), &fileStat) == 0) &&
				(S_ISREG(fileStat.st_mode)))
			{
				void *pHandle = dlopen(fileName.c_str(), DLOPEN_FLAGS);
				if (pHandle != NULL)
				{
					// What type does this export ?
					getModuleTypeFunc *pTypeFunc = (getModuleTypeFunc *)dlsym(pHandle,
						GETMODULETYPEFUNC);
					if (pTypeFunc != NULL)
					{
						string moduleType((*pTypeFunc)());
						LoadableModule module(moduleType, fileName, pHandle);

						// Can it search ?
						getSearchEngineFunc *pSearchFunc = (getSearchEngineFunc *)dlsym(pHandle,
							GETSEARCHENGINEFUNC);
						if (pSearchFunc != NULL)
						{
							module.m_canSearch = true;
						}

						// Can it index ?
						getIndexFunc *pIndexFunc = (getIndexFunc *)dlsym(pHandle,
							GETINDEXFUNC);
						if (pIndexFunc != NULL)
						{
							module.m_canIndex = true;
						}

						// Add a record for this module
						m_types.insert(pair<string, LoadableModule>(moduleType, module));
#ifdef DEBUG
						cout << "ModuleFactory::loadModules: type " << moduleType
							<< " is supported by " << pEntryName << endl;
#endif
					}
					else cerr << "ModuleFactory::loadModules: " << dlerror() << endl;
				}
				else cerr << "ModuleFactory::loadModules: " << dlerror() << endl;
			}
#ifdef DEBUG
			else cout << "ModuleFactory::loadModules: "
				<< pEntryName << " is not a file" << endl;
#endif
		}

		// Next entry
		pDirEntry = readdir(pDir);
	}
	closedir(pDir);

	return count;
}

bool ModuleFactory::openOrCreateIndex(const string &type, const string &option,
	bool &obsoleteFormat, bool readOnly, bool overwrite)
{
	map<string, LoadableModule>::iterator typeIter = m_types.find(type);
	if ((typeIter == m_types.end()) ||
		(typeIter->second.m_canIndex == false))
	{
		// We don't know about this type, or doesn't support indexes
		return false;
	}

	void *pHandle = typeIter->second.m_pHandle;
	if (pHandle == NULL)
	{
		return false;
	}

	openOrCreateIndexFunc *pFunc = (openOrCreateIndexFunc *)dlsym(pHandle,
		OPENORCREATEINDEXFUNC);
	if (pFunc != NULL)
	{
		return (*pFunc)(option, obsoleteFormat, readOnly, overwrite);
	}
#ifdef DEBUG
	cout << "ModuleFactory::openOrCreateIndex: couldn't find export openOrCreateIndex" << endl;
#endif

	return false;
}

bool ModuleFactory::mergeIndexes(const string &type, const string &option0,
	const string &option1, const string &option2)
{
	map<string, LoadableModule>::iterator typeIter = m_types.find(type);
	if ((typeIter == m_types.end()) ||
		(typeIter->second.m_canIndex == false))
	{
		// We don't know about this type, or doesn't support indexes
		return false;
	}

	void *pHandle = typeIter->second.m_pHandle;
	if (pHandle == NULL)
	{
		return false;
	}

	mergeIndexesFunc *pFunc = (mergeIndexesFunc *)dlsym(pHandle,
		MERGEINDEXESFUNC);
	if (pFunc != NULL)
	{
		return (*pFunc)(option0, option1, option2);
	}
#ifdef DEBUG
	cout << "ModuleFactory::mergeIndexes: couldn't find export mergeIndexes" << endl;
#endif

	return false;
}

IndexInterface *ModuleFactory::getIndex(const string &type, const string &option)
{
	IndexInterface *pIndex = NULL;

	// Choice by type
	// Do we need to nest it in a DBusIndex ?
	if (type.substr(0, 5) == "dbus-")
	{
#ifdef DEBUG
		cout << "ModuleFactory::mergeIndexes: sub-type " << type.substr(5) << endl;
#endif
		pIndex = getLibraryIndex(type.substr(5), option);
		if (pIndex != NULL)
		{
			return new DBusIndex(pIndex);
		}

		return NULL;
	}

	return getLibraryIndex(type, option);
}

SearchEngineInterface *ModuleFactory::getSearchEngine(const string &type, const string &option)
{
	SearchEngineInterface *pEngine = NULL;

	// Choice by type
	if (
#ifdef HAVE_BOOST_SPIRIT
		(type == "sherlock") ||
#endif
		(type == "opensearch"))
	{
		pEngine = new PluginWebEngine(option);
	}
#ifdef HAVE_GOOGLEAPI
	else if (type == "googleapi")
	{
		pEngine = new GoogleAPIEngine(option);
	}
#endif

	if (pEngine != NULL)
	{
		return pEngine;
	}

	return getLibrarySearchEngine(type, option);
}

string ModuleFactory::getSearchEngineName(const string &type, const string &option)
{
	if (
#ifdef HAVE_BOOST_SPIRIT
		(type == "sherlock") ||
#endif
		(type == "opensearch"))
	{
		SearchPluginProperties properties;

		if (PluginWebEngine::getDetails(option, properties) == true)
		{
			return properties.m_name;
		}

		return "";
	}
	else
	{
		return option;
	}

	return type;
}

void ModuleFactory::getSupportedEngines(map<string, bool> &engines)
{
	engines.clear();

	// Built-in engines
#ifdef HAVE_BOOST_SPIRIT
	engines["sherlock"] = false;
#endif
	engines["opensearch"] = false;
#ifdef HAVE_GOOGLEAPI
	engines["googleapi"] = false;
#endif
	// Library-handled engines
	for (map<string, LoadableModule>::iterator typeIter = m_types.begin();
		typeIter != m_types.end(); ++typeIter)
	{
		engines[typeIter->first] = true;
	}
}

bool ModuleFactory::isSupported(const string &type, bool asIndex)
{
	if (asIndex == true)
	{
		// Only backends implement index functionality
		map<string, LoadableModule>::const_iterator typeIter = m_types.find(type);
		if (typeIter != m_types.end())
		{
			return typeIter->second.m_canIndex;
		}

		return false;
	}

	if (
#ifdef HAVE_GOOGLEAPI
		(type == "googleapi") ||
#endif
#ifdef HAVE_BOOST_SPIRIT
		(type == "sherlock") ||
#endif
		(type == "opensearch"))
	{
		return true;
	}
	else
	{
		// Does this backend implement search functionality ?
		map<string, LoadableModule>::const_iterator typeIter = m_types.find(type);
		if (typeIter != m_types.end())
		{
			return typeIter->second.m_canSearch;
		}
	}

	return false;	
}

void ModuleFactory::unloadModules(void)
{
	for (map<string, LoadableModule>::iterator typeIter = m_types.begin(); typeIter != m_types.end(); ++typeIter)
	{
		void *pHandle = typeIter->second.m_pHandle;
		if (pHandle == NULL)
		{
			continue;
		}

		closeAllFunc *pFunc = (closeAllFunc *)dlsym(pHandle, CLOSEALLFUNC);
		if (pFunc != NULL)
		{
			(*pFunc)();
		}
#ifdef DEBUG
		else cout << "ModuleFactory::unloadModules: couldn't find export closeAll" << endl;
#endif

		if (dlclose(pHandle) != 0)
		{
#ifdef DEBUG
			cout << "ModuleFactory::unloadModules: failed on " << typeIter->first << endl;
#endif
		}
	}

	m_types.clear();
}


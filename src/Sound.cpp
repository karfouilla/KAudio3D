/**
 *
 * @file MultiSource.cpp
 * @author karfouilla
 * @version 1.0
 * @date 30 avril 2019
 * @brief Fichier contenant la classe de source sonore multiple (CPP)
 *
 */
////////////////////////////////////////////////////////////////////////////////
//
// This file is part of KAudio3D.
// KAudio3D is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// KAudio3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with KAudio3D.  If not, see <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

#include "Sound.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

#include <fstream>
#include <sstream>
#include <string>

namespace KA3D
{

Sound::Sound(uint32_t instanceMax):
	m_tblSources(new Source[instanceMax]),
	m_pData(nullptr),
	m_pConfig(nullptr),
	m_uInstanceMax(instanceMax),
	m_uCurrent(0),
	m_removeData(false)
{ }

Sound::Sound(SourceConfigure* pConfig, uint32_t instanceMax):
	m_tblSources(new Source[instanceMax]),
	m_pData(nullptr),
	m_pConfig(pConfig),
	m_uInstanceMax(instanceMax),
	m_uCurrent(0),
	m_removeData(false)
{ }

Sound::~Sound() noexcept
{
	delete[] m_tblSources;
}

void Sound::setData(Data* pData, bool removeData) noexcept
{
	m_pData = pData;
	m_removeData = removeData;
}

void Sound::setWav(std::iostream& file)
{
	m_pData = Data::fromWav(file);
	m_removeData = true;
}

void Sound::setConfig(SourceConfigure* pConfig)
{
	m_pConfig = pConfig;
}

void Sound::Init(bool forceLoad)
{
	assert(m_pData);
	loadSource(0);
	if(forceLoad)
	{
		for(uint32_t i=1; i<m_uInstanceMax; ++i)
		{
			loadSource(i);
		}
	}
}

void Sound::loadSource(SoundInstance instance)
{
	m_tblSources[instance].Init(m_pData);
	if(m_pConfig)
		(*m_pConfig)(m_tblSources);
}

void Sound::Quit()
{
	for(uint32_t i=0; i<m_uInstanceMax; ++i)
	{
		if(m_tblSources[i].isInitialized())
			m_tblSources[i].Quit();
	}
	if(m_removeData)
		delete m_pData;
}

void Sound::play()
{
	if(!m_tblSources[m_uCurrent].isInitialized())
	{
		loadSource(m_uCurrent);
	}
	m_tblSources[m_uCurrent].play();
	++m_uCurrent;
	m_uCurrent %= m_uInstanceMax;
}

Sound* Sound::fromWav(const std::string& filename)
{
	Sound* sound(nullptr);
	std::fstream file;
	file.open(filename, std::ios_base::in);
	if(!file.good())
	{
		std::ostringstream msg;
		msg << "Unable to open wav file '" << filename << "': "
		    << std::strerror(errno);
		throw std::runtime_error(msg.str());
	}
	try
	{
		sound = new Sound;
		sound->setWav(file);
		file.close();
	}
	catch(...)
	{
		file.close();
		delete sound;
		throw;
	}
	return sound;
}

} // namespace KA3D

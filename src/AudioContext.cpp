/**
 *
 * @file AudioContext.cpp
 * @author karfouilla
 * @version 1.0
 * @date 27 avril 2019
 * @brief Fichier contenant le contexte audio pour #AudioListener (CPP)
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

#include "AudioContext.h"

#include <cstring>
#include <sstream>

#include "Error.h"


AudioContext::AudioContext(const char* deviceName):
	m_pDevices(nullptr),
	m_pContext(nullptr),
	m_szDeviceName(nullptr)
{
	if(deviceName)
	{
		m_szDeviceName = new ALCchar[std::strlen(deviceName)+1];
		std::strcpy(m_szDeviceName, deviceName);
	}
}
AudioContext::~AudioContext() noexcept
{
	if(m_szDeviceName)
		delete[] m_szDeviceName;
}

void AudioContext::Init()
{
	try
	{
		m_pDevices = alcOpenDevice(m_szDeviceName);
		if(m_pDevices == nullptr)
		{
			std::ostringstream msg;
			msg << "Unable to open device '" << m_szDeviceName << "'";
			throw std::runtime_error(msg.str());
		}

		m_pContext = alcCreateContext(m_pDevices, nullptr);
		if(m_pContext == nullptr)
			checkALCError(device());

		if(alcMakeContextCurrent(m_pContext) != ALC_TRUE)
			checkALCError(device());
	}
	catch(...)
	{
		if(m_pContext)
		{
			alcMakeContextCurrent(nullptr);
			alcDestroyContext(m_pContext);
		}
		if(m_pDevices)
			alcCloseDevice(m_pDevices);

		throw;
	}
}

void AudioContext::Quit()
{
	if(alcMakeContextCurrent(nullptr) != ALC_TRUE)
		checkALCError(device());

	alcDestroyContext(m_pContext);
	checkALCError(device());

	alcCloseDevice(m_pDevices);
	checkALCError(device());
}

ALCdevice* AudioContext::device() const noexcept
{
	return m_pDevices;
}
ALCcontext* AudioContext::context() const noexcept
{
	return m_pContext;
}

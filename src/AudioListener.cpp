/**
 *
 * @file AudioListener.cpp
 * @author karfouilla
 * @version 1.0
 * @date 27 avril 2019
 * @brief Contient la gestion de l'audio, représentant celui qui écoute (CPP)
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

#include "AudioListener.h"

#include <cstring>
#include <list>
#include <sstream>
#include <string>
#include <stdexcept>

#include <AL/al.h>
#include <AL/alc.h>

#include "AudioContext.h"
#include "Error.h"

namespace KA3D
{

struct
{
	const char* name;
	ALenum alValue;
} tblDistanceModel[] = {
	"DM_NONE", AL_NONE,
	"DM_INVERSE", AL_INVERSE_DISTANCE,
	"DM_INVERSE_CLAMPED", AL_INVERSE_DISTANCE_CLAMPED,
	"DM_LINEAR", AL_LINEAR_DISTANCE,
	"DM_LINEAR_CLAMPED", AL_LINEAR_DISTANCE_CLAMPED,
	"DM_EXPONENT", AL_EXPONENT_DISTANCE,
	"DM_EXPONENT_CLAMPED", AL_EXPONENT_DISTANCE_CLAMPED,

	"DM_LAST", AL_INVALID
};

ALint tblContextAttributes[] = {
	ALC_FREQUENCY,
	ALC_REFRESH,
	ALC_SYNC,
	ALC_MONO_SOURCES,
	ALC_STEREO_SOURCES
};

const int CONTEXT_ATTRIBUTES_COUNT = 5;

AudioListener* AudioListener::pCurrent(nullptr);

AudioListener::AudioListener(const char* deviceName):
	m_pData(new AudioContext(deviceName)),
	m_tblAttrib(nullptr)
{ }

AudioListener::~AudioListener() noexcept
{
	if(m_tblAttrib)
	{
		for(int i=0; i<CONTEXT_ATTRIBUTES_COUNT; ++i)
			delete m_tblAttrib[i];
	}
	delete[] m_tblAttrib;
	delete m_pData;
}

void AudioListener::Init()
{
	try
	{
		if(m_tblAttrib)
		{
			// Comptage des attributs
			int nbAttr(0);
			for(int i=0; i<CONTEXT_ATTRIBUTES_COUNT; ++i)
				if(m_tblAttrib[i])
					++nbAttr;

			// Allocation de la nouvelle structure (clé+valeur)
			int* attrib = new int[2*nbAttr];

			// Copie dans la nouvelle structure
			int num(0);
			for(int i=0; i<CONTEXT_ATTRIBUTES_COUNT; ++i)
			{
				if(m_tblAttrib[i])
				{
					attrib[num++] = tblContextAttributes[i];
					attrib[num++] = *m_tblAttrib[i];
				}
			}

			m_pData->Init(attrib);

			// Suppression des deux structures
			delete[] attrib;
			for(int i=0; i<CONTEXT_ATTRIBUTES_COUNT; ++i)
				delete m_tblAttrib[i];
			delete[] m_tblAttrib;
		}
		else
		{
			m_pData->Init(nullptr);
		}
		m_pData->makeCurrent();
		pCurrent = this;
	}
	catch(std::exception& e)
	{
		std::ostringstream msg;
		msg << "Unable to initialize audio listener: " << e.what();
		throw std::runtime_error(msg.str());
	}
}

void AudioListener::Quit()
{
	try
	{
		if(this == pCurrent)
			m_pData->clearCurrent();
		m_pData->Quit();
	}
	catch(std::exception& e)
	{
		std::ostringstream msg;
		msg << "Unable to release audio listener: " << e.what();
		throw std::runtime_error(msg.str());
	}
}

std::list<std::string> AudioListener::devices()
{
	std::list<std::string> lstDevices;
	const ALCchar* str = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	while(*str)
	{
		std::string dev(str);
		str += dev.size();

		lstDevices.push_back(std::move(dev));

		++str;
	}
	return lstDevices;
}

std::string AudioListener::defaultDevice()
{
	return std::string(alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
}

std::string AudioListener::device() const
{
	return std::string(alcGetString(m_pData->device(), ALC_DEVICE_SPECIFIER));
}

void AudioListener::setFrequency(int iFrequency)
{
	if(!m_tblAttrib)
		m_tblAttrib = new int*[CONTEXT_ATTRIBUTES_COUNT];
	if(!m_tblAttrib[0])
		m_tblAttrib[0] = new int(iFrequency);
	else
		*m_tblAttrib[0] = iFrequency;
}

void AudioListener::setRefresh(int iRefresh)
{
	if(!m_tblAttrib)
		m_tblAttrib = new int*[CONTEXT_ATTRIBUTES_COUNT];
	if(!m_tblAttrib[1])
		m_tblAttrib[1] = new int(iRefresh);
	else
		*m_tblAttrib[1] = iRefresh;
}

void AudioListener::setSync(bool isSync)
{
	if(!m_tblAttrib)
		m_tblAttrib = new int*[CONTEXT_ATTRIBUTES_COUNT];
	if(!m_tblAttrib[2])
		m_tblAttrib[2] = new int(isSync ? AL_TRUE : AL_FALSE);
	else
		*m_tblAttrib[2] = (isSync ? AL_TRUE : AL_FALSE);
}

void AudioListener::setMonoSource(int iMonoSource)
{
	if(!m_tblAttrib)
		m_tblAttrib = new int*[CONTEXT_ATTRIBUTES_COUNT];
	if(!m_tblAttrib[3])
		m_tblAttrib[3] = new int(iMonoSource);
	else
		*m_tblAttrib[3] = iMonoSource;
}

void AudioListener::setStereoSource(int iStereoSource)
{
	if(!m_tblAttrib)
		m_tblAttrib = new int*[CONTEXT_ATTRIBUTES_COUNT];
	if(!m_tblAttrib[4])
		m_tblAttrib[4] = new int(iStereoSource);
	else
		*m_tblAttrib[4] = iStereoSource;
}

void AudioListener::makeCurrent(bool enable)
{
	if(enable)
		m_pData->makeCurrent();
	else
		if(isCurrent())
			m_pData->clearCurrent();
}

void AudioListener::clearCurrent()
{
	AudioContext::clearCurrent();
}

void AudioListener::suspend()
{
	m_pData->suspend();
}

void AudioListener::process()
{
	m_pData->process();
}

bool AudioListener::isCurrent() const noexcept
{
	return (pCurrent == this);
}

AudioListener* AudioListener::current() noexcept
{
	return pCurrent;
}

void AudioListener::setGain(float gain)
{
	alListenerf(AL_GAIN, gain);
	checkALError();
}

void AudioListener::setPosition(float xpos, float ypos, float zpos)
{
	alListener3f(AL_POSITION, xpos, ypos, zpos);
	checkALError();
}

void AudioListener::setVelocity(float xvel, float yvel, float zvel)
{
	alListener3f(AL_VELOCITY, xvel, yvel, zvel);
	checkALError();
}

void AudioListener::setOrientation(float xat, float yat, float zat,
	                               float xup, float yup, float zup)
{
	ALfloat orientation[] = {xat, yat, zat, xup, yup, zup};
	alListenerfv(AL_ORIENTATION, orientation);
	checkALError();
}

void AudioListener::setDopplerFactor(float factor)
{
	alDopplerFactor(factor);
	checkALError();
}

void AudioListener::setSpeedSound(float fSpeedSound)
{
	alSpeedOfSound(fSpeedSound);
	checkALError();
}

void AudioListener::setDistanceModel(DistanceModel model)
{
	alDistanceModel(tblDistanceModel[model].alValue);
	checkALError();
}

float AudioListener::gain() const
{
	float val;
	alGetListenerf(AL_GAIN, &val);
	checkALError();
	return val;
}

void AudioListener::position(float& xpos, float& ypos, float& zpos) const
{
	alGetListener3f(AL_POSITION, &xpos, &ypos, &zpos);
	checkALError();
}

void AudioListener::velocity(float& xvel, float& yvel, float& zvel) const
{
	alGetListener3f(AL_VELOCITY, &xvel, &yvel, &zvel);
	checkALError();
}

void AudioListener::orientation(float& xat, float& yat, float& zat,
                                float& xup, float& yup, float& zup) const
{
	float vals[6];
	alGetListenerfv(AL_ORIENTATION, vals);
	checkALError();
	xat = vals[0];
	yat = vals[1];
	zat = vals[2];
	xup = vals[3];
	yup = vals[4];
	zup = vals[5];
}

float AudioListener::dopplerFactor() const
{
	float val = alGetFloat(AL_DOPPLER_FACTOR);
	checkALError();
	return val;
}

float AudioListener::speedSound() const
{
	float val = alGetFloat(AL_SPEED_OF_SOUND);
	checkALError();
	return val;
}

DistanceModel AudioListener::distanceModel() const
{
	ALenum model = alGetInteger(AL_DISTANCE_MODEL);
	checkALError();
	switch(model)
	{
	case AL_NONE:
		return DM_NONE;
	case AL_INVERSE_DISTANCE:
		return DM_INVERSE;
	case AL_INVERSE_DISTANCE_CLAMPED:
		return DM_INVERSE_CLAMPED;
	case AL_LINEAR_DISTANCE:
		return DM_LINEAR;
	case AL_LINEAR_DISTANCE_CLAMPED:
		return DM_LINEAR_CLAMPED;
	case AL_EXPONENT_DISTANCE:
		return DM_EXPONENT;
	case AL_EXPONENT_DISTANCE_CLAMPED:
		return DM_EXPONENT_CLAMPED;
	default:
		return DM_LAST;
	}
}

} // namespace KA3D

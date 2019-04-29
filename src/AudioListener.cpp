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
#include <sstream>

#include <AL/al.h>
#include <AL/alc.h>

#include "AudioContext.h"
#include "Error.h"

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

AudioListener::AudioListener(const char* deviceName):
	m_pData(new AudioContext(deviceName))
{ }

AudioListener::~AudioListener() noexcept
{
	delete m_pData;
}

void AudioListener::Init()
{
	try
	{
		m_pData->Init();
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
		m_pData->Quit();
	}
	catch(std::exception& e)
	{
		std::ostringstream msg;
		msg << "Unable to release audio listener: " << e.what();
		throw std::runtime_error(msg.str());
	}
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

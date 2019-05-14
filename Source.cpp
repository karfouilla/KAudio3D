/**
 *
 * @file Source.cpp
 * @author karfouilla
 * @version 1.0Q
 * @date 27 avril 2019
 * @brief Fichier contenant la classe de source sonore (CPP)
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

#include "KA3D/Source.h"

#include <cstdint>
#include <sstream>
#include <stdexcept>

#include <AL/al.h>

#include "DataPrivate.h"
#include "Error.h"

namespace KA3D
{

Source::Source():
	m_pData(nullptr),
	m_pSource(new DataPrivate)
{ }
Source::~Source() noexcept
{
	delete m_pSource;
}

void Source::Init(Data* pData)
{
	try
	{
		m_pData = pData;
		alGenSources(1, &m_pSource->handle);
		checkALError();
		alSourcei(m_pSource->handle, AL_BUFFER, m_pData->data()->handle);
		checkALError();
	}
	catch(std::exception& e)
	{
		std::ostringstream msg;
		msg << "Unable to initialize audio source: " << e.what();
		throw std::runtime_error(msg.str());
	}
}

void Source::Quit()
{
	try
	{
		alDeleteSources(1, &m_pSource->handle);
		checkALError();
		m_pData = nullptr;
	}
	catch(std::exception& e)
	{
		std::ostringstream msg;
		msg << "Unable to quit audio source: " << e.what();
		throw std::runtime_error(msg.str());
	}
}

bool Source::isInitialized() const noexcept
{
	return m_pData;
}

void Source::play()
{
	alSourcePlay(m_pSource->handle);
	checkALError();
}

void Source::pause()
{
	alSourcePause(m_pSource->handle);
	checkALError();
}

void Source::stop()
{
	alSourceStop(m_pSource->handle);
	checkALError();
}

void Source::rewind()
{
	alSourceRewind(m_pSource->handle);
	checkALError();
}

void Source::setPosition(float xpos, float ypos, float zpos)
{
	alSource3f(m_pSource->handle, AL_POSITION, xpos, ypos, zpos);
	checkALError();
}

void Source::setVelocity(float xvel, float yvel, float zvel)
{
	alSource3f(m_pSource->handle, AL_VELOCITY, xvel, yvel, zvel);
	checkALError();
}

void Source::setDirection(float xat, float yat, float zat)
{
	alSource3f(m_pSource->handle, AL_DIRECTION, xat, yat, zat);
	checkALError();
}

void Source::setPitch(float factor)
{
	alSourcef(m_pSource->handle, AL_PITCH, factor);
	checkALError();
}

void Source::setGain(float fGain)
{
	alSourcef(m_pSource->handle, AL_GAIN, fGain);
	checkALError();
}

void Source::setMaxDistance(float fMaxDistance)
{
	alSourcef(m_pSource->handle, AL_MAX_DISTANCE, fMaxDistance);
	checkALError();
}

void Source::setRollOffFactor(float fRollOff)
{
	alSourcef(m_pSource->handle, AL_ROLLOFF_FACTOR, fRollOff);
	checkALError();
}

void Source::setReferenceDistance(float fRefDistance)
{
	alSourcef(m_pSource->handle, AL_REFERENCE_DISTANCE, fRefDistance);
	checkALError();
}

void Source::setMinGain(float fMinGain)
{
	alSourcef(m_pSource->handle, AL_MIN_GAIN, fMinGain);
	checkALError();
}

void Source::setMaxGain(float fMaxGain)
{
	alSourcef(m_pSource->handle, AL_MAX_GAIN, fMaxGain);
	checkALError();
}

void Source::setConeOuterGain(float fConeOuterGain)
{
	alSourcef(m_pSource->handle, AL_CONE_OUTER_GAIN, fConeOuterGain);
	checkALError();
}

void Source::setConeInnerAngle(float fConeInnerAngle)
{
	alSourcef(m_pSource->handle, AL_CONE_INNER_ANGLE, fConeInnerAngle);
	checkALError();
}

void Source::setConeOuterAngle(float fConeOuterAngle)
{
	alSourcef(m_pSource->handle, AL_CONE_OUTER_ANGLE, fConeOuterAngle);
	checkALError();
}

void Source::setRelative(bool isRelative)
{
	ALint val(isRelative ? AL_TRUE : AL_FALSE);
	alSourcei(m_pSource->handle, AL_SOURCE_RELATIVE, val);
	checkALError();
}

void Source::setOffsetSec(float second)
{
	alSourcef(m_pSource->handle, AL_SEC_OFFSET, second);
	checkALError();
}

void Source::setOffset(quint32 sample)
{
	alSourcei(m_pSource->handle, AL_SAMPLE_OFFSET, static_cast<ALint>(sample));
	checkALError();
}

void Source::setAutoLoop(bool isLooping)
{
	ALint val(isLooping ? AL_TRUE : AL_FALSE);
	alSourcei(m_pSource->handle, AL_LOOPING, val);
	checkALError();
}

void Source::position(float& xpos, float& ypos, float& zpos) const
{
	alGetSource3f(m_pSource->handle, AL_POSITION, &xpos, &ypos, &zpos);
	checkALError();
}

void Source::velocity(float& xvel, float& yvel, float& zvel) const
{
	alGetSource3f(m_pSource->handle, AL_VELOCITY, &xvel, &yvel, &zvel);
	checkALError();
}

void Source::direction(float& xat, float& yat, float& zat) const
{
	alGetSource3f(m_pSource->handle, AL_DIRECTION, &xat, &yat, &zat);
	checkALError();
}

float Source::pitch() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_PITCH, &val);
	checkALError();
	return val;
}

float Source::gain() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_GAIN, &val);
	checkALError();
	return val;
}

float Source::maxDistance() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_MAX_DISTANCE, &val);
	checkALError();
	return val;
}

float Source::rollOffFactor() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_ROLLOFF_FACTOR, &val);
	checkALError();
	return val;
}

float Source::referenceDistance() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_REFERENCE_DISTANCE, &val);
	checkALError();
	return val;
}

float Source::minGain() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_MIN_GAIN, &val);
	checkALError();
	return val;
}

float Source::maxGain() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_MAX_GAIN, &val);
	checkALError();
	return val;
}

float Source::coneOuterGain() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_CONE_OUTER_GAIN, &val);
	checkALError();
	return val;
}

float Source::coneInnerAngle() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_CONE_INNER_ANGLE, &val);
	checkALError();
	return val;
}

float Source::coneOuterAngle() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_CONE_OUTER_ANGLE, &val);
	checkALError();
	return val;
}

bool Source::isRelative() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_RELATIVE, &val);
	checkALError();
	return (val == AL_TRUE);
}

float Source::offsetSec() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_SEC_OFFSET, &val);
	checkALError();
	return val;
}

quint32 Source::offset() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SAMPLE_OFFSET, &val);
	checkALError();
	return static_cast<quint32>(val);
}

bool Source::isLooping() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_LOOPING, &val);
	checkALError();
	return (val == AL_TRUE);
}

bool Source::isPlaying() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_STATE, &val);
	checkALError();
	return (val == AL_PLAYING);
}

bool Source::isPaused() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_STATE, &val);
	checkALError();
	return (val == AL_PAUSED);
}

bool Source::isStopped() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_STATE, &val);
	checkALError();
	return (val == AL_STOPPED);
}

bool Source::isInitial() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_STATE, &val);
	checkALError();
	return (val == AL_INITIAL);
}

} // namespace KA3D

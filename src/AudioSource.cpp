/**
 *
 * @file AudioSource.cpp
 * @author karfouilla
 * @version 1.0
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

#include "AudioSource.h"

#include <cstdint>
#include <sstream>
#include <stdexcept>

#include <AL/al.h>

#include "AudioDataPrivate.h"
#include "Error.h"

namespace KA3D
{

AudioSource::AudioSource():
	m_pData(nullptr),
	m_pSource(new AudioDataPrivate)
{ }
AudioSource::~AudioSource() noexcept
{
	delete m_pSource;
}

void AudioSource::Init(AudioData* pData)
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

void AudioSource::Quit()
{
	try
	{
		alDeleteSources(1, &m_pSource->handle);
		checkALError();
	}
	catch(std::exception& e)
	{
		std::ostringstream msg;
		msg << "Unable to quit audio source: " << e.what();
		throw std::runtime_error(msg.str());
	}
}

void AudioSource::play()
{
	alSourcePlay(m_pSource->handle);
	checkALError();
}

void AudioSource::pause()
{
	alSourcePause(m_pSource->handle);
	checkALError();
}

void AudioSource::stop()
{
	alSourceStop(m_pSource->handle);
	checkALError();
}

void AudioSource::rewind()
{
	alSourceRewind(m_pSource->handle);
	checkALError();
}

void AudioSource::setPosition(float xpos, float ypos, float zpos)
{
	alSource3f(m_pSource->handle, AL_POSITION, xpos, ypos, zpos);
	checkALError();
}

void AudioSource::setVelocity(float xvel, float yvel, float zvel)
{
	alSource3f(m_pSource->handle, AL_VELOCITY, xvel, yvel, zvel);
	checkALError();
}

void AudioSource::setDirection(float xat, float yat, float zat)
{
	alSource3f(m_pSource->handle, AL_DIRECTION, xat, yat, zat);
	checkALError();
}

void AudioSource::setPitch(float factor)
{
	alSourcef(m_pSource->handle, AL_PITCH, factor);
	checkALError();
}

void AudioSource::setGain(float fGain)
{
	alSourcef(m_pSource->handle, AL_GAIN, fGain);
	checkALError();
}

void AudioSource::setMaxDistance(float fMaxDistance)
{
	alSourcef(m_pSource->handle, AL_MAX_DISTANCE, fMaxDistance);
	checkALError();
}

void AudioSource::setRollOffFactor(float fRollOff)
{
	alSourcef(m_pSource->handle, AL_ROLLOFF_FACTOR, fRollOff);
	checkALError();
}

void AudioSource::setReferenceDistance(float fRefDistance)
{
	alSourcef(m_pSource->handle, AL_REFERENCE_DISTANCE, fRefDistance);
	checkALError();
}

void AudioSource::setMinGain(float fMinGain)
{
	alSourcef(m_pSource->handle, AL_MIN_GAIN, fMinGain);
	checkALError();
}

void AudioSource::setMaxGain(float fMaxGain)
{
	alSourcef(m_pSource->handle, AL_MAX_GAIN, fMaxGain);
	checkALError();
}

void AudioSource::setConeOuterGain(float fConeOuterGain)
{
	alSourcef(m_pSource->handle, AL_CONE_OUTER_GAIN, fConeOuterGain);
	checkALError();
}

void AudioSource::setConeInnerAngle(float fConeInnerAngle)
{
	alSourcef(m_pSource->handle, AL_CONE_INNER_ANGLE, fConeInnerAngle);
	checkALError();
}

void AudioSource::setConeOuterAngle(float fConeOuterAngle)
{
	alSourcef(m_pSource->handle, AL_CONE_OUTER_ANGLE, fConeOuterAngle);
	checkALError();
}

void AudioSource::setRelative(bool isRelative)
{
	ALint val(isRelative ? AL_TRUE : AL_FALSE);
	alSourcei(m_pSource->handle, AL_SOURCE_RELATIVE, val);
	checkALError();
}

void AudioSource::setOffsetSec(float second)
{
	alSourcef(m_pSource->handle, AL_SEC_OFFSET, second);
	checkALError();
}

void AudioSource::setOffset(std::uint32_t sample)
{
	alSourcei(m_pSource->handle, AL_SAMPLE_OFFSET, static_cast<ALint>(sample));
	checkALError();
}

void AudioSource::setAutoLoop(bool isLooping)
{
	ALint val(isLooping ? AL_TRUE : AL_FALSE);
	alSourcei(m_pSource->handle, AL_LOOPING, val);
	checkALError();
}

void AudioSource::position(float& xpos, float& ypos, float& zpos) const
{
	alGetSource3f(m_pSource->handle, AL_POSITION, &xpos, &ypos, &zpos);
	checkALError();
}

void AudioSource::velocity(float& xvel, float& yvel, float& zvel) const
{
	alGetSource3f(m_pSource->handle, AL_VELOCITY, &xvel, &yvel, &zvel);
	checkALError();
}

void AudioSource::direction(float& xat, float& yat, float& zat) const
{
	alGetSource3f(m_pSource->handle, AL_DIRECTION, &xat, &yat, &zat);
	checkALError();
}

float AudioSource::pitch() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_PITCH, &val);
	checkALError();
	return val;
}

float AudioSource::gain() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_GAIN, &val);
	checkALError();
	return val;
}

float AudioSource::maxDistance() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_MAX_DISTANCE, &val);
	checkALError();
	return val;
}

float AudioSource::rollOffFactor() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_ROLLOFF_FACTOR, &val);
	checkALError();
	return val;
}

float AudioSource::referenceDistance() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_REFERENCE_DISTANCE, &val);
	checkALError();
	return val;
}

float AudioSource::minGain() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_MIN_GAIN, &val);
	checkALError();
	return val;
}

float AudioSource::maxGain() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_MAX_GAIN, &val);
	checkALError();
	return val;
}

float AudioSource::coneOuterGain() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_CONE_OUTER_GAIN, &val);
	checkALError();
	return val;
}

float AudioSource::coneInnerAngle() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_CONE_INNER_ANGLE, &val);
	checkALError();
	return val;
}

float AudioSource::coneOuterAngle() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_CONE_OUTER_ANGLE, &val);
	checkALError();
	return val;
}

bool AudioSource::isRelative() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_RELATIVE, &val);
	checkALError();
	return (val == AL_TRUE);
}

float AudioSource::offsetSec() const
{
	float val;
	alGetSourcef(m_pSource->handle, AL_SEC_OFFSET, &val);
	checkALError();
	return val;
}

std::uint32_t AudioSource::offset() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SAMPLE_OFFSET, &val);
	checkALError();
	return static_cast<std::uint32_t>(val);
}

bool AudioSource::isLooping() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_LOOPING, &val);
	checkALError();
	return (val == AL_TRUE);
}

bool AudioSource::isPlaying() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_STATE, &val);
	checkALError();
	return (val == AL_PLAYING);
}

bool AudioSource::isPaused() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_STATE, &val);
	checkALError();
	return (val == AL_PAUSED);
}

bool AudioSource::isStopped() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_STATE, &val);
	checkALError();
	return (val == AL_STOPPED);
}

bool AudioSource::isInitial() const
{
	ALint val;
	alGetSourcei(m_pSource->handle, AL_SOURCE_STATE, &val);
	checkALError();
	return (val == AL_INITIAL);
}

} // namespace KA3D

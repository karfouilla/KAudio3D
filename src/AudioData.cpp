/**
 *
 * @file AudioData.cpp
 * @author karfouilla
 * @version 1.0
 * @date 27 avril 2019
 * @brief Fichier contenant la classe de gestion des données audio (CPP)
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

#include "AudioData.h"


#include <AL/al.h>
#include <AL/alc.h>

#include <cassert>

#include <sstream>

#include "AudioDataPrivate.h"
#include "Error.h"
#include "WaveFile.h"


static const struct
{
	const char* name;
	uint16_t channels, bytesPerSample;
	ALenum format;
} tblAudioFormat[] = {
	"ADF_MONO8", 1, 1, AL_FORMAT_MONO8,
	"ADF_MONO16", 1, 2, AL_FORMAT_MONO16,
	"ADF_STEREO8", 2, 1, AL_FORMAT_STEREO8,
	"ADF_STEREO16", 2, 2, AL_FORMAT_STEREO16,

	"ADF_LAST", 0, 0, 0
};

static inline ALenum audioDataFormatConvert(AudioDataFormat format);


AudioData* AudioData::fromData(const std::vector<uint8_t>& tblData,
                               AudioDataFormat format, int32_t freq)
{
	AudioDataPrivate* privateData(new AudioDataPrivate);
	try
	{
		alGenBuffers(1, &privateData->handle);
		checkALError();
		alBufferData(privateData->handle, audioDataFormatConvert(format),
		             tblData.data(), tblData.size(), freq);
		checkALError();
	}
	catch(std::runtime_error& e)
	{
		if(privateData->handle)
			alDeleteBuffers(1, &privateData->handle);
		delete privateData;

		std::ostringstream msg;
		msg << "Unable to create audio buffer data: " << e.what();
		throw std::runtime_error(msg.str());
	}
	return new AudioData(privateData);
}

AudioData* AudioData::fromWav(std::iostream& file)
{
	std::vector<uint8_t> tblData;
	AudioDataFormat format;
	uint32_t freq;

	WaveFile waveFile(file);
	waveFile.open(std::ios_base::in);

	format = waveFile.format();
	freq = waveFile.samplesPerSec();
	tblData.resize(waveFile.size());

	waveFile.read(tblData.data(), waveFile.size());

	waveFile.close();

	return fromData(tblData, format, freq);
}

AudioData::AudioData(AudioDataPrivate* pData) noexcept:
	m_pData(pData)
{ }

AudioData::~AudioData() noexcept
{
	alDeleteBuffers(1, &m_pData->handle);
	delete m_pData;
}
AudioDataPrivate* AudioData::data() noexcept
{
	return m_pData;
}

const char* AudioData::formatName(AudioDataFormat format) noexcept
{
	assert(format < ADF_LAST);
	return tblAudioFormat[format].name;
}

uint16_t AudioData::formatChannels(AudioDataFormat format) noexcept
{
	assert(format < ADF_LAST);
	return tblAudioFormat[format].channels;
}

uint16_t AudioData::formatBytesPerSample(AudioDataFormat format) noexcept
{
	assert(format < ADF_LAST);
	return tblAudioFormat[format].bytesPerSample;
}

uint16_t AudioData::formatPitch(AudioDataFormat format) noexcept
{
	return formatChannels(format) * formatBytesPerSample(format);
}
AudioDataFormat AudioData::formatFromPerSample(uint16_t channels,
                                               uint16_t bytesPerSample) noexcept
{
	if(channels == 1 && bytesPerSample == 1)
		return ADF_MONO8;
	if(channels == 1 && bytesPerSample == 2)
		return ADF_MONO16;
	if(channels == 2 && bytesPerSample == 1)
		return ADF_STEREO8;
	if(channels == 2 && bytesPerSample == 2)
		return ADF_STEREO16;
	return ADF_LAST;
}

static inline ALenum audioDataFormatConvert(AudioDataFormat format)
{
	assert(format < ADF_LAST);
	return tblAudioFormat[format].format;
}

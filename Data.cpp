/**
 *
 * @file Data.cpp
 * @author karfouilla
 * @version 1.0Q
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

#include "KA3D/Data.h"


#include <AL/al.h>


#include <cassert>
#include <cstdint>

#include <sstream>
#include <stdexcept>
#include <vector>


#include "DataPrivate.h"
#include "Error.h"
#include "KA3D/WaveFile.h"

namespace KA3D
{

static const struct
{
	const char* name;
	quint16 channels, bytesPerSample;
	ALenum format;
} tblAudioFormat[] = {
	{"DF_MONO8", 1, 1, AL_FORMAT_MONO8},
	{"DF_MONO16", 1, 2, AL_FORMAT_MONO16},
	{"DF_STEREO8", 2, 1, AL_FORMAT_STEREO8},
	{"DF_STEREO16", 2, 2, AL_FORMAT_STEREO16},

	{"DF_LAST", 0, 0, 0}
};

static inline ALenum audioDataFormatConvert(DataFormat format);


Data* Data::fromData(const std::vector<quint8>& tblData,
                               DataFormat format, qint32 freq)
{
	DataPrivate* privateData(new DataPrivate);
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
	return new Data(privateData);
}

Data* Data::fromWav(QIODevice& file)
{
	std::vector<quint8> tblData;
	DataFormat format;
	quint32 freq;

	WaveFile waveFile(file);
	waveFile.open(QIODevice::ReadOnly);

	format = waveFile.format();
	freq = waveFile.samplesPerSec();
	tblData.resize(waveFile.size());

	waveFile.read(tblData.data(), waveFile.size());

	waveFile.close();

	return fromData(tblData, format, freq);
}

Data::Data(DataPrivate* pData) noexcept:
	m_pData(pData)
{ }

Data::~Data() noexcept
{
	alDeleteBuffers(1, &m_pData->handle);
	delete m_pData;
}
DataPrivate* Data::data() noexcept
{
	return m_pData;
}

const char* Data::formatName(DataFormat format) noexcept
{
	assert(format < DF_LAST);
	return tblAudioFormat[format].name;
}

quint16 Data::formatChannels(DataFormat format) noexcept
{
	assert(format < DF_LAST);
	return tblAudioFormat[format].channels;
}

quint16 Data::formatBytesPerSample(DataFormat format) noexcept
{
	assert(format < DF_LAST);
	return tblAudioFormat[format].bytesPerSample;
}

quint16 Data::formatPitch(DataFormat format) noexcept
{
	return formatChannels(format) * formatBytesPerSample(format);
}
DataFormat
Data::formatFromPerSample(quint16 channels,
                               quint16 bytesPerSample) noexcept
{
	if(channels == 1 && bytesPerSample == 1)
		return DF_MONO8;
	if(channels == 1 && bytesPerSample == 2)
		return DF_MONO16;
	if(channels == 2 && bytesPerSample == 1)
		return DF_STEREO8;
	if(channels == 2 && bytesPerSample == 2)
		return DF_STEREO16;
	return DF_LAST;
}

static inline ALenum audioDataFormatConvert(DataFormat format)
{
	assert(format < DF_LAST);
	return tblAudioFormat[format].format;
}

} // namespace KA3D

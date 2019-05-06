/**
 *
 * @file WavFile.cpp
 * @author karfouilla
 * @version 1.0
 * @date 27 avril 2019
 * @brief Contient la gestion des fichiers RIFF/WAVE (CPP)
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

#include "KA3D/WaveFile.h"

#include <cassert>
#include <cstring>
#include <cstdint>

#include <iostream>
#include <sstream>

#include "Endianness.h"

namespace KA3D
{

// WAVE structure and data
struct fmtCommon
{
	std::uint16_t wFormatTag;
	std::uint16_t wChannels;
	std::uint32_t dwSamplesPerSec;
	std::uint32_t dwAvgBytesPerSec;
	std::uint16_t wBlockAlign;
};
struct fmtSpecificPCM
{
	std::uint16_t wBitsPerSample;
};

const std::uint16_t WAVE_FORMAT_PCM = 0x0001;

const std::uint8_t RIFF_TAG_RIFF[] = {'R', 'I', 'F', 'F'};
const std::uint8_t RIFF_TAG_WAVE[] = {'W', 'A', 'V', 'E'};
const std::uint8_t RIFF_TAG_FMT[] = {'f', 'm', 't', ' '};
const std::uint8_t RIFF_TAG_DATA[] = {'d', 'a', 't', 'a'};

/*
 * WAVE chunk				4	4	-
 * 	format chunk			4	8	-
 * 	format size				4	12	-
 * 		wFormatTag			2	14	2
 * 		wChannels			2	16	4
 * 		dwSamplesPerSec		4	20	8
 * 		dwAvgBytesPerSec	4	24	12
 * 		wBlockAlign			2	26	14
 * 		wBitsPerSample		2	28	16
 * 	data chunk				4	32	-
 * 	data size				4	36	-
 *
 * TOTAL FORMAT				16
 * TOTAL HEADER				36
 */
const std::uint32_t DEFAULT_FORMAT_SIZE = 16;
const std::uint32_t DEFAULT_HEADER_SIZE = 36;


WaveFile::WaveFile(std::iostream& refFile) noexcept:
	m_refFile(refFile),
	m_uFileSize(4),
	m_uFileRemaining(m_uFileSize),
	m_uSamplesPerSec(0),
	m_format(DF_LAST),
	m_uSize(0),
	m_uRemaining(0)
{ }

WaveFile::~WaveFile() noexcept
{ }

void WaveFile::open(std::ios_base::openmode mode)
{
	if(mode == std::ios_base::in)
	{
		readHeaders();
	}
	else if(mode == std::ios_base::out)
	{
		writeHeaders();
	}
	else
	{
		std::cerr << "Invalid mode '" << static_cast<int>(mode);
		std::cerr << "': only ios_base::in or ios_base::out" << std::endl;
		abort();
	}
}

void WaveFile::readHeaders()
{
	std::uint32_t cksz;
	fmtCommon fmtCom;
	fmtSpecificPCM fmtPCM;
	std::uint16_t audioPitch;

	// En-tête RIFF/WAVE
	checkNextChunk(RIFF_TAG_RIFF, m_uFileSize);

	m_uFileRemaining = m_uFileSize;

	checkChunk(RIFF_TAG_WAVE);

	// Format
	findNextChunk(RIFF_TAG_FMT, cksz);

	// Commun
	readWord(fmtCom.wFormatTag);
	readWord(fmtCom.wChannels);
	readDWord(fmtCom.dwSamplesPerSec);
	readDWord(fmtCom.dwAvgBytesPerSec);
	readWord(fmtCom.wBlockAlign);

	// Specifique
	if(fmtCom.wFormatTag != WAVE_FORMAT_PCM)
		throw std::runtime_error("Can't parse proprietary wave format, "
		                         "only WAVE_FORMAT_PCM (0x0001) supported");

	readWord(fmtPCM.wBitsPerSample);

	// Vérification
	if(fmtCom.wChannels != 1 && fmtCom.wChannels != 2)
		throw std::runtime_error("Unsupported format: "
		                         "only mono and stereo supported");
	if(fmtPCM.wBitsPerSample != 8 && fmtPCM.wBitsPerSample != 16)
		throw std::runtime_error("Unsupported format: "
		                         "only 8 and 16 bits/sample supported");
	if(fmtCom.dwSamplesPerSec <= 0)
		throw std::runtime_error("Invalid samples/second");

	if(8*fmtCom.wBlockAlign != fmtCom.wChannels*fmtPCM.wBitsPerSample)
		throw std::runtime_error("Incoherent block align");
	if(fmtCom.dwAvgBytesPerSec != fmtCom.wBlockAlign*fmtCom.dwSamplesPerSec)
		throw std::runtime_error("Incoherent bytes/second");

	// Enregistrement
	m_uSamplesPerSec = fmtCom.dwSamplesPerSec;
	m_format = Data::formatFromPerSample(fmtCom.wChannels,
	                                          fmtPCM.wBitsPerSample/8);
	audioPitch = Data::formatPitch(m_format);

	// Données
	findNextChunk(RIFF_TAG_DATA, m_uSize);

	// Vérification
	if(m_uFileRemaining < m_uSize)
		throw std::runtime_error("Incoherent data size");
	if(m_uSize % audioPitch != 0)
		throw std::runtime_error("Incoherent data size");

	// Fichier ouvert, curseur au début des données
	m_uRemaining = m_uSize;
}

void WaveFile::writeHeaders()
{
	std::uint16_t bytesPerSample(Data::formatBytesPerSample(m_format));
	fmtCommon fmtCom;
	fmtSpecificPCM fmtPCM;
	fmtCom.wFormatTag = WAVE_FORMAT_PCM;
	fmtCom.wChannels = Data::formatChannels(m_format);
	fmtCom.dwSamplesPerSec = m_uSamplesPerSec;
	fmtCom.wBlockAlign = fmtCom.wChannels * bytesPerSample;
	fmtCom.dwAvgBytesPerSec = fmtCom.wBlockAlign * m_uSamplesPerSec;
	fmtPCM.wBitsPerSample = 8*bytesPerSample;

	// En-tête RIFF/WAVE
	m_uFileSize = DEFAULT_HEADER_SIZE + m_uSize;
	writeChunk(RIFF_TAG_RIFF, m_uFileSize);

	m_uFileRemaining = m_uFileSize;

	writeChunk(RIFF_TAG_WAVE);

	// Format
	writeChunk(RIFF_TAG_FMT, DEFAULT_FORMAT_SIZE);

	// Commun
	writeWord(fmtCom.wFormatTag);
	writeWord(fmtCom.wChannels);
	writeDWord(fmtCom.dwSamplesPerSec);
	writeDWord(fmtCom.dwAvgBytesPerSec);
	writeWord(fmtCom.wBlockAlign);
	// Specifique
	writeWord(fmtPCM.wBitsPerSample);

	// Données
	writeChunk(RIFF_TAG_DATA, m_uSize);

	// Fichier ouvert, curseur au début des données
	m_uRemaining = m_uSize;
	assert(m_uFileRemaining == m_uRemaining);
}

std::uint64_t WaveFile::read(void* data, std::uint64_t size)
{
	std::uint32_t readable;
	readable = std::min(static_cast<std::uint64_t>(m_uRemaining), size);
	rawRead(data, readable);

	if(Data::formatBytesPerSample(m_format) == 2)
	{
		std::uint16_t* data16(static_cast<std::uint16_t*>(data));
		std::uint32_t count(readable/2);
		for(std::uint32_t i=0; i<count; ++i)
		{
			letoh(data16[i]);
		}
	}

	m_uRemaining -= readable;
	return readable;
}

void WaveFile::write(const void* data, std::uint64_t size)
{
	assert(m_uRemaining >= size);

	if(Data::formatBytesPerSample(m_format) == 2)
	{
		std::uint16_t count(size/2);
		std::uint16_t* data16(new std::uint16_t[count]);
		std::memcpy(data16, data, size);
		try
		{
			for(std::uint32_t i=0; i<count; ++i)
			{
				letoh(data16[i]);
			}
			rawWrite(data16, size);
		}
		catch(...)
		{
			delete[] data16;
		}
	}
	else
	{
		rawWrite(data, size);
	}
	m_uRemaining -= size;
}
std::int64_t WaveFile::seek(std::int64_t offset, std::ios_base::seekdir whence)
{
	std::uint32_t done(m_uSize - m_uRemaining);
	// distance relative curseur-début
	std::int64_t min(- static_cast<std::int64_t>(done));
	// distance relative curseur-fin
	std::int64_t max(static_cast<std::int64_t>(m_uRemaining));

	std::int64_t realOffset(0);

	if(whence == std::ios_base::cur)
		realOffset = offset;
	else if(whence == std::ios_base::beg)
		realOffset = - static_cast<std::int64_t>(done) + offset;
	else if(whence == std::ios_base::end)
		realOffset = static_cast<std::int64_t>(m_uRemaining) + offset;
	else
	{
		std::cerr << "Invalid whence '"
		          << static_cast<int>(whence) << "'" << std::endl;
		abort();
	}

	realOffset = std::min(std::max(realOffset, min), max);

	m_refFile.seekg(realOffset, std::ios_base::cur);
	m_uFileRemaining -= realOffset;
	m_uRemaining -= realOffset;

	return (m_uSize - m_uRemaining);
}

void WaveFile::close()
{
	skipRead(m_uFileRemaining);
}

void WaveFile::setSamplesPerSec(std::uint32_t dwSamplesPerSec) noexcept
{
	// Avant ouverture et en mode écriture seulement
	m_uSamplesPerSec = dwSamplesPerSec;
}
void WaveFile::setFormat(DataFormat format) noexcept
{
	// Avant ouverture et en mode écriture seulement
	m_format = format;
}
void WaveFile::setSize(std::uint32_t dwSize) noexcept
{
	// Avant ouverture et en mode écriture seulement
	m_uSize = dwSize;
}
std::uint32_t WaveFile::samplesPerSec() const noexcept
{
	return m_uSamplesPerSec;
}
DataFormat WaveFile::format() const noexcept
{
	return m_format;
}
std::uint32_t WaveFile::size() const noexcept
{
	return m_uSize;
}

void WaveFile::rawRead(void* data, std::uint64_t size, std::uint64_t n)
{
	m_refFile.read(static_cast<std::iostream::char_type*>(data), n*size);
	if(!m_refFile.good())
	{
		throw std::runtime_error("Reading error");
	}
	m_uFileRemaining -= size*n;
}
void WaveFile::rawWrite(const void* data, std::uint64_t size, std::uint64_t n)
{
	m_refFile.write(static_cast<const std::iostream::char_type*>(data), n*size);
	if(!m_refFile.good())
	{
		throw std::runtime_error("Writing error");
	}
	m_uFileRemaining -= size*n;
}


void WaveFile::skipRead(std::uint32_t size)
{
	m_refFile.seekg(size, std::ios_base::cur);
	m_uRemaining -= size;
	m_uFileRemaining -= size;
}
void WaveFile::skipWord()
{
	std::uint16_t empty;
	rawRead(&empty, sizeof(std::uint16_t));
}
void WaveFile::readWord(std::uint16_t& word)
{
	rawRead(&word, sizeof(std::uint16_t));
	letoh(word);
}
void WaveFile::readDWord(std::uint32_t& dword)
{
	rawRead(&dword, sizeof(std::uint32_t));
	letoh(dword);
}
void WaveFile::readChunk(std::uint8_t* chunk)
{
	rawRead(chunk, 1, 4);
}
void WaveFile::checkChunk(const std::uint8_t* value)
{
	std::uint8_t chunk[4];
	readChunk(chunk);

	if(std::memcmp(chunk, value, 4) != 0) // chunk != value
	{
		std::ostringstream msg;
		msg << "Expected chunk " << value[0] << value[1]
		                         << value[2] << value[3];
		throw std::runtime_error(msg.str());
	}
}

void WaveFile::nextChunk(std::uint8_t* chunk, std::uint32_t& cksz)
{
	readChunk(chunk);
	readDWord(cksz);
}

void WaveFile::checkNextChunk(const std::uint8_t* value, std::uint32_t& cksz)
{
	std::uint8_t chunk[4];
	nextChunk(chunk, cksz);

	if(std::memcmp(chunk, value, 4) != 0) // chunk != value
	{
		std::ostringstream msg;
		msg << "Expected chunk " << value[0] << value[1]
		                         << value[2] << value[3];
		throw std::runtime_error(msg.str());
	}
}

void WaveFile::findNextChunk(const std::uint8_t* value, std::uint32_t& cksz)
{
	try
	{
		std::uint8_t chunk[4];

		nextChunk(chunk, cksz);
		while(std::memcmp(chunk, value, 4) != 0) // while(chunck != value)
		{
			skipRead(cksz);
			nextChunk(chunk, cksz);
		}
	}
	catch(std::exception& e)
	{
		if(m_refFile.eof())
		{
			std::ostringstream msg;
			msg << "Missing chunk " << value[0] << value[1]
			                        << value[2] << value[3];
			throw std::runtime_error(msg.str());
		}
		else
		{
			throw;
		}
	}
}

void WaveFile::writeWord(std::uint16_t word)
{
	htole(word);
	rawWrite(&word, sizeof(std::uint16_t));
}
void WaveFile::writeDWord(std::uint32_t dword)
{
	htole(dword);
	rawWrite(&dword, sizeof(std::uint32_t));
}
void WaveFile::writeChunk(const std::uint8_t* chunk)
{
	rawWrite(chunk, 1, 4);
}
void WaveFile::writeChunk(const std::uint8_t* chunk, std::uint32_t cksz)
{
	writeChunk(chunk);
	writeDWord(cksz);
}

} // namespace KA3D

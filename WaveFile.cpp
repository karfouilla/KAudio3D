/**
 *
 * @file WavFile.cpp
 * @author karfouilla
 * @version 1.0Q
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

#include <algorithm>
#include <iostream>
#include <sstream>

#include <QtEndian>

namespace KA3D
{

// WAVE structure and data
struct fmtCommon
{
	quint16 wFormatTag;
	quint16 wChannels;
	quint32 dwSamplesPerSec;
	quint32 dwAvgBytesPerSec;
	quint16 wBlockAlign;
};
struct fmtSpecificPCM
{
	quint16 wBitsPerSample;
};

const quint16 WAVE_FORMAT_PCM = 0x0001;

const quint8 RIFF_TAG_RIFF[] = {'R', 'I', 'F', 'F'};
const quint8 RIFF_TAG_WAVE[] = {'W', 'A', 'V', 'E'};
const quint8 RIFF_TAG_FMT[] = {'f', 'm', 't', ' '};
const quint8 RIFF_TAG_DATA[] = {'d', 'a', 't', 'a'};

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
const quint32 DEFAULT_FORMAT_SIZE = 16;
const quint32 DEFAULT_HEADER_SIZE = 36;


WaveFile::WaveFile(QIODevice& refFile) noexcept:
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

void WaveFile::open(QIODevice::OpenMode )
{
	if(m_refFile.openMode() == QIODevice::ReadOnly)
	{
		readHeaders();
	}
	else if(m_refFile.openMode() == QIODevice::WriteOnly)
	{
		writeHeaders();
	}
	else
	{
		std::cerr << "Invalid mode '" << static_cast<int>(m_refFile.openMode());
		std::cerr << "': only ios_base::in or ios_base::out" << std::endl;
		abort();
	}
}

void WaveFile::readHeaders()
{
	quint32 cksz;
	fmtCommon fmtCom;
	fmtSpecificPCM fmtPCM;
	quint16 audioPitch;

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
	quint16 bytesPerSample(Data::formatBytesPerSample(m_format));
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

quint64 WaveFile::read(void* data, quint64 size)
{
	quint32 readable;
	readable = std::min(static_cast<quint64>(m_uRemaining), size);
	rawRead(data, readable);

	if(Data::formatBytesPerSample(m_format) == 2)
	{
		quint16* data16(static_cast<quint16*>(data));
		quint32 count(readable/2);

		for(quint32 i=0; i<count; ++i)
		{
			data16[i] = qFromLittleEndian(data16[i]);
		}
	}

	m_uRemaining -= readable;
	return readable;
}

void WaveFile::write(const void* data, quint64 size)
{
	assert(m_uRemaining >= size);

	if(Data::formatBytesPerSample(m_format) == 2)
	{
		quint32 count(size/2);
		const quint16* data16h(static_cast<const quint16*>(data));

		quint16* data16(new quint16[count]);
		for(quint32 i=0; i<count; ++i)
			data16[i] = qToLittleEndian(data16h[i]);

		try
		{
			rawWrite(data16, size);
		}
		catch(...)
		{
			delete[] data16;
			throw;
		}
		delete[] data16;
	}
	else
	{
		rawWrite(data, size);
	}
	m_uRemaining -= size;
}
void WaveFile::seek(qint64 offset)
{
	quint32 done(m_uSize - m_uRemaining);
	// distance relative curseur-début
	qint64 min(- static_cast<qint64>(done));
	// distance relative curseur-fin
	qint64 max(static_cast<qint64>(m_uRemaining));

	qint64 relOffset(- static_cast<qint64>(done) + offset);
	relOffset = std::min(std::max(relOffset, min), max);

	m_refFile.seek(m_refFile.pos()+relOffset);
	m_uFileRemaining -= relOffset;
	m_uRemaining -= relOffset;
}

void WaveFile::close()
{
	skipRead(m_uFileRemaining);
}

void WaveFile::setSamplesPerSec(quint32 dwSamplesPerSec) noexcept
{
	// Avant ouverture et en mode écriture seulement
	m_uSamplesPerSec = dwSamplesPerSec;
}
void WaveFile::setFormat(DataFormat format) noexcept
{
	// Avant ouverture et en mode écriture seulement
	m_format = format;
}
void WaveFile::setSize(quint32 dwSize) noexcept
{
	// Avant ouverture et en mode écriture seulement
	m_uSize = dwSize;
}
quint32 WaveFile::samplesPerSec() const noexcept
{
	return m_uSamplesPerSec;
}
DataFormat WaveFile::format() const noexcept
{
	return m_format;
}
quint32 WaveFile::size() const noexcept
{
	return m_uSize;
}

void WaveFile::rawRead(void* data, qint64 size, qint64 n)
{
	if(m_refFile.read(static_cast<char*>(data), n*size) < n*size)
	{
		throw std::runtime_error("Reading error");
	}
	m_uFileRemaining -= size*n;
}
void WaveFile::rawWrite(const void* data, qint64 size, qint64 n)
{
	if(m_refFile.write(static_cast<const char*>(data), n*size) < n*size)
	{
		throw std::runtime_error("Writing error");
	}
	m_uFileRemaining -= size*n;
}


void WaveFile::skipRead(quint32 size)
{
	m_refFile.seek(m_refFile.pos()+size);
	m_uRemaining -= size;
	m_uFileRemaining -= size;
}
void WaveFile::skipWord()
{
	quint16 empty;
	rawRead(&empty, sizeof(quint16));
}
void WaveFile::readWord(quint16& word)
{
	rawRead(&word, sizeof(quint16));
	word = qFromLittleEndian(word);
}
void WaveFile::readDWord(quint32& dword)
{
	rawRead(&dword, sizeof(quint32));
	dword = qFromLittleEndian(dword);
}
void WaveFile::readChunk(quint8* chunk)
{
	rawRead(chunk, 1, 4);
}
void WaveFile::checkChunk(const quint8* value)
{
	quint8 chunk[4];
	readChunk(chunk);

	if(std::memcmp(chunk, value, 4) != 0) // chunk != value
	{
		std::ostringstream msg;
		msg << "Expected chunk " << value[0] << value[1]
		                         << value[2] << value[3];
		throw std::runtime_error(msg.str());
	}
}

void WaveFile::nextChunk(quint8* chunk, quint32& cksz)
{
	readChunk(chunk);
	readDWord(cksz);
}

void WaveFile::checkNextChunk(const quint8* value, quint32& cksz)
{
	quint8 chunk[4];
	nextChunk(chunk, cksz);

	if(std::memcmp(chunk, value, 4) != 0) // chunk != value
	{
		std::ostringstream msg;
		msg << "Expected chunk " << value[0] << value[1]
		                         << value[2] << value[3];
		throw std::runtime_error(msg.str());
	}
}

void WaveFile::findNextChunk(const quint8* value, quint32& cksz)
{
	try
	{
		quint8 chunk[4];

		nextChunk(chunk, cksz);
		while(std::memcmp(chunk, value, 4) != 0) // while(chunck != value)
		{
			skipRead(cksz);
			nextChunk(chunk, cksz);
		}
	}
	catch(std::exception& e)
	{
		if(m_refFile.atEnd())
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

void WaveFile::writeWord(quint16 word)
{
	word = qToLittleEndian(word);
	rawWrite(&word, sizeof(quint16));
}
void WaveFile::writeDWord(quint32 dword)
{
	dword = qToLittleEndian(dword);
	rawWrite(&dword, sizeof(quint32));
}
void WaveFile::writeChunk(const quint8* chunk)
{
	rawWrite(chunk, 1, 4);
}
void WaveFile::writeChunk(const quint8* chunk, quint32 cksz)
{
	writeChunk(chunk);
	writeDWord(cksz);
}

} // namespace KA3D

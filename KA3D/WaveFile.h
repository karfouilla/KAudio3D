#ifndef WAVFILE_H_INCLUDED
#define WAVFILE_H_INCLUDED
/**
 *
 * @file WavFile.h
 * @author karfouilla
 * @version 1.0
 * @date 27 avril 2019
 * @brief Contient la gestion des fichiers RIFF/WAVE (H)
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

#include <cstdint>
#include <iostream>

#include "Data.h"

#ifndef __GNUC__
#ifndef __clang__
#  define __attribute__(X)
#endif
#endif

namespace KA3D
{

/**
 * Classe de gestion des fichiers .wav (RIFF/WAVE)
 */
class WaveFile
{
public:
	/**
	 * Permet de lire/écrire dans un fichier en wave
	 * @param pFile fichier à ouvrir (peut être une portion de fichier)
	 * @param isClose Est-ce qu'on ferme le fichier à la fin (lors de close)
	 */
	WaveFile(std::iostream& refFile) noexcept;

	//! Copie interdite
	WaveFile(const WaveFile& other) noexcept = delete;
	//! Copie interdite
	WaveFile& operator=(const WaveFile& other) noexcept = delete;

	//! destructeur
	virtual ~WaveFile() noexcept;
	/**
	 * @brief Permet d'ouvrir le fichier s'il n'est pas ouvert
	 * Ouvre le fichier dans le cas ou ça n'est pas fait puis
	 *  - En lecture : lit les en-têtes du fichier wave,
	 * infos accessible dans les attributs (format, samplesPerSec et size)
	 *  - En écriture : écrit les en-têtes dans le fichiers wave
	 * les informations des en-têtes doivent avoir été renseigné avant
	 * dans les attributs (format, samplesPerSec et size)
	 */
	virtual void open(std::ios_base::openmode mode);
	/**
	 * @brief Permet de se déplacer dans les données audio
	 * @param offset Nombre d'octets par rapport à l'origine \a whence
	 * @param whence Position de référence (origine) parmi
	 * SEEK_SET	début du fichier
	 * SEEK_CUR	position courante
	 * SEEK_END	fin du fichier
	 * @return La nouvelle position dans les données audio
	 */
	virtual std::int64_t seek(std::int64_t offset,
	                          std::ios_base::seekdir whence);
	/**
	 * @brief Permet de lire les données audio du wave (au format #format)
	 * @param[out] data Variable dans laquelle on stocke les données lu
	 * @param[in] size Taille des données à lire en octets
	 * @return Taille des données lu en octets
	 */
	virtual std::uint64_t read(void* data, std::uint64_t size)
		__attribute__((nonnull));
	/**
	 * @brief Permet d'écrire les données audio du wave
	 * Le total des données ajouté dans le fichier ne doit pas dépasser
	 * la taille spécifié au début pour les en-tête (voir #size)
	 * @param data Données audio à écrire (voir #format pour le format)
	 * @param size Taille des données à écrire
	 */
	virtual void write(const void* data, std::uint64_t size)
		__attribute__((nonnull));
	/**
	 * @brief Permet de fermet le fichier wave
	 * Si le flux de fichier doit être fermé, il sera fermé,
	 * sinon, le curseur est placé à la fin de ce fichier wave
	 * (permettant ainsi de lire d'autres données située après)
	 */
	virtual void close();

	//! Permet de définir la fréquence d'échantillonage
	//! (à définir en mode écriteur et avant ouverture du fichier)
	void setSamplesPerSec(std::uint32_t dwSamplesPerSec) noexcept;
	//! Permet de définir le format des données audio (cf. #DataFormat)
	//! (à définir en mode écriteur et avant ouverture du fichier)
	void setFormat(DataFormat format) noexcept;
	//! Permet de définir la taille des données audio en octets
	//! (à définir en mode écriteur et avant ouverture du fichier)
	void setSize(std::uint32_t dwSize) noexcept;
	//! Permet d'obtenir la fréquence d'échantillonage
	//! (disponible après ouverture en mode lecture)
	std::uint32_t samplesPerSec() const noexcept;
	//! Permet d'obtenir le format des données audio
	//! (disponible après ouverture en mode lecture)
	DataFormat format() const noexcept;
	//! Permet d'obtenir la taille (en octets) données audio
	//! (disponible après ouverture en mode lecture)
	std::uint32_t size() const noexcept;

private:
	void rawRead(void* data, std::uint64_t size, std::uint64_t n = 1);
	void rawWrite(const void* data, std::uint64_t size, std::uint64_t n = 1);


	void readHeaders();

	void skipRead(std::uint32_t size);
	void skipWord();

	void readWord(std::uint16_t& word);
	void readDWord(std::uint32_t& dword);

	void readChunk(std::uint8_t* chunk);
	void checkChunk(const std::uint8_t* chunk);

	void nextChunk(std::uint8_t* chunk, std::uint32_t& cksz);
	void checkNextChunk(const std::uint8_t* chunk, std::uint32_t& cksz);
	void findNextChunk(const std::uint8_t* chunk, std::uint32_t& cksz);


	void writeHeaders();

	void writeWord(std::uint16_t word);
	void writeDWord(std::uint32_t dword);

	void writeChunk(const std::uint8_t* chunk);
	void writeChunk(const std::uint8_t* chunk, std::uint32_t cksz);

private:
	std::iostream& m_refFile; //!< Flux du wave
	std::uint32_t m_uFileSize; //!< Taille du flux RIFF/WAVE
	std::uint32_t m_uFileRemaining; //!< Nombre d'octets restant (à lire/écrire)
	std::uint32_t m_uSamplesPerSec; //!< Fréquence d'échantillonage de l'audio
	DataFormat m_format; //!< Format des données audio
	std::uint32_t m_uSize; //!< Taille (en octets) des données audio
	std::uint32_t m_uRemaining; //!< Nombre d'octets restant (à lire ou écrire)
};

} // namespace KA3D

#endif // WAVFILE_H_INCLUDED

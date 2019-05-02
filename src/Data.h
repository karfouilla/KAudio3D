#ifndef AUDIODATA_H_INCLUDED
#define AUDIODATA_H_INCLUDED
/**
 *
 * @file Data.h
 * @author karfouilla
 * @version α.0.1
 * @date 27 avril 2019
 * @brief Fichier contenant la classe de gestion des données audio (H)
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

#include <vector>
#include <iostream>

namespace KA3D
{

class DataPrivate;

//! Liste des formats audio brute
enum DataFormat {
	DF_MONO8, //!< Mono 8 bit par échantillon
	DF_MONO16, //!< Mono 16 bit par échantillon
	DF_STEREO8, //!< Stéreo 8 bit par échantillon
	DF_STEREO16, //!< Stéreo 16 bit par échantillon

	DF_LAST //!< Borne de fin
};

/**
 * @brief Classe représentant des données audio (une instance = une piste)
 */
class Data
{
public:
	/**
	 * @brief Permet d'obtenir le nom d'un format audio
	 * @param format Format audio (cf. #DataFormat)
	 * @return Chaîne de caractère constante (type C)
	 */
	static const char* formatName(DataFormat format) noexcept
		__attribute__((pure));
	/**
	 * @brief Permet d'obtenir le nombre de canaux d'un format (mono, stereo...)
	 * @param format Format audio (cf. #DataFormat)
	 * @return Nombre de canaux du format (sous forme d'un entier)
	 */
	static std::uint16_t formatChannels(DataFormat format) noexcept
		__attribute__((pure));
	/**
	 * @brief Permet d'obtenir le nombre d'octets par échantillon et par canal
	 * @param format Format audio (cf. #DataFormat)
	 * @return Nombre d'octet (sous forme d'un entier)
	 */
	static std::uint16_t formatBytesPerSample(DataFormat format) noexcept
		__attribute__((pure));
	/**
	 * @brief Permet d'obtenir le nombre d'octets par échantillon d'un format
	 * @param format Format audio (cf. #DataFormat)
	 * @return Nombre d'octet (sous forme d'un entier)
	 */
	static std::uint16_t formatPitch(DataFormat format) noexcept
		__attribute__((pure));
	/**
	 * @brief Permet d'obtenir le format audio correspondant
	 * Permet d'obtenir le format audio correspondant à
	 * un nombre de canaux
	 * et un nombre d'octets par échantillon et par canaux
	 * @param channels Nombre de canaux
	 * @param bytesPerSample Nombre d'octets par échantillon et par canaux
	 * @return Format audio (cf. #DataFormat) correspondant aux paramètre
	 */
	static DataFormat
	formatFromPerSample(std::uint16_t channels, std::uint16_t bytesPerSample)
		noexcept __attribute__((pure));

public:
	Data(Data& other) noexcept = delete; //!< Copie interdite
	//! Copie interdite
	const Data& operator=(Data& other) noexcept = delete;
	/**
	 * @brief Permet de charger des données audio à partir d'un tableau d'octet
	 * @param tblData Données brutes
	 * @param format Format des données brute (cf. #DataFormat)
	 * @param freq Fréquence d'échantillonage des données
	 * @return Pointeur alloué dynamiquement (avec new) vers le buffer de donnée
	 */
	static Data* fromData(const std::vector<std::uint8_t>& tblData,
	                      DataFormat format, std::int32_t freq);

	/**
	 * @brief Permet de charger des données audio à partir d'un contenue wav
	 * @param file Flux contenant le fichier audio
	 * @return Pointeur alloué dynamiquement (avec new) vers le buffer de donnée
	 */
	static Data* fromWav(std::iostream& file);

	/**
	 * @brief Destructeur
	 */
	~Data() noexcept;

	/**
	 * @brief Retourne les données interne privée de la classe
	 * @return Pointeur vers AudioDataPrivate
	 */
	DataPrivate* data() noexcept;

private:
	//! Constructeur privée, utiliser fromData, fromWav, fromOgg...
	Data(DataPrivate* pData) noexcept;

private:
	DataPrivate* m_pData;
};

} // namespace KA3D

#endif // AUDIODATA_H_INCLUDED

#ifndef AUDIODATA_H_INCLUDED
#define AUDIODATA_H_INCLUDED
/**
 *
 * @file AudioData.h
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

#include <vector>
#include <iostream>

class AudioDataPrivate;

//! Liste des formats audio brute
enum AudioDataFormat {
	ADF_MONO8, //!< Mono 8 bit par échantillon
	ADF_MONO16, //!< Mono 16 bit par échantillon
	ADF_STEREO8, //!< Stéreo 8 bit par échantillon
	ADF_STEREO16, //!< Stéreo 16 bit par échantillon

	ADF_LAST //!< Borne de fin
};

/**
 * @brief Classe représentant des données audio (une instance = une piste)
 */
class AudioData
{
public:
	/**
	 * @brief Permet d'obtenir sous forme de chaîne de caractère, le nom d'un format audio
	 * @param format Format audio dont on veux obtenir le nom (cf. #AudioDataFormat)
	 * @return Chaîne de caractère constante (type C)
	 */
	static const char* formatName(AudioDataFormat format) noexcept
		__attribute__((pure));
	/**
	 * @brief Permet d'obtenir le nombre de canaux d'un format audio (mono, stereo...)
	 * @param format Format audio dont on veux obtenir le nombre de canaux (cf. #AudioDataFormat)
	 * @return Nombre de canaux du format (sous forme d'un entier)
	 */
	static uint16_t formatChannels(AudioDataFormat format) noexcept
		__attribute__((pure));
	/**
	 * @brief Permet d'obtenir le nombre d'octets par échantillon et par canal d'un format audio
	 * @param format Format audio dont on veux obtenir cet information (cf. #AudioDataFormat)
	 * @return Nombre d'octet (sous forme d'un entier)
	 */
	static uint16_t formatBytesPerSample(AudioDataFormat format) noexcept
		__attribute__((pure));
	/**
	 * @brief Permet d'obtenir le nombre d'octets par échantillon d'un format audio
	 * @param format Format audio dont on veux obtenir cet information (cf. #AudioDataFormat)
	 * @return Nombre d'octet (sous forme d'un entier)
	 */
	static uint16_t formatPitch(AudioDataFormat format) noexcept
		__attribute__((pure));
	/**
	 * @brief Permet d'obtenir le format audio correspondant à un nombre de canaux et d'octets par échantillon et par canaux
	 * @param channels Nombre de canaux
	 * @param bytesPerSample Nombre d'octets par échantillon et par canaux
	 * @return Le format audio (cf. #AudioDataFormat) correspondant au deux paramètre donnés
	 */
	static AudioDataFormat
	formatFromPerSample(uint16_t channels, uint16_t bytesPerSample) noexcept
		__attribute__((pure));

public:
	AudioData(AudioData& other) noexcept = delete; //!< Copie interdite
	const AudioData& operator=(AudioData& other) noexcept = delete; //!< Copie interdite
	/**
	 * @brief Permet de charger des données audio à partir d'un tableau d'octet
	 * @param tblData Données brutes
	 * @param format Format des données brute (cf. #AudioDataFormat)
	 * @param freq Fréquence d'échantillonage des données
	 * @return Pointeur alloué dynamiquement (avec new) vers le buffer de donnée
	 */
	static AudioData* fromData(const std::vector<uint8_t>& tblData,
	                           AudioDataFormat format, int32_t freq);

	/**
	 * @brief Permet de charger des données audio à partir d'un fichier wav
	 * @param file Fichier (qui sera ouvert s'il ne l'est pas déjà) où se trouve l'image
	 * @return Pointeur alloué dynamiquement (avec new) vers le buffer de donnée
	 */
	static AudioData* fromWav(std::iostream& file);

	/**
	 * @brief Destructeur
	 */
	~AudioData() noexcept;

	/**
	 * @brief Retourne les données interne privée de la classe
	 * @return Pointeur vers AudioDataPrivate
	 */
	AudioDataPrivate* data() noexcept;

private:
	AudioData(AudioDataPrivate* pData) noexcept; //!< Constructeur privée, utiliser fromData, fromWav, fromOgg...

private:
	AudioDataPrivate* m_pData;
};

#endif // AUDIODATA_H_INCLUDED

#ifndef MULTISOURCE_H_INCLUDED
#define MULTISOURCE_H_INCLUDED
/**
 *
 * @file Sound.h
 * @author karfouilla
 * @version 1.0
 * @date 30 avril 2019
 * @brief Fichier contenant la classe de gestion d'un son (H)
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

#include "Source.h"

namespace KA3D
{

//! Instance sonore
typedef uint32_t SoundInstance;

/**
 * @brief Classe de configuration de son. Hériter et redéfinir operator()
 */
class SourceConfigure
{
public:
	//! Destructeur
	virtual ~SourceConfigure() noexcept { }
	/**
	 * @brief Fonction de configuration à redéfinir
	 * @param instance Numéro d'instance (maybe @REMOVEME)
	 * @param pSource Source à configurer
	 */
	virtual void operator()(SoundInstance instance, Source* pSource) = 0;
};

/**
 * @brief Classe permettant la gestion des sons
 * Une instance de cette classe représente un type de son,
 * c'est-à-dire une piste audio et sa configuration 3D
 */
class Sound
{
public:
	/**
	 * @brief Charge un son à partir d'un fichier wav
	 * @param filename Nom du fichier
	 * @return Pointeur alloué dynamiquement sur le son
	 */
	static Sound* fromWav(const std::string& filename);

public:
	/**
	 * @brief Constructeur
	 * @param instanceMax Nombre de lecture simultanée maximum
	 */
	Sound(uint32_t instanceMax = 4);
	/**
	 * @brief Constructeur
	 * @param pConfig Configuration de la source du son
	 * @param instanceMax Nombre de lecture simultanée maximum
	 */
	Sound(SourceConfigure* pConfig, uint32_t instanceMax = 4);
	//! Copie interdite
	Sound(const Sound& other) = delete;
	//! Copie interdite
	Sound& operator=(const Sound& other) = delete;
	/**
	 * @brief Destructeur
	 */
	virtual ~Sound() noexcept;

	/**
	 * @brief Permet de définir les données audio (avant initialisation)
	 * @param pData Données audio
	 */
	void setData(Data* pData, bool removeData) noexcept;

	/**
	 * @brief Permet de définir les données à partir d'un flux en wav
	 * @param file Flux à lire
	 */
	void setWav(std::iostream& file);

	/**
	 * @brief Permet de jouer le son
	 */
	void play();

	/**
	 * @brief Initialise le son
	 * @param forceLoad données audio
	 */
	void Init(bool forceLoad = false);

	/**
	 * @brief Permet de libérer la mémoire initialisée par #Init
	 */
	void Quit();

protected:
	void loadSource(SoundInstance instance);

private:
	Source* m_tblSources;
	Data* m_pData;
	SourceConfigure* m_pConfig;
	uint32_t m_uInstanceMax;
	SoundInstance m_uCurrent;
	bool m_removeData;
};

}

#endif // MULTISOURCE_H_INCLUDED

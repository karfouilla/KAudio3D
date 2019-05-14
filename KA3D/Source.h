#ifndef AUDIOSOURCE_H_INCLUDED
#define AUDIOSOURCE_H_INCLUDED
/**
 *
 * @file Source.h
 * @author karfouilla
 * @version 1.0Q
 * @date 27 avril 2019
 * @brief Fichier contenant la classe de source sonore (H)
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

#include "Data.h"

namespace KA3D
{

class DataPrivate;

/**
 * @brief Classe représentant une source audio
 */
class Source
{
public:
	/**
	 * @brief Constructeur
	 */
	Source();
	//! Copie interdite
	Source(const Source& other) noexcept = delete;
	//! Copie interdite
	Source& operator=(const Source& other) noexcept = delete;
	/**
	 * @brief Destructeur
	 */
	virtual ~Source() noexcept;

	/**
	 * @brief Initialise la source audio avec des données audio
	 * @param pData données audio
	 */
	void Init(Data* pData);

	/**
	 * @brief Permet de savoir si la source a été initialisée
	 */
	bool isInitialized() const noexcept;

	/**
	 * @brief Permet de libérer la mémoire initialisé par #Init
	 * @note Ne libère pas la mémoire de #AudioData en paramètre de #Init
	 */
	void Quit();
	/**
	 * @brief Permet de lire la source audio
	 */
	void play();
	/**
	 * @brief Permet de mettre en pause la source audio
	 */
	void pause();
	/**
	 * @brief Permet de stopper la lecture de la source audio
	 */
	void stop();
	/**
	 * @brief Remet au début la source audio (et l'arrête)
	 */
	void rewind();
	/**
	 * @brief Permet de définir la position de cette source
	 * @param xpos position x 3D de celui qui écoute
	 * @param ypos position y 3D de celui qui écoute
	 * @param zpos position z 3D de celui qui écoute
	 */
	void setPosition(float xpos, float ypos, float zpos);
	/**
	 * @brief Permet de définir la vitesse (vecteur vitesse) de cette source
	 * @param xvel x du vecteur vitesse de celui qui écoute
	 * @param yvel y du vecteur vitesse de celui qui écoute
	 * @param zvel z du vecteur vitesse de celui qui écoute
	 */
	void setVelocity(float xvel, float yvel, float zvel);
	/**
	 * @brief Permet de définir la direction de cette source
	 * @param xat x du vecteur représentant la direction de cette source
	 * @param yat y du vecteur représentant la direction de cette source
	 * @param zat z du vecteur représentant la direction de cette source
	 */
	void setDirection(float xat, float yat, float zat);
	/**
	 * @brief Permet de définir le pitch de la source (échantillon par seconde)
	 * @param factor facteur multiplicateur du pitch
	 */
	void setPitch(float factor);
	/**
	 * @brief Permet de définir le volume global de cette source
	 * Valeur de référence du volume :
	 * 0	volume minimum (pas de son)
	 * 1	volume standard
	 * > 1	amplification (peut conduire à une saturation du son)
	 * @param fGain Volume de la source
	 */
	void setGain(float fGain);
	/**
	 * @brief Définit la distance maximum d'atténuation (cf. #DistanceModel)
	 * @param fMaxDistance distance max d'atténuation
	 */
	void setMaxDistance(float fMaxDistance);
	/**
	 * @brief Définit le facteur d'atténuation (cf. #DistanceModel)
	 * @param fRollOff facteur d'atténuation
	 */
	void setRollOffFactor(float fRollOff);
	/**
	 * @brief Définit la distance de référence (cf. #DistanceModel)
	 * @param fRefDistance distance de référence
	 */
	void setReferenceDistance(float fRefDistance);

	/**
	 * @brief Volume minimum
	 * @param fMinGain volume minimum (entre 0 et 1)
	 */
	void setMinGain(float fMinGain);
	/**
	 * @brief Volume maximum
	 * @param fMinGain volume maximum (entre 0 et 1)
	 */
	void setMaxGain(float fMaxGain);
	/**
	 * @brief Volume en dehors du cône de son
	 * @param fConeOuterGain facteur de volume (entre 0 et 1)
	 */
	void setConeOuterGain(float fConeOuterGain);
	/**
	 * @brief Angle du volume de cône interne (volume max)
	 * Le cône est définit en 3 zones :
	 * Inner : facteur multiplicateur de volume à 1
	 * Milieu: facteur multiplicateur de volume intérpolé avec l'angle
	 * Outer : facteur multiplicateur de volume définit par #setConeOuterGain
	 * @param fConeInnerAngle angle interne en degrée
	 */
	void setConeInnerAngle(float fConeInnerAngle);
	/**
	 * @brief Angle du volume de cône interne (volume min)
	 * @param fConeInnerAngle angle interne en degrée
	 */
	void setConeOuterAngle(float fConeOuterAngle);
	/**
	 * @brief Permet de définir les positions comme étant relative au listener
	 */
	void setRelative(bool isRelative);
	/**
	 * @brief Définit la position dans l'audio
	 * @param second nombre de second
	 */
	void setOffsetSec(float second);
	/**
	 * @brief Définit la position dans l'audio
	 * @param second nombre d'échantillon
	 */
	void setOffset(quint32 sample);
	/**
	 * @brief Permet de définir si on revient au début quand la lecture est fini
	 */
	void setAutoLoop(bool isLooping);

	/**
	 * @brief Permet de définir la position de la source
	 * @param xpos position x 3D de la source
	 * @param ypos position y 3D de la source
	 * @param zpos position z 3D de la source
	 */
	void position(float& xpos, float& ypos, float& zpos) const;
	/**
	 * @brief Permet de définir la vitesse (vecteur vitesse) de la source
	 * @param xvel x du vecteur vitesse de la source
	 * @param yvel y du vecteur vitesse de la source
	 * @param zvel z du vecteur vitesse de la source
	 */
	void velocity(float& xvel, float& yvel, float& zvel) const;
	/**
	 * @brief Permet de définir la direction de la source
	 * @param xat x du vecteur direction de la source
	 * @param yat y du vecteur direction de la source
	 * @param zat z du vecteur direction de la source
	 */
	void direction(float& xat, float& yat, float& zat) const;

	/**
	 * @brief Permet d'obtenir le facteur multiplicateur du pitch
	 */
	float pitch() const;
	/**
	 * @brief Permet d'obtenir le volume de la source
	 */
	float gain() const;
	/**
	 * @brief Permet d'obtenir la distance maximum (cf. #DistanceModel)
	 */
	float maxDistance() const;
	/**
	 * @brief Permet d'obtenir le facteur d'aténuation (cf. #DistanceModel)
	 */
	float rollOffFactor() const;
	/**
	 * @brief Permet d'obtenir la distance de référence (cf. #DistanceModel)
	 */
	float referenceDistance() const;
	/**
	 * @brief Permet d'obtenir le volume minimum de la source
	 */
	float minGain() const;
	/**
	 * @brief Permet d'obtenir le volume maximum de la source
	 */
	float maxGain() const;
	/**
	 * @brief Permet d'obtenir le facteur du volume du cone externe
	 */
	float coneOuterGain() const;
	/**
	 * @brief Permet d'obtenir l'angle du cone interne
	 */
	float coneInnerAngle() const;
	/**
	 * @brief Permet d'obtenir l'angle du cone externe
	 */
	float coneOuterAngle() const;
	/**
	 * @brief Permet de savoir si la source est relative à l'écouteur
	 */
	bool isRelative() const;
	/**
	 * @brief Permet d'obtenir la position dans l'audio (en secondes)
	 */
	float offsetSec() const;
	/**
	 * @brief Permet d'obtenir la position dans l'audio (en échantillons)
	 */
	quint32 offset() const;
	/**
	 * @brief Permet de savoir si la lecture est en boucle
	 */
	bool isLooping() const;

	/**
	 * @brief Permet de savoir si la lecture est en cours
	 */
	bool isPlaying() const;
	/**
	 * @brief Permet de savoir si la lecture est en pause
	 */
	bool isPaused() const;
	/**
	 * @brief Permet de savoir si la lecture stoppé
	 */
	bool isStopped() const;
	/**
	 * @brief Permet de savoir si la lecture est dans l'état initial
	 */
	bool isInitial() const;

private:
	Data* m_pData; //!< Données de la source audio
	DataPrivate* m_pSource; //!< Contenu privé de la source audio
};

} // namespace KA3D

#endif // AUDIOSOURCE_H_INCLUDED

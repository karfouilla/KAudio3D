#ifndef AUDIOLISTENER_H_INCLUDED
#define AUDIOLISTENER_H_INCLUDED
/**
 *
 * @file AudioListener.h
 * @author karfouilla
 * @version 1.0
 * @date 27 avril 2019
 * @brief Contient la gestion de l'audio, représentant celui qui écoute (H)
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

class AudioContext;

//! Modèle d'atténuation des sons avec la distance
//! Pour plus d'information, se référer à la documentation d'OpenAL
enum DistanceModel {
	/**
	 * Aucune atténuation
	 * Le volume est le même quelque soit la distance (fonction constante)
	 * factor(dist) = 1
	 */
	DM_NONE,
	/**
	 * Atténuation avec la fonction inverse
	 * factor(dist) = REF / (REF + ROLLOFF * (dist - REF))
	 */
	DM_INVERSE,
	/**
	 * Atténuation avec la fonction inverse avec distance bloqué entre min/max
	 * factor(dist) = REF / (REF + ROLLOFF * (clamp(dist, REF, MAX) - REF))
	 */
	DM_INVERSE_CLAMPED,
	/**
	 * Atténuation linéaire (fonction affine)
	 * factor(dist) = (1 - ROLLOFF * (dist - REF) / (MAX - REF)
	 */
	DM_LINEAR,
	/**
	 * Atténuation linéaire (fonction affine) avec distance bloqué entre min/max
	 * factor(dist) = (1 - ROLLOFF * (clamp(dist, REF, MAX) - REF) / (MAX - REF)
	 */
	DM_LINEAR_CLAMPED,
	/**
	 * Atténuation exponentiel
	 * factor(dist) = (dist / REF)^(-ROLLOFF)
	 */
	DM_EXPONENT,
	/**
	 * Atténuation exponentiel avec distance bloqué entre min/max
	 * factor(dist) = (clamp(dist, REF, MAX) / REF)^(-ROLLOFF)
	 */
	DM_EXPONENT_CLAMPED,

	DM_LAST //!< Borne de fin de l'énumération (pas une valeur valide)
};

/**
 * @brief Classe représentant celui qui entend le son et le context audio
 */
class AudioListener
{
public:
	/**
	 * @brief Constructeur
	 * @param device périphérique de sortie du son
	 */
	AudioListener(const char* device = nullptr);
	//! Copie interdite
	AudioListener(AudioListener& other) noexcept = delete;
	//! Copie interdite
	AudioListener& operator=(AudioListener& other) noexcept = delete;
	/**
	 * @brief Destructeur
	 */
	~AudioListener() noexcept;

	/**
	 * @brief Initialise le context audio
	 * Cette fonction doit être appelé avant toutes opérations
	 */
	void Init();

	/**
	 * @brief Libère la mémoire du context audio
	 * Doit être appelé avant la destruction de la classe si #Init à été appelé
	 * Aucune opération ne peut être effectué après cette fonction
	 */
	void Quit();

	/**
	 * @brief Permet de définir le volume général de l'écouteur
	 * Valeur de référence du volume :
	 * 0	volume minimum (pas de son)
	 * 1	volume standard
	 * > 1	amplification (peut conduire à une saturation du son)
	 * @param gain Volume générale de l'écouteur
	 */
	void setGain(float gain);
	/**
	 * @brief Permet de définir la position de celui qui écoute
	 * @param xpos position x 3D de celui qui écoute
	 * @param ypos position y 3D de celui qui écoute
	 * @param zpos position z 3D de celui qui écoute
	 */
	void setPosition(float xpos, float ypos, float zpos);
	/**
	 * @brief Permet de définir la vitesse (vecteur vitesse) de celui qui écoute
	 * @param xvel x du vecteur vitesse de celui qui écoute
	 * @param yvel y du vecteur vitesse de celui qui écoute
	 * @param zvel z du vecteur vitesse de celui qui écoute
	 */
	void setVelocity(float xvel, float yvel, float zvel);
	/**
	 * @brief Permet de définir l'orientation de celui qui écoute
	 * @param xat x du vecteur représentant la direction de celui qui écoute
	 * @param yat y du vecteur représentant la direction de celui qui écoute
	 * @param zat z du vecteur représentant la direction de celui qui écoute
	 * @param xup x du vecteur représentant l'axe vertical
	 * @param yup y du vecteur représentant l'axe vertical
	 * @param zup z du vecteur représentant l'axe vertical
	 */
	void setOrientation(float xat, float yat, float zat,
	                    float xup, float yup, float zup);

	/**
	 * @brief Permet de définir le facteur d'éxagération de l'effet doppler
	 * @param factor facteur de l'effet doppler
	 */
	void setDopplerFactor(float factor);
	/**
	 * @brief Permet de définir la vitesse du son
	 * @param fSpeedSound Vitesse du son en "unité" par seconde
	 */
	void setSpeedSound(float fSpeedSound);
	/**
	 * @brief Permet de définir le modèle d'atténuation (voir #DistanceModel)
	 * @param model Type du modèle
	 */
	void setDistanceModel(DistanceModel model);

	/**
	 * @brief Permet d'obtenir le volume de l'écouteur
	 */
	float gain() const;
	/**
	 * @brief Permet d'obtenir la position de l'écouteur
	 * @param[out] xpos Coordonée x de la position
	 * @param[out] ypos Coordonée y de la position
	 * @param[out] zpos Coordonée z de la position
	 */
	void position(float& xpos, float& ypos, float& zpos) const;
	/**
	 * @brief Permet d'obtenir la vitesse (vecteur vitesse) de l'écouteur
	 * @param[out] xpos Coordonée x du vecteur vitesse
	 * @param[out] ypos Coordonée y du vecteur vitesse
	 * @param[out] zpos Coordonée z du vecteur vitesse
	 */
	void velocity(float& xvel, float& yvel, float& zvel) const;
	/**
	 * @brief Permet d'obtenir l'orientation de l'écouteur
	 * @param xat Coordonée x du vecteur direction
	 * @param yat Coordonée y du vecteur direction
	 * @param zat Coordonée z du vecteur direction
	 * @param xup Coordonée x de l'axe vertical
	 * @param yup Coordonée y de l'axe vertical
	 * @param zup Coordonée z de l'axe vertical
	 */
	void orientation(float& xat, float& yat, float& zat,
	                 float& xup, float& yup, float& zup) const;

	/**
	 * @brief Permet d'obtenir le facteur d'éxagération de l'effet doppler
	 */
	float dopplerFactor() const;
	/**
	 * @brief Permet d'obtenir la vitesse du son
	 */
	float speedSound() const;
	/**
	 * @brief Permet d'obtenir le modèle d'atténuation (voir #DistanceModel)
	 */
	DistanceModel distanceModel() const;

private:
	AudioContext* m_pData; //!< Données interne à la classe
};

#endif // AUDIOLISTENER_H_INCLUDED

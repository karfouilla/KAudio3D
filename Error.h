#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED
/**
 *
 * @file Error.h
 * @author karfouilla
 * @version 1.0Q
 * @date 27 avril 2019
 * @brief Contient la gestion des erreurs (H)
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

#include <stdexcept>

#include <AL/al.h>
#include <AL/alc.h>

#define ALIBTESTERR(ERRAL) case ERRAL: return #ERRAL

static inline std::string alErrorString(ALenum error)
{
	switch(error)
	{
	ALIBTESTERR(AL_INVALID_NAME);
	ALIBTESTERR(AL_INVALID_ENUM);
	ALIBTESTERR(AL_INVALID_VALUE);
	ALIBTESTERR(AL_INVALID_OPERATION);
	ALIBTESTERR(AL_OUT_OF_MEMORY);
	default:
		return "Unknow error";
	}
}

static inline void checkALError()
{
	ALenum error(alGetError());
	if(error != AL_NO_ERROR)
		throw std::runtime_error(alErrorString(error));
}

static inline std::string alcErrorString(ALCenum error)
{
	switch(error)
	{
	ALIBTESTERR(ALC_INVALID_DEVICE);
	ALIBTESTERR(ALC_INVALID_CONTEXT);
	ALIBTESTERR(ALC_INVALID_ENUM);
	ALIBTESTERR(ALC_INVALID_VALUE);
	ALIBTESTERR(ALC_OUT_OF_MEMORY);
	default:
		return "Unknow error";
	}
}

static inline void checkALCError(ALCdevice* device)
{
	ALCenum error(alcGetError(device));
	if(error != ALC_NO_ERROR)
		throw std::runtime_error(alcErrorString(error));
}

#endif // ERROR_H_INCLUDED

#ifndef AUDIODATAPRIVATE_H_INCLUDED
#define AUDIODATAPRIVATE_H_INCLUDED
/**
 *
 * @file DataPrivate.h
 * @author karfouilla
 * @version α.0.1
 * @date 27 avril 2019
 * @brief Fichier contenant la classe de gestion privée des données audio (H)
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

#include <AL/al.h>

namespace KA3D
{

class DataPrivate
{
public:
	DataPrivate() noexcept: handle(0) { }
	~DataPrivate() noexcept { }

public:
	ALuint handle;
};

} // namespace KA3D

#endif // AUDIODATAPRIVATE_H_INCLUDED

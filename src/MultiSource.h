#ifndef MULTISOURCE_H_INCLUDED
#define MULTISOURCE_H_INCLUDED
/**
 *
 * @file MultiSource.h
 * @author karfouilla
 * @version 1.0
 * @date 30 avril 2019
 * @brief Fichier contenant la classe de source sonore multiple (H)
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

namespace KA3D
{

class MultiSource
{
public:
	MultiSource();
	MultiSource(const MultiSource& rhs) = delete;
	virtual ~MultiSource() noexcept;
	MultiSource& operator=(const MultiSource& rhs) = delete;

private:

};

}

#endif // MULTISOURCE_H_INCLUDED

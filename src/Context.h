#ifndef AUDIOLISTENERDATA_H_INCLUDED
#define AUDIOLISTENERDATA_H_INCLUDED
/**
 *
 * @file Context.h
 * @author karfouilla
 * @version 1.0
 * @date 27 avril 2019
 * @brief Fichier contenant le contexte audio pour #Listener (H)
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

#include <AL/alc.h>

namespace KA3D
{
class Context
{
public:
	Context(const char* deviceName = nullptr);
	Context(Context& ) noexcept = delete;
	Context& operator=(Context& ) noexcept = delete;
	~Context() noexcept;

	ALCdevice* device() const noexcept;
	ALCcontext* context() const noexcept;

	void Init(const int* attributes);
	void Quit();

	void makeCurrent();
	static void clearCurrent();
	void suspend();
	void process();

private:
	ALCdevice* m_pDevices;
	ALCcontext* m_pContext;
	ALCchar* m_szDeviceName;
};
} // namespace KA3D

#endif // AUDIOLISTENERDATA_H_INCLUDED

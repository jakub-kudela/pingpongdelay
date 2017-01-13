// Ping Pong Delay 1.0.0.0, VST 2.4 plugin.
// Copyright (C) 2012  Jakub Kúdela
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


/**
 * Main.Cpp:
 *
 * Declaration of createEffectInstance(audioMasterCallback audioMaster)
 * returning new instance of PingPongDelayEffect, called by VST host.
 *
 * @author  Jakub Kúdela
 * @version 1.0
 * @since 2012-01-21
 *
 * @see PingPongDelayEffect
 */


#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "PingPongDelayEffect.h"


using namespace PingPongDelay;


/**
 * Creates an instance of Ping PongDelay Effect VST.
 * @param audioMaster an audio master for callbacks.
 * @return pointer to a new PingPongDelayEffect.
 */
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new PingPongDelayEffect(audioMaster);
}

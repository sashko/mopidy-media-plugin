/****************************************************************************
** Copyright (c) 2016 Alexandre Petitjean
** Copyright (c) 2019 Luxoft Sweden AB

** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:

** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.

** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "mixercontroller.h"

using namespace mopidy;

MixerController::MixerController()
    : BaseController("core.mixer")
{}

MixerController::~MixerController() {}

void MixerController::getMute()
{
    auto rhdl = [&](const QJsonValue &v) { emit muteReceived(v.toBool()); };
    sendRequest("get_mute", rhdl);
}

void MixerController::setMute(bool mute)
{
    auto rhdl = [&](const QJsonValue &v) { emit muteSetted(v.toBool()); };
    sendRequest("set_mute", rhdl, {{"mute", mute}});
}

void MixerController::getVolume()
{
    auto rhdl = [&](const QJsonValue &v) { emit volumeReceived(v.toInt()); };
    sendRequest("get_volume", rhdl);
}

void MixerController::setVolume(int volume)
{
    auto rhdl = [&](const QJsonValue &v) { emit volumeSetted(v.toBool()); };
    sendRequest("set_volume", rhdl, {{"volume", volume}});
}

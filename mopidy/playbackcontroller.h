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

#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include "basecontroller.h"
#include "models.h"

namespace mopidy {

class PlaybackController : public BaseController
{
    Q_OBJECT

public:
    explicit PlaybackController();
    ~PlaybackController();

    //
    void play();
    void play(const mopidy::TlTrack &tltrack);
    void play(int tlid);
    void next();
    void previous();
    void stop();
    void pause();
    void resume();
    void seek(int timePosition);

    // Current track
    void getCurrentTlTrack();
    void getCurrentTrack();
    void getStreamTitle();
    void getTimePosition();
    void getState();
    void setState(const mopidy::PlaybackState &state);

signals:
    void currentTlTrackReceived(const mopidy::TlTrack &tltrack);
    void currentTrackReceived(const mopidy::Track &track);
    void stateReceived(mopidy::PlaybackState state);
    void streamTitleReceived(const QString &title);
    void timePositionReceived(int tp);
    void seeked(bool ack);
};

} // namespace mopidy

#endif //PLAYBACKCONTROLLER_H

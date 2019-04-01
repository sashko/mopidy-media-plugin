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

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "jsonrpchandler.h"
#include "models.h"
#include <QObject>

namespace mopidy {

class EventHandlerPrivate;
class EventHandler : public QObject
{
    Q_OBJECT

public:
    EventHandler();
    ~EventHandler();

    void setJsonRpcHandler(QSharedPointer<mopidy::JsonRpcHandler> jsRpcHdl);

signals:
    void optionsChanged();
    void playbackStateChanged(mopidy::PlaybackState oldState, mopidy::PlaybackState newState);
    void playlistChanged(const mopidy::Playlist &playlist);
    void playlistDeleted(const QString &uri);
    void playlistsLoaded();
    void seeked(int time_position);
    void trackPlaybackEnded(const mopidy::TlTrack &tl_track, int time_position);
    void trackPlaybackPaused(const mopidy::TlTrack &tl_track, int time_position);
    void trackPlaybackResumed(const mopidy::TlTrack &tl_track, int time_position);
    void trackPlaybackStarted(const mopidy::TlTrack &tl_track);
    void tracklistChanged();
    void streamTitleChanged(const QString &title);

private:
    Q_DECLARE_PRIVATE(EventHandler)
    QScopedPointer<EventHandlerPrivate> d_ptr;
};

} // namespace mopidy

#endif //EVENTHANDLER_H

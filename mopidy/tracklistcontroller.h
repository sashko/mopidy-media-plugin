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

#ifndef TRACKLISTCONTROLLER_H
#define TRACKLISTCONTROLLER_H

#include "basecontroller.h"
#include "models.h"

namespace mopidy {

class TracklistController : public BaseController
{
    Q_OBJECT

public:
    explicit TracklistController();
    ~TracklistController();

    // Manipulating
    void add(const QStringList &uris, int atPosition = -1);
    void remove(const QMap<QString, QVariantList> &criteria);
    void clear();
    void move(int start, int end, int toPosition);
    void shuffle(int start = -1, int stop = -1);

    // Current state
    void getTlTracks();
    void index(const mopidy::TlTrack &tltrack);
    void index(int tlid);
    void index();
    void getVersion();
    void getLength();
    void getTracks();
    void slice(int start, int stop);
    void filter(const QMap<QString, QVariantList> &criteria);

    // Future state
    void eotTlId();
    void nextTlId();
    void previousTlId();
    void eotTrack(const mopidy::TlTrack &tltrack);
    void nextTrack(const mopidy::TlTrack &tltrack = mopidy::TlTrack());
    void previousTrack(const mopidy::TlTrack &tltrack = mopidy::TlTrack());

    // Options
    void getConsume();
    void setConsume(bool consume);
    void getRandom();
    void setRandom(bool random);
    void getRepeat();
    void setRepeat(bool repeat);
    void getSingle();
    void setSingle(bool single);

signals:
    void added(const mopidy::TlTracks &tltracks);
    void removed(const mopidy::TlTracks &tltracks);
    void tlTracksReceived(const mopidy::TlTracks &tltracks);
    void indexReceived(int index);
    void versionReceived(int version);
    void lengthReceived(int length);
    void tracksReceived(const mopidy::Tracks &tracks);
    void sliced(const mopidy::TlTrack &tltrack);
    void filtered(const mopidy::TlTracks &tltracks);
    void eotTlIdReceived(int tlid);
    void nextTlIdReceived(int tlid);
    void previousTlIdReceived(int tlid);
    void eotTrackReceived(const mopidy::TlTrack &tltrack);
    void nextTrackReceived(const mopidy::TlTrack &tltrack);
    void previousTrackReceived(const mopidy::TlTrack &tltrack);
    void consumeReceived(bool isConsume);
    void randomReceived(bool isRandom);
    void repeatReceived(bool isRepeat);
    void singleReceived(bool isSingle);
};

} // namespace mopidy

#endif //TRACKLISTCONTROLLER_H

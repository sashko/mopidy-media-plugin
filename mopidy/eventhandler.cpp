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

#include "eventhandler.h"
#include "eventhandler_p.h"

#include "modeltranslator.h"

using namespace mopidy;

EventHandlerPrivate::EventHandlerPrivate(EventHandler *parent)
    : q_ptr(parent)
{}

EventHandlerPrivate::~EventHandlerPrivate() {}

void EventHandlerPrivate::onEventReceived(const QString &event, const QJsonObject &params)
{
    Q_Q(EventHandler);

    if (event == "options_changed") {
        emit q->optionsChanged();
    } else if (event == "playback_state_changed") {
        QString os = params.value("old_state").toString();
        QString ns = params.value("new_state").toString();
        emit q->playbackStateChanged(ModelTranslator::getState(os), ModelTranslator::getState(ns));
    } else if (event == "playlist_changed") {
        Playlist pl;
        ModelTranslator::fromJson(params.value("playlist").toObject(), pl);
        emit q->playlistChanged(pl);
    } else if (event == "playlists_loaded") {
        emit q->playlistsLoaded();
    } else if (event == "playlist_deleted") {
        emit q->playlistDeleted(params.value("uri").toString());
    } else if (event == "seeked") {
        int tp = params.value("time_position").toDouble();
        emit q->seeked(tp);
    } else if (event == "track_playback_ended") {
        TlTrack tlt;
        ModelTranslator::fromJson(params.value("tl_track").toObject(), tlt);
        int tp = params.value("time_position").toDouble();
        emit q->trackPlaybackEnded(tlt, tp);
    } else if (event == "track_playback_paused") {
        TlTrack tlt;
        ModelTranslator::fromJson(params.value("tl_track").toObject(), tlt);
        int tp = params.value("time_position").toDouble();
        emit q->trackPlaybackPaused(tlt, tp);
    } else if (event == "track_playback_resumed") {
        TlTrack tlt;
        ModelTranslator::fromJson(params.value("tl_track").toObject(), tlt);
        int tp = params.value("time_position").toDouble();
        emit q->trackPlaybackResumed(tlt, tp);
    } else if (event == "track_playback_started") {
        TlTrack tlt;
        ModelTranslator::fromJson(params.value("tl_track").toObject(), tlt);
        emit q->trackPlaybackStarted(tlt);
    } else if (event == "tracklist_changed") {
        emit q->tracklistChanged();
    } else if (event == "stream_title_changed") {
        emit q->streamTitleChanged(params.value("title").toString());
    } else {
        qDebug() << __FUNCTION__ << QObject::tr("Unhandled event:") << event;
    }
}

EventHandler::EventHandler()
    : d_ptr(new EventHandlerPrivate(this))
{}

EventHandler::~EventHandler() {}

void EventHandler::setJsonRpcHandler(QSharedPointer<JsonRpcHandler> jsRpcHdl)
{
    Q_D(EventHandler);
    d->jsRpcHdl = jsRpcHdl;
    if (d->jsRpcHdl)
        connect(d->jsRpcHdl.data(),
                &JsonRpcHandler::eventReceived,
                d,
                &EventHandlerPrivate::onEventReceived);
}

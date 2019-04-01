/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "mediaplayerbackend.h"

#include <QDebug>

MediaPlayerBackend::MediaPlayerBackend(QSharedPointer<mopidy::JsonRpcHandler> jsonRpcHandler,
                                       QObject *parent)
    : QIviMediaPlayerBackendInterface(parent)
{
    m_eventHandler.setJsonRpcHandler(jsonRpcHandler);
    m_mixerController.setJsonRpcHandler(jsonRpcHandler);
    m_playbackController.setJsonRpcHandler(jsonRpcHandler);
    m_tracklistController.setJsonRpcHandler(jsonRpcHandler);

    // playback
    connect(&m_playbackController,
            &mopidy::PlaybackController::currentTlTrackReceived,
            this,
            &MediaPlayerBackend::onCurrentTlTrackReceived);
    connect(&m_playbackController,
            &mopidy::PlaybackController::stateReceived,
            this,
            &MediaPlayerBackend::onStateReceived);
    connect(&m_eventHandler,
            &mopidy::EventHandler::playbackStateChanged,
            this,
            &MediaPlayerBackend::onPlaybackStateChanged);
    connect(&m_eventHandler, &mopidy::EventHandler::seeked, this, &MediaPlayerBackend::onSeeked);
    connect(&m_eventHandler,
            &mopidy::EventHandler::trackPlaybackStarted,
            this,
            &MediaPlayerBackend::onTrackPlaybackStarted);
    connect(&m_eventHandler,
            &mopidy::EventHandler::tracklistChanged,
            this,
            &MediaPlayerBackend::onTracklistChanged);

    // tracklist
    connect(&m_tracklistController,
            &mopidy::TracklistController::indexReceived,
            this,
            &MediaPlayerBackend::onCurrentIndexReceived);
    connect(&m_tracklistController,
            &mopidy::TracklistController::tracksReceived,
            this,
            &MediaPlayerBackend::onTracksReceivedInitial);
    connect(&m_tracklistController,
            &mopidy::TracklistController::singleReceived,
            this,
            &MediaPlayerBackend::onSingleReceived);
    connect(&m_tracklistController,
            &mopidy::TracklistController::repeatReceived,
            this,
            &MediaPlayerBackend::onRepeatReceived);
    connect(&m_tracklistController,
            &mopidy::TracklistController::randomReceived,
            this,
            &MediaPlayerBackend::onRandomReceived);
    m_tracklistController.getTracks();
    m_tracklistController.index();

    // mixer
    connect(&m_mixerController,
            &mopidy::MixerController::muteReceived,
            this,
            &MediaPlayerBackend::onMuteReceived);
    connect(&m_mixerController,
            &mopidy::MixerController::volumeReceived,
            this,
            &MediaPlayerBackend::onVolumeReceived);
}

void MediaPlayerBackend::initialize()
{
    qDebug() << "initialize";
    emit initializationDone();
}

bool MediaPlayerBackend::canReportCount()
{
    return true;
}

void MediaPlayerBackend::fetchData(int start, int count)
{
    qDebug() << "fetchData";
    //    m_tracklistController.getTracks();

    //    connect(&m_tracklistController, &mopidy::TracklistController::tracksReceived,
    //            [this, start, count](const mopidy::Tracks &tracks) {
    //        qDebug() << "fetchData processing";
    //        m_trackList = tracks;
    //        emit dataChanged(list, start, m_trackList.count());
    //updateTracklist(start, count, OperationType::Select);
    //    });
}

void MediaPlayerBackend::insert(int index, const QIviPlayableItem *item)
{
    qDebug() << "insert: " << index;
}

void MediaPlayerBackend::remove(int index)
{
    qDebug() << "remove: " << index;
}

void MediaPlayerBackend::move(int cur_index, int new_index)
{
    qDebug() << "move";
    m_tracklistController.move(cur_index, cur_index, new_index);
}

void MediaPlayerBackend::next()
{
    qDebug() << "next";
    m_playbackController.next();
    m_tracklistController.index();
}

void MediaPlayerBackend::pause()
{
    qDebug() << "pause";
    m_playbackController.pause();
}

void MediaPlayerBackend::play()
{
    qDebug() << "play";
    m_playbackController.play();
    m_tracklistController.index();
}

void MediaPlayerBackend::previous()
{
    qDebug() << "previous";
    m_playbackController.previous();
    m_tracklistController.index();
}

void MediaPlayerBackend::seek(qint64 offset)
{
    qDebug() << "seek";
    connect(&m_playbackController,
            &mopidy::PlaybackController::timePositionReceived,
            [this, offset](int currentPosition) {
                m_playbackController.seek(currentPosition + offset);
            });
}

// TODO doesn't work
// prbly coz TLID is unique for mopidy
void MediaPlayerBackend::setCurrentIndex(int index)
{
    qDebug() << "setCurrentIndex: " << index;
    m_tracklistController.index(index);
}

void MediaPlayerBackend::setMuted(bool muted)
{
    m_mixerController.setMute(muted);
}

void MediaPlayerBackend::setPlayMode(QIviMediaPlayer::PlayMode playMode)
{
    switch (playMode) {
    case QIviMediaPlayer::Normal:
        m_tracklistController.setSingle(false);
        m_tracklistController.setRepeat(false);
        m_tracklistController.setRandom(false);
        break;

    case QIviMediaPlayer::RepeatTrack:
        m_tracklistController.setSingle(true);
        m_tracklistController.setRepeat(true);
        m_tracklistController.setRandom(false);
        break;

    case QIviMediaPlayer::RepeatAll:
        m_tracklistController.setSingle(false);
        m_tracklistController.setRepeat(true);
        m_tracklistController.setRandom(false);
        break;

    case QIviMediaPlayer::Shuffle:
        m_tracklistController.setSingle(false);
        m_tracklistController.setRepeat(false);
        m_tracklistController.setRandom(true);
        break;
    };
}

void MediaPlayerBackend::setPosition(qint64 position)
{
    qDebug() << "setPosition";
    m_playbackController.seek(position);
}

void MediaPlayerBackend::setVolume(int volume)
{
    qDebug() << "setVolume";
    m_mixerController.setVolume(volume);
}

void MediaPlayerBackend::stop()
{
    qDebug() << "stop";
    m_playbackController.stop();
}

/*
 * Mopidy playback calls
*/

void MediaPlayerBackend::onCurrentTlTrackReceived(const mopidy::TlTrack &tlTrack)
{
    qDebug() << "onCurrentTlTrackReceived";
    m_tracklistController.index();
}

void MediaPlayerBackend::onStateReceived(mopidy::PlaybackState state)
{
    qDebug() << "onStateReceived";
    switch (state) {
    case mopidy::PlaybackState::Paused:
        m_playbackState = QIviMediaPlayer::Paused;
        m_playbackController.getCurrentTlTrack();
        break;

    case mopidy::PlaybackState::Playing:
        m_playbackState = QIviMediaPlayer::Playing;
        m_playbackController.getCurrentTlTrack();
        m_playbackController.getTimePosition();
        break;

    case mopidy::PlaybackState::Stopped:
        m_playbackState = QIviMediaPlayer::Stopped;
        emit positionChanged(0);
        break;
    }

    emit playStateChanged(m_playbackState);
}

void MediaPlayerBackend::onPlaybackStateChanged(mopidy::PlaybackState oldState,
                                                mopidy::PlaybackState newState)
{
    qDebug() << "onPlaybackStateChanged";
    switch (newState) {
    case mopidy::PlaybackState::Paused:
        m_playbackState = QIviMediaPlayer::Paused;
        m_playbackController.getCurrentTlTrack();
        break;

    case mopidy::PlaybackState::Playing:
        m_playbackState = QIviMediaPlayer::Playing;
        m_playbackController.getCurrentTlTrack();
        m_playbackController.getTimePosition();
        break;

    case mopidy::PlaybackState::Stopped:
        m_playbackState = QIviMediaPlayer::Stopped;
        emit positionChanged(0);
        break;
    }

    emit playStateChanged(m_playbackState);
}

void MediaPlayerBackend::onSeeked(int timePosition)
{
    qDebug() << "onSeeked";
    emit positionChanged(timePosition);
}

void MediaPlayerBackend::onTrackPlaybackStarted(const mopidy::TlTrack &tlTrack)
{
    qDebug() << "onEhTrackPlaybackStarted";
    m_tracklistController.index();
}

void MediaPlayerBackend::onTracklistChanged()
{
    qDebug() << "onTracklistChanged";
    m_tracklistController.getTracks();
}

/*
 * Mopidy tacklist callbacks
*/

void MediaPlayerBackend::onCurrentIndexReceived(int index)
{
    qDebug() << "onCurrentIndexReceived";

    qDebug() << "current index: " << index;

    m_currentIndex = index;

    emit currentIndexChanged(m_currentIndex);
    emit currentTrackChanged(QVariant());
    emit durationChanged(0);
}

void MediaPlayerBackend::onTracksReceivedInitial(const mopidy::Tracks &tracks)
{
    qDebug() << "onTracksReceivedInitial";
    int id = 0;
    m_trackList.clear();
//    emit countChanged(m_trackList.count());
    emit dataFetched(m_trackList, 0, false);

    // disconnect as later this signal will be handled by onTracksReceived()
    disconnect(&m_tracklistController,
               &mopidy::TracklistController::tracksReceived,
               this,
               &MediaPlayerBackend::onTracksReceivedInitial);

    // TODO: from and until count
    for (auto track : tracks) {
        QIviAudioTrackItem item;
        qDebug() << "adding: " << track.name;

        item.setId(QString(id++));
        item.setTitle(track.name);
        item.setArtist("artist");
        item.setAlbum(track.album.name);
        item.setUrl(track.uri);

        m_trackList.append(QVariant::fromValue(item));
    }

    emit countChanged(m_trackList.count());
    emit dataFetched(m_trackList, 0, false);

    connect(&m_tracklistController,
            &mopidy::TracklistController::tracksReceived,
            this,
            &MediaPlayerBackend::onTracksReceived);
}

void MediaPlayerBackend::onTracksReceived(const mopidy::Tracks &tracks)
{
    qDebug() << "OnTracksReceived()";
    int id = 0;
    m_trackList.clear();
    emit countChanged(m_trackList.count());
    emit dataFetched(m_trackList, 0, false);

    // TODO: from and until count
    for (auto track : tracks) {
        QIviAudioTrackItem item;
        qDebug() << "adding: " << track.name;

        item.setId(QString(id++));
        item.setTitle(track.name);
        item.setArtist("artist");
        item.setAlbum(track.album.name);
        item.setUrl(track.uri);

        m_trackList.append(QVariant::fromValue(item));
    }

//    emit countChanged(m_trackList.count());
    emit dataFetched(m_trackList, 0, false);
}

void MediaPlayerBackend::changePlayMode()
{
    QIviMediaPlayer::PlayMode playMode = QIviMediaPlayer::Normal;

    if (!m_singleMode && !m_repeatMode && m_randomMode)
        playMode = QIviMediaPlayer::Shuffle;
    else if (m_singleMode && m_repeatMode && !m_randomMode)
        playMode = QIviMediaPlayer::RepeatTrack;
    else if (!m_singleMode && m_repeatMode && !m_randomMode)
        playMode = QIviMediaPlayer::RepeatAll;

    emit playModeChanged(playMode);
}

void MediaPlayerBackend::onSingleReceived(bool isSingle)
{
    qDebug() << "onSingleReceived";
    m_singleMode = isSingle;
    changePlayMode();
}

void MediaPlayerBackend::onRepeatReceived(bool isRepeat)
{
    qDebug() << "onRepeatReceived";
    m_repeatMode = isRepeat;
    changePlayMode();
}

void MediaPlayerBackend::onRandomReceived(bool isRandom)
{
    qDebug() << "onRandomReceived";
    m_randomMode = isRandom;
    changePlayMode();
}

/*
 * Mixer callbacks
*/

void MediaPlayerBackend::onMuteReceived(bool isMuted)
{
    qDebug() << "onMuteReceive";
    emit setMuted(isMuted);
}

void MediaPlayerBackend::onVolumeReceived(int volume)
{
    qDebug() << "onVolumeReceived";
    emit setVolume(volume);
}

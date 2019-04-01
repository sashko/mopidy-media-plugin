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

#ifndef MEDIAPLAYERBACKEND_MOPIDY_H
#define MEDIAPLAYERBACKEND_MOPIDY_H

#include "mediaplugin.h"

#include "mopidy/eventhandler.h"
#include "mopidy/jsonrpchandler.h"
#include "mopidy/mixercontroller.h"
#include "mopidy/playbackcontroller.h"
#include "mopidy/tracklistcontroller.h"

#include <QtIviMedia/QIviMediaPlayerBackendInterface>

#include <QtConcurrent/QtConcurrent>
#include <QtMultimedia/QMediaPlayer>
#include <QtWebSockets/QtWebSockets>

class MediaPlayerBackend : public QIviMediaPlayerBackendInterface
{
    Q_OBJECT

public:
    MediaPlayerBackend(QSharedPointer<mopidy::JsonRpcHandler> jsonRpcHandler,
                       QObject *parent = nullptr);

    bool canReportCount() override;
    void fetchData(int start, int count) override;
    void insert(int index, const QIviPlayableItem *item) override;
    void remove(int index) override;
    void move(int cur_index, int new_index) override;
    void next() override;
    void pause() override;
    void play() override;
    void previous() override;
    void seek(qint64 offset) override;
    void setCurrentIndex(int index) override;
    void setMuted(bool muted) override;
    void setPlayMode(QIviMediaPlayer::PlayMode playMode) override;
    void setPosition(qint64 position) override;
    void setVolume(int volume) override;
    void stop() override;

    void initialize() override;

private:
    mopidy::EventHandler m_eventHandler;
    mopidy::MixerController m_mixerController;
    mopidy::PlaybackController m_playbackController;
    mopidy::TracklistController m_tracklistController;

    QIviMediaPlayer::PlayState m_playbackState;

    QVariantList m_trackList;

    bool m_repeatMode;
    bool m_randomMode;
    bool m_singleMode;

    int m_currentIndex;

    // Mopidy playback callbacks
    void onCurrentTlTrackReceived(const mopidy::TlTrack &tlTrack);
    void onPlaybackStateChanged(mopidy::PlaybackState oldState, mopidy::PlaybackState newState);
    void onSeeked(int timePosition);
    void onTrackPlaybackStarted(const mopidy::TlTrack &tlTrack);
    void onTracklistChanged();
    void onStateReceived(mopidy::PlaybackState state);

    // Mopidy tracklist callbacks
    void onTracksReceivedInitial(const mopidy::Tracks &tracks);
    void onTracksReceived(const mopidy::Tracks &tracks);
    void onCurrentIndexReceived(int index);
    void onSingleReceived(bool isSingle);
    void onRepeatReceived(bool isRepeat);
    void onRandomReceived(bool isRandom);
    void changePlayMode();

    // Mopidy mixer callbacks
    void onMuteReceived(bool isMuted);
    void onVolumeReceived(int volume);
};

#endif // MEDIAPLAYERBACKEND_MOPIDY_H

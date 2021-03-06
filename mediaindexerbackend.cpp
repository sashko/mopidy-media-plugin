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

#include "mediaindexerbackend.h"
#include "logging.h"

#include <QtConcurrent/QtConcurrent>

#include <QDirIterator>
#include <QImage>
#include <QStandardPaths>
#include <QThreadPool>
#include <QtDebug>

#ifndef QTIVI_NO_TAGLIB
#include <attachedpictureframe.h>
#include <fileref.h>
#include <id3v2frame.h>
#include <id3v2header.h>
#include <id3v2tag.h>
#include <mpegfile.h>
#include <tag.h>
#include <taglib.h>
#include <tstring.h>
#endif

MediaIndexerBackend::MediaIndexerBackend(QSharedPointer<mopidy::JsonRpcHandler> jsonRpcHandler,
                                         QObject *parent)
    : QIviMediaIndexerControlBackendInterface(parent)
    , m_state(QIviMediaIndexerControl::Idle)
    , m_threadPool(new QThreadPool(this))
{
    m_threadPool->setMaxThreadCount(1);
    m_tracklistController.setJsonRpcHandler(jsonRpcHandler);

    connect(&m_watcher, &QFutureWatcherBase::finished, this, &MediaIndexerBackend::onScanFinished);

    QStringList mediaFolderList;
    const QByteArray customMediaFolder = qgetenv("QTIVIMEDIA_SIMULATOR_LOCALMEDIAFOLDER");
    if (!customMediaFolder.isEmpty()) {
        qCInfo(media) << "QTIVIMEDIA_SIMULATOR_LOCALMEDIAFOLDER environment variable is set to:"
                      << customMediaFolder;
        mediaFolderList.append(customMediaFolder);
    } else {
        mediaFolderList = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
        qCInfo(media) << "Searching for music files in the following locations: "
                      << mediaFolderList;
    }

#ifdef QTIVI_NO_TAGLIB
    qCCritical(media) << "The indexer simulation doesn't work without an installed taglib";
#endif

    //We want to have the indexer running also when the Indexing interface is not used.
    for (const QString &folder : qAsConst(mediaFolderList))
        addMediaFolder(folder);
}

void MediaIndexerBackend::initialize()
{
    emit stateChanged(m_state);
    emit initializationDone();
}

void MediaIndexerBackend::pause()
{
    static const QLatin1String error("SIMULATION: Pausing the indexing is not supported");
    qCWarning(media) << error;
    emit errorChanged(QIviAbstractFeature::InvalidOperation, error);
}

void MediaIndexerBackend::resume()
{
    static const QLatin1String error("SIMULATION: Resuming the indexing is not supported");
    qCWarning(media) << error;
    emit errorChanged(QIviAbstractFeature::InvalidOperation, error);
}

void MediaIndexerBackend::addMediaFolder(const QString &path)
{
    ScanData data;
    data.remove = false;
    data.folder = path;
    m_folderQueue.append(data);

    scanNext();
}

void MediaIndexerBackend::removeMediaFolder(const QString &path)
{
    ScanData data;
    data.remove = true;
    data.folder = path;
    m_folderQueue.append(data);

    scanNext();
}

bool MediaIndexerBackend::scanWorker(const QString &mediaDir, bool removeData)
{
    setState(QIviMediaIndexerControl::Active);

    qCInfo(media) << "Scanning path: " << mediaDir;

    QStringList mediaFiles{QStringLiteral("*.mp3")};

    QVector<QString> files;
    QDirIterator it(mediaDir, mediaFiles, QDir::Files, QDirIterator::Subdirectories);
    qCInfo(media) << "Calculating total file count";

    while (it.hasNext())
        files.append(it.next());

    int totalFileCount = files.size();
    qCInfo(media) << "total files: " << totalFileCount;
    int currentFileIndex = 0;

    QStringList uris;

    // Clear tracklist
    m_tracklistController.clear();

    for (const QString &fileName : qAsConst(files)) {
        qCInfo(media) << "Processing file:" << fileName;

        QString fName = "file://" + fileName;
        uris.append(fName);

        if (qApp->closingDown())
            return false;

        QString defaultCoverArtUrl = fileName + QStringLiteral(".png");
        QString coverArtUrl;
#ifndef QTIVI_NO_TAGLIB
        TagLib::FileRef f(TagLib::FileName(QFile::encodeName(fileName)));
        if (f.isNull())
            continue;
        QString trackName = TStringToQString(f.tag()->title());
        QString albumName = TStringToQString(f.tag()->album());
        QString artistName = TStringToQString(f.tag()->artist());
        QString genre = TStringToQString(f.tag()->genre());
        unsigned int number = f.tag()->track();

        // Extract cover art
        if (fileName.endsWith(QLatin1String("mp3"))) {
            auto *file = static_cast<TagLib::MPEG::File *>(f.file());
            TagLib::ID3v2::Tag *tag = file->ID3v2Tag(true);
            TagLib::ID3v2::FrameList frameList = tag->frameList("APIC");

            if (frameList.isEmpty()) {
                qCWarning(media) << "No cover art was found";
            } else if (!QFile::exists(defaultCoverArtUrl)) {
                auto *coverImage = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(
                    frameList.front());

                QImage coverQImg;
                coverArtUrl = defaultCoverArtUrl;

                coverQImg.loadFromData((const uchar *) coverImage->picture().data(),
                                       coverImage->picture().size());
                coverQImg.save(coverArtUrl, "PNG");
            } else {
                coverArtUrl = defaultCoverArtUrl;
            }
        }

#endif // QTIVI_NO_TAGLIB
        emit progressChanged(qreal(++currentFileIndex) / qreal(totalFileCount));
    }

    m_tracklistController.add(uris, 0);
}

void MediaIndexerBackend::onScanFinished()
{
    if (!m_folderQueue.isEmpty()) {
        scanNext();
        return;
    }

    qCInfo(media) << "Scanning done";
#ifdef QTIVI_NO_TAGLIB
    qCCritical(media) << "No data was added, this is just a simulation";
#endif
    emit progressChanged(1);
    emit indexingDone();

    //If the last run didn't succeed we will stay in the Error state
    if (m_watcher.future().result())
        setState(QIviMediaIndexerControl::Idle);
}

void MediaIndexerBackend::scanNext()
{
    if (m_watcher.isRunning())
        return;

    ScanData data = m_folderQueue.dequeue();
    m_currentFolder = data.folder;
    m_watcher.setFuture(
        QtConcurrent::run(this, &MediaIndexerBackend::scanWorker, m_currentFolder, data.remove));
}

void MediaIndexerBackend::setState(QIviMediaIndexerControl::State state)
{
    m_state = state;
    emit stateChanged(state);
}

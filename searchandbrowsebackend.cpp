/****************************************************************************
**
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

#include "searchandbrowsebackend.h"

#include <QDebug>

#include <mopidy/models.h>

static const QString artistLiteral = QStringLiteral("artist");
static const QString albumLiteral = QStringLiteral("album");
static const QString trackLiteral = QStringLiteral("track");

SearchAndBrowseBackend::SearchAndBrowseBackend(QSharedPointer<mopidy::JsonRpcHandler> jrhdl, QObject *parent)
    : QIviSearchAndBrowseModelInterface(parent)
{
    qRegisterMetaType<SearchAndBrowseItem>();
    registerContentType<SearchAndBrowseItem>(artistLiteral);
    registerContentType<SearchAndBrowseItem>(albumLiteral);
    registerContentType<QIviAudioTrackItem>(trackLiteral);

    m_libraryCtrl.setJsonRpcHandler(jrhdl);
    connect(&m_libraryCtrl, &mopidy::LibraryController::searchResult, this, &SearchAndBrowseBackend::onLcSearchResult);
}

void SearchAndBrowseBackend::initialize()
{
    emit initializationDone();
}

QString SearchAndBrowseBackend::fieldFromType(const QString &type)
{
    if (type == artistLiteral)
        return QStringLiteral("artist");
    else if (type == albumLiteral)
        return QStringLiteral("album");
    else if (type == trackLiteral)
        return QStringLiteral("track_name");
    else
        return type;
}

void SearchAndBrowseBackend::fetchData(const QUuid &identifier, const QString &type, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms, int start, int count)
{
    Q_UNUSED(orderTerms) // TODO: TBD in post-processing ?
    // The usage of get_distinct would appear to be very convenient for artists and albums retrieval but
    // it turns out that it is not supported over HTTP https://github.com/mopidy/mopidy-spotify/issues/126

    emit supportedCapabilitiesChanged(identifier, QIviSearchAndBrowseModel::Capabilities(
                                          QIviSearchAndBrowseModel::SupportsFiltering |
                                          QIviSearchAndBrowseModel::SupportsSorting |
                                          QIviSearchAndBrowseModel::SupportsAndConjunction |
                                          QIviSearchAndBrowseModel::SupportsOrConjunction |
                                          QIviSearchAndBrowseModel::SupportsStatelessNavigation |
                                          QIviSearchAndBrowseModel::SupportsGetSize
                                          ));

    //Determine the current type and which items got selected previously to define the base filter.
    QHash<QString, QStringList> query;
    QStringList types = type.split('/');
    for (const QString &filter_type : types) {
        QStringList parts = filter_type.split('?');
        if (parts.count() != 2)
            continue;

        QString filter = QString::fromUtf8(QByteArray::fromBase64(parts.at(1).toUtf8(), QByteArray::Base64UrlEncoding));

        query.insert(fieldFromType(parts.at(0)), QStringList(filter));
    }
    QString current_type = types.last();

/*    QString filterClause = createWhereClause(current_type, term);
    if (!filterClause.isEmpty())
        where_clauses.append(filterClause); */

    m_typeOfFetch.insert(identifier, current_type);
    m_libraryCtrl.search(query, {"local:"}, false, identifier);
}

void SearchAndBrowseBackend::onLcSearchResult(const QUuid &identifier, const mopidy::SearchResult &sr)
{
    QVariantList list;
    QString type = m_typeOfFetch.value(identifier);

    QSet<QString> retSet;
    for (int i = 0; i < sr.tracks.size(); ++i) {
        mopidy::Track tr = sr.tracks.at(i);
        QString artist = tr.artists.at(0).name;
        QString album = tr.album.name;

        if (type == trackLiteral) {
            QIviAudioTrackItem item;
            item.setId(tr.uri);
            item.setTitle(tr.name);
            item.setArtist(artist);
            item.setAlbum(album);
            item.setUrl(QUrl::fromLocalFile(tr.uri));
            item.setCoverArtUrl(QUrl::fromLocalFile(""));
            list.append(QVariant::fromValue(item));
        } else {
            SearchAndBrowseItem item;
            item.setType(type);
            if (type == artistLiteral && !retSet.contains(artist)) {
                item.setName(artist);
                item.setData(QVariantMap{{"coverArtUrl", QUrl::fromLocalFile("")}});
                retSet.insert(artist);
                list.append(QVariant::fromValue(item));
            } else if (type == albumLiteral && !retSet.contains(album)) {
                item.setName(album);
                item.setData(QVariantMap{{"artist", artist},
                                         {"coverArtUrl", QUrl::fromLocalFile("")}
                                         });
                retSet.insert(album);
                list.append(QVariant::fromValue(item));
            }
        }
    }

    emit countChanged(identifier, list.count());
    emit dataFetched(identifier, list, 0, false);
}

bool SearchAndBrowseBackend::canGoBack(const QUuid &identifier, const QString &type)
{
    Q_UNUSED(identifier)
    return type.split('/').count() >= 2;
}

QString SearchAndBrowseBackend::goBack(const QUuid &identifier, const QString &type)
{
    Q_UNUSED(identifier)
    QStringList types = type.split('/');

    if (types.count() < 2)
        return QString();

    types.removeLast();
    types.replace(types.count() - 1, types.at(types.count() - 1).split('?').at(0));

    return types.join('/');
}

bool SearchAndBrowseBackend::canGoForward(const QUuid &identifier, const QString &type, const QString &itemId)
{
    return !goForward(identifier, type, itemId).isEmpty();
}

QString SearchAndBrowseBackend::goForward(const QUuid &identifier, const QString &type, const QString &itemId)
{
    Q_UNUSED(identifier)
    QStringList types = type.split('/');
    QString current_type = types.last();
    QString new_type = type + QStringLiteral("?%1").arg(QLatin1String(itemId.toUtf8().toBase64(QByteArray::Base64UrlEncoding)));

    if (current_type == artistLiteral)
        new_type += QLatin1String("/album");
    else if (current_type == albumLiteral)
        new_type += QLatin1String("/track");
    else
        return QString();

    return new_type;
}

QIviPendingReply<void> SearchAndBrowseBackend::insert(const QUuid &identifier, const QString &type, int index, const QIviSearchAndBrowseModelItem *item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(index)
    Q_UNUSED(item)

    return QIviPendingReply<void>::createFailedReply();
}

QIviPendingReply<void> SearchAndBrowseBackend::remove(const QUuid &identifier, const QString &type, int index)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(index)

    return QIviPendingReply<void>::createFailedReply();
}

QIviPendingReply<void> SearchAndBrowseBackend::move(const QUuid &identifier, const QString &type, int currentIndex, int newIndex)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(currentIndex)
    Q_UNUSED(newIndex)

    return QIviPendingReply<void>::createFailedReply();
}

QIviPendingReply<int> SearchAndBrowseBackend::indexOf(const QUuid &identifier, const QString &type, const QIviSearchAndBrowseModelItem *item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(item)

    return QIviPendingReply<int>::createFailedReply();
}

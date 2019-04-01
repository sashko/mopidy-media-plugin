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

#include "modeltranslator.h"

#include <QDebug>

#define MOPIDY_SHORT_DATE QString("yyyy")
#define MOPIDY_LONG_DATE QString("yyyy-MM-dd")

using namespace mopidy;

QJsonObject ModelTranslator::toJson(const Artist &artist)
{
    QJsonObject jo;
    jo.insert("__model__", QString("Artist"));
    jo.insert("uri", artist.uri);
    jo.insert("name", artist.name);
    jo.insert("sortname", artist.sortname);
    jo.insert("musicbrainz_id", artist.musicbrainz_id);
    return jo;
}

QJsonObject ModelTranslator::toJson(const Album &album)
{
    QJsonObject jo;
    jo.insert("__model__", QString("Album"));
    jo.insert("uri", album.uri);
    jo.insert("artists", toJsonArray<Artist>(album.artists));
    jo.insert("date", toMopidyDate(album.date));
    jo.insert("musicbrainz_id", album.musicbrainz_id);
    jo.insert("name", album.name);
    jo.insert("num_discs", album.num_discs);
    jo.insert("num_tracks", album.num_tracks);
    return jo;
}

QJsonObject ModelTranslator::toJson(const Track &track)
{
    QJsonObject jo;
    jo.insert("__model__", QString("Track"));
    jo.insert("uri", track.uri);
    jo.insert("album", toJson(track.album));
    jo.insert("artists", toJsonArray<Artist>(track.artists));
    jo.insert("bitrate", track.bitrate);
    jo.insert("date", toMopidyDate(track.date));
    jo.insert("disc_no", track.disc_no);
    jo.insert("length", track.length);
    jo.insert("musicbrainz_id", track.musicbrainz_id);
    jo.insert("name", track.name);
    jo.insert("track_no", track.track_no);
    return jo;
}

QJsonObject ModelTranslator::toJson(const Playlist &playlist)
{
    QJsonObject jo;
    jo.insert("__model__", QString("Playlist"));
    jo.insert("uri", playlist.uri);
    jo.insert("name", playlist.name);
    jo.insert("last_modified", playlist.last_modified.toMSecsSinceEpoch());
    jo.insert("tracks", toJsonArray<Track>(playlist.tracks));
    return jo;
}

QJsonObject ModelTranslator::toJson(const TlTrack &tltrack)
{
    QJsonObject jo;
    jo.insert("__model__", QString("TlTrack"));
    jo.insert("tlid", tltrack.tlid);
    jo.insert("track", toJson(tltrack.track));
    return jo;
}

/*
 * Decoders
 */
bool ModelTranslator::fromJson(const QJsonObject &jo, Artist &artist)
{
    if (jo.value("__model__").toString() == "Artist") {
        artist.uri = jo.value("uri").toString();
        artist.name = jo.value("name").toString();
        artist.sortname = jo.value("sortname").toString();
        artist.musicbrainz_id = jo.value("musicbrainz_id").toString();
        return true;
    }
    return false;
}

bool ModelTranslator::fromJson(const QJsonObject &jo, Album &album)
{
    if (jo.value("__model__").toString() == "Album") {
        album.uri = jo.value("uri").toString();
        album.name = jo.value("name").toString();
        album.artists = fromJsonArray<Artist>(jo.value("artists").toArray());
        album.num_tracks = jo.value("num_tracks").toDouble();
        album.num_discs = jo.value("num_discs").toDouble();
        album.date = getMopidyDate(jo.value("date").toString());
        album.musicbrainz_id = jo.value("musicbrainz_id").toString();
        return true;
    }
    return false;
}

bool ModelTranslator::fromJson(const QJsonObject &jo, Track &track)
{
    if (jo.value("__model__").toString() == "Track") {
        track.uri = jo.value("uri").toString();
        track.name = jo.value("name").toString();
        track.artists = fromJsonArray<Artist>(jo.value("artists").toArray());
        fromJson(jo.value("album").toObject(), track.album);
        track.composers = fromJsonArray<Artist>(jo.value("composers").toArray());
        track.performers = fromJsonArray<Artist>(jo.value("performers").toArray());
        track.genre = jo.value("genre").toString();
        track.track_no = jo.value("track_no").toDouble();
        track.disc_no = jo.value("disc_no").toDouble();
        track.date = getMopidyDate(jo.value("date").toString());
        track.length = jo.value("length").toDouble();
        track.bitrate = jo.value("bitrate").toDouble();
        track.comment = jo.value("comment").toString();
        track.musicbrainz_id = jo.value("musicbrainz_id").toString();
        track.last_modified = QDateTime::fromMSecsSinceEpoch(jo.value("last_modified").toDouble());
        return true;
    }
    return false;
}

bool ModelTranslator::fromJson(const QJsonObject &jo, Playlist &playlist)
{
    if (jo.value("__model__").toString() == "Playlist") {
        playlist.uri = jo.value("uri").toString();
        playlist.name = jo.value("name").toString();
        playlist.tracks = fromJsonArray<Track>(jo.value("tracks").toArray());
        playlist.last_modified = QDateTime::fromMSecsSinceEpoch(
            jo.value("last_modified").toDouble());
        return true;
    }
    return false;
}

bool ModelTranslator::fromJson(const QJsonObject &jo, SearchResult &searchResult)
{
    if (jo.value("__model__").toString() == "SearchResult") {
        searchResult.uri = jo.value("uri").toString();
        searchResult.tracks = fromJsonArray<Track>(jo.value("tracks").toArray());
        searchResult.albums = fromJsonArray<Album>(jo.value("albums").toArray());
        searchResult.artists = fromJsonArray<Artist>(jo.value("artists").toArray());
        return true;
    }
    return false;
}

bool ModelTranslator::fromJson(const QJsonObject &jo, TlTrack &tlTrack)
{
    if (jo.value("__model__").toString() == "TlTrack") {
        tlTrack.tlid = jo.value("tlid").toDouble();
        fromJson(jo.value("track").toObject(), tlTrack.track);
        return true;
    }
    return false;
}

bool ModelTranslator::fromJson(const QJsonObject &jo, Ref &ref)
{
    if (jo.value("__model__").toString() == "Ref") {
        ref.uri = jo.value("uri").toString();
        ref.name = jo.value("name").toString();
        ref.type = getRefType(jo.value("type").toString());
        return true;
    }
    return false;
}

bool ModelTranslator::fromJson(const QJsonObject &jo, PlaybackState &state)
{
    return false;
}

bool ModelTranslator::fromJson(const QJsonObject &jo, Image &image)
{
    if (jo.value("__model__").toString() == "Image") {
        image.uri = jo.value("uri").toString();
        image.width = jo.value("width").toInt();
        image.height = jo.value("height").toInt();
        return true;
    }
    return false;
}

QDate ModelTranslator::getMopidyDate(const QString &strDate)
{
    if (strDate.isEmpty())
        return QDate();

    if (strDate.length() == MOPIDY_SHORT_DATE.size())
        return QDate::fromString(strDate, MOPIDY_SHORT_DATE);
    return QDate::fromString(strDate, MOPIDY_LONG_DATE);
}

QString ModelTranslator::toMopidyDate(const QDate &date)
{
    if (!date.isValid())
        return "";
    if (date.isNull())
        return "";
    if ((date.day() == 1) && (date.month() == 1))
        return date.toString(MOPIDY_SHORT_DATE);
    return date.toString(MOPIDY_LONG_DATE);
}

PlaybackState ModelTranslator::getState(const QString &stateStr)
{
    if (stateStr == "paused")
        return PlaybackState::Paused;
    if (stateStr == "playing")
        return PlaybackState::Playing;
    if (stateStr == "stopped")
        return PlaybackState::Stopped;
    return PlaybackState::Stopped;
}

QString ModelTranslator::getStateStr(const PlaybackState &state)
{
    switch (state) {
    case PlaybackState::Paused:
        return "paused";
    case PlaybackState::Playing:
        return "playing";
    case PlaybackState::Stopped:
        return "stopped";
    default:
        return "";
    }
}

QJsonObject ModelTranslator::toJsonDict(const QHash<QString, QString> &d)
{
    QJsonObject jo;

    for (QHash<QString, QString>::const_iterator it = d.begin(); it != d.end(); ++it)
        jo.insert(it.key(), it.value());

    return jo;
}

RefType ModelTranslator::getRefType(const QString &typeStr)
{
    if (typeStr.toLower() == "album")
        return RefType::Album;
    if (typeStr.toLower() == "artist")
        return RefType::Artist;
    if (typeStr.toLower() == "directory")
        return RefType::Directory;
    if (typeStr.toLower() == "playlist")
        return RefType::Playlist;
    if (typeStr.toLower() == "track")
        return RefType::Track;
    return RefType::Directory;
}

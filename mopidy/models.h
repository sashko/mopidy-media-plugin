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

#ifndef MOPIDYMODELS_H
#define MOPIDYMODELS_H

#include <QDate>
#include <QStringList>
#include <QVariant>

namespace mopidy {

enum class PlaybackState { Paused, Playing, Stopped };

enum class RefType { Album, Artist, Directory, Playlist, Track };

struct Artist
{
    QString uri;
    QString name;
    QString sortname;
    QString musicbrainz_id;
};
typedef QList<Artist> Artists;

struct Album
{
    QString uri;
    QString name;
    Artists artists;
    int num_tracks;
    int num_discs;
    QDate date;
    QString musicbrainz_id;
};
typedef QList<Album> Albums;

struct Track
{
    QString uri;
    QString name;
    Artists artists;
    Album album;
    Artists composers;
    Artists performers;
    QString genre;
    int track_no;
    int disc_no;
    QDate date;
    int length;
    int bitrate;
    QString comment;
    QString musicbrainz_id;
    QDateTime last_modified;
};
typedef QList<Track> Tracks;

struct Playlist
{
    QString uri;
    QString name;
    Tracks tracks;
    QDateTime last_modified;
};
typedef QList<Playlist> Playlists;

struct SearchResult
{
    QString uri;
    Tracks tracks;
    Albums albums;
    Artists artists;
};

struct TlTrack
{
    int tlid = -1;
    Track track;
};
typedef QList<TlTrack> TlTracks;

struct Ref
{
    QString uri;
    QString name;
    RefType type;
};
typedef QList<Ref> Refs;

struct Image
{
    QString uri;
    int width;
    int height;
};
typedef QList<Image> Images;

typedef QMap<unsigned long, Ref> History;

// QVariant, signals, ...
void registerMopidyTypes();

} // namespace mopidy

//
Q_DECLARE_METATYPE(mopidy::Album)
Q_DECLARE_METATYPE(mopidy::Artist)
Q_DECLARE_METATYPE(mopidy::Image)
Q_DECLARE_METATYPE(mopidy::Playlist)
Q_DECLARE_METATYPE(mopidy::Ref)
Q_DECLARE_METATYPE(mopidy::SearchResult)
Q_DECLARE_METATYPE(mopidy::TlTrack)
Q_DECLARE_METATYPE(mopidy::Track)
Q_DECLARE_METATYPE(mopidy::History)

#endif // MOPIDYMODELS_H

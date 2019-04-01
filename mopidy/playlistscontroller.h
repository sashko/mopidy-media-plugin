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

#ifndef PLAYLISTSCONTROLLER_H
#define PLAYLISTSCONTROLLER_H

#include "basecontroller.h"
#include "models.h"

namespace mopidy {

class PlaylistsController : public BaseController
{
    Q_OBJECT

public:
    explicit PlaylistsController();
    ~PlaylistsController();

    void uriShemes();

    // Fetching
    void asList();
    void getItems(const QString &uri);
    void lookup(const QString &uri);
    void refresh(const QString &uri_scheme = "");

    // Manipulating
    void create(const QString &name, const QString &uri_scheme = "");
    void save(const mopidy::Playlist &pl);
    void deletePlaylist(const QString &uri);

signals:
    void uriSchemesReceived(const QStringList &schemes);
    void listReceived(const mopidy::Refs &refs);
    void itemsReceived(const mopidy::Refs &refs);
    void lookedup(const mopidy::Playlist &playlist);
    void created(const mopidy::Playlist &playlist);
    void saved(const mopidy::Playlist playlist);
};

} // namespace mopidy

#endif //PLAYLISTSCONTROLLER_H

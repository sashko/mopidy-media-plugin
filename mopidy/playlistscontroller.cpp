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

#include "playlistscontroller.h"
#include "modeltranslator.h"

using namespace mopidy;

PlaylistsController::PlaylistsController()
    : BaseController("core.playlists")
{}

PlaylistsController::~PlaylistsController() {}

void PlaylistsController::uriShemes()
{
    auto rhdl = [&](const QJsonValue &v) {
        QStringList uris;
        for (QJsonValue jv : v.toArray())
            uris << jv.toString();
        emit uriSchemesReceived(uris);
    };

    sendRequest("get_uri_schemes", rhdl);
}

void PlaylistsController::asList()
{
    auto rhdl = [&](const QJsonValue &v) {
        Refs refs = ModelTranslator::fromJsonArray<Ref>(v.toArray());
        emit listReceived(refs);
    };

    sendRequest("as_list", rhdl);
}

void PlaylistsController::getItems(const QString &uri)
{
    auto rhdl = [&](const QJsonValue &v) {
        Refs refs = ModelTranslator::fromJsonArray<Ref>(v.toArray());
        emit itemsReceived(refs);
    };

    sendRequest("get_items", rhdl, {{"uri", uri}});
}

void PlaylistsController::lookup(const QString &uri)
{
    auto rhdl = [&](const QJsonValue &v) {
        Playlist playlist;
        ModelTranslator::fromJson(v.toObject(), playlist);
        emit lookedup(playlist);
    };

    sendRequest("lookup", rhdl, {{"uri", uri}});
}

void PlaylistsController::refresh(const QString &uri_scheme)
{
    sendNotification("refresh", {{"uri_scheme", uri_scheme}});
}

void PlaylistsController::create(const QString &name, const QString &uri_scheme)
{
    auto rhdl = [&](const QJsonValue &v) {
        Playlist playlist;
        ModelTranslator::fromJson(v.toObject(), playlist);
        emit created(playlist);
    };

    QJsonObject params;
    params.insert("name", name);
    params.insert("uri_scheme", uri_scheme);

    sendRequest("create", rhdl, params);
}

void PlaylistsController::save(const Playlist &pl)
{
    auto rhdl = [&](const QJsonValue &v) {
        Playlist playlist;
        ModelTranslator::fromJson(v.toObject(), playlist);
        emit saved(playlist);
    };

    QJsonObject params;
    params.insert("playlist", ModelTranslator::toJson(pl));

    sendRequest("save", rhdl, params);
}

void PlaylistsController::deletePlaylist(const QString &uri)
{
    sendNotification("delete", {{"uri", uri}});
}

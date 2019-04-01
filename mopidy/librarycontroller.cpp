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

#include "librarycontroller.h"
#include "modeltranslator.h"

#include <QDebug>

using namespace mopidy;

LibraryController::LibraryController()
    : BaseController("core.library")
{}

LibraryController::~LibraryController() {}

void LibraryController::browse(const QString &uri)
{
    auto rhdl = [&](const QJsonValue &v) {
        Refs refs = ModelTranslator::fromJsonArray<Ref>(v.toArray());
        emit browsed(refs);
    };

    QJsonObject params;
    params["uri"] = uri.isEmpty() ? QJsonValue() : uri;

    sendRequest("browse", rhdl, params);
}

void LibraryController::search(const QHash<QString, QStringList> &query,
                               const QStringList &uris,
                               bool exact,
                               const QUuid &identifier)
{
    const auto rhdl = [&](const QJsonValue &v) {
        SearchResult sr;
        ModelTranslator::fromJson(v.toArray().at(0).toObject(), sr);
        emit searchResult(identifier, sr);
    };

    QJsonObject params;

    QJsonObject joQuery;
    for (auto it = query.cbegin(); it != query.cend(); ++it)
        joQuery.insert(it.key(), QJsonArray::fromStringList(it.value()));
    params["query"] = joQuery;
    params["uris"] = QJsonArray::fromStringList(uris);
    params["exact"] = exact;

    sendRequest("search", rhdl, params);
}

void LibraryController::lookup(const QStringList &uris)
{
    auto rhdl = [&](const QJsonValue &v) {
        Tracks tracks = ModelTranslator::fromJsonArray<Track>(v.toArray());
        emit lookedup(tracks);
    };

    QJsonObject params;
    if (uris.size() == 1)
        params["uri"] = uris.first();
    else if (uris.size() > 1)
        params["uris"] = QJsonArray::fromStringList(uris);

    sendRequest("lookup", rhdl, params);
}

void LibraryController::refresh(const QString &uri)
{
    QJsonObject params;
    params["uri"] = uri.isEmpty() ? QJsonValue() : uri;
    sendNotification("refresh", params);
}

void LibraryController::getImages(const QStringList &uris)
{
    auto rhdl = [&](const QJsonValue &v) {
        QMap<QString, mopidy::Images> uri_images;
        const QJsonObject joUriImages = v.toObject();
        for (auto uri : joUriImages.keys()) {
            Images images = ModelTranslator::fromJsonArray<Image>(joUriImages.value(uri).toArray());
            uri_images.insert(uri, images);
        }
        emit imagesReceived(uri_images);
    };

    QJsonObject params;
    params["uris"] = QJsonArray::fromStringList(uris);

    sendRequest("get_images", rhdl, params);
}

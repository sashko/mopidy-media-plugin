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

#ifndef LIBRARYCONTROLLER_H
#define LIBRARYCONTROLLER_H

#include "basecontroller.h"
#include "models.h"

#include <QUuid>

namespace mopidy {

class LibraryController : public BaseController
{
    Q_OBJECT

public:
    explicit LibraryController();
    ~LibraryController();

    void browse(const QString &uri = "");
    void search(const QHash<QString, QStringList> &query,
                const QStringList &uris,
                bool exact,
                const QUuid &identifier);
    void lookup(const QStringList &uris = QStringList());
    void refresh(const QString &uri = "");
    void getImages(const QStringList &uris);

signals:
    void browsed(const mopidy::Refs &refs);
    void lookedup(const mopidy::Tracks &tracks);
    void searchResult(const QUuid &identifier, const mopidy::SearchResult &sr);
    void imagesReceived(const QMap<QString, mopidy::Images> &uri_images);
};

} // namespace mopidy

#endif //LIBRARYCONTROLLER_H

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

#include "libraryhelper.h"
#include "libraryhelper_p.h"

#include <QDebug>

using namespace mopidy;

LibraryHelperPrivate::LibraryHelperPrivate(LibraryHelper *parent)
    : q_ptr(parent)
{
    connect(&lc, &mopidy::LibraryController::browsed, this, &LibraryHelperPrivate::onLibraryBrowsed);
}

LibraryHelperPrivate::~LibraryHelperPrivate() {}

void LibraryHelperPrivate::onLibraryBrowsed(const mopidy::Refs &refs)
{
    for (auto ref : refs) {
        if (ref.type == mopidy::RefType::Track) {
            currentRefs.append(ref);
        } else {
            ++currentBrowseCount;
            lc.browse(ref.uri);
        }
    }

    --currentBrowseCount;

    if (currentBrowseCount == 0) {
        Q_Q(LibraryHelper);
        emit q->tracksInDirectoryFetched(currentUriRecursive, currentRefs);
    }
}

LibraryHelper::LibraryHelper()
    : d_ptr(new LibraryHelperPrivate(this))
{}

LibraryHelper::~LibraryHelper() {}

void LibraryHelper::setJsonRpcHandler(QSharedPointer<JsonRpcHandler> jrhdl)
{
    Q_D(LibraryHelper);
    d->lc.setJsonRpcHandler(jrhdl);
}

void LibraryHelper::requestTracksInDirectory(const QString &uri)
{
    Q_D(LibraryHelper);

    d->currentUriRecursive = uri;
    d->currentBrowseCount = 1;
    d->currentRefs.clear();

    d->lc.browse(uri);
}

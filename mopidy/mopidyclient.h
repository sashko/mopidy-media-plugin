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

#ifndef MOPIDY_CLIENT_H
#define MOPIDY_CLIENT_H

#include "jsonrpchandler.h"
#include <QObject>

namespace mopidy {

class MopidyClientPrivate;
class MopidyClient : public QObject
{
    Q_OBJECT

public:
    MopidyClient(QObject *parent = 0);
    ~MopidyClient();

    void connectToServer(const QString &host = "127.0.0.1",
                         quint16 port = 6680,
                         const QString &path = "/mopidy/ws");
    void disconnectFromServer();

    QSharedPointer<JsonRpcHandler> jsonrpcHandler();

signals:
    // connections signals
    void clientConnected();
    void clientDisconnected();
    void connectionError(int code, const QString &message);

private:
    Q_DECLARE_PRIVATE(MopidyClient)
    QScopedPointer<MopidyClientPrivate> d_ptr;
};

} // namespace mopidy

#endif //MOPIDY_CLIENT_H

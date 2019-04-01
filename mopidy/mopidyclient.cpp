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

#include "mopidyclient.h"
#include "mopidyclient_p.h"

using namespace mopidy;

MopidyClientPrivate::MopidyClientPrivate(MopidyClient *parent)
    : q_ptr(parent)
{
    ws = QSharedPointer<QWebSocket>::create("qtivi_media_mopidy");
    connect(ws.data(), &QWebSocket::connected, parent, &MopidyClient::clientConnected);
    connect(ws.data(), &QWebSocket::disconnected, parent, &MopidyClient::clientDisconnected);
    connect(ws.data(),
            static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
            this,
            &MopidyClientPrivate::onWsError);

    jsrpcHdl = QSharedPointer<JsonRpcHandler>::create();
    jsrpcHdl->setSocket(ws);
}

MopidyClientPrivate::~MopidyClientPrivate() {}

void MopidyClientPrivate::onWsError(QAbstractSocket::SocketError error)
{
    Q_Q(MopidyClient);
    emit q->connectionError(static_cast<int>(error), ws->errorString());
}

MopidyClient::MopidyClient(QObject *parent)
    : QObject(parent)
    , d_ptr(new MopidyClientPrivate(this))
{}

MopidyClient::~MopidyClient()
{
    Q_D(MopidyClient);
}

void MopidyClient::connectToServer(const QString &host, quint16 port, const QString &path)
{
    Q_D(MopidyClient);

    // if already connected, will disconnect before
    disconnectFromServer();

    // create connection URL
    QUrl url = QUrl::fromUserInput(host);
    url.setScheme("ws");
    url.setPort(port);
    url.setPath(path);

    // Try to open websocket connection
    d->ws->open(url);
}

void MopidyClient::disconnectFromServer()
{
    Q_D(MopidyClient);
    if (d->ws->state() == QAbstractSocket::ConnectedState)
        d->ws->close();
}

QSharedPointer<JsonRpcHandler> MopidyClient::jsonrpcHandler()
{
    Q_D(MopidyClient);
    return d->jsrpcHdl;
}

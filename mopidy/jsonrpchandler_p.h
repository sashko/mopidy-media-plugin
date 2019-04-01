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

#ifndef JSONRPCHANDLER_P_H
#define JSONRPCHANDLER_P_H

#include "jsonrpchandler.h"

#include <QByteArray>
#include <QMap>
#include <QMutex>
#include <QQueue>

namespace mopidy {

class JsonRpcHandlerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit JsonRpcHandlerPrivate(JsonRpcHandler *parent);
    virtual ~JsonRpcHandlerPrivate();

    QSharedPointer<QWebSocket> ws;
    QMap<QString, JsonRpcHandler::ResponseHandler> requests;
    QMutex mutex;

    QString buildMsgId();
    bool sendJsonRpcMessage(const QString &method,
                            const QJsonObject &params = QJsonObject(),
                            const QString &id = QString());

public slots:
    void onWsTextReceived(const QString &message);
    void onClientConnected();
    void onClientDisconnected();

private:
    JsonRpcHandler *const q_ptr;
    Q_DECLARE_PUBLIC(JsonRpcHandler)
    QQueue<QByteArray> m_msgsQueue;
    bool m_connected;
};

} // namespace mopidy

#endif //JSONRPCHANDLER_P_H

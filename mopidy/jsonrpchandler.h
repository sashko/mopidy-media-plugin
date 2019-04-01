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

#ifndef JSONRPCHANDLER_H
#define JSONRPCHANDLER_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QWebSocket>

#include <functional>

namespace mopidy {

class JsonRpcHandlerPrivate;
class JsonRpcHandler : public QObject
{
    Q_OBJECT

public:
    enum class RpcErrorCode { JsonRpcError, SocketError };

    // Handler used to process reponse to requests
    typedef std::function<void(const QJsonValue &)> ResponseHandler;

    explicit JsonRpcHandler(QObject *parent = Q_NULLPTR);
    virtual ~JsonRpcHandler();

    void setSocket(QSharedPointer<QWebSocket> ws);

    void sendRequest(const QString &method,
                     ResponseHandler rhdl,
                     const QJsonObject &params = QJsonObject());
    void sendNotification(const QString &method, const QJsonObject &params = QJsonObject());

signals:
    void eventReceived(const QString &event, const QJsonObject &params);
    void error(RpcErrorCode ec, const QJsonObject &jsonRpcErrorObj);

private:
    Q_DECLARE_PRIVATE(JsonRpcHandler)
    QScopedPointer<JsonRpcHandlerPrivate> d_ptr;
};

} // namespace mopidy

#endif //JSONRPCHANDLER_H

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

#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "jsonrpchandler.h"
#include <QObject>

namespace mopidy {

class BaseControllerPrivate;
class BaseController : public QObject
{
    Q_OBJECT

public:
    explicit BaseController(const QString &controllerName);
    virtual ~BaseController();

    void setJsonRpcHandler(QSharedPointer<mopidy::JsonRpcHandler> jsRpcHdl);

protected:
    // shortcut to add full method name
    void sendRequest(const QString &method,
                     mopidy::JsonRpcHandler::ResponseHandler rhdl,
                     const QJsonObject &params = QJsonObject());
    void sendNotification(const QString &method, const QJsonObject &params = QJsonObject());

private:
    Q_DECLARE_PRIVATE(BaseController)
    QScopedPointer<BaseControllerPrivate> d_ptr;
};

} // namespace mopidy

#endif //ABSTRACTCONTROLLER_H

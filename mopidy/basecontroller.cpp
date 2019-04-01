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

#include "basecontroller.h"
#include "basecontroller_p.h"

using namespace mopidy;

BaseControllerPrivate::BaseControllerPrivate(const QString &ctrlName)
    : controllerName(ctrlName)
{}

BaseControllerPrivate::~BaseControllerPrivate() {}

BaseController::BaseController(const QString &controllerName)
    : d_ptr(new BaseControllerPrivate(controllerName))
{}

BaseController::~BaseController() {}

void BaseController::setJsonRpcHandler(QSharedPointer<JsonRpcHandler> jsRpcHdl)
{
    Q_D(BaseController);
    d->rpc = jsRpcHdl;
}

void BaseController::sendRequest(const QString &method,
                                 JsonRpcHandler::ResponseHandler rhdl,
                                 const QJsonObject &params)
{
    Q_D(BaseController);
    if (d->rpc)
        d->rpc->sendRequest(QString("%1.%2").arg(d->controllerName).arg(method), rhdl, params);
}

void BaseController::sendNotification(const QString &method, const QJsonObject &params)
{
    Q_D(BaseController);
    if (d->rpc)
        d->rpc->sendNotification(QString("%1.%2").arg(d->controllerName).arg(method), params);
}

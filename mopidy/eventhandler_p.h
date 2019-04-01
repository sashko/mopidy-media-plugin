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

#ifndef EVENTHANDLER_P_H
#define EVENTHANDLER_P_H

#include "eventhandler.h"

namespace mopidy {

class EventHandlerPrivate : public QObject
{
    Q_OBJECT

public:
    EventHandlerPrivate(EventHandler *parent);
    ~EventHandlerPrivate();

    QSharedPointer<mopidy::JsonRpcHandler> jsRpcHdl;

public slots:
    void onEventReceived(const QString &event, const QJsonObject &params);

private:
    EventHandler *const q_ptr;
    Q_DECLARE_PUBLIC(EventHandler)
};
} // namespace mopidy

#endif //EVENTHANDLER_P_H

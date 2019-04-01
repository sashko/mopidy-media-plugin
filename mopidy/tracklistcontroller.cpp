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

#include "tracklistcontroller.h"
#include "modeltranslator.h"

using namespace mopidy;

TracklistController::TracklistController()
    : BaseController("core.tracklist")
{}

TracklistController::~TracklistController() {}

void TracklistController::add(const QStringList &uris, int atPosition)
{
    auto rhdl = [&](const QJsonValue &v) {
        TlTracks tltracks = ModelTranslator::fromJsonArray<TlTrack>(v.toArray());
        emit added(tltracks);
    };

    QJsonObject params;
    params.insert("uris", QJsonArray::fromStringList(uris));
    if (atPosition >= 0)
        params.insert("at_position", atPosition);

    sendRequest("add", rhdl, params);
}

void TracklistController::remove(const QMap<QString, QVariantList> &criteria)
{
    auto rhdl = [&](const QJsonValue &v) {
        TlTracks tltracks = ModelTranslator::fromJsonArray<TlTrack>(v.toArray());
        emit removed(tltracks);
    };

    QJsonObject params;
    for (auto itCriteria = criteria.cbegin(); itCriteria != criteria.cend(); ++itCriteria) {
        QJsonArray criteriaValues = QJsonArray::fromVariantList(itCriteria.value());
        params.insert(itCriteria.key(), criteriaValues);
    }

    sendRequest("remove", rhdl, params);
}

void TracklistController::clear()
{
    sendNotification("clear");
}

void TracklistController::move(int start, int end, int toPosition)
{
    QJsonObject params;
    params.insert("start", start);
    params.insert("end", end);
    params.insert("to_position", toPosition);

    sendNotification("move", params);
}

void TracklistController::shuffle(int start, int stop)
{
    QJsonObject params;
    params.insert("start", start >= 0 ? start : QJsonValue::Null);
    params.insert("stop", stop >= 0 ? stop : QJsonValue::Null);

    sendNotification("shuffle", params);
}

void TracklistController::getTlTracks()
{
    auto rhdl = [&](const QJsonValue &v) {
        TlTracks tltracks = ModelTranslator::fromJsonArray<TlTrack>(v.toArray());
        emit tlTracksReceived(tltracks);
    };

    sendRequest("get_tl_tracks", rhdl);
}

void TracklistController::index(const TlTrack &tltrack)
{
    auto rhdl = [&](const QJsonValue &v) { emit indexReceived(v.toInt()); };

    QJsonObject params;
    params.insert("tl_track", ModelTranslator::toJson(tltrack));

    sendRequest("index", rhdl, params);
}

void TracklistController::index(int tlid)
{
    auto rhdl = [&](const QJsonValue &v) { emit indexReceived(v.toInt()); };
    sendRequest("index", rhdl, {{"tlid", tlid}});
}

void TracklistController::index()
{
    auto rhdl = [&](const QJsonValue &v) { emit indexReceived(v.toInt()); };
    sendRequest("index", rhdl);
}

void TracklistController::getVersion()
{
    auto rhdl = [&](const QJsonValue &v) { emit versionReceived(v.toInt()); };
    sendRequest("get_version", rhdl);
}

void TracklistController::getLength()
{
    auto rhdl = [&](const QJsonValue &v) { emit lengthReceived(v.toInt()); };
    sendRequest("get_length", rhdl);
}

void TracklistController::getTracks()
{
    auto rhdl = [&](const QJsonValue &v) {
        Tracks tracks = ModelTranslator::fromJsonArray<Track>(v.toArray());
        emit tracksReceived(tracks);
    };

    sendRequest("get_tracks", rhdl);
}

void TracklistController::slice(int start, int stop)
{
    auto rhdl = [&](const QJsonValue &v) {
        TlTrack tlTrack;
        ModelTranslator::fromJson(v.toObject(), tlTrack);
        emit sliced(tlTrack);
    };

    QJsonObject params;
    params.insert("start", start);
    params.insert("stop", stop);

    sendRequest("slice", rhdl, params);
}

void TracklistController::filter(const QMap<QString, QVariantList> &criteria)
{
    auto rhdl = [&](const QJsonValue &v) {
        TlTracks tltracks = ModelTranslator::fromJsonArray<TlTrack>(v.toArray());
        emit filtered(tltracks);
    };

    QJsonObject params;
    for (auto itCriteria = criteria.cbegin(); itCriteria != criteria.cend(); ++itCriteria) {
        QJsonArray criteriaValues = QJsonArray::fromVariantList(itCriteria.value());
        params.insert(itCriteria.key(), criteriaValues);
    }

    sendRequest("filter", rhdl, params);
}

void TracklistController::eotTlId()
{
    auto rhdl = [&](const QJsonValue &v) { emit eotTlIdReceived(v.toInt()); };
    sendRequest("get_eot_tlid", rhdl);
}

void TracklistController::nextTlId()
{
    auto rhdl = [&](const QJsonValue &v) { emit nextTlIdReceived(v.toInt()); };
    sendRequest("get_next_tlid", rhdl);
}

void TracklistController::previousTlId()
{
    auto rhdl = [&](const QJsonValue &v) { emit previousTlIdReceived(v.toInt()); };
    sendRequest("get_previous_tlid", rhdl);
}

void TracklistController::eotTrack(const TlTrack &tltrack)
{
    auto rhdl = [&](const QJsonValue &v) {
        TlTrack tlTrack;
        ModelTranslator::fromJson(v.toObject(), tlTrack);
        emit eotTrackReceived(tltrack);
    };

    QJsonObject params;
    params.insert("tl_track", ModelTranslator::toJson(tltrack));

    sendRequest("eot_track", rhdl, params);
}

void TracklistController::nextTrack(const TlTrack &tltrack)
{
    auto rhdl = [&](const QJsonValue &v) {
        TlTrack tlTrack;
        ModelTranslator::fromJson(v.toObject(), tlTrack);
        emit nextTrackReceived(tltrack);
    };

    QJsonObject params;
    if (tltrack.tlid >= 0)
        params.insert("tl_track", ModelTranslator::toJson(tltrack));
    else
        params.insert("tl_track", QJsonValue::Null);

    sendRequest("next_track", rhdl, params);
}

void TracklistController::previousTrack(const TlTrack &tltrack)
{
    auto rhdl = [&](const QJsonValue &v) {
        TlTrack tlTrack;
        ModelTranslator::fromJson(v.toObject(), tlTrack);
        emit previousTrackReceived(tltrack);
    };

    QJsonObject params;
    if (tltrack.tlid >= 0)
        params.insert("tl_track", ModelTranslator::toJson(tltrack));
    else
        params.insert("tl_track", QJsonValue::Null);

    sendRequest("previous_track", rhdl, params);
}

void TracklistController::getConsume()
{
    auto rhdl = [&](const QJsonValue &v) { emit consumeReceived(v.toBool()); };
    sendRequest("get_consume", rhdl);
}

void TracklistController::setConsume(bool consume)
{
    sendNotification("set_consome", {{"consume", consume}});
}
void TracklistController::getRandom()
{
    auto rhdl = [&](const QJsonValue &v) { emit randomReceived(v.toBool()); };
    sendRequest("get_random", rhdl);
}

void TracklistController::setRandom(bool random)
{
    sendNotification("set_random", {{"random", random}});
}

void TracklistController::getRepeat()
{
    auto rhdl = [&](const QJsonValue &v) { emit repeatReceived(v.toBool()); };
    sendRequest("get_repeat", rhdl);
}

void TracklistController::setRepeat(bool repeat)
{
    sendNotification("set_repeat", {{"repeat", repeat}});
}

void TracklistController::getSingle()
{
    auto rhdl = [&](const QJsonValue &v) { emit singleReceived(v.toBool()); };
    sendRequest("get_single", rhdl);
}

void TracklistController::setSingle(bool single)
{
    sendNotification("set_single", {{"single", single}});
}

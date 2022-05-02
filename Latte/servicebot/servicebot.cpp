#include <QJsonObject>
#include <QJsonDocument>

#include "servicebot.h"

ServiceBot::ServiceBot(QObject *parent) : QObject(parent)
{
    networkAccessManager = new QNetworkAccessManager(this);
}

ServiceBot::~ServiceBot()
{

}

void ServiceBot::sendMessage(const QString &qq, const QString &message)
{
    QNetworkRequest request(QUrl("http://101.32.29.26:7391/message"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObject;
    jsonObject["qq"] = qq;
    jsonObject["message"] = message;
    QJsonDocument jsonDocument(jsonObject);
    QByteArray data = jsonDocument.toJson();

    networkAccessManager->post(request, data);
}

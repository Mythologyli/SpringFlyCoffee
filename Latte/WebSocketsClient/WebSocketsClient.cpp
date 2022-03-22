#include "WebSocketsClient.h"

WebSocketsClient::WebSocketsClient(const QString &url, QObject *parent) :
        url(url),
        QObject(parent)
{
    qInfo() << "WebSocket server:" << url;

    connect(&webSocket, &QWebSocket::connected,
            [&]()
            {
                qInfo() << "WebSocket connected.";
            });

    connect(&webSocket, &QWebSocket::disconnected,
            [&]()
            {
                qWarning() << "WebSocket disconnected.";
            });

    connect(&webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            [&](QAbstractSocket::SocketError error)
            {
                qWarning() << "WebSocket error:" << qt_getEnumName(error);
            });

    connect(&webSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketsClient::onTextMessageReceived);

    connect(&webSocket, &QWebSocket::disconnected,
            this, &WebSocketsClient::reconnect);

    webSocket.open(url);
}

void WebSocketsClient::onTextMessageReceived(const QString &message)
{
    qInfo() << "WebSocket text message received:" << message;

    if (message == "check")
    {
        emit commandCheckReceived();
    }
    else if (message == "save")
    {
        emit commandSaveReceived();
    }
}

void WebSocketsClient::reconnect()
{
    qInfo() << "WebSocket reconnecting...";
    webSocket.abort();
    webSocket.open(url);
}

void WebSocketsClient::send(const QString &text)
{
    webSocket.sendTextMessage(text);
}

void WebSocketsClient::close()
{
    disconnect(&webSocket, &QWebSocket::disconnected,
               this, &WebSocketsClient::reconnect);
    webSocket.close();
}

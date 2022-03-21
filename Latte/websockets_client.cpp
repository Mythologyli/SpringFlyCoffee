#include "websockets_client.h"

WebSocketsClient::WebSocketsClient(const QString &url, QObject *parent) :
        url(url),
        QObject(parent)
{
    qInfo() << "WebSocket server:" << url;
    connect(&websocket, &QWebSocket::connected,
            this, &WebSocketsClient::on_connected);
    connect(&websocket, &QWebSocket::disconnected,
            this, &WebSocketsClient::on_disconnected);
    connect(&websocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &WebSocketsClient::on_error);
    connect(&websocket, &QWebSocket::disconnected,
            this, &WebSocketsClient::reconnect);
    websocket.open(url);
}

void WebSocketsClient::on_connected()
{
    qInfo() << "WebSocket connected.";
    connect(&websocket, &QWebSocket::textMessageReceived,
            this, &WebSocketsClient::on_text_message_received);
}

void WebSocketsClient::on_disconnected()
{
    qWarning() << "WebSocket disconnected.";
    disconnect(&websocket, &QWebSocket::textMessageReceived,
               this, &WebSocketsClient::on_text_message_received);
}

void WebSocketsClient::on_error(QAbstractSocket::SocketError error)
{
    qWarning() << "WebSocket error:" << qt_getEnumName(error);
}

void WebSocketsClient::on_text_message_received(const QString &message)
{
    qInfo() << "WebSocket text message received:" << message;

    if (message == "check")
    {
        emit command_check_received();
    }
    else if (message == "save")
    {
        emit command_save_received();
    }
}

void WebSocketsClient::reconnect()
{
    qInfo() << "WebSocket reconnecting...";
    websocket.abort();
    websocket.open(url);
}

void WebSocketsClient::send(const QString &text)
{
    websocket.sendTextMessage(text);
}

void WebSocketsClient::close()
{
    disconnect(&websocket, &QWebSocket::disconnected,
               this, &WebSocketsClient::reconnect);
    websocket.close();
}

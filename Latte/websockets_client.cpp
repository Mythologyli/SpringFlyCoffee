#include "websockets_client.h"

WebSocketsClient::WebSocketsClient(const QString &url, QObject *parent) : QObject(parent)
{
    qInfo() << "WebSocket server:" << url;
    connect(&websocket, &QWebSocket::connected, this, &WebSocketsClient::on_connected);
    websocket.open(url);
}

void WebSocketsClient::on_connected()
{
    qInfo() << "WebSocket connected.";
    connect(&websocket, &QWebSocket::textMessageReceived, this, &WebSocketsClient::on_text_message_received);
    websocket.sendTextMessage("Hello, world!");
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

void WebSocketsClient::close()
{
    websocket.close();
}

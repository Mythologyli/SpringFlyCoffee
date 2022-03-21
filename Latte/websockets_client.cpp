#include "websockets_client.h"

WebSocketsClient::WebSocketsClient(const QString &url, QObject *parent) :
        url(url),
        QObject(parent)
{
    qInfo() << "WebSocket server:" << url;
    connect(&websocket, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(&websocket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
    websocket.open(url);
}

void WebSocketsClient::on_connected()
{
    qInfo() << "WebSocket connected.";
    connect(&websocket, SIGNAL(textMessageReceived(QString)), this, SLOT(on_text_message_received(QString)));
}

void WebSocketsClient::on_disconnected()
{
    qInfo() << "WebSocket disconnected. Reconnecting...";
    websocket.abort();
    websocket.open(url);
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

void WebSocketsClient::send()
{
    websocket.sendTextMessage("Hello, world!");
}

void WebSocketsClient::close()
{
    disconnect(&websocket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
    websocket.close();
}

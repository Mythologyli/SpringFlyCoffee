#ifndef LATTE_WEBSOCKETS_CLIENT_H
#define LATTE_WEBSOCKETS_CLIENT_H

#include <QObject>
#include <QWebSocket>

class WebSocketsClient : public QObject
{
Q_OBJECT
public:
    explicit WebSocketsClient(const QString &url, QObject *parent = nullptr);

public slots:

    void send(const QString &text);

    void close();

private slots:

    void reconnect();

    void on_text_message_received(const QString &message);

private:
    QString url;
    QWebSocket websocket;

signals:

    void command_check_received();

    void command_save_received();
};

#endif // LATTE_WEBSOCKETS_CLIENT_H

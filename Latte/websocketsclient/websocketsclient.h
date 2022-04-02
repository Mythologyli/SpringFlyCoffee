#ifndef LATTE_WEBSOCKETSCLIENT_H
#define LATTE_WEBSOCKETSCLIENT_H

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

    void onTextMessageReceived(const QString &message);

private:
    QString url;
    QWebSocket *webSocket;

signals:

    void commandCheckReceived();

    void commandSaveReceived();
};

#endif // LATTE_WEBSOCKETSCLIENT_H

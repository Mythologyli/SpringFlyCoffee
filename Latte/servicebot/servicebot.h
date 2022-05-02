#ifndef LATTE_SERVICEBOT_H
#define LATTE_SERVICEBOT_H

#include <QObject>
#include <QNetworkAccessManager>

class ServiceBot : public QObject
{
Q_OBJECT
public:
    explicit ServiceBot(QObject *parent);

    ~ServiceBot() override;

public slots:

    void sendMessage(const QString &qq, const QString &message);

private:
    QNetworkAccessManager *networkAccessManager;
};

#endif //LATTE_SERVICEBOT_H

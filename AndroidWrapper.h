#ifndef WRAPPER_H
#define WRAPPER_H

#include <QtAndroid>
#include <QAndroidActivityResultReceiver>
#include <QAndroidJniObject>
#include <QObject>

class AndroidWrapper : public QObject, public QAndroidActivityResultReceiver {
    Q_OBJECT
public:
    explicit AndroidWrapper(QObject *parent = 0);
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);
    Q_INVOKABLE int getAndroidSdk();
    Q_INVOKABLE void openFile();
//    void Java_org_firebird_emu_MainActivity_receivedFile();
};

#endif // WRAPPER_H

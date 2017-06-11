#ifndef WRAPPER_H
#define WRAPPER_H

#include <QtAndroid>
#include <QAndroidActivityResultReceiver>
#include <QAndroidJniObject>
#include <QObject>
#include <android/log.h>

class AndroidWrapper : public QObject, public QAndroidActivityResultReceiver {
    Q_OBJECT
public:
    explicit AndroidWrapper(QObject *parent = 0);
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);
    Q_INVOKABLE int getAndroidSdk();
    Q_INVOKABLE void openFile();

signals:
    void filePicked(QString fileUrl);
};

static int SDCARD_DOCUMENT_REQUEST = 2;
int android_get_fd_for_uri(const char *path, const char *mode);

#endif // WRAPPER_H

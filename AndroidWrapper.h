#ifndef ANDROIDWRAPPER_H
#define ANDROIDWRAPPER_H

#include <QObject>
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QAndroidActivityResultReceiver>

/*/
#include <android/log.h>
#define TAG "org.firebird.emu"
//*/

class AndroidWrapper : public QObject, public QAndroidActivityResultReceiver {
    Q_OBJECT
public:
    explicit AndroidWrapper(QObject *parent = 0);
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);
    Q_INVOKABLE static int is_content_provider_supported();
    Q_INVOKABLE void selectFile(bool selectExisting);

private:
    const static int SDCARD_DOCUMENT_REQUEST = 2;
    const static int API__STORAGE_ACCESS_FRAMEWORK = 21;

signals:
    void filePicked(QString fileUrl);
};

extern "C" int is_android_provider_file(const char *path);
extern "C" int android_get_fd_for_uri(const char *path, const char *mode);

#endif // ANDROIDWRAPPER_H

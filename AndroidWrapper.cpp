#include "AndroidWrapper.h"

#define TAG "org.firebird.emu"

AndroidWrapper::AndroidWrapper(QObject *parent) : QObject(parent) {}

int AndroidWrapper::getAndroidSdk() {
    return QtAndroid::androidSdkVersion();
}

void AndroidWrapper::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data) {
    __android_log_print(ANDROID_LOG_WARN, TAG, "handleActivityResult + receiverRequestCode %d", receiverRequestCode);

    int RESULT_OK = QAndroidJniObject::getStaticField<int>("android/app/Activity", "RESULT_OK");
    if (receiverRequestCode == SDCARD_DOCUMENT_REQUEST && resultCode == RESULT_OK && data != NULL) {
        QAndroidJniObject uri = data.callObjectMethod("getData", "()Landroid/net/Uri;");

//        QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
//        QAndroidJniObject context = activity.callObjectMethod("getApplicationContext","()Landroid/content/Context;");
//        QAndroidJniObject contentResolver = context.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");

        QAndroidJniObject filePath = uri.callObjectMethod("getPath", "()Ljava/lang/String;");
        QString filePathString = filePath.toString();

        emit filePicked(filePathString);
        __android_log_print(ANDROID_LOG_WARN, TAG, "filePathString = %s", filePathString.toStdString().c_str());
    }
}

void AndroidWrapper::openFile() {
    __android_log_print(ANDROID_LOG_WARN, TAG, "openFile NATIVE");

    QAndroidJniObject intent("android/content/Intent");
    QAndroidJniObject setAction = QAndroidJniObject::fromString(QLatin1String("android.intent.action.OPEN_DOCUMENT"));
    QAndroidJniObject setType = QAndroidJniObject::fromString(QLatin1String("*/*"));

    if (setAction.isValid() && intent.isValid()) {
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", setAction.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", setType.object<jstring>());
        QtAndroid::startActivity(intent.object<jobject>(), SDCARD_DOCUMENT_REQUEST, this);
    }
}

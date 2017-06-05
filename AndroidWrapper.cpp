#include <android/log.h>
#include "AndroidWrapper.h"

#define TAG "org.firebird.emu"

AndroidWrapper::AndroidWrapper(QObject *parent) : QObject(parent) {}

int AndroidWrapper::getAndroidSdk() {
    return QtAndroid::androidSdkVersion();
}

#define NATIVE_ONLY 1

void AndroidWrapper::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data) {
    (void) resultCode;
    (void) data;
    __android_log_print(ANDROID_LOG_WARN, TAG, "handleActivityResult + receiverRequestCode %d", receiverRequestCode);
}

void AndroidWrapper::openFile() {
    __android_log_print(ANDROID_LOG_WARN, TAG, "openFile NATIVE");

#if NATIVE_ONLY
//    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/firebird/emu/MainActivity",
//                                            "activity", "()Landroid/app/Activity;");
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",
                                                "activity", "()Landroid/app/Activity;");

    QAndroidJniObject intent("android/content/Intent");
    QAndroidJniObject setAction = QAndroidJniObject::fromString(QLatin1String("android.intent.action.OPEN_DOCUMENT"));
    QAndroidJniObject setType = QAndroidJniObject::fromString(QLatin1String("*/*"));

    if (setAction.isValid() && intent.isValid()) {
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", setAction.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", setType.object<jstring>());

        //        QtAndroid::startActivity(intent.object<jobject>(), 2, this);
//        activity.startActivity(intent.object<jobject>(), 2, this);
        activity.callMethod<void>("startActivityForResult",
                    "(Landroid/content/Intent;)V", intent.object<jobject>(), 2);
    }
#else // NATIVE_ONLY
    QAndroidJniObject::callStaticMethod<void>("org/firebird/emu/MainActivity",
        "openFile",
        "()V"
        );
#endif // NATIVE_ONLY
}

//void AndroidWrapper::Java_org_firebird_emu_MainActivity_receivedFile() {
//    __android_log_print(ANDROID_LOG_WARN, TAG, "receivedFile :: ::");
//}

package org.firebird.emu;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.Manifest;
import android.util.Log;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;

import org.qtproject.qt5.android.bindings.QtActivity;

public class MainActivity
        extends QtActivity
        implements ActivityCompat.OnRequestPermissionsResultCallback {

    private static final String TAG = "org.firebird.emu";
    private static final int STORAGE_PERMISSIONS_REQUEST = 1;

    public final static String[] storagePermissions = {
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
        Manifest.permission.READ_EXTERNAL_STORAGE
    };

    public static boolean storagePermissionsGranted(Context context) {
        for (String perm : storagePermissions) {
            if (ContextCompat.checkSelfPermission(context, perm) != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

    public static void requestStoragePermissions(Activity context) {
        ActivityCompat.requestPermissions(context, storagePermissions, STORAGE_PERMISSIONS_REQUEST);
    }

    public void onResume() {
        super.onResume();
        if (!storagePermissionsGranted(this)) {
            Log.w(TAG, "Needs Permissions");
            requestStoragePermissions(this);
        } else {
            Log.w(TAG, "Permissions ok");
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch(requestCode) {
            case STORAGE_PERMISSIONS_REQUEST:
                for (int i = 0; i < permissions.length; i++) {
                   if (grantResults[i] != PackageManager.PERMISSION_GRANTED) {
                        Log.w(TAG, "Permission DENIED - Finish");
                        finish();
                    }
                }
                Log.w(TAG, "Permissions ok");
                break;
        }
    }
}

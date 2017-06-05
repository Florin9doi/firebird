package org.firebird.emu;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.Manifest;
import android.net.Uri;
import android.util.Log;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.provider.DocumentFile;
import android.widget.Toast;

import org.qtproject.qt5.android.bindings.QtActivity;

public class MainActivity
        extends QtActivity
        implements ActivityCompat.OnRequestPermissionsResultCallback {

    private static final String TAG = "org.firebird.emu";
    private static final int STORAGE_PERMISSIONS_REQUEST = 1;
    private static final int SDCARD_DOCUMENT_REQUEST = 2;
    private static Context mContext = null;
    private static Activity mActivity = null;

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
        mContext = getApplicationContext();
        mActivity = this;
        if (!storagePermissionsGranted(this)) {
            Log.d(TAG, "Lacks permissions - call requestStoragePermissions()");
            requestStoragePermissions(this);
        }
    }

//    private static native void receivedFile();

    public static void openFile() {
        Log.w(TAG, "openFile - INTENT");
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.setType("*/*");
        mActivity.startActivityForResult(intent, SDCARD_DOCUMENT_REQUEST);
    }


    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent resultData) {
        super.onActivityResult(requestCode, resultCode, resultData);

        switch(requestCode) {
            case SDCARD_DOCUMENT_REQUEST:
                if (resultCode == RESULT_OK) {
                    Log.w(TAG, "onActivityResult - SDCARD_DOCUMENT_TREE_REQUEST: OK  !!!");
                    Uri treeUri = resultData.getData();
                    Log.w(TAG, "onActivityResult - URI:" + treeUri.toString());

//                    receivedFile();

//                    DocumentFile pickedDir = DocumentFile.fromTreeUri(this, treeUri);

//                    // List all existing files inside picked directory
//                    for (DocumentFile file : pickedDir.listFiles()) {
//                        Log.d(TAG, "Found file " + file.getName() + " with size " + file.length());
//                    }
                }

                break;
            default:
                Log.w(TAG, "onActivityResult - requestCode:" + requestCode + " resultCode:" + resultCode);
        }
    }


    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch(requestCode) {
            case STORAGE_PERMISSIONS_REQUEST:
                for (int i = 0; i < permissions.length; i++) {
                   if (grantResults[i] != PackageManager.PERMISSION_GRANTED) {
                        Toast.makeText(getApplicationContext(), "Permission denied - exit!", Toast.LENGTH_LONG).show();
                        finish();
                    }
                }
                break;
            default:
                Log.w(TAG, "onRequestPermissionsResult - requestCode:" + requestCode);
        }
    }
}

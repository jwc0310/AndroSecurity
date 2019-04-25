package com.andy.androsecurity.jni;

public class jni {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public static String getStringFromJNI() {
        return stringFromJNI();
    }

    public static int encryptSo() {
        return encryptSO();
    }

    private static native String stringFromJNI();

    private static native String stringFromJNIEncrypt();

    private static native int encryptSO();
}

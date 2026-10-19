package com.oasisfeng.island.analytics;

import android.util.Log;

/** Local diagnostics for the demo build. No external crash-reporting service is used. */
public abstract class CrashReport {
    private static final String TAG = "Island.CrashReport";

    static void logException(final Throwable error) { Log.e(TAG, "Reported exception", error); }
    static void log(final String message) { Log.i(TAG, message); }
    static void setProperty(final String key, final String value) { Log.d(TAG, key + "=" + value); }
    static void setProperty(final String key, final int value) { Log.d(TAG, key + "=" + value); }
    static void setProperty(final String key, final boolean value) { Log.d(TAG, key + "=" + value); }

    public static void initCrashHandler() { /* Android's default handler writes crashes to logcat. */ }
}

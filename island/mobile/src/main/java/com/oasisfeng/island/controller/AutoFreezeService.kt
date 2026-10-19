package com.oasisfeng.island.controller

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Build.VERSION.SDK_INT
import android.os.Build.VERSION_CODES.O
import android.os.IBinder
import com.oasisfeng.island.engine.IslandManager
import com.oasisfeng.island.mobile.R

/** Freezes selected apps when the screen turns off, inside the managed profile. */
class AutoFreezeService : Service() {

    private val packagesToFreeze = linkedSetOf<String>()
    private var receiverRegistered = false

    private val screenOffReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            packagesToFreeze.toList().forEach { IslandManager.ensureAppHiddenState(this@AutoFreezeService, it, true) }
            packagesToFreeze.clear()
            stopSelf()
        }
    }

    override fun onCreate() {
        super.onCreate()
        if (SDK_INT >= O) {
            val manager = getSystemService(NotificationManager::class.java)
            manager.createNotificationChannel(NotificationChannel(CHANNEL_ID, getString(R.string.auto_freeze_channel), NotificationManager.IMPORTANCE_LOW))
        }
        val notification = (if (SDK_INT >= O) Notification.Builder(this, CHANNEL_ID) else Notification.Builder(this))
            .setSmallIcon(android.R.drawable.ic_lock_idle_lock)
            .setContentTitle(getString(R.string.auto_freeze_notification_title))
            .setContentText(getString(R.string.auto_freeze_notification_text))
            .setOngoing(true)
            .build()
        startForeground(NOTIFICATION_ID, notification)
        registerReceiver(screenOffReceiver, IntentFilter(Intent.ACTION_SCREEN_OFF))
        receiverRegistered = true
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        intent?.getStringExtra(EXTRA_PACKAGE)?.let(packagesToFreeze::add)
        if (packagesToFreeze.isEmpty()) stopSelf(startId)
        return START_NOT_STICKY
    }

    override fun onDestroy() {
        if (receiverRegistered) unregisterReceiver(screenOffReceiver)
        super.onDestroy()
    }

    override fun onBind(intent: Intent?): IBinder? = null

    companion object {
        private const val CHANNEL_ID = "auto_freeze"
        private const val NOTIFICATION_ID = 0xAF31
        private const val EXTRA_PACKAGE = "package"

        @JvmStatic fun start(context: Context, packageName: String) {
            val intent = Intent(context, AutoFreezeService::class.java).putExtra(EXTRA_PACKAGE, packageName)
            if (SDK_INT >= O) context.startForegroundService(intent) else context.startService(intent)
        }
    }
}

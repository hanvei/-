package com.oasisfeng.android.content.pm


import android.content.ComponentName
import android.content.Context
import androidx.core.content.ContextCompat

private const val COMPONENT_ENABLED_STATE_DEFAULT = 0
private const val COMPONENT_ENABLED_STATE_DISABLED = 2
private const val COMPONENT_ENABLED_STATE_ENABLED = 1
private const val DONT_KILL_APP = 1

inline fun <reified T : Any> Context.getSystemService(): T? =
		ContextCompat.getSystemService(this, T::class.java)

inline fun <reified T> Context.disableComponent() =
		packageManager.setComponentEnabledSetting(ComponentName(this, T::class.java), COMPONENT_ENABLED_STATE_DISABLED, DONT_KILL_APP)
inline fun <reified T> Context.enableComponent() =
		packageManager.setComponentEnabledSetting(ComponentName(this, T::class.java), COMPONENT_ENABLED_STATE_ENABLED, DONT_KILL_APP)
inline fun <reified T> Context.resetComponentState() =
		packageManager.setComponentEnabledSetting(ComponentName(this, T::class.java), COMPONENT_ENABLED_STATE_DEFAULT, DONT_KILL_APP)




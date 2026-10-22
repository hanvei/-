package top.niunaijun.blackbox.fake.device;

import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.util.DisplayMetrics;

/** Experimental resource view for apps running inside the PAD proof-of-concept. */
public final class VirtualTabletDisplay {
    private VirtualTabletDisplay() {
    }

    @SuppressWarnings("deprecation")
    public static void apply(Context context) {
        if (context == null) return;
        Resources resources = context.getResources();
        if (resources == null) return;

        Configuration configuration = new Configuration(resources.getConfiguration());
        DisplayMetrics metrics = new DisplayMetrics();
        metrics.setTo(resources.getDisplayMetrics());

        // Report a tablet-sized resource configuration inside the virtual process only.
        configuration.screenWidthDp = 800;
        configuration.screenHeightDp = 1280;
        configuration.smallestScreenWidthDp = 800;
        configuration.densityDpi = 320;
        metrics.widthPixels = 1600;
        metrics.heightPixels = 2560;
        metrics.densityDpi = 320;
        metrics.density = 2.0f;
        metrics.scaledDensity = 2.0f;

        resources.updateConfiguration(configuration, metrics);
    }
}

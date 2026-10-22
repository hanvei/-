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
        // HUAWEI MatePad 11: 2560 x 1600, about 275 ppi. Android uses the
        // nearest stable logical density bucket, producing a genuine tablet sw.
        configuration.screenWidthDp = 914;
        configuration.screenHeightDp = 1463;
        configuration.smallestScreenWidthDp = 914;
        configuration.densityDpi = 280;
        metrics.widthPixels = 1600;
        metrics.heightPixels = 2560;
        metrics.densityDpi = 280;
        metrics.density = 1.75f;
        metrics.scaledDensity = 1.75f;

        resources.updateConfiguration(configuration, metrics);
    }
}

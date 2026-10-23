#include <sys/system_properties.h>
#include <cstring>
#include <mutex>
#include <string>
#include "./xdl.h"
#include <android/log.h>
#include <dlfcn.h>
#include "Dobby/dobby.h"
#include "VirtualSpoof.h"


#define LOG_TAG "VirtualSpoof"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

static int (*orig_system_property_get)(const char *name, char *value) = nullptr;
typedef void (*property_read_callback)(void *cookie, const char *name,
                                      const char *value, uint32_t serial);
static void (*orig_system_property_read_callback)(const prop_info *pi,
                                                  property_read_callback callback,
                                                  void *cookie) = nullptr;
static std::mutex g_spoof_mutex;
static std::string g_manufacturer = "Google";
static std::string g_brand = "google";
static std::string g_model = "Pixel 6";
static std::string g_device = "oriole";
static std::string g_product = "oriole";
static std::string g_fingerprint = "google/oriole/oriole:12/SP1A.210812.015/7679548:user/release-keys";
static std::string g_serial = "1A2B3C4D5E6F";
static std::string g_characteristics = "default";

static int copy_spoofed_value(const std::string &source, char *value) {
    strcpy(value, source.c_str());
    return static_cast<int>(source.size());
}

void setDeviceSpoofValues(
        const char *manufacturer,
        const char *brand,
        const char *model,
        const char *device,
        const char *product,
        const char *fingerprint,
        const char *serial,
        const char *characteristics) {
    std::lock_guard<std::mutex> lock(g_spoof_mutex);
    if (manufacturer != nullptr) g_manufacturer = manufacturer;
    if (brand != nullptr) g_brand = brand;
    if (model != nullptr) g_model = model;
    if (device != nullptr) g_device = device;
    if (product != nullptr) g_product = product;
    if (fingerprint != nullptr) g_fingerprint = fingerprint;
    if (serial != nullptr) g_serial = serial;
    if (characteristics != nullptr) g_characteristics = characteristics;
    LOGD("[spoof] Updated profile: %s %s", g_manufacturer.c_str(), g_model.c_str());
}


static bool property_is(const char *name, const char *a, const char *b = nullptr,
                        const char *c = nullptr, const char *d = nullptr) {
    return strcmp(name, a) == 0 || (b && strcmp(name, b) == 0)
           || (c && strcmp(name, c) == 0) || (d && strcmp(name, d) == 0);
}

int my_system_property_get(const char *name, char *value) {
    std::lock_guard<std::mutex> lock(g_spoof_mutex);
    if (property_is(name, "ro.product.model", "ro.product.system.model",
                    "ro.product.vendor.model", "ro.product.odm.model")) {
        return copy_spoofed_value(g_model, value);
    }
    if (strcmp(name, "ro.build.characteristics") == 0) {
        return copy_spoofed_value(g_characteristics, value);
    }
    if (property_is(name, "ro.product.brand", "ro.product.system.brand",
                    "ro.product.vendor.brand", "ro.product.odm.brand")) {
        return copy_spoofed_value(g_brand, value);
    }
    if (property_is(name, "ro.product.manufacturer", "ro.product.system.manufacturer",
                    "ro.product.vendor.manufacturer", "ro.product.odm.manufacturer")) {
        return copy_spoofed_value(g_manufacturer, value);
    }
    if (property_is(name, "ro.product.device", "ro.product.system.device",
                    "ro.product.vendor.device", "ro.product.odm.device")) {
        return copy_spoofed_value(g_device, value);
    }
    if (property_is(name, "ro.product.name", "ro.product.system.name",
                    "ro.product.vendor.name", "ro.product.odm.name")) {
        return copy_spoofed_value(g_product, value);
    }
    if (strcmp(name, "ro.build.fingerprint") == 0) {
        return copy_spoofed_value(g_fingerprint, value);
    }
    if (strcmp(name, "ro.serialno") == 0) {
        return copy_spoofed_value(g_serial, value);
    }
    if (strcmp(name, "ro.hardware") == 0) {
        return copy_spoofed_value("kirin9000E", value);
    }
    if (strcmp(name, "ro.product.board") == 0) {
        return copy_spoofed_value("WGR", value);
    }
    if (strcmp(name, "ro.build.type") == 0) {
        return copy_spoofed_value("user", value);
    }
    if (strcmp(name, "ro.build.tags") == 0) {
        return copy_spoofed_value("release-keys", value);
    }
    if (strcmp(name, "ro.build.display.id") == 0) {
        return copy_spoofed_value("WGR-W09 4.2.0.213(C00E100R1P9)", value);
    }
    if (strcmp(name, "ro.build.id") == 0) {
        return copy_spoofed_value("HUAWEIWGR-W09", value);
    }
    if (strcmp(name, "ro.build.version.incremental") == 0) {
        return copy_spoofed_value("104.2.0.213C00", value);
    }
    if (strcmp(name, "ro.build.version.emui") == 0) {
        return copy_spoofed_value("EmotionUI_14.2.0", value);
    }
    if (strcmp(name, "ro.sf.lcd_density") == 0) {
        return copy_spoofed_value("320", value);
    }
    if (orig_system_property_get) {
        return orig_system_property_get(name, value);
    }
    value[0] = '\0';
    return 0;
}

static bool get_spoofed_property(const char *name, std::string &result) {
    if (property_is(name, "ro.product.model", "ro.product.system.model",
                    "ro.product.vendor.model", "ro.product.odm.model")) result = g_model;
    else if (strcmp(name, "ro.build.characteristics") == 0) result = g_characteristics;
    else if (property_is(name, "ro.product.brand", "ro.product.system.brand",
                         "ro.product.vendor.brand", "ro.product.odm.brand")) result = g_brand;
    else if (property_is(name, "ro.product.manufacturer", "ro.product.system.manufacturer",
                         "ro.product.vendor.manufacturer", "ro.product.odm.manufacturer")) result = g_manufacturer;
    else if (property_is(name, "ro.product.device", "ro.product.system.device",
                         "ro.product.vendor.device", "ro.product.odm.device")) result = g_device;
    else if (property_is(name, "ro.product.name", "ro.product.system.name",
                         "ro.product.vendor.name", "ro.product.odm.name")) result = g_product;
    else if (strcmp(name, "ro.build.fingerprint") == 0) result = g_fingerprint;
    else if (strcmp(name, "ro.serialno") == 0) result = g_serial;
    else if (strcmp(name, "ro.hardware") == 0 || strcmp(name, "ro.board.platform") == 0) result = "kirin9000E";
    else if (strcmp(name, "ro.product.board") == 0) result = "WGR";
    else if (strcmp(name, "ro.build.type") == 0) result = "user";
    else if (strcmp(name, "ro.build.tags") == 0) result = "release-keys";
    else if (strcmp(name, "ro.build.display.id") == 0) result = "WGR-W09 4.2.0.213(C00E100R1P9)";
    else if (strcmp(name, "ro.build.id") == 0) result = "HUAWEIWGR-W09";
    else if (strcmp(name, "ro.build.version.incremental") == 0) result = "104.2.0.213C00";
    else if (strcmp(name, "ro.build.version.emui") == 0) result = "EmotionUI_14.2.0";
    else if (strcmp(name, "ro.sf.lcd_density") == 0) result = "320";
    else return false;
    return true;
}

struct ReadCallbackContext {
    property_read_callback callback;
    void *cookie;
};

static void spoofing_read_callback(void *opaque, const char *name,
                                   const char *value, uint32_t serial) {
    ReadCallbackContext *context = static_cast<ReadCallbackContext *>(opaque);
    std::string replacement;
    {
        std::lock_guard<std::mutex> lock(g_spoof_mutex);
        if (get_spoofed_property(name, replacement)) value = replacement.c_str();
    }
    context->callback(context->cookie, name, value, serial);
}

void my_system_property_read_callback(const prop_info *pi,
                                      property_read_callback callback,
                                      void *cookie) {
    if (!orig_system_property_read_callback || !callback) return;
    ReadCallbackContext context = { callback, cookie };
    orig_system_property_read_callback(pi, spoofing_read_callback, &context);
}

void install_property_get_hook() {
    void* handle = xdl_open("libc.so", XDL_DEFAULT);
    void* target = xdl_dsym(handle, "__system_property_get", nullptr);
    if (target) {
        if (DobbyHook(target, (void*)my_system_property_get, (void**)&orig_system_property_get) == 0) {
            LOGD("Spoof installed successfully");
        } else {
            LOGD("Spoof hook failed");
        }
        xdl_close(handle);
    } else{
        xdl_close(handle);
    }

    handle = xdl_open("libc.so", XDL_DEFAULT);
    target = xdl_dsym(handle, "__system_property_read_callback", nullptr);
    if (target && DobbyHook(target, (void*)my_system_property_read_callback,
                            (void**)&orig_system_property_read_callback) == 0) {
        LOGD("__system_property_read_callback hook installed successfully");
    } else {
        LOGD("__system_property_read_callback hook failed");
    }
    if (handle) xdl_close(handle);

}


__attribute__((constructor)) void init_virtual_spoof()
{
    install_property_get_hook();
    LOGD("VirtualSpoof: __system_property_get hook loaded");
}

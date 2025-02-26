/*
 * Copyright (C) 2022 Paranoid Android
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdlib>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <vector>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;
using std::string;

std::vector<std::string> ro_props_default_source_order = {
    "",
    "odm.",
    "odm_dlkm.",
    "product.",
    "system.",
    "system_ext.",
    "vendor.",
    "vendor_dlkm.",
};

void property_override(string prop, string value)
{
    auto pi = (prop_info*) __system_property_find(prop.c_str());

    if (pi != nullptr)
        __system_property_update(pi, value.c_str(), value.size());
    else
        __system_property_add(prop.c_str(), prop.size(), value.c_str(), value.size());
}

void load_redmi_fog() {
    property_override("ro.product.brand", "Redmi");
    property_override("ro.product.device", "fog");
    property_override("ro.product.manufacturer", "Xiaomi");
    property_override("ro.product.marketname", "Redmi 10C");
    property_override("ro.product.model", "220333QAG");
    property_override("ro.product.mod_device", "aosp_fog");
    property_override("ro.product.name", "fog_global");
}

void load_redmi_fog_global2() {
    property_override("ro.product.brand", "Redmi");
    property_override("ro.product.device", "fog");
    property_override("ro.product.manufacturer", "Xiaomi");
    property_override("ro.product.marketname", "Redmi 10 Power");
    property_override("ro.product.model", "220333QBI");
    property_override("ro.product.mod_device", "aosp_fog_global2");
    property_override("ro.product.name", "fog_global2");
}

void load_redmi_fog_in() {
    property_override("ro.product.brand", "Redmi");
    property_override("ro.product.device", "fog");
    property_override("ro.product.manufacturer", "Xiaomi");
    property_override("ro.product.marketname", "Redmi 10");
    property_override("ro.product.model", "220333QBI");
    property_override("ro.product.mod_device", "aosp_fog_in");
    property_override("ro.product.name", "fog_in");
}

void load_redmi_rain() {
    property_override("ro.product.brand", "Redmi");
    property_override("ro.product.device", "rain");
    property_override("ro.product.manufacturer", "Xiaomi");
    property_override("ro.product.marketname", "Redmi 10C");
    property_override("ro.product.model", "220333QNY");
    property_override("ro.product.mod_device", "aosp_rain");
    property_override("ro.product.name", "rain_global");
}

void load_redmi_wind() {
    property_override("ro.product.brand", "Redmi");
    property_override("ro.product.device", "wind");
    property_override("ro.product.manufacturer", "Xiaomi");
    property_override("ro.product.marketname", "Redmi 10C");
    property_override("ro.product.model", "220333QL");
    property_override("ro.product.mod_device", "aosp_wind");
    property_override("ro.product.name", "wind_global");
}

void vendor_load_properties() {
    std::string hwname = GetProperty("ro.boot.hwname", "");
    if (hwname == "fog_in") {
        load_redmi_fog_in();
    } else if (hwname == "fog") {
        load_redmi_fog();
    } else if (hwname == "rain") {
        load_redmi_rain();
    } else if (hwname == "wind") {
        load_redmi_wind();
    } else {
        load_redmi_fog_global2();
    }

    // Set hardware revision
    property_override("ro.boot.hardware.revision", GetProperty("ro.boot.hwversion", "").c_str());

    // Set dalvik heap configuration
    std::string heapstartsize, heapgrowthlimit, heapsize, heapminfree,
			heapmaxfree, heaptargetutilization;

    struct sysinfo sys;
    sysinfo(&sys);

    if (sys.totalram > 5072ull * 1024 * 1024) {
        // from - phone-xhdpi-6144-dalvik-heap.mk
        heapstartsize = "16m";
        heapgrowthlimit = "256m";
        heapsize = "512m";
        heaptargetutilization = "0.5";
        heapminfree = "8m";
        heapmaxfree = "32m";
    } else if (sys.totalram > 3072ull * 1024 * 1024) {
        // from - phone-xhdpi-4096-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heaptargetutilization = "0.6";
        heapminfree = "8m";
        heapmaxfree = "16m";
    } else {
        // from - phone-xhdpi-2048-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heaptargetutilization = "0.75";
        heapminfree = "512k";
        heapmaxfree = "8m";
    }

    property_override("dalvik.vm.heapstartsize", heapstartsize);
    property_override("dalvik.vm.heapgrowthlimit", heapgrowthlimit);
    property_override("dalvik.vm.heapsize", heapsize);
    property_override("dalvik.vm.heaptargetutilization", heaptargetutilization);
    property_override("dalvik.vm.heapminfree", heapminfree);
    property_override("dalvik.vm.heapmaxfree", heapmaxfree);
}

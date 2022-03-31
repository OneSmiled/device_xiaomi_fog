#
# Copyright (C) 2022 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Soong namespaces
PRODUCT_SOONG_NAMESPACES += $(LOCAL_PATH)

# Inherit from vendor if exists
$(call inherit-product-if-exists, vendor/xiaomi/fog/fog-vendor.mk)

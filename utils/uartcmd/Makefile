include $(TOPDIR)/rules.mk

PKG_NAME:=uartcmd
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/uartcmd
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=USB UART Command Utility
  DEPENDS:=+kmod-usb-acm
endef

define Package/uartcmd/description
  Send commands to a USB UART device at /dev/ttyACM0 using 115200 baud.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) \
		$(TARGET_LDFLAGS) \
		-o $(PKG_BUILD_DIR)/uartcmd \
		$(PKG_BUILD_DIR)/uartcmd.c
endef

define Package/uartcmd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/uartcmd \
		$(1)/usr/bin/uartcmd
endef

$(eval $(call BuildPackage,uartcmd))

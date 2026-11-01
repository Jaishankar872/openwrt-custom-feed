
include $(TOPDIR)/rules.mk

PKG_NAME:=helloworld
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/helloworld
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Hello World Application
endef

define Package/helloworld/description
  Simple Hello World application for OpenWrt.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) \
		$(TARGET_LDFLAGS) \
		-o $(PKG_BUILD_DIR)/helloworld \
		$(PKG_BUILD_DIR)/helloworld.c
endef

define Package/helloworld/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/helloworld \
		$(1)/usr/bin/helloworld
endef

$(eval $(call BuildPackage,helloworld))

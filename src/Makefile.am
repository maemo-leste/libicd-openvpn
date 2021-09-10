MAINTAINERCLEANFILES = \
	Makefile.in

INCLUDES = \
	@GLIB_CFLAGS@ \
	@GCONF_CFLAGS@ \
	@ICD2_CFLAGS@ \
	@OSSO_IC_DEV_CFLAGS@

LDFLAGS = -avoid-version

plugindir = @ICD_PLUGIN_DIR@

plugin_LTLIBRARIES = \
	libicd_provider_openvpn.la \
	libicd_network_openvpn.la

libicd_provider_openvpn_la_SOURCES = \
	libicd_provider_openvpn.c \
	libicd_openvpn_config.c \
	libicd_openvpn.h

libicd_network_openvpn_la_SOURCES = \
	libicd_network_openvpn.c \
	libicd_network_openvpn_helpers.c \
	libicd_network_openvpn_dbus.c \
	libicd_network_openvpn.h \
	dbus_openvpn.c \
	dbus_openvpn.h \
	libicd_openvpn_config.c \
	libid_openvpn_shared.h \
	libicd_openvpn.h

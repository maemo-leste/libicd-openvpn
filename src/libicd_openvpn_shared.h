/*
 * This file is part of libicd-openvpn
 *
 * Copyright (C) 2021, Merlijn Wajer <merlijn@wizzup.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 3.0 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef __LIBICD_OPENVPN_SHARED_H
#define __LIBICD_OPENVPN_SHARED_H

#define OPENVPN_NETWORK_TYPE "OPENVPN"
#define OPENVPN_PROVIDER_TYPE "OPENVPN"
#define OPENVPN_PROVIDER_NAME "OpenVPN Provider"

#define OPENVPN_DEFAULT_SERVICE_ATTRIBUTES 0
#define OPENVPN_DEFAULT_SERVICE_PRIORITY 0

#define GC_OPENVPN "/system/osso/connectivity/providers/openvpn"
#define GC_ICD_OPENVPN_AVAILABLE_IDS "/system/osso/connectivity/srv_provider/OPENVPN/available_ids"

#define GC_NETWORK_TYPE "/system/osso/connectivity/network_type/OPENVPN"
#define GC_OPENVPN_ACTIVE  GC_NETWORK_TYPE"/active_config"
#define GC_OPENVPN_SYSTEM  GC_NETWORK_TYPE"/system_wide_enabled"

#define GC_CONFIG_OVERRIDE "config_file_override"

#define ICD_OPENVPN_DBUS_INTERFACE "org.maemo.OpenVPN"
#define ICD_OPENVPN_DBUS_PATH "/org/maemo/OpenVPN"

#define ICD_OPENVPN_METHOD_GETSTATUS ICD_OPENVPN_DBUS_INTERFACE".GetStatus"

#define ICD_OPENVPN_SIGNAL_STATUSCHANGED      "StatusChanged"
#define ICD_OPENVPN_SIGNAL_STATUSCHANGED_FILTER "member='" ICD_OPENVPN_SIGNAL_STATUSCHANGED "'"

#define ICD_OPENVPN_SIGNALS_STATUS_STATE_CONNECTED "Connected"
#define ICD_OPENVPN_SIGNALS_STATUS_STATE_STARTED "Started"
#define ICD_OPENVPN_SIGNALS_STATUS_STATE_STOPPED "Stopped"

#define ICD_OPENVPN_SIGNALS_STATUS_MODE_NORMAL "Normal"
#define ICD_OPENVPN_SIGNALS_STATUS_MODE_PROVIDER "Provider"

enum OPENVPN_DBUS_METHOD_START_RESULT {
	OPENVPN_DBUS_METHOD_START_RESULT_OK,
	OPENVPN_DBUS_METHOD_START_RESULT_FAILED,
	OPENVPN_DBUS_METHOD_START_RESULT_INVALID_CONFIG,
	OPENVPN_DBUS_METHOD_START_RESULT_INVALID_ARGS,
	OPENVPN_DBUS_METHOD_START_RESULT_ALREADY_RUNNING,
	OPENVPN_DBUS_METHOD_START_RESULT_REFUSED,
};

enum OPENVPN_DBUS_METHOD_STOP_RESULT {
	OPENVPN_DBUS_METHOD_STOP_RESULT_OK,
	OPENVPN_DBUS_METHOD_STOP_RESULT_NOT_RUNNING,
	OPENVPN_DBUS_METHOD_STOP_RESULT_REFUSED,
};

#endif				/* __LIBICD_OPENVPN_SHARED_H */

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

#ifndef __LIBICD_NETWORK_OPENVPN_H
#define __LIBICD_NETWORK_OPENVPN_H
#include <string.h>
#include <stdio.h>
#include <glib.h>
#include <pwd.h>

#include <gconf/gconf-client.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <osso-ic-gconf.h>
#include <network_api.h>

#include "icd/support/icd_log.h"

#include "dbus_openvpn.h"
#include "libicd_openvpn.h"

struct _network_openvpn_state {
	/* State data here, since without IAP we do not have openvpn_network_data */
	gboolean system_wide_enabled;
	gchar *active_config;
	gboolean iap_connected;
	gboolean service_provider_mode;

	gboolean openvpn_running;

	gboolean gconf_transition_ongoing;

	gboolean dbus_failed_to_start;
};
typedef struct _network_openvpn_state network_openvpn_state;

struct _network_openvpn_private {
	/* For pid monitoring */
	icd_nw_watch_pid_fn watch_cb;
	gpointer watch_cb_token;

	icd_nw_close_fn close_cb;

#if 0
	icd_srv_limited_conn_fn limited_conn_fn;
#endif

	GSList *network_data_list;

	GConfClient *gconf_client;
	guint gconf_cb_id_systemwide;

	network_openvpn_state state;
};
typedef struct _network_openvpn_private network_openvpn_private;

struct _openvpn_network_data {
	network_openvpn_private *private;

	icd_nw_ip_up_cb_fn ip_up_cb;
	gpointer ip_up_cb_token;

	icd_nw_ip_down_cb_fn ip_down_cb;
	gpointer ip_down_cb_token;

	/* Tor pid */
	pid_t openvpn_pid;

	/* For matching / callbacks later on (like close and limited_conn callback) */
	gchar *network_type;
	guint network_attrs;
	gchar *network_id;
};
typedef struct _openvpn_network_data openvpn_network_data;

gboolean icd_nw_init(struct icd_nw_api *network_api,
		     icd_nw_watch_pid_fn watch_fn, gpointer watch_fn_token,
		     icd_nw_close_fn close_fn, icd_nw_status_change_fn status_change_fn, icd_nw_renew_fn renew_fn);

void openvpn_state_change(network_openvpn_private * private, openvpn_network_data * network_data,
			  network_openvpn_state new_state, int source);

/* Helpers */
void network_stop_all(openvpn_network_data * network_data);
void network_free_all(openvpn_network_data * network_data);
pid_t spawn_as(const char *username, const char *pathname, char *args[]);
openvpn_network_data *icd_openvpn_find_first_network_data(network_openvpn_private * private);
openvpn_network_data *icd_openvpn_find_network_data(const gchar * network_type,
						    guint network_attrs,
						    const gchar * network_id, network_openvpn_private * private);
gboolean string_equal(const char *a, const char *b);
int startup_openvpn(openvpn_network_data * network_data, char *config);

enum icd_openvpn_event_source_type {
	EVENT_SOURCE_IP_UP,
	EVENT_SOURCE_IP_DOWN,
	EVENT_SOURCE_GCONF_CHANGE,
	EVENT_SOURCE_OPENVPN_PID_EXIT,
	EVENT_SOURCE_DBUS_CALL_START,
	EVENT_SOURCE_DBUS_CALL_STOP,
};

/* DBus methods */
DBusHandlerResult start_callback(DBusConnection * connection, DBusMessage * message, void *user_data);
DBusHandlerResult stop_callback(DBusConnection * connection, DBusMessage * message, void *user_data);
DBusHandlerResult getstatus_callback(DBusConnection * connection, DBusMessage * message, void *user_data);
void emit_status_signal(network_openvpn_state state);

#endif

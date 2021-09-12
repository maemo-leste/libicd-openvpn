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

#include <stdio.h>

#include <glib.h>
#include <gconf/gconf-client.h>

#include "libicd_openvpn.h"

gboolean config_is_known(const char *config_name)
{
	GConfClient *gconf_client;
	GSList *providers = NULL, *l = NULL;
	gboolean match = FALSE;

	gconf_client = gconf_client_get_default();

	providers = gconf_client_get_list(gconf_client, GC_ICD_OPENVPN_AVAILABLE_IDS, GCONF_VALUE_STRING, NULL);
	for (l = providers; l; l = l->next) {
		if (!strcmp(l->data, config_name)) {
			match = TRUE;
			break;
		}
	}
	g_slist_free_full(providers, g_free);
	g_object_unref(gconf_client);

	return match;
}

gboolean network_is_openvpn_provider(const char *network_id, char **ret_gconf_service_id)
{
	GConfClient *gconf_client;
	gchar *iap_gconf_key;
	char *gconf_service_type = NULL;
	char *gconf_service_id = NULL;
	gboolean service_id_known = FALSE;
	gboolean match = FALSE;

	gconf_client = gconf_client_get_default();

	iap_gconf_key = g_strdup_printf("/system/osso/connectivity/IAP/%s/service_type", network_id);
	gconf_service_type = gconf_client_get_string(gconf_client, iap_gconf_key, NULL);
	g_free(iap_gconf_key);

	iap_gconf_key = g_strdup_printf("/system/osso/connectivity/IAP/%s/service_id", network_id);
	gconf_service_id = gconf_client_get_string(gconf_client, iap_gconf_key, NULL);
	g_free(iap_gconf_key);
	g_object_unref(gconf_client);

	service_id_known = gconf_service_id && config_is_known(gconf_service_id);

	if (ret_gconf_service_id)
		*ret_gconf_service_id = g_strdup(gconf_service_id);

	match = service_id_known && (g_strcmp0(OPENVPN_PROVIDER_TYPE, gconf_service_type) == 0);

	if (gconf_service_type) {
		g_free(gconf_service_type);
	}

	if (gconf_service_id) {
		g_free(gconf_service_id);
	}

	return match;
}

gboolean get_system_wide_enabled(void)
{
	GConfClient *gconf;
	gboolean enabled = FALSE;

	gconf = gconf_client_get_default();

	enabled = gconf_client_get_bool(gconf, GC_OPENVPN_SYSTEM, NULL);

	g_object_unref(gconf);

	return enabled;
}

char *get_active_config(void)
{
	GConfClient *gconf;
	char *active_config = NULL;

	gconf = gconf_client_get_default();

	active_config = gconf_client_get_string(gconf, GC_OPENVPN_ACTIVE, NULL);

	g_object_unref(gconf);

	return active_config;
}

char *get_config_file(const char *config_name)
{
	GConfClient *gconf;
	gchar *config = NULL;

	gconf = gconf_client_get_default();

	gchar *gc_file = g_strjoin("/", GC_OPENVPN, config_name, GC_CONFIG_OVERRIDE, NULL);
	config = gconf_client_get_string(gconf, gc_file, NULL);
	g_free(gc_file);

	return config;
}

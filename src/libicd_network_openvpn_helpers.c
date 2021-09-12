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

#include "libicd_network_openvpn.h"

/* XXX: Taken from ipv4 module */
gboolean string_equal(const char *a, const char *b)
{
	if (!a)
		return !b;

	if (b)
		return !strcmp(a, b);

	return FALSE;
}

openvpn_network_data *icd_openvpn_find_first_network_data(network_openvpn_private * private)
{
	GSList *l;

	for (l = private->network_data_list; l; l = l->next) {
		openvpn_network_data *found = (openvpn_network_data *) l->data;

		if (!found)
			ON_WARN("openvpn network data is NULL");
		else {
			return found;
		}
	}

	return NULL;
}

openvpn_network_data *icd_openvpn_find_network_data(const gchar * network_type,
						    guint network_attrs,
						    const gchar * network_id, network_openvpn_private * private)
{
	GSList *l;

	for (l = private->network_data_list; l; l = l->next) {
		openvpn_network_data *found = (openvpn_network_data *) l->data;

		if (!found)
			ON_WARN("openvpn network data is NULL");
		else {
			if (found->network_attrs == network_attrs &&
			    string_equal(found->network_type, network_type) &&
			    string_equal(found->network_id, network_id)) {
				return found;
			}
		}
	}

	return NULL;
}

/* pathname and arg are like in execv, returns pid, 0 is error */
pid_t spawn_as(const char *username, const char *pathname, char *args[])
{
	struct passwd *ent = getpwnam(username);
	if (ent == NULL) {
		ON_CRIT("spawn_as: getpwnam failed\n");
		return 0;
	}

	pid_t pid = fork();
	if (pid < 0) {
		ON_CRIT("spawn_as: fork() failed\n");
		return 0;
	} else if (pid == 0) {
		if (setgid(ent->pw_gid)) {
			ON_CRIT("setgid failed\n");
			exit(1);
		}
		if (setuid(ent->pw_uid)) {
			ON_CRIT("setuid failed\n");
			exit(1);
		}
		execv(pathname, args);

		ON_CRIT("execv failed\n");
		exit(1);
	} else {
		ON_DEBUG("spawn_as got pid: %d\n", pid);
		return pid;
	}

	return 0;		// Failure
}

void network_free_all(openvpn_network_data * network_data)
{
	network_openvpn_private *priv = network_data->private;
	if (priv->network_data_list) {
		priv->network_data_list = g_slist_remove(priv->network_data_list, network_data);
	}

	g_free(network_data->network_type);
	g_free(network_data->network_id);

	network_data->private = NULL;

	g_free(network_data);
}

void network_stop_all(openvpn_network_data * network_data)
{
	if (network_data->openvpn_pid != 0) {
		kill(network_data->openvpn_pid, SIGTERM);
	}
}

int startup_openvpn(openvpn_network_data * network_data, char *config)
{
	char *config_filename = NULL;

	config_filename = get_config_file(config);

	if (config_filename == NULL || !g_str_has_prefix(config_filename, "/etc/openvpn/")) {
		ON_WARN("config_filename is NULL or does not start with /etc/openvpn");
		g_free(config_filename);
		return 1;
	}

	char *argss[] = { "/usr/sbin/openvpn", "--config", config_filename, NULL };
	/* TODO: do we run this as root? */
	pid_t pid = spawn_as("root", "/usr/sbin/openvpn", argss);
	if (pid == 0) {
		ON_WARN("Failed to start OpenVPN\n");
		return 1;
	}

	if (config_filename)
		g_free(config_filename);

	ON_INFO("Got OpenVPN_pid: %d\n", pid);
	network_data->openvpn_pid = pid;
	network_data->private->watch_cb(pid, network_data->private->watch_cb_token);

	return 0;
}

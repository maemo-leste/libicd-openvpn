#ifndef __LIBICD_OPENVPN_H
#define __LIBICD_OPENVPN_H

#include <glib.h>
#include "libicd_openvpn_shared.h"

gboolean config_is_known(const char *config_name);
gboolean network_is_openvpn_provider(const char *network_id, char **ret_gconf_service_id);
gboolean get_system_wide_enabled(void);
char *get_config_file(const char *config_name);
char *get_active_config(void);

#define ON_DEBUG(fmt, ...) ILOG_DEBUG(("[OPENVPN NETWORK] "fmt), ##__VA_ARGS__)
#define ON_INFO(fmt, ...) ILOG_INFO(("[OPENVPN NETWORK] " fmt), ##__VA_ARGS__)
#define ON_WARN(fmt, ...) ILOG_WARN(("[OPENVPN NETWORK] %s.%d:" fmt), __func__, __LINE__, ##__VA_ARGS__)
#define ON_ERR(fmt, ...) ILOG_ERR(("[OPENVPN NETWORK] %s.%d:" fmt), __func__, __LINE__, ##__VA_ARGS__)
#define ON_CRIT(fmt, ...) ILOG_CRIT(("[OPENVPN NETWORK] %s.%d:" fmt), __func__, __LINE__, ##__VA_ARGS__)

#endif				/* __LIBICD_OPENVPN_H */

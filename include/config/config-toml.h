/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef LABWC_CONFIG_TOML_H
#define LABWC_CONFIG_TOML_H

#include <stdbool.h>
#include "tomlc17.h"

struct wl_list;

/* Append the actions declared in a TOML array. */
void config_toml_append_actions(toml_datum_t actions, struct wl_list *list);

/*
 * Scan all *.toml files in the labwc-next config directory (XDG order,
 * alphabetical within each dir) and populate the global rc struct.
 *
 * Returns true if at least one .toml file was found and parsed.
 */
bool config_toml_read(void);

#endif /* LABWC_CONFIG_TOML_H */

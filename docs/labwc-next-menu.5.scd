labwc-next-menu(5)

# NAME

labwc-next-menu - configure labwc-next menus

# STATIC MENUS

Define menus with *[[menu]]* entries in a TOML file under
*~/.config/labwc-next/*, for example *menu.toml*. A menu needs an *id* and
may have a *label* and an icon theme name in *icon*. The built-in desktop
and window menus are *root-menu* and *client-menu*, respectively; defining
either ID replaces its defaults.

```
[[menu]]
id = "root-menu"

[[menu.item]]
label = "_Terminal"
actions = [{ name = "Execute", command = "foot" }]

[[menu.item]]
separator = true

[[menu.item]]
label = "_Reload"
actions = [{ name = "Reconfigure" }]
```

An item can have a *label*, an optional *icon*, and an *actions* array.
Use *separator = true* for a line or set *separator* to a string for a
heading. Use *submenu* to reference an already-defined menu ID or nest
*[[menu.item.item]]* entries inside an item to create an inline submenu.

# PIPE MENUS

Set *execute* on a menu or menu item to run a command when the menu is
opened or selected. The command must write TOML to standard output, with
*[[menu]]* definitions as above. Generated menus are kept until the entire
menu closes. For example, a generated menu might print:

```
[[menu]]
id = "generated-tools"
label = "Tools"

[[menu.item]]
label = "Terminal"
actions = [{ name = "Execute", command = "foot" }]
```

# ACCELERATORS

The first character in a label is its keyboard accelerator. Put an
underscore before a different character to use that character instead;
for example, "e_Macs" uses 'm'. Double an underscore to display it
literally. A unique accelerator activates the item or enters its submenu;
a shared accelerator cycles through the matching items.

# SEE ALSO

labwc-next(1), labwc-next-actions(5)

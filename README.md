# Palmdrop's DWM fork

This is a custom build of dwm with a focus on usability, configurability and completeness. The basic dwm build [provided by the suckless team](https://dwm.suckless.org/) is a great base for a desktop setup, but is rarely enough. There are loads of patches out there for extending the basic functionality, and many of these are quite essential to make dwm properly usable and configurable. 

I have applied many of these patches that I deemed essential for a working setup, many patches for enabling more configuration of the look-and-feel of the window manager, and quite a few patches that allow extended control over dwm (for example the IPC patch). All of the patches applied are listed below.

In addition to these patches, I've done quie a few tweaks and changes on my own. These are also listed below, although exact documentation of the changes done in each "tweak" does not exist.

The keybindings can be found in the `config.def.h` file. I will not list them here, since they are tailored to my personal usage and should probably be altered by anyone who wishes to use this build. Many of my keybindings also use many of my personal scripts. I suggest changing these to scripts of your own.

NOTE: This build should be used with care. Some of the changes done are experimental. However, for my personal usage, the build has been stable.

# Patches
Most of the patches applied can be found over at the [suckless website](https://dwm.suckless.org/patches/). Some patches are found elsewhere. If this is the case, I've tried to include a link.

These are the patches I've applied and a short description:
* actualfullscreen - fullscreen is no longer just monocle layout and bar toggle
* alpha - enable bar and borders transparency 
* alwaysfullscreen - prevents focus drift when in fullscreen
* anybar - any bar (not just standard dwm bar) is now compatible with dwm (more can be read here: https://github.com/mihirlad55/dwm-anybar)
* aspectresize - resize floating windows while keeping the aspect ratio
* attachbelow-toggleable - attaches the next client below the currently focused one in the stack
* autoresize - automatically resizes currently invisible clients
* center - adds a rule for centering certain clients when float is toggled
* cfacts - assigns different weights to clients which determines how much screen space they take up (only applicable for non-master clients)
* cfacts-vanitygaps - vanity gaps patch (adds gaps and a bunch of new layouts) compatible with cfacts patch (can be found here: https://github.com/bakkeby/patches/blob/master/dwm/dwm-cfacts-vanitygaps-6.2.diff)
* combo - you can now multi-view or multi-tag by pressing multiple number keys at the same time
* cool-autostart - autostart applications using commands placed in autostart array within config.def.h
* float-border-color - allows for different border colors for floating windows. Source code has been modded slightly. However, the diff-file is unchanged.
* focusonnetactive - focuses "urgent" windows (modded to only optionally be applied)
* focusadjacenttag - view or tags an adjacent tag to the left or right
* focusurgent - function for focusing the next urgent window 
* insets - adds optional insets around the screen. I've modded code to allow for toggling between two different pre-defined insets. This helped me solve an issue with toggling polybar.
* IPC - interprocess communication for dwm. Required to make dwm polybar-compatible. Can be found here: https://github.com/mihirlad55/dwm-ipc
* keymodes - old patch for adding different key modes to dwm. Makes dwm more vim-like. Had to fetch an ancient method (`isprotodel(Client *)`) from an old dwm-version to make this compatible with dwm 6.2.
* monoclesymbol - always display the same symbol for monocle layout
* moveresize - adds functions for moving and resizing floating clients. Modded slightly to fix focus issues.
* movestack - adds functions for moving clients up and down the stack
* noborder (floating fix) - no borders if only one client visible. I created a variable in config.def.h which toggles this functionality.
* savefloats - the previous floating dimensions and position of a client are restored on float toggle.
* scratchpad - scratchpad, multiple scratchpads possible. The patch can be found here: https://github.com/GasparVardanyan/dwm-scratchpad/blob/master/dwm-scratchpad-20200727-bb2e7222baeec7776930354d0e9f210cc2aaad5f.diff
* selonlyborders - disables borders for unfocused clients. I added a variable to toggle this option. Patch found in this reddit thread: https://www.reddit.com/r/suckless/comments/ix2olv/dwm_no_border_for_unselected_clients/
* sticky - sticky windows follow you around to every tag
* swallow - adds an option for configuring terminals to "swallow" applications, i.e, the terminal is hidden and the launched application takes up the space.
* unfloatvisible - unfloats all visible windows (NOTE: I moved the function to dwm.c instead of creating a separate file)
* xresources - adds support for reading from xresources in config.def.h.

# Tweaks
Below follows the changes I've done on my own. Hopefully, I haven't forgotten anything important.

* disable fullscreen on focus loss - mitigates confusion when a new client is opened while another one is in fullscreen
* different borders for tiled and floating clients - mostly used for having no borders for floating windows.
* changed floating layout behavior to actually set the "isfloating" variable to 1 when the floating layout is enabled. New windows are also spawned in actual floating mode. The window floating state is restored when another layout is enabled. This is done to make sure the floating layout is compatible with patches like moveresize.
* made focusonnetactive patch optional using a variable in config.h
* tagandview - a method for simultaneously tagging a client and switching the view to the same tag. Variations of this function has been added for the focusadjacenttag patch (i.e, tagandviewleft() + tagandviewright() has been added).
* next unoccupied tag - a few functions for viewing and tagging the next unoccupied tag
* compacttags - function for removing gaps between used tags, i.e, if tags 2 and 4 are occupied, after applying this function, 1 and 2 will be occupied instead. A similar patch exists here: https://dwm.suckless.org/patches/reorganizetags
* moveresizedouble - function for doubling or halving the width or height of a floating window
* next floating - function for spawning the next window in floating mode
* tagall - tag all visible clients with the specified tag. Also added "tagandviewall" for tagging all visible clients and viewing that tag simultaneously. Added the same functionality for focusing adjacent tags.
* swaptags - swap the currently visible tags with a selected one. Also added swapandviewtags, to simultaneously swap and view the target tag. Added the same functionality for adjacent tags.
* conditionalfunction - function for executing one function if the focused client is in a specific state, and another function if the client is in another state. Here, it's used for changing m-/c-facts if the client is in tiled mode and move the client if it's floating. "CONDITIONAL" macro can be used to more easily use this functionality (NOTE: Experimental, probably won't work for Client fields that are not integers).

# Dependencies
Most patches do not introduce any external dependencies. However, there's one exception: the [IPC patch](https://github.com/mihirlad55/dwm-ipc) has a dependency on [yajl](https://github.com/lloyd/yajl). 

# Other
My build depends on a bunch of personal scripts. Those scripts can be found here: https://github.com/palmdrop/dots

NOTE: I've migrated away from suckless, and hence this repository is no longer maintained. Also, most of my personal scripts, if you wish to use them, are no longer tailored for dwm. Here's a link to the most recent dwm branch of my dotfiles repository: https://github.com/palmdrop/dots/tree/proxy-love


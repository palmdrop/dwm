# dwm - dynamic window manager

Palmdrop's custom build of dwm. Original dwm build by the suckless team found here: https://dwm.suckless.org/

# Patches
Most patches can also be found here: https://dwm.suckless.org/patches/
Some patches are found elsewhere.

* actualfullscreen - fullscreen is not just monocle layout + bar toggle
* alpha - bar and borders transparency 
* alwaysfullscreen - prevents focus drift when in fullscreen
* anybar - any bar (not just standard dwm bar) is now compatible with dwm (sort of) (more can be read here: https://github.com/mihirlad55/dwm-anybar)
* aspectresize - resize floating windows while keeping aspect ratio
* attachbelow-toggleable - attaches next client below the currently focused one in the stack
* autoresize - automatically resizes currently invisible clients
* center - adds rule for centering certain clients when float is toggled
* cfacts - assigns different weights to clients which determines how much screen space they take up (only applicable for non-master clients)
* cfacts-vanitygaps - vanity gaps patch (adds gaps and a bunch of new layouts) compatible with cfacts patch (can be found here: https://github.com/bakkeby/patches/blob/master/dwm/dwm-cfacts-vanitygaps-6.2.diff)
* combo - you can now multi-view or multi-tag by pressing multiple number keys at the same time
* cool-autostart - autostart applications using commands placed in autostart array within config.def.h
* float-border-color - allows for different border colors for floating windows. Source code has been modded slightly a few bugs. However, diff-file is unchaged.
* focusonnetactive - focuses "urgent" windows (modded to only optionally be applied)
* focusadjacenttag - view or tags an adjacent tag to the left or right
* focusurgent - function for focusing the next urgent window 
* insets - adds optional insets around screen. I've modded code to allow for toggling between two different pre-defined insets. This helped me solve an issue with toggling polybar.
* IPC - interprocess communication for dwm. Required to make dwm polybar-compatible. Can be found here: https://github.com/mihirlad55/dwm-ipc
* keymodes - old patch for adding different keymodes to dwm. Makes dwm more vim-like. Had to fetch an antient method (`isprotodel(Client *)`) from an old dwm-version to make this compatible with dwm 6.2.
* monoclesymbol - always display the same symbol for monocle layout
* moveresize - adds functions for moving and resizing floating clients. Modded slightly to fix focus issues.
* movestack - adds functions for moving clients up and down the stack
* noborder (floating fix) - no borders if only one client visible. I created a variable in config.def.h which toggles this functionality.
* savefloats - the previous floating dimensions and position of a client is restored on float toggle.
* scratchpad - scratchpad, multiple scratchpads possible. Patch can be found here: https://github.com/GasparVardanyan/dwm-scratchpad/blob/master/dwm-scratchpad-20200727-bb2e7222baeec7776930354d0e9f210cc2aaad5f.diff
* selonlyborders - disables borders for unfocused clients. I added a variable to toggle this option. Patch found in this reddit thread: https://www.reddit.com/r/suckless/comments/ix2olv/dwm_no_border_for_unselected_clients/
* sticky - a sticky windows follow you around to every tag
* swallow - adds option for configuring terminals to "swallow" applications, i.e, the terminal is hidden and the launched application takes up the space.
* unfloatvisible - unfloats all visible windows (NOTE: I moved the function to dwm.c instead of creating separate file)
* xresources - adds support for reading from xresources in config.def.h.

# Tweaks and "patches" of my own
These are tweaks and changes I've done for my own purposes. I haven't made proper patches out of these changes. I've probably forgotten a bunch of things.

* disable fullscreen on focus loss - mitigates confusion when a new client is opened while another one is in fullscreen
* different borders for tiled and floating clients - mostly used for having no borders for floating windows.
* made focusonnetactive patch optional using a variable in config.h
* tagandview - method for simultaneously tagging a client and switching the view to same tag. Variations of this function has been added for the focusadjacenttag patch (i.e, tagandviewleft() + tagandviewright() has been added).
* next unoccupied tag - a few functions for viewing and tagging the next unoccupied tag
* compacttags - function for removing gaps between used tags, i.e, if tag 2 and 4 are occupied, after applying this function, 1 and 2 will be occupied instead. A similar patch exists here: https://dwm.suckless.org/patches/reorganizetags
* moveresizedouble - function for doubling or halving the width or height of a floating window
* next floating - function for spawning the next window in floating mode
* tagall - tag all visible clients with the specified tag. Also added "tagandviewall" for tagging all visible clients and viewing that tag simultaneously. Added the same functionality for focusing adjacent tags.
* swaptags - swap the currently visible tags with a selected one. Also added swapandviewtags, to simultaneously swap and view the target tag. Added the same functionality for adjacent tags.
* conditionalfunction - function for executing one function if the focused client is in a specific state, and another function if the client is in another state. Here, it's used for changing m-/c-facts if the client is in tiled mode and move the client if it's floating. "CONDITIONAL" macro can be used to more easily use this functionality (NOTE: Experimental, probably won't work for Client fields that are not integers).

# Other
My build depends on a bunch of personal scripts. Those scripts can be found here: https://github.com/palmdrop/dots

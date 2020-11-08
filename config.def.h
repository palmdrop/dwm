/* See LICENSE file for copyright and license details. */

#include "movestack.c"
#include <X11/XF86keysym.h>

/* appearance */
static       unsigned int borderpx  = 1;        /* border pixel of regular windows */
static   unsigned int floatborderpx = 1;        /* border pixel of floating windows */
static       unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gaps      = 25;       /* gaps */
static const unsigned int outergaps = 40;
static       unsigned int gappih    = gaps;     /* horiz inner gap between windows */
static       unsigned int gappiv    = gaps;     /* vert inner gap between windows */
static       unsigned int gappoh    = outergaps;/* horiz outer gap between windows and screen edge */
static       unsigned int gappov    = outergaps;/* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static       int smartborders       = 1;        /* 1 means no borders if only one window is open */
static       int selonlyborders     = 0;        /* 1 means only borders for focused window */

static       int showbar            = 1;        /* 0 means no bar */
static       int topbar             = 0;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *alttrayname      = "tray";    /* Polybar tray instance name */
static const char *altbarcmd        = "$HOME/.config/dwm-polybar/launch.sh"; /* Alternate bar launch command */
static const char *fonts[]          = { "Cozette:size=15" };
static const char dmenufont[]       = "Cozette:size=15";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char inactivefgcolor[]       = "#777777";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";

static char selfloatbordercolor[]   = "#005577";
static char normfloatbordercolor[]  = "#005577";

static const unsigned int baralpha = 0xFF;
static const unsigned int borderalpha = OPAQUE;

static char *colors[][4] = {
       /*                   fg               bg           border           float border */
       [SchemeNorm]     = { normfgcolor,     normbgcolor, normbordercolor, normfloatbordercolor  },
       [SchemeSel]      = { selfgcolor,      selbgcolor,  selbordercolor,  selfloatbordercolor },
};

static const unsigned int alphas[][4]      = {
	/*               fg      bg        border       float*/
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha, borderalpha},
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha, borderalpha},
};


static Inset default_inset = {
	.x = 0,
	.y = 11,
	.w = 0,
	.h = 0,
};

static const Inset alt_inset = {
	.x = 0,
	.y = 0,
	.w = 0,
	.h = 0,
};

/* Paths */
static char keymodepath[] = "/tmp/dwm.keymode";

/* Standard values */
#define TERM "st"
#define TERMSIZE "100x28"
#define BROWSER "brave"

static const char *const autostart[] = {
    TERM, "-t", "hidden", "-g", TERMSIZE, "-e", "vit", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* include(s) defining functions */
#include "keymodes.pre.h"

static Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
    /* class               instance       title           tags mask        iscentered  isfloating  isterminal  noswallow  monitor */
	{ "St",                NULL,          NULL,           0,               1,          0,          1,          0,        -1 },
	{ "Brave-browser",     NULL,          NULL,           0,               1,          0,          0,          0,        -1 },
	{ "Darktable",         NULL,          NULL,           1 << 2,          0,          0,          0,          0,        -1 },
    { "Signal",            NULL,          NULL,           1 << 3,          0,          0,          0,          0,        -1 },
    { "discord",           NULL,          NULL,           1 << 3,          0,          0,          0,          0,        -1 },
    { "Slack",             NULL,          NULL,           1 << 3,          0,          0,          0,          0,        -1 },
    { "Spotify",           NULL,          NULL,           1 << 8,          0,          0,          0,          0,        -1 },
    { "jetbrains-idea-ce", NULL,          NULL,           1 << 1,          0,          0,          0,          0,        -1 },
    { NULL,                "libreoffice", NULL,           1 << 2,          0,          0,          0,          0,        -1 },

    // Windows with "hidden" title are automatically crated as scratchpads
	{ NULL,                NULL,          "hidden",       SCRATCHPAD_MASK, 1,          1,          0,          0,        -1 },
    // Windows with "flaoting" title are automatically opened in flaoting mode
	{ NULL,                NULL,          "floating",     0,               1,          1,          0,          0,        -1 },

    // Temporary rules
    { "game.App",          NULL,          NULL,           0,               1,          1,          0,          0,        -1 },
    { "application.App",   NULL,          NULL,           0,               1,          1,          0,          0,        -1 },
    { "glfw.main",         NULL,          NULL,           0,               1,          1,          0,          0,        -1 },

    // Unused rules
	//{ "URxvt",             NULL,          NULL,           0,               1,          0,          1,          0,        -1 },
	//{ "Alacritty",         NULL,          NULL,           0,               1,          0,          1,          0,        -1 },
    
    // Event tester 
	{ NULL,                NULL,          "Event Tester", 0,               0,          0,          0,          1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static int  nmaster     = 1;    /* number of clients in master area */
static int  resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static int  attachbelow = 1;    /* 1 means attach after the currently active window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol   arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      monocle },
	{ "",      dwindle },
	{ "",      grid },
	{ "",      centeredmaster },
	{ "",      spiral },
	{ "",      bstack },
	//{ "H[]",      deck },
	//{ "",      bstackhoriz },
	//{ "",      nrowgrid },
	//{ "---",      horizgrid },
	//{ "",      gaplessgrid },
	//{ ">M>",      centeredfloatingmaster },
	{ "",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      tagandview,     {.ui = 1 << TAG} }

	//{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	//{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0";
//static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-i", "-F", NULL };
static const char *dmenucmd[] = { "dmenu_run_notify", NULL };
static const char *wificmd[] = { "networkmanager_dmenu", "-m", dmenumon, "-i", "-F", NULL };

static const char *prntscrncmd[] = { "flameshot", "full", "-p", "/home/xan/Pictures/screenshots/", NULL };
static const char *capturecmd[]  = { "flameshot", "gui", NULL };

static const char *termcmd[]     = { TERM, "-g", TERMSIZE, NULL };
static const char *rangercmd[]   = { TERM, "-g", TERMSIZE, "-e", "ranger", NULL };

static const char *mountcmd[]    = { TERM, "-t", "floating", "-g", "50x10", "-e", "sh", "-c", "sudo dmount", NULL };
static const char *umountcmd[]   = { TERM, "-t", "floating", "-g", "50x10", "-e", "sh", "-c", "sudo dumount", NULL };
static const char *rsscmd[]      = { TERM, "-g", TERMSIZE, "-e", "newsboat", NULL };
static const char *taskcmd[]     = { TERM, "-t", "floating", "-g", TERMSIZE, "-e", "sh", "-c", "vit", NULL };
static const char *mpvcmd[]     = { "mpvplay", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",          STRING,  &normbgcolor },
		{ "normbordercolor",      STRING,  &normbordercolor },
		{ "normfgcolor",          STRING,  &normfgcolor },
		{ "inactivefgcolor",      STRING,  &inactivefgcolor },
		{ "selbgcolor",           STRING,  &selbgcolor },
		{ "selbordercolor",       STRING,  &selbordercolor },
		{ "selfgcolor",           STRING,  &selfgcolor },
        { "normfloatbordercolor", STRING,  &normfloatbordercolor },
        { "selfloatbordercolor",  STRING,  &selfloatbordercolor },
		{ "borderpx",          	  INTEGER, &borderpx },
		{ "floatborderpx",     	  INTEGER, &floatborderpx },
		{ "snap",          		  INTEGER, &snap },
        { "y_inset",              INTEGER, &default_inset.y },
		{ "nmaster",          	  INTEGER, &nmaster },
		{ "resizehints",       	  INTEGER, &resizehints },
		{ "mfact",      	 	  FLOAT,   &mfact },
		{ "smartborders", 	 	  INTEGER, &smartborders },
        { "selonlyborders",       INTEGER, &selonlyborders },
        { "gaps",                 INTEGER, &gappih },  
        { "gaps",                 INTEGER, &gappiv },  
        { "outergaps",            INTEGER, &gappoh },  
        { "outergaps",            INTEGER, &gappov },  
        { "keymodepath",          STRING,  &keymodepath },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
    { MODKEY,                       XK_Escape, setkeymode,     {.ui = COMMANDMODE} },

    // Standard settings
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD("togglebardwm") },
	//{ MODKEY|ShiftMask,             XK_b,      toggleinset,    {0} },

	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
    { MODKEY,                       XK_x,      spawn,          SHCMD("slock") },

    { MODKEY|ShiftMask,             XK_x,      spawn,          SHCMD("xkill") },

    // Move stack
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },

    // Fullscreen 
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },

    // Next floating
    { MODKEY,                       XK_space,  togglenextfloating, {0} },

    // Scratchpad
    { MODKEY,                       XK_s,      scratchpad_show,  {0} },
	{ MODKEY|ShiftMask,             XK_s,      scratchpad_hide,  {0} },
	{ MODKEY|ControlMask,           XK_s,      scratchpad_remove,{0} },

    // Window switcher
	{ MODKEY,                       XK_f,      spawn,            SHCMD("dswitcher") },

    // System info shortcuts
    { MODKEY,                       XK_o,      spawn,           SHCMD("herbe-info") },
    { MODKEY,                       XK_p,      spawn,           SHCMD("play") },

    // Volume control
	{ 0,                            XF86XK_AudioRaiseVolume,   spawn,   SHCMD("vol up") },
    { 0,                            XF86XK_AudioLowerVolume,   spawn,   SHCMD("vol down") },
	{ 0,                            XF86XK_AudioMute,          spawn,   SHCMD("vol mute") },

    // Spotify control
	{ 0,                            XF86XK_AudioPlay,          spawn,   SHCMD("play pause") },
    { 0,                            XF86XK_AudioStop,          spawn,   SHCMD("play stop") },
	{ 0,                            XF86XK_AudioPrev,          spawn,   SHCMD("play prev") },
    { 0,                            XF86XK_AudioNext,          spawn,   SHCMD("play next") },

    // Brightness control
	{ 0,                            XF86XK_MonBrightnessUp,    spawn,   SHCMD("bri up") },
	{ 0,                            XF86XK_MonBrightnessDown,  spawn,   SHCMD("bri down") },

    // Screen capture
	{ 0,                            XK_Print,   spawn,         {.v = prntscrncmd } },
    { MODKEY,                       XK_Print,   spawn,         {.v = capturecmd  } },

    // Mount control
	{ MODKEY,                       XK_m,       spawn,         {.v = mountcmd  } },
	{ MODKEY,                       XK_u,       spawn,         {.v = umountcmd  } },

    // Program shortcuts
	{ MODKEY,                       XK_w,       spawn,         {.v = wificmd    } },
	{ MODKEY,                       XK_e,       spawn,         {.v = rangercmd  } },
	{ MODKEY,                       XK_b,       spawn,         SHCMD(BROWSER) },
	{ MODKEY,                       XK_g,       spawn,         SHCMD("gimp") },
	{ MODKEY,                       XK_c,       spawn,         SHCMD("idea") },
	{ MODKEY,                       XK_r,       spawn,         {.v = rsscmd } },
    { MODKEY,                       XK_t,       spawn,         {.v = taskcmd } },
    { MODKEY,                       XK_y,       spawn,         {.v = mpvcmd } },

    { MODKEY|ShiftMask,             XK_d,       spawn,         SHCMD("dmenu_dots_edit") },
    { MODKEY|ShiftMask,             XK_e,       spawn,         SHCMD("dmenu_dirs_edit") },

    // Tags
	TAGKEYS(                        XK_1,                      0),
	TAGKEYS(                        XK_2,                      1),
	TAGKEYS(                        XK_3,                      2),
	TAGKEYS(                        XK_4,                      3),
	TAGKEYS(                        XK_5,                      4),
	TAGKEYS(                        XK_6,                      5),
	TAGKEYS(                        XK_7,                      6),
	TAGKEYS(                        XK_8,                      7),
	TAGKEYS(                        XK_9,                      8),

    { MODKEY,                       XK_a,      nextview,       {0} },
    { MODKEY|ShiftMask,             XK_a,      nexttag,        {0} },
    { MODKEY|ControlMask,           XK_a,      nexttagandview, {0} },

    { MODKEY,                       XK_h,      viewtoleft,     {0} },
    { MODKEY,                       XK_l,      viewtoright,    {0} },
    { MODKEY|ShiftMask,             XK_h,      tagtoleft,      {0} },
    { MODKEY|ShiftMask,             XK_l,      tagtoright,     {0} },
    { MODKEY|ControlMask,           XK_h,      tagandviewtoleft,  {0} },
    { MODKEY|ControlMask,           XK_l,      tagandviewtoright, {0} },

    // Quit
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} }, // Reload
	{ MODKEY|ControlMask,           XK_e,      spawn,          SHCMD("shutdownmenu") },
};

static Key cmdkeys[] = {
	/* modifier                    keys                     function         argument */
	{ 0,                           XK_Escape,               clearcmd,        {0} },
	{ ControlMask,                 XK_c,                    clearcmd,        {0} },
	{ 0,                           XK_i,                    setkeymode,      {.ui = INSERTMODE} },
	{ MODKEY,                      XK_Escape,               setkeymode,      {.ui = INSERTMODE} },
};


#define TAGKEYSCMD(KEY,TAG) \
	{ {0, 0, 0, 0},                       {KEY,  0, 0, 0},      comboview,      {.ui = 1 << TAG} }, \
	{ {ShiftMask, 0, 0, 0},               {KEY,  0, 0, 0},      combotag,       {.ui = 1 << TAG} }, \
	{ {0, 0, 0, 0},                       {XK_w, KEY, 0, 0},    combotag,       {.ui = 1 << TAG} }, \
    { {0, 0, 0, 0},                       {XK_w, XK_w, KEY, 0}, tagall,         {.ui = 1 << TAG} }, \
    { {ControlMask, 0, 0, 0},             {KEY, 0, 0, 0},       tagandview,     {.ui = 1 << TAG} }, \
    { {0, 0, 0, 0},                       {XK_m, KEY, 0, 0},    tagandview,     {.ui = 1 << TAG} }, \
    { {0, 0, 0, 0},                       {XK_m, XK_m, KEY, 0}, tagandviewall,  {.ui = 1 << TAG} }, \
    { {0, 0, 0, 0},                       {XK_s, KEY, 0, 0},    swaptags,       {.ui = 1 << TAG} }, \
    { {0, 0, 0, 0},                       {XK_s, XK_s, KEY, 0}, swapandviewtags,{.ui = 1 << TAG} }

static Command commands[] = {
	/* modifier (4 keys)                          keysyms (4 keys)                                function         argument */
    
    // Stack control
	{ {0, 0, 0, 0},                       {XK_j, 0, 0, 0},     focusstack,     {.i = +1 } },
	{ {0, 0, 0, 0},                       {XK_k, 0, 0, 0},     focusstack,     {.i = -1 } },
	{ {ShiftMask, 0, 0, 0},               {XK_j, 0, 0, 0},     movestack,      {.i = +1 } },
	{ {ShiftMask, 0, 0, 0},               {XK_k, 0, 0, 0},     movestack,      {.i = -1 } },

	{ {0, 0, 0, 0},                       {XK_m, XK_j, 0, 0},  movestack,      {.i = +1 } },
	{ {0, 0, 0, 0},                       {XK_m, XK_k, 0, 0},  movestack,      {.i = -1 } },

	{ {0, 0, 0, 0},                       {XK_Return, 0, 0, 0}, zoom,           {0} },
	{ {0, 0, 0, 0},                       {XK_Tab, 0, 0, 0},    view,           {0} },

    // Bar
	{ {ShiftMask, 0, 0, 0},               {XK_b, 0, 0, 0},     spawn,           SHCMD("togglebardwm") },

    // Layout 
	{ {ShiftMask, 0, 0, 0},               {XK_i, 0, 0, 0},     incnmaster,     {.i = +1 } },
	{ {ShiftMask, 0, 0, 0},               {XK_u, 0, 0, 0},     incnmaster,     {.i = -1 } },
	{ {0, 0, 0, 0},                       {XK_h, 0, 0, 0},     setmfact,       {.f = -0.05} },
	{ {0, 0, 0, 0},                       {XK_l, 0, 0, 0},     setmfact,       {.f = +0.05} },

    // cfact
    { {ShiftMask, 0, 0, 0},               {XK_h, 0, 0, 0},     setcfact,       {.f = -0.25} },
	{ {ShiftMask, 0, 0, 0},               {XK_l, 0, 0, 0},     setcfact,       {.f = +0.25} },
	{ {ShiftMask, 0, 0, 0},               {XK_o, 0, 0, 0},     setcfact,       {.f =  0.00} },

    // Layouts
	{ {0, 0, 0, 0},                       {XK_F1, 0, 0, 0},     setlayout,      {.v = &layouts[0]} }, // Tile
	{ {0, 0, 0, 0},                       {XK_F2, 0, 0, 0},     setlayout,      {.v = &layouts[1]} }, // Monicle
	{ {0, 0, 0, 0},                       {XK_F3, 0, 0, 0},     setlayout,      {.v = &layouts[2]} }, // Dwindle
	{ {0, 0, 0, 0},                       {XK_F4, 0, 0, 0},     setlayout,      {.v = &layouts[3]} }, // Grid
	{ {0, 0, 0, 0},                       {XK_F5, 0, 0, 0},     setlayout,      {.v = &layouts[4]} }, // Centered master
	{ {0, 0, 0, 0},                       {XK_F6, 0, 0, 0},     setlayout,      {.v = &layouts[5]} }, // Spiral
	{ {0, 0, 0, 0},                       {XK_F7, 0, 0, 0},     setlayout,      {.v = &layouts[6]} }, // Bottomstack
    // ...
	{ {0, 0, 0, 0},                       {XK_F10, 0, 0, 0},    setlayout,      {.v = &layouts[7]} }, // Floating

    // Tags
    TAGKEYSCMD(                           XK_1,                0),
    TAGKEYSCMD(                           XK_2,                1),
    TAGKEYSCMD(                           XK_3,                2),
    TAGKEYSCMD(                           XK_4,                3),
    TAGKEYSCMD(                           XK_5,                4),
    TAGKEYSCMD(                           XK_6,                5),
    TAGKEYSCMD(                           XK_7,                6),
    TAGKEYSCMD(                           XK_8,                7),
    TAGKEYSCMD(                           XK_9,                8),

    { {0, 0, 0, 0},                       {XK_a, 0, 0, 0},     nextview,        {0} },
    { {ShiftMask, 0, 0, 0},               {XK_a, 0, 0, 0},     nexttag,         {0} },
    { {0, 0, 0, 0},                       {XK_m, XK_a, 0, 0},  nexttagandview,  {0} },
    { {ControlMask, 0, 0, 0},             {XK_a, 0, 0, 0},     nexttagandview,  {0} },

    { {0, 0, 0, 0},                       {XK_BackSpace, 0, 0, 0}, compacttags, {.ui = 7 } }, // Compacting function, but ignore 8 and 9

	//{ {0, 0, 0, 0},                       XK_0,                view,           {.ui = ~0 } },

    // Clients
    { {0, 0, 0, 0},                       {XK_d, XK_d, 0, 0},  killclient,     {0} },
	{ {ShiftMask, 0, 0, 0},               {XK_d, 0, 0, 0},     killclient,     {0} },

    { {ShiftMask, 0, 0, 0},               {XK_s, 0, 0, 0},     togglesticky,   {0} },

    // Vanity gaps
	{ {0, 0, 0, 0},                       {XK_minus, 0, 0, 0}, togglegaps,     {0} },
	{ {0, 0, 0, 0},                       {XK_comma,0, 0, 0},  incrgaps,       {.i = -1 } },
	{ {0, 0, 0, 0},                       {XK_period,0, 0, 0}, incrgaps,       {.i = +1 } },
    { {ShiftMask, 0, 0, 0},               {XK_minus, 0, 0, 0}, defaultgaps,    {0} },

    // Floating control
	{ {0, 0, 0, 0},                       {XK_space, 0, 0, 0}, togglefloating, {0} },

    { {ControlMask, 0, 0, 0},             {XK_j, 0, 0, 0},     moveresize,     {.v = "0x 25y 0w 0h" } },
	{ {ControlMask, 0, 0, 0},             {XK_k, 0, 0, 0},     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ {ControlMask, 0, 0, 0},             {XK_l, 0, 0, 0},     moveresize,     {.v = "25x 0y 0w 0h" } },
	{ {ControlMask, 0, 0, 0},             {XK_h, 0, 0, 0},     moveresize,     {.v = "-25x 0y 0w 0h" } },

	{ {ShiftMask|ControlMask, 0, 0, 0},   {XK_j, 0, 0, 0},     moveresize,     {.v = "0x -15y 0w 30h" } },
	{ {ShiftMask|ControlMask, 0, 0, 0},   {XK_k, 0, 0, 0},     moveresize,     {.v = "0x 15y 0w -30h" } },
	{ {ShiftMask|ControlMask, 0, 0, 0},   {XK_l, 0, 0, 0},     moveresize,     {.v = "-15x 0y 30w 0h" } },
	{ {ShiftMask|ControlMask, 0, 0, 0},   {XK_h, 0, 0, 0},     moveresize,     {.v = "15x 0y -30w 0h" } },

	{ {0, 0, 0, 0},                       {XK_r, XK_j, 0, 0},  moveresizedouble,{.v = "h" } },
	{ {0, 0, 0, 0},                       {XK_r, XK_k, 0, 0},  moveresizedouble,{.v = "H" } },
	{ {0, 0, 0, 0},                       {XK_r, XK_h, 0, 0},  moveresizedouble,{.v = "w" } },
	{ {0, 0, 0, 0},                       {XK_r, XK_l, 0, 0},  moveresizedouble,{.v = "W" } },

	{ {0, 0, 0, 0},                       {XK_z, XK_j, 0, 0},  moveresizeedge,     {.v = "b" } },
	{ {0, 0, 0, 0},                       {XK_z, XK_k, 0, 0},  moveresizeedge,     {.v = "t" } },
	{ {0, 0, 0, 0},                       {XK_z, XK_l, 0, 0},  moveresizeedge,     {.v = "r" } },
	{ {0, 0, 0, 0},                       {XK_z, XK_h, 0, 0},  moveresizeedge,     {.v = "l" } },

 	{ {0, 0, 0, 0},                       {XK_y, 0, 0, 0},     movecorner,     {.v = "y"} },
 	{ {0, 0, 0, 0},                       {XK_u, 0, 0, 0},     movecorner,     {.v = "u"} },
 	{ {0, 0, 0, 0},                       {XK_b, 0, 0, 0},     movecorner,     {.v = "b"} },
 	{ {0, 0, 0, 0},                       {XK_n, 0, 0, 0},     movecorner,     {.v = "n"} },

 	{ {0, 0, 0, 0},                       {XK_z, XK_z, 0, 0},  movecenter,     {0} },

    { {ControlMask, 0, 0, 0},             {XK_u, 0, 0, 0},     aspectresize,   {.i = +25} },
    { {ControlMask|ShiftMask, 0, 0, 0},   {XK_u, 0, 0, 0},     aspectresize,   {.i = +50} },
    { {ControlMask, 0, 0, 0},             {XK_d, 0, 0, 0},     aspectresize,   {.i = -25} },
    { {ControlMask|ShiftMask, 0, 0, 0},   {XK_d, 0, 0, 0},     aspectresize,   {.i = -50} },

    // Unfloat
 	{ {ShiftMask, 0, 0, 0},               {XK_space, 0, 0, 0}, unfloatvisible, {0} },

    // Fullscreen
	{ {0, 0, 0, 0},                       {XK_f, 0, 0, 0},     togglefullscr,   {0} },

    // Other
	{ {0, 0, 0, 0},                       {XK_p, XK_p, 0, 0}, spawn,           {.v = termcmd} },
	{ {ControlMask, ShiftMask, 0, 0},     {XK_w, XK_o, 0, 0}, onlyclient,      {0} },

    // System info
	{ {0, 0, 0, 0},                       {XK_o, 0, 0, 0},    spawn,           SHCMD("herbe-info") },

    // Style of vim commands
	{ {ShiftMask, 0, 0, 0},               {XK_period, XK_e, 0, 0}, spawn,      {.v = dmenucmd} },
	{ {ShiftMask, 0, 0, 0},               {XK_period, XK_o, 0, 0}, spawn,      {.v = dmenucmd} },
	{ {ShiftMask, 0, 0, 0},               {XK_period, XK_q, XK_Return, 0},     quit,            {0} },
	{ {ShiftMask, 0, ShiftMask, 0},       {XK_period, XK_q, XK_1, XK_Return},  spawn,           SHCMD("shutdownmenu") },


    // Volume control
	{ {0, 0, 0, 0},                       {XF86XK_AudioRaiseVolume, 0, 0, 0},   spawn,   SHCMD("vol up") },
    { {0, 0, 0, 0},                       {XF86XK_AudioLowerVolume, 0, 0, 0},   spawn,   SHCMD("vol down") },
	{ {0, 0, 0, 0},                       {XF86XK_AudioMute, 0, 0, 0},          spawn,   SHCMD("vol mute") },

    // Spotify control
	{ {0, 0, 0, 0},                       {XF86XK_AudioPlay, 0, 0, 0},          spawn,   SHCMD("play pause") },
    { {0, 0, 0, 0},                       {XF86XK_AudioStop, 0, 0, 0},          spawn,   SHCMD("play stop") },
	{ {0, 0, 0, 0},                       {XF86XK_AudioPrev, 0, 0, 0},          spawn,   SHCMD("play prev") },
    { {0, 0, 0, 0},                       {XF86XK_AudioNext, 0, 0, 0},          spawn,   SHCMD("play next") },

    // Brightness control
	{ {0, 0, 0, 0},                       {XF86XK_MonBrightnessUp, 0, 0, 0},    spawn,   SHCMD("bri up") },
	{ {0, 0, 0, 0},                       {XF86XK_MonBrightnessDown, 0, 0, 0},  spawn,   SHCMD("bri down") },

    // Testing
	{ {0, 0, 0, 0}, {XK_t, 0, 0, 0},      CONDITIONAL(isfloating, 1, togglefullscr, {0}, togglefloating, {0})},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  spawn,               1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

/* include(s) depending on the configuration variables */
#include "keymodes.post.h"

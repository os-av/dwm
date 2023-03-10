/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 12, .gappx = 12};
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 8;        /* added bar height */
static const char *fonts[]          = { "IBMPlexMono:style=Regular:size=16", "Font Awesome 6 Free:style=Solid:size=13" };
static const char dmenufont[]       = "terminus:size=12";
static const char col_bar[]         = "#000000"; /* bar */
static const char col_tabsel[]      = "#000000"; /* selected tab */
static const char col_winsel[]      = "#7a7a7a"; /* selected window border */
static const char col_text[]        = "#e5e5e5"; /* text */
static const char col_inactive[]    = "#494949";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]      = { col_text, col_bar,     col_inactive },
	[SchemeSel]       = { col_text, col_bar,     col_winsel   },
	[SchemeStatus]    = { col_text, col_bar,     "#000000"    },
	[SchemeTagsSel]   = { col_text, col_tabsel,  "#000000"    },
	[SchemeTagsNorm]  = { col_text, col_bar,     "#000000"    },
	[SchemeInfoSel]   = { col_text, col_tabsel,  "#000000"    },
	[SchemeInfoNorm]  = { col_text, col_bar,     "#000000"    },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.62; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *fmcmd[] = { "pcmanfm", NULL };
static const char *browsercmd[] = { "firefox", NULL };
static const char *miccmd[] = { "amixer", "-q", "set", "Capture", "toggle", NULL };
static const char *screenshotcmd[] = { "screenshot", NULL };
static const char *mstartcmd[] = { "cmus-pause", NULL };
static const char *mstopcmd[] = { "cmus-pause", NULL };
static const char *mnextcmd[] = { "cmus-remote", "--next", NULL };
static const char *mprevcmd[] = { "cmus-remote", "--prev", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,           XK_p,      	     spawn, {.v = dmenucmd } },
	{ MODKEY,			XK_Return, 	     spawn, {.v = termcmd } },
	{ MODKEY,			XK_e,      	     spawn, {.v = fmcmd } },
	{ MODKEY,			XK_r,      	     spawn, {.v = browsercmd } },
    { 0,                XK_Print,  	     spawn, {.v = screenshotcmd } },
    { 0,  XF86XK_AudioMicMute, spawn, {.v = miccmd } },
	{ 0,  XF86XK_MonBrightnessUp,  spawn, SHCMD("brightnessctl set +10%; brightness-notify; kill -44 $(pidof dwmblocks)") },  
	{ 0,  XF86XK_MonBrightnessDown,spawn, SHCMD("brightnessctl set 10%-; brightness-notify; kill -44 $(pidof dwmblocks)") },  
    { 0,  XF86XK_AudioLowerVolume, spawn, SHCMD("amixer sset Master 5%-; volume-notify; kill -44 $(pidof dwmblocks)") },
    { 0,  XF86XK_AudioRaiseVolume, spawn, SHCMD("amixer sset Master 5%+; volume-notify; kill -44 $(pidof dwmblocks)") },
    { 0,  XF86XK_AudioMute,        spawn, SHCMD("amixer set Master toggle; volume-notify; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPlay, spawn, {.v = mstartcmd} },
	{ 0, XF86XK_AudioStop, spawn, {.v = mstartcmd} },
	{ 0, XF86XK_AudioNext, spawn, {.v = mnextcmd} },
	{ 0, XF86XK_AudioPrev, spawn, {.v = mprevcmd} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	// { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	// { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.005} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.005} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f = 0.00} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -4 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +4 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

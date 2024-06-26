/* See LICENSE file for copyright and license details. */
#include "gaplessgrid.c"
#include "tatami.c"
#include "movestack.c"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "ComicShannsMono Nerd Font:size=10:style=Bold" };
static const char col_black[]       = "#2E3440";
static const char col_yellow[]      = "#EBCB8B";
static const char col_white[]       = "#E5E9F0";
static const char col_white_x[]     = "#ECEFF4";
static const char col_green[]       = "#A3BE8c";
static const char col_blue[]        = "#81A1C1";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*                    fg          bg         border   */
	[SchemeNorm]     = { col_white, col_black, col_black },
	[SchemeSel]      = { col_green, col_black, col_green },
	[SchemeLayout]   = { col_blue,  col_black, col_black },
	[SchemeXsetroot] = { col_white_x, col_black, col_black },
};
static const unsigned int alphas[][3]      = {
    /*                      fg      bg        border*/
    [SchemeNorm]      = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]       = { OPAQUE, baralpha, borderalpha },
	[SchemeLayout]    = { OPAQUE, baralpha, borderalpha },
	[SchemeXsetroot]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "󰎤", "󰎧", "󰎪", "󰎭", "󰎱", "󰎳" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ " 󰡃",        tatami },    /* first entry is default */
	{ " 󰕳",		   gaplessgrid },    /* no layout function means floating behavior */
	{ " 󰝤",      monocle },
	
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
static const char *termcmd[]  = { "st", NULL };
static const char *alacritty[]  = { "alacritty", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x20", NULL };
static const char *rofi[]  = { "rofi", "-show", "drun", "-theme", "rounded-nord-dark", NULL };
static const char *bower[]  = { "microsoft-edge-stable", NULL };
static const char *volup[]  = { "amixer", "sset", "Master", "5%+", "umute", NULL };
static const char *voldown[]  = { "amixer", "sset", "Master", "5%-", "umute", NULL };
static const char *voltoggle[]  = { "amixer", "sset", "Master", "toggle", NULL };
static const char *brightup[]  = { "brightnessctl", "set", "5%+", NULL };
static const char *brightdown[]  = { "brightnessctl", "set", "5%-", NULL };
static const char *yazi[]  = { "st", "-e", "yazi", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_a,      spawn,          {.v = alacritty } },
	{ MODKEY,                       XK_t,      togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_r,      spawn,          {.v = rofi } },
	{ MODKEY,                       XK_w,      spawn,          {.v = bower } },
	{ ControlMask,                  XK_o,      spawn,          {.v = volup } },
	{ ControlMask,                  XK_p,      spawn,          {.v = voldown } },
	{ ControlMask,                  XK_i,      spawn,          {.v = voltoggle } },
	{ ControlMask,                  XK_z,      spawn,          {.v = brightup } },
	{ ControlMask,                  XK_x,      spawn,          {.v = brightdown } },
	{ MODKEY,                       XK_y,      spawn,          {.v = yazi } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_b,      focusmaster,    {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ ControlMask,                  XK_h,      setmfact,       {.f = -0.05} },
	{ ControlMask,                  XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_f,      fullscreen,     {0} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_g,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
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
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


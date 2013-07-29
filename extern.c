/*
 * GoomwWM, Get out of my way, Window Manager!
 * MIT/X11 License
 * Copyright (c) 2012 Sean Pringle <sean.pringle@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "goomwwm.h"

const char *gatom_names[] = {
	GOOMWWM_ATOMS(ATOM_CHAR)
};

const char *netatom_names[] = {
	EWMH_ATOMS(ATOM_CHAR)
};

const char *atom_names[] = {
	GENERAL_ATOMS(ATOM_CHAR)
};

char *empty = "";

winrulemap rulemap[] = {
	{ "tag1", TAG1 },
	{ "tag2", TAG2 },
	{ "tag3", TAG3 },
	{ "tag4", TAG4 },
	{ "tag5", TAG5 },
	{ "tag6", TAG6 },
	{ "tag7", TAG7 },
	{ "tag8", TAG8 },
	{ "tag9", TAG9 },
	{ "ignore", RULE_IGNORE },
	{ "above", RULE_ABOVE },
	{ "sticky", RULE_STICKY },
	{ "below", RULE_BELOW },
	{ "fullscreen", RULE_FULLSCREEN },
	{ "maximize_horz", RULE_MAXHORZ },
	{ "maximize_vert", RULE_MAXVERT },
	{ "top",    RULE_TOP },
	{ "bottom", RULE_BOTTOM },
	{ "left",   RULE_LEFT },
	{ "right",  RULE_RIGHT },
	{ "center", RULE_CENTER },
	{ "pointer", RULE_POINTER },
	{ "small",  RULE_SMALL },
	{ "medium", RULE_MEDIUM },
	{ "large",  RULE_LARGE },
	{ "cover", RULE_COVER },
	{ "replace", RULE_REPLACE },
	{ "steal", RULE_STEAL },
	{ "block", RULE_BLOCK },
	{ "hlock", RULE_HLOCK },
	{ "vlock", RULE_VLOCK },
	{ "expand", RULE_EXPAND },
	{ "contract", RULE_CONTRACT },
	{ "skip_taskbar", RULE_SKIPTBAR },
	{ "skip_pager", RULE_SKIPPAGE },
	{ "raise", RULE_RAISE },
	{ "lower", RULE_LOWER },
	{ "snap_left", RULE_SNAPLEFT },
	{ "snap_right", RULE_SNAPRIGHT },
	{ "snap_up", RULE_SNAPUP },
	{ "snap_down", RULE_SNAPDOWN },
	{ "duplicate", RULE_DUPLICATE },
	{ "minimize", RULE_MINIMIZE },
	{ "restore", RULE_RESTORE },
	{ "monitor1", RULE_MONITOR1 },
	{ "monitor2", RULE_MONITOR2 },
	{ "monitor3", RULE_MONITOR3 },
	{ "once", RULE_ONCE },
	{ "htile", RULE_HTILE },
	{ "vtile", RULE_VTILE },
	{ "huntile", RULE_HUNTILE },
	{ "vuntile", RULE_VUNTILE },
	{ "reset", RULE_RESET },
	{ "minimize_auto", RULE_AUTOMINI },
	{ "lower_auto", RULE_AUTOLOWER },
};

bool quit_pressed_once = 0;
bool prefix_mode_active = 0;
struct mouse_drag *mouse_dragger = NULL;
Time latest = CurrentTime;
unsigned int NumlockMask = 0;

KeySym keymap[] = { KEYLIST(KEY_KSYM), 0 };
unsigned int keymodmap[] = { KEYLIST(KEY_KMOD), 0 };
char *keyargs[] = { KEYLIST(KEY_CARG), NULL };

winrule *config_rules = NULL;

KeySym config_apps_keysyms[] = {
	XK_1,
	XK_2,
	XK_3,
	XK_4,
	XK_5,
	XK_6,
	XK_7,
	XK_8,
	XK_9,
	XK_0,
	0
};

KeySym config_tags_keysyms[] = {
	XK_F1,
	XK_F2,
	XK_F3,
	XK_F4,
	XK_F5,
	XK_F6,
	XK_F7,
	XK_F8,
	XK_F9,
	0,
	0
};

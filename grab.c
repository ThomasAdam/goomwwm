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

// bind to a keycode in all lock states

#include "goomwwm.h"

void
grab_keycode(unsigned int mask, KeyCode keycode)
{
	XGrabKey(display, keycode, mask, root, True, GrabModeAsync,
	    GrabModeAsync);
	XGrabKey(display, keycode, mask | LockMask, root, True, GrabModeAsync,
	    GrabModeAsync);
	if (NumlockMask) {
		XGrabKey(display, keycode, mask | NumlockMask, root, True,
		    GrabModeAsync, GrabModeAsync);
		XGrabKey(display, keycode, mask | NumlockMask | LockMask, root,
		    True, GrabModeAsync, GrabModeAsync);
	}
}

// grab a MODKEY+key combo
void
grab_key(unsigned int mask, KeySym key)
{
	int	 i, j, min_code, max_code, syms_per_code;

	grab_keycode(mask, XKeysymToKeycode(display, key));
	// if xmodmap is in use to remap keycodes to keysyms, a simple XKeysymToKeycode
	// may not suffice here. so we also walk the entire map of keycodes and bind to
	// each code mapped to "key"
	XDisplayKeycodes(display, &min_code, &max_code);
	KeySym *map =
	    XGetKeyboardMapping(display, min_code, max_code - min_code,
	    &syms_per_code);
	for (i = 0; map && i < (max_code - min_code); i++)
		for (j = 0; j < syms_per_code; j++)
			if (key == map[i * syms_per_code + j])
				grab_keycode(mask, i + min_code);
	if (map)
		XFree(map);
}

// run at startup and on MappingNotify
void
grab_keys_and_buttons()
{
	int	 i;

	XUngrabKey(display, AnyKey, AnyModifier, root);

	// only grab keys if prefix mode is disabled (default)
	if (!config_prefix_mode) {
		// configurable keys
		for (i = 0; keymap[i]; i++)
			if (keymap[i] != XK_VoidSymbol)
				grab_key(keymodmap[i], keymap[i]);

		// 1-9 app keys
		for (i = 0; config_apps_keysyms[i]; i++)
			if (config_apps_patterns[i]) {
				grab_key(config_modkey,
				    config_apps_keysyms[i]);
				grab_key(config_modkey | ShiftMask,
				    config_apps_keysyms[i]);
			}
		// F1-F9 tag keys
		for (i = 0; config_tags_keysyms[i]; i++) {
			grab_key(config_modkey, config_tags_keysyms[i]);
			grab_key(config_modkey | ShiftMask,
			    config_tags_keysyms[i]);
		}
	}
	// prefix mode key switches to XGrabKeyboard
	else
		grab_key(config_modkey, keymap[KEY_PREFIX]);
	// grab mouse buttons for click-to-focus. these get passed through to the windows
	// not binding on button4 which is usually wheel scroll
	XUngrabButton(display, AnyButton, AnyModifier, root);
	XGrabButton(display, Button1, AnyModifier, root, True, ButtonPressMask,
	    GrabModeSync, GrabModeSync, None, None);
	XGrabButton(display, Button2, AnyModifier, root, True, ButtonPressMask,
	    GrabModeSync, GrabModeSync, None, None);
	XGrabButton(display, Button3, AnyModifier, root, True, ButtonPressMask,
	    GrabModeSync, GrabModeSync, None, None);
}

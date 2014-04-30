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

// find the dimensions of the monitor displaying point x,y

#include "goomwwm.h"

void
monitor_dimensions(int x, int y, workarea * mon)
{
	int	 monitors, i;
	memset(mon, 0, sizeof(workarea));

	mon->w = WidthOfScreen(screen);
	mon->h = HeightOfScreen(screen);

	// locate the current monitor
	if (XineramaIsActive(display)) {
		XineramaScreenInfo *info =
		    XineramaQueryScreens(display, &monitors);
		if (info) {
			for (i = 0; i < monitors; i++) {
				if (INTERSECT(x, y, 1, 1, info[i].x_org,
					info[i].y_org, info[i].width,
					info[i].height)) {
					mon->x = info[i].x_org;
					mon->y = info[i].y_org;
					mon->w = info[i].width;
					mon->h = info[i].height;
					mon->screen = info[i].screen_number;
					break;
				}
			}
			XFree(info);
		}
	}
}

// find the dimensions, EXCLUDING STRUTS, of the monitor displaying point x,y
void
monitor_dimensions_struts(int x, int y, workarea *mon)
{
	int			 i;
	int			 left, right, top, bottom;
	workarea		*cm;
	XWindowAttributes	*rattr;
	Window			 win;
	unsigned long		 strut[12];

	// check cache. we may be able to avoid walking all windows
	for (i = 0; i < sizeof(cache_monitor) / sizeof(workarea); i++) {
		cm = &cache_monitor[i];
		if (!cm->w)
			continue;
		if (INTERSECT(x, y, 1, 1, cm->x - cm->l, cm->y - cm->t,
			cm->w + cm->l + cm->r, cm->h + cm->t + cm->b)) {
			memmove(mon, cm, sizeof(workarea));
			return;
		}
	}

	monitor_dimensions(x, y, mon);

	// strut cardinals are relative to the root window size, which is not necessarily the monitor size
	rattr = window_get_attributes(root);
	left = 0, right = 0, top = 0, bottom = 0;

	// walk the open apps and check for struts
	// this is fairly lightweight thanks to some caches
	winlist_ascend(windows_in_play(), i, win) {
		XWindowAttributes *attr = window_get_attributes(win);
		if (attr && !attr->override_redirect
		    && INTERSECT(attr->x, attr->y, attr->width, attr->height,
			mon->x, mon->y, mon->w, mon->h)) {
			memset(strut, 0, sizeof(strut));
			if (window_get_cardinal_prop(win,
				netatoms[_NET_WM_STRUT_PARTIAL], strut, 12)
			    || window_get_cardinal_prop(win,
				netatoms[_NET_WM_STRUT], strut, 4)) {
				// we only pay attention to the first four params
				// this is no more complex that _NET_WM_STRUT, but newer stuff uses _PARTIAL
				left = MAX(left, strut[0]);
				right = MAX(right, strut[1]);
				top = MAX(top, strut[2]);
				bottom = MAX(bottom, strut[3]);
			}
		}
	}
	// limit struts on any side to 1/3 monitor size
	mon->l = MIN(mon->w / 3, MAX(0, left - mon->x));
	mon->r =
	    MIN(mon->w / 3, MAX(0,
		(mon->x + mon->w) - (rattr->width - right)));
	mon->t = MIN(mon->h / 3, MAX(0, top - mon->y));
	mon->b =
	    MIN(mon->h / 3, MAX(0,
		(mon->y + mon->h) - (rattr->height - bottom)));
	mon->x += mon->l;
	mon->y += mon->t;
	mon->w -= (mon->l + mon->r);
	mon->h -= (mon->t + mon->b);

	// update cache. strust change rarely, so this is long-lived.
	// see handle_propertynotify() also
	for (i = 0; i < sizeof(cache_monitor) / sizeof(workarea); i++) {
		cm = &cache_monitor[i];
		if (!cm->w) {
			memmove(cm, mon, sizeof(workarea));
			break;
		}
	}
}

// determine which monitor holds the active window, or failing that the mouse pointer
void
monitor_active(workarea *mon)
{
	client	*c = client_active(0);
	if (c) {
		client_extended_data(c);
		memmove(mon, &c->monitor, sizeof(workarea));
		return;
	}

	monitor_of_pointer(mon);
	if (mon != NULL)
		return;

	monitor_dimensions_struts(0, 0, mon);
}

void
monitor_of_pointer(workarea *mon)
{
	int		 x, y;


	if (pointer_get(&x, &y))
		monitor_dimensions_struts(x, y, mon);
}

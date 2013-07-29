
/* GoomwWM, Get out of my way, Window Manager!

MIT/X11 License
Copyright (c) 2012 Sean Pringle <sean.pringle@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "goomwwm.h"

// for the benefit of EWMH type pagers, tag = desktop
// but, since a window can have multiple tags... oh well
unsigned int
tag_to_desktop(unsigned int tag)
{
	unsigned int i;
	for (i = 0; i < TAGS; i++)
		if (tag & (1 << i))
			return i;
	return 0xffffffff;
}

unsigned int
desktop_to_tag(unsigned int desktop)
{
	return (desktop == 0xffffffff) ? 0 : 1 << desktop;
}

// update current desktop on all roots
void
tag_set_current(unsigned int tag)
{
	workarea	 mon;

	/* Get the active monitor. */
	monitor_active(&mon);
	mon.current_tag = tag;

	fprintf(stderr, "TAG SET: mon: %d, tag: %u\n", mon.num, mon.current_tag);

	unsigned long d = tag_to_desktop(get_current_tag());
	window_set_cardinal_prop(root, netatoms[_NET_CURRENT_DESKTOP], &d, 1);
}

unsigned int
get_current_tag(void)
{
	workarea	 mon;

	/* Get the active monitor. */
	monitor_active(&mon);

	/* Get the tag for this monitor. */
	fprintf(stderr, "TAG GET: mon: <%d>, tag: <%u>\n", mon.num, mon.current_tag);
	return (mon.current_tag);
}

// raise all windows in a tag
void
tag_raise(unsigned int tag)
{
	int	 i, found = 0, shaded = 0;
	Window	 w;
	client	*c;
	winlist *stack;
	workarea mon;

	// Get the active monitor.  When we come to switch tags, we treat the
	// tag switching as a workarea, and reassign the tags back to the
	// monitor structure.
	//
	// The monitor holds the reference to which tag it's using, and a
	// monitor is derived from the client which we're going to using.
	monitor_active(&mon);

	// if this tag was previously hidden, reveal it
	clients_ascend(windows_shaded, i, w, c) {
		if (c->manage && c->cache->tags & tag
			&& client_on_monitor(c, &mon)) {
			client_reveal(c);
			shaded++;
		}
	}

	if (shaded) {
		XSync(display, False);
		reset_cache_xattr();
		reset_cache_client();
		reset_cache_inplay();
	}

	stack = winlist_new();

	// locate windows with _NET_WM_STATE_ABOVE and _NET_WM_STATE_STICKY
	managed_descend(i, w, c) {
	    if (winlist_find(stack, w) < 0 && c->visible && c->trans == None
	    && client_has_state(c, netatoms[_NET_WM_STATE_ABOVE])
	    && client_has_state(c, netatoms[_NET_WM_STATE_STICKY])
	    && client_on_monitor(c, &mon))
		client_stack_family(c, stack);
	}

	// locate windows with _NET_WM_STATE_ABOVE in this tag
	tag_descend(i, w, c, tag) {
		if (winlist_find(stack, w) < 0 && c->visible && c->trans == None
		&& client_has_state(c, netatoms[_NET_WM_STATE_ABOVE])
		&& client_on_monitor(c, &mon)) {
			client_stack_family(c, stack);
			found++;
		}
	}

	// locate _NET_WM_WINDOW_TYPE_DOCK windows
	clients_descend(windows_in_play(), i, w, c) {
	    if (winlist_find(stack, w) < 0 && c->visible && c->trans == None
	    && c->type == netatoms[_NET_WM_WINDOW_TYPE_DOCK]
	    && client_on_monitor(c, &mon))
		client_stack_family(c, stack);
	}

	// locate all other windows in the tag
	tag_descend(i, w, c, tag) {
		if (winlist_find(stack, w) < 0 && c->trans == None
		    && client_on_monitor(c, &mon)) {
			client_stack_family(c, stack);
			found++;
		}
	}

	// raise the top window in the stack
	if (stack->len)
		XRaiseWindow(display, stack->array[0]);
	// stack everything else, in order, underneath top window
	if (stack->len > 1)
		XRestackWindows(display, stack->array, stack->len);

	winlist_free(stack);
	tag_set_current(tag);
	if (config_only_auto)
		tag_only(tag);

	// focus the last-focused client in the tag
	clients_descend(windows_activated, i, w, c) {
		if (c->cache->tags & tag &&
	    	    client_on_monitor(c, &mon)) {
			client_activate(c, RAISE, WARPDEF);
			break;
		}
	}

	tag_set_current(tag);	

	// in case no windows are in the tag, show some activity
	if (found)
		notice("Tag %d", tag_to_desktop(tag) + 1);
	else
		notice("Tag %d (empty!)", tag_to_desktop(tag) + 1);
}

// check active client. if
void
tag_auto_switch()
{
	client	*c = client_active(0);
	workarea mon;

	monitor_active(&mon);

	if (c && c->cache->tags && !(c->cache->tags & get_current_tag())
		&& client_on_monitor(c, &mon)) {

		int	 i, n = 0;
		Window	 w;
		client	*o;
		tag_descend(i, w, o, get_current_tag()) {
			if (client_on_monitor(o, &mon))
				n++;
		}
		
		if (!n) {
			tag_raise(desktop_to_tag(
				tag_to_desktop(c->cache-> tags)));
		}
	}
}

void
tag_only(unsigned int tag)
{
	int	 i;
	Window	 w;
	client	*c;
	workarea mon;

	monitor_active(&mon);

	managed_descend(i, w, c) {
	    if (!(c->cache->tags & tag) &&
		client_on_monitor(c, &mon))
		client_shade(c);
	}
}

void
tag_close(unsigned int tag)
{
	int	 i;
	Window	 w;
	client	*c;
	workarea mon;

	monitor_active(&mon);

	tag_descend(i, w, c, tag) {
		if (client_on_monitor(c, &mon))
			client_close(c);
	}
}

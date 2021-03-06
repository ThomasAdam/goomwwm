/* box.c */
box *box_create(Window parent, bitmap flags, short x, short y, short w, short h, char *color);
void box_color(box *b, char *color);
void box_moveresize(box *b, short x, short y, short w, short h);
void box_show(box *b);
void box_hide(box *b);
void box_draw(box *b);
void box_free(box *b);

/* cli.c */
void cli_message(Atom atom, char *cmd);
int cli_main(int argc, char *argv[]);

/* client.c */
void client_flush_state(client *c);
int client_has_state(client *c, Atom state);
void client_add_state(client *c, Atom state);
void client_remove_state(client *c, Atom state);
void client_remove_all_states(client *c);
void client_set_state(client *c, Atom state, int on);
void client_descriptive_data(client *c);
void client_extended_data(client *c);
int client_rule_match(client *c, winrule *r);
winrule *client_rule(client *c, bitmap flags);
client *client_create(Window win);
client *client_recreate(Window w);
void client_free(client *c);
int clients_intersect(client *a, client *b);
int client_protocol_event(client *c, Atom protocol);
void client_close(client *c);
int client_warp_check(client *c, int x, int y);
void client_warp_pointer(client *c);
void client_process_size_hints(client *c, int *x, int *y, int *w, int *h);
void client_moveresize(client *c, unsigned int flags, int fx, int fy, int fw, int fh);
void client_commit(client *c);
void client_rollback(client *c);
void client_save_position(client *c);
void client_save_position_horz(client *c);
void client_save_position_vert(client *c);
void client_restore_position(client *c, unsigned int smart, int x, int y, int w, int h);
void client_restore_position_horz(client *c, unsigned int smart, int x, int w);
void client_restore_position_vert(client *c, unsigned int smart, int y, int h);
winlist *clients_fully_visible(workarea *zone, unsigned int tag, Window ignore);
winlist *clients_partly_visible(workarea *zone, unsigned int tag, Window ignore);
void client_expand(client *c, int directions, int x1, int y1, int w1, int h1, int mx, int my, int mw, int mh);
void client_contract(client *c, int directions);
void client_snapto(client *c, int direction);
void client_toggle_large(client *c, int side);
void client_flash(client *c, char *color, int delay, int title);
void client_stack_family(client *c, winlist *stack);
void client_raise(client *c, int priority);
void client_raise_under(client *c, client *under);
void client_lower(client *c, int priority);
void client_review_border(client *c);
void client_review_nws_actions(client *c);
void client_review_position(client *c);
void client_review_desktop(client *c);
void client_full_review(client *c);
void client_redecorate(client *c);
void client_deactivate(client *c, client *a);
void client_activate(client *c, int raise, int warp);
void client_set_wm_state(client *c, unsigned long state);
unsigned long client_get_wm_state(client *c);
client *client_active(unsigned int tag);
void client_toggle_vlock(client *c);
void client_toggle_hlock(client *c);
void client_nws_fullscreen(client *c, int action);
void client_nws_above(client *c, int action);
void client_nws_below(client *c, int action);
void client_nws_sticky(client *c, int action);
void client_nws_maxvert(client *c, int action);
void client_nws_maxhorz(client *c, int action);
winlist *clients_tiled_horz_with(client *c);
winlist *clients_tiled_vert_with(client *c);
winlist *clients_tiled_with(client *c);
void client_switch_to(client *c);
void client_cycle(client *c);
void client_htile(client *c);
void client_huntile(client *c);
void client_vtile(client *c);
void client_vuntile(client *c);
client *client_over_there_ish(client *c, int direction);
void client_focusto(client *c, int direction);
void client_swapto(client *c, int direction);
void client_replace(client *c);
void client_duplicate(client *c);
void client_minimize(client *c);
void client_restore(client *c);
void client_shade(client *c);
void client_reveal(client *c);
void client_switcher(unsigned int tag);
void client_toggle_tag(client *c, unsigned int tag, int flash);
client *client_find(char *pattern);
void client_start(char *pattern);
void client_find_or_start(char *pattern);
void client_rules_ewmh(client *c);
void client_rules_monitor(client *c);
void client_rules_moveresize(client *c);
void client_rules_locks(client *c);
void client_rules_tags(client *c);
void client_rules_moveresize_post(client *c);
void client_rules_apply(client *c, bool reset);

/* ewmh.c */
void ewmh_client_list(void);
void ewmh_active_window(Window w);
void ewmh_desktop_list(void);

/* goomwwm.c */
int main(int argc, char *argv[]);

/* grab.c */
void grab_keycode(unsigned int mask, KeyCode keycode);
void grab_key(unsigned int mask, KeySym key);
void grab_keys_and_buttons(void);

/* handle.c */
void handle_keypress(XEvent *ev);
void handle_buttonpress(XEvent *ev);
void handle_buttonrelease(XEvent *ev);
void handle_motionnotify(XEvent *ev);
void handle_createnotify(XEvent *ev);
void handle_destroynotify(XEvent *ev);
void handle_configurerequest(XEvent *ev);
void handle_configurenotify(XEvent *ev);
void handle_maprequest(XEvent *ev);
void handle_mapnotify(XEvent *ev);
void handle_unmapnotify(XEvent *ev);
void handle_clientmessage(XEvent *ev);
void handle_propertynotify(XEvent *ev);
void handle_enternotify(XEvent *ev);
void handle_mappingnotify(XEvent *ev);
void handle_expose(XEvent *ev);

/* menu.c */
void menu_draw(textbox *text, textbox **boxes, int max_lines, int selected, char **filtered);
int menu(char **lines, char **input, char *prompt, int selected);
char *prompt(char *ps);

/* monitor.c */
void monitor_dimensions(int x, int y, workarea *mon);
void monitor_dimensions_struts(int x, int y, workarea *mon);
void monitor_active(workarea *mon);

/* rule.c */
int rule_parse(char *rulestr);
void rule_free(winrule *rule);
void ruleset_switcher(void);
void rulelist_apply(winrule *list);
void rule_apply(winrule *rule);
void rule_execute(char *rulestr);
void ruleset_execute(char *name);

/* tag.c */
unsigned int tag_to_desktop(unsigned int tag);
unsigned int desktop_to_tag(unsigned int desktop);
void tag_set_current(unsigned int tag);
void tag_raise(unsigned int tag);
void tag_auto_switch(void);
void tag_only(unsigned int tag);
void tag_close(unsigned int tag);

/* textbox.c */
textbox *textbox_create(Window parent, bitmap flags, short x, short y, short w, short h, char *font, char *fg, char *bg, char *text, char *prompt);
void textbox_font(textbox *tb, char *font, char *fg, char *bg);
void textbox_extents(textbox *tb);
void textbox_text(textbox *tb, char *text);
void textbox_prompt(textbox *tb, char *text);
void textbox_moveresize(textbox *tb, int x, int y, int w, int h);
void textbox_show(textbox *tb);
void textbox_hide(textbox *tb);
void textbox_free(textbox *tb);
void textbox_draw(textbox *tb);
void textbox_cursor(textbox *tb, int pos);
void textbox_cursor_inc(textbox *tb);
void textbox_cursor_dec(textbox *tb);
void textbox_cursor_home(textbox *tb);
void textbox_cursor_end(textbox *tb);
void textbox_insert(textbox *tb, int pos, char *str);
void textbox_delete(textbox *tb, int pos, int dlen);
void textbox_cursor_ins(textbox *tb, char c);
void textbox_cursor_del(textbox *tb);
void textbox_cursor_bkspc(textbox *tb);
int textbox_keypress(textbox *tb, XEvent *ev);

/* util.c */
void *allocate(unsigned long bytes);
void *allocate_clear(unsigned long bytes);
void *reallocate(void *ptr, unsigned long bytes);
char *strtrim(char *str);
double timestamp(void);
void catch_exit(int sig);
int execsh(char *cmd);
pid_t exec_cmd(char *cmd);
int find_arg(int argc, char *argv[], char *key);
char *find_arg_str(int argc, char *argv[], char *key, char *def);
int find_arg_int(int argc, char *argv[], char *key, int def);
int regquick(char *pat, char *str);
int in_array_keysym(KeySym *array, KeySym code);
unsigned int color_get(const char *name);
int pointer_get(int *x, int *y);
int keycode_is_mod(unsigned int code);
int modkey_is_down(void);
int take_keyboard(Window w);
int take_pointer(Window w, unsigned long mask, Cursor cur);
void release_keyboard(void);
void release_pointer(void);
void message_box(int delay, int x, int y, char *fgc, char *bgc, char *bc, char *txt);
void notice(const char *fmt, ...);
void notification(int delay, const char *fmt, ...);

/* window.c */
void window_select(Window w);
XWindowAttributes *window_get_attributes(Window w);
int window_get_prop(Window w, Atom prop, Atom *type, int *items, void *buffer, int bytes);
char *window_get_text_prop(Window w, Atom atom);
int window_set_text_prop(Window w, Atom atom, char *txt);
int window_get_atom_prop(Window w, Atom atom, Atom *list, int count);
void window_set_atom_prop(Window w, Atom prop, Atom *atoms, int count);
int window_get_cardinal_prop(Window w, Atom atom, unsigned long *list, int count);
void window_set_cardinal_prop(Window w, Atom prop, unsigned long *values, int count);
void window_unset_prop(Window w, Atom prop);
int window_send_message(Window target, Window subject, Atom atom, unsigned long protocol, unsigned long mask);
winlist *windows_in_play(void);
winlist *window_children(void);
int window_is_active(Window w);

/* winlist.c */
winlist *winlist_new(void);
int winlist_append(winlist *l, Window w, void *d);
void winlist_prepend(winlist *l, Window w, void *d);
void winlist_empty(winlist *l);
void winlist_free(winlist *l);
int winlist_find(winlist *l, Window w);
int winlist_forget(winlist *l, Window w);
void winlist_reverse(winlist *l);

/* wm.c */
int oops(Display *d, XErrorEvent *ee);
void reset_lazy_caches(void);
void reset_cache_xattr(void);
void reset_cache_client(void);
void reset_cache_inplay(void);
void setup_screen(void);
unsigned int parse_key_mask(char *keystr, unsigned int def);
void setup_keyboard_options(int ac, char *av[]);
void setup_general_options(int ac, char *av[]);
void setup_rule_options(int ac, char *av[]);
int wm_main(int argc, char *argv[]);

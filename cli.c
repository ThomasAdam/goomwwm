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

void
cli_message(Atom atom, char *cmd)
{
	Window cli = XCreateSimpleWindow(display, root, 0, 0, 1, 1, 0,
	    None, None);

	if (cmd)
		window_set_text_prop(cli, gatoms[GOOMWWM_MESSAGE], cmd);

	window_send_message(root, cli, atom, 0,
	    SubstructureNotifyMask | SubstructureRedirectMask);
}

/* command line interface */
int
cli_main(int argc, char *argv[])
{
	char	*arg, *tmp;
	int	 delay; 

	if ((arg = find_arg_str(argc, argv, "-log", NULL)))
		cli_message(gatoms[GOOMWWM_LOG], arg);

	if (find_arg(argc, argv, "-restart") >= 0)
		cli_message(gatoms[GOOMWWM_RESTART], argv[0]);

	if ((arg = find_arg_str(argc, argv, "-exec", NULL)))
		cli_message(gatoms[GOOMWWM_RESTART], arg);

	if ((arg = find_arg_str(argc, argv, "-ruleset", NULL)))
		cli_message(gatoms[GOOMWWM_RULESET], arg);

	if ((arg = find_arg_str(argc, argv, "-rule", NULL)))
		cli_message(gatoms[GOOMWWM_RULE], arg);

	if ((arg = find_arg_str(argc, argv, "-findstart", NULL)))
		cli_message(gatoms[GOOMWWM_FIND_OR_START], arg);

	if (find_arg(argc, argv, "-quit") >= 0)
		cli_message(gatoms[GOOMWWM_QUIT], NULL);

	if ((arg = find_arg_str(argc, argv, "-notice", NULL))) {
		/* optional duration in seconds. 0 means SAYMS */

		delay = find_arg_int(argc, argv, "-duration", 0);
		
		/* bit of a hack for v1... write delay inline */
		tmp = alloca(strlen(arg) + 10);
		sprintf(tmp, "%d %s", delay, arg);
		cli_message(gatoms[GOOMWWM_NOTICE], tmp);
	}
	/* TODO: make this a two-way event exchange */
	usleep(500000);
	return (EXIT_SUCCESS);
}

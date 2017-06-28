/*-
 * Copyright (c) 2014-2017 MongoDB, Inc.
 * Copyright (c) 2008-2014 WiredTiger, Inc.
 *	All rights reserved.
 *
 * See the file LICENSE for redistribution information.
 */

#include "util.h"

static int usage(void);

int
util_downgrade(WT_SESSION *session, WT_CONNECTION *conn, int argc, char *argv[])
{
	WT_DECL_RET;
	int ch;
	char config_str[128], *release;

	release = NULL;
	while ((ch = __wt_getopt(progname, argc, argv, "V:")) != EOF)
		switch (ch) {
		case 'V':
			release = __wt_optarg;
			break;
		case '?':
		default:
			return (usage());
		}
	argc -= __wt_optind;
	argv += __wt_optind;

	/* The remaining argument is the table name. */
	if (argc != 0 || release == NULL)
		return (usage());
	if ((ret = __wt_snprintf(config_str, sizeof(config_str),
	    "compatibility=(release=%s)", release)) != 0)
		return (util_err(session, ret, NULL));
	if ((ret = conn->reconfigure(conn, config_str)) != 0)
		return (util_err(session, ret, "conn.downgrade"));
	else {
		/*
		 * Verbose configures a progress counter, move to the next
		 * line.
		 */
		if (verbose)
			printf("\n");
	}
	return (ret);
}

static int
usage(void)
{
	(void)fprintf(stderr,
	    "usage: %s %s "
	    "downgrade -V release\n",
	    progname, usage_prefix);
	return (1);
}
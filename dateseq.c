#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERR(args...)	{fprintf(stderr, args); fprintf(stderr, "\n"); usage(); exit(1);}
#define LENGTH(x)	(sizeof x / sizeof x[0])

#define DATEFMT	"%Y-%m-%d"
#define DATELEN	11
#define DAY	24 * 3600

struct unit {
	char prefix;
	int seconds;
};

static struct unit units[] = {
	{ 'd',   1 * DAY },
	{ 'w',   7 * DAY },
	{ 'm',  30 * DAY },
	{ 'y', 365 * DAY },
};

static char *name; // Program name, i.e. argv[0]

void usage();
int prefix2seconds(char prefix);

int main(int argc, char **argv) {
	name = argv[0];
	if (argc < 2) usage();

	char *input = argv[1];

	time_t iter_date;
	time_t end_date;
	time(&iter_date);
	time(&end_date);

	struct tm tm;
	if (input[0] == '-' || input[0] == '+') {
		int delta;
		char prefix;
		if (
			2 != sscanf(input, "%d%c", &delta, &prefix) ||
			! ( delta *= prefix2seconds(prefix) )
		) {
			ERR("Could not parse time diff: %s\n", input);
		}
		if (input[0] == '-')
			iter_date = end_date + delta;
		else
			end_date += delta;
	} else if (! strptime(input, DATEFMT, &tm)) {
		ERR("Could not parse date: %s\n", input);
	} else {
		iter_date = mktime(&tm);
	}

	for (; iter_date < end_date; iter_date += DAY) {
		struct tm tm;
		char formatted[DATELEN];
		localtime_r(&iter_date, &tm);
		strftime(formatted, DATELEN, DATEFMT, &tm);
		printf("%s\n", formatted);
	}
}

void usage() {
	fprintf(stderr, "Usage:\t%s 2021-05-29\n", name);
	fprintf(stderr, "      \t%s -2d\n", name);
	fprintf(stderr, "      \t%s +2w\n", name);
	fprintf(stderr, "\nSupported relative units: d, w, m, y\n");
	exit(1);
}

int prefix2seconds(char prefix) {
	for (int i = 0; i < LENGTH(units); i++) {
		struct unit u = units[i];
		if (u.prefix == prefix) return u.seconds;
	}
	return 0;
}

/* vim: set ts=8: */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERR(args...)	{fprintf(stderr, args); fprintf(stderr, "\n"); usage(); exit(1);}

#define DATEFMT	"%Y-%m-%d"
#define DATELEN	11
#define DAY	24 * 3600

static char *name; // Program name, i.e. argv[0]

void usage();
int prefix2seconds(char prefix);
time_t parse_date(char *input);

int main(int argc, char **argv) {
	name = argv[0];

	time_t iter_date;
	time_t end_date;
	switch (argc) {
		case 2:
			iter_date = parse_date(argv[1]);
			time(&end_date);
			break;
		case 3:
			iter_date = parse_date(argv[1]);
			end_date = parse_date(argv[2]);
			break;
		default:
			usage();
			return 1;
	}

	if (iter_date > end_date) {
		time_t tmp = iter_date;
		iter_date = end_date;
		end_date = tmp;
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
	fprintf(stderr, "      \t%s 2021-06-05 2021-06-10\n", name);
	fprintf(stderr, "\nSupported relative units: d, w, m, y.\n");
	exit(1);
}

int prefix2seconds(char prefix) {
	switch (prefix) {
		case 'd': return   1 * DAY;
		case 'w': return   7 * DAY;
		case 'm': return  30 * DAY;
		case 'y': return 365 * DAY;
	}
	return 0;
}

time_t parse_date(char *input) {
	time_t date;
	time(&date);

	struct tm tm = {0};
	if (input[0] == '-' || input[0] == '+') {
		int delta;
		char prefix;
		if (
			2 != sscanf(input, "%d%c", &delta, &prefix) ||
			! ( delta *= prefix2seconds(prefix) )
		) {
			ERR("Could not parse time diff: %s\n", input);
		}
		date += delta;
	} else if (! strptime(input, DATEFMT, &tm)) {
		ERR("Could not parse date: %s\n", input);
	} else {
		date = mktime(&tm);
	}

	return date;
}

/* vim: set ts=8: */

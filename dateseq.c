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
void print_date(time_t date);

int main(int argc, char **argv) {
	name = argv[0];

	time_t iter_date;
	time_t end_date;
	switch (argc) {
		case 2: {
			time_t date = parse_date(argv[1]);
			time(&iter_date);
			time(&end_date);
			if (date < end_date) iter_date = date;
			else end_date = date;
			break;
			}
		case 3:
			iter_date = parse_date(argv[1]);
			end_date = parse_date(argv[2]);
			break;
		default:
			usage();
			return 1;
	}

	if (iter_date < end_date) {
		for (; iter_date < end_date; iter_date += DAY)
			print_date(iter_date);
	} else {
		for (; end_date < iter_date; iter_date -= DAY)
			print_date(iter_date);
	}

}

void usage() {
	fprintf(stderr, "Usage:\t%s 2021-05-29\n", name);
	fprintf(stderr, "      \t%s -2d\n", name);
	fprintf(stderr, "      \t%s +2w\n", name);
	fprintf(stderr, "      \t%s 2021-06-10 2021-06-05\n", name);
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

void print_date(time_t date) {
	struct tm tm;
	char formatted[DATELEN];
	localtime_r(&date, &tm);
	strftime(formatted, DATELEN, DATEFMT, &tm);
	puts(formatted);
}

/* vim: set ts=8: */

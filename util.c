#include "util.h"

#include <stdio.h>
#include <time.h>

/**
 * Converts a size in bytes to human-readable format string (KiB, MiB, etc.).
 *
 * @param buf Buffer to print the human-readable format to as a string
 * @param buf_sz Size of the buffer to write to
 * @param size Size to be converted
 * @param decimals Number of decimals to display in the formatted string
 */
void human_readable_size(char *buf, size_t buf_sz, double size, unsigned int decimals)
{
	char *suffix[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "ZiB"};
	char length = sizeof(suffix) / sizeof(suffix[0]);

	int i = 0;
	
	if (size > 1024) {
		for (i = 0; (size / 1024) > 0 && i<length-1; i++, size /= 1024)
			decimals = size / 1024;
	}
	sprintf(buf, "%i %s", decimals, suffix[i]);
	return;
}

/**
 * Converts a time_t timespec to a simplified format, (Day Mon Year), or
 * "DD MMM YYYY".
 *
 * @param buf Buffer to print the formatted time string to
 * @param buf_sz Size of the buffer to write to
 * @param time Time to convert to a string
 *
 * @return Number of bytes copied to the buffer (excluding terminating NUL). If
 * buf_sz is not large enough, returns 0.
 */
size_t simple_time_format(char *buf, size_t buf_sz, time_t time)
{
	struct tm *now = localtime(&time);
	strftime(buf, buf_sz, "%B %d %Y", now);
	size_t bytes = sizeof(buf);
	if (buf_sz < bytes) {
		return 0;
	}
	return bytes;
}

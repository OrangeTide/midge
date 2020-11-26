#include "loadfile.h"
#include <stdio.h>
#include <stdlib.h>

void *
loadfile(const char *filename, size_t *out_len)
{
	if (!filename)
		return NULL;
	FILE *f = fopen(filename, "rb");
	if (!f)
		return NULL;
	if (fseek(f, 0, SEEK_END)) {
		fclose(f);
		return NULL;
	}
	long len = ftell(f);
	if (fseek(f, 0, SEEK_SET)) {
		fclose(f);
		return NULL;
	}
	char *data = malloc(len + 1);
	size_t result = fread(data, 1, len, f);
	fclose(f);
	if (!result || result != len || ferror(f)) {
		fclose(f);
		free(data);
		return data;
	}
	data[result] = 0;
	if (out_len)
		*out_len = result;
	return data;
}

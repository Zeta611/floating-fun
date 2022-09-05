#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int hex_to_int(char c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	return -1;
}

void print_binary(int is_32, uint64_t n, int index)
{
	if ((is_32 && index == 32) || (!is_32 && index == 64)) {
		return;
	}
	print_binary(is_32, n >> 1, index + 1);
	if ((is_32 && (index == 22 || index == 30)
	     || (!is_32 && (index == 62 || index == 51)))
	) {
		putchar('|');
	}
	putchar('0' + (n & 1));
}

int main(void)
{
	int is_32 = 1;
	do {
		printf("32bit? [Y/n:64bit] ");
		int ch = getchar();
		if (ch == EOF) {
			goto failure;
		}
		switch (ch) {
		case '\n':
		case 'Y':
		case 'y':
			goto end_loop;
		case 'N':
		case 'n':
			is_32 = 0;
			goto end_loop;
		}
	} while (1);
end_loop:
	fflush(stdin);

	int precision = 5;
	printf("Precision for printing FP? ");
	if (scanf("%d", &precision) == EOF) {
		goto failure;
	}
	fflush(stdin);

	printf("\nType a hexadecimal representation of a number"
	       "\n(The following is a guide for your convenience!):"
	       "\n0x%s\n0x", is_32 ? "76543210" : "fedcba9876543210");

	uint64_t raw_bits = 0;
	for (int i = (is_32 ? 7 : 15); i >= 0; --i) {
		const int c = getchar();
		if (c == EOF) {
			goto failure;
		}
		const int d = hex_to_int((char)c);
		if (d < 0) {
			fprintf(stderr, "Malformed input character %c!\n", (char)c);
			goto failure;
		}
		raw_bits |= (uint64_t)d << (i << 2);
	}

	float* float_bits = (float*)&raw_bits;
	double* double_bits = (double*)&raw_bits;

	printf("BINARY: ");
	print_binary(is_32, raw_bits, 0);
	putchar('\n');
	printf("UINT%d: %" PRIu64 "\n"
	       " INT%d: %" PRId64 "\n",
	       is_32 ? 32 : 64, raw_bits,
	       is_32 ? 32 : 64, (int64_t)raw_bits);
	if (is_32) {
		printf(" FLOAT: %.*E\n", precision, *float_bits);
	} else {
		printf("DOUBLE: %.*lE\n", precision, *double_bits);
	}
	return 0;

failure:
	putchar('\n');
	exit(EXIT_FAILURE);
}

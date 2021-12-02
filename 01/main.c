#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_NOF_ENTRIES (2000)

static uint32_t input[MAX_NOF_ENTRIES];
static uint32_t count;

static void _parse_line(char *line)
{
    assert(count < MAX_NOF_ENTRIES);
    input[count] = atoi(line);
    count++;
}

static void _parse_input(void)
{
    char *data_read = NULL;
    size_t len;
    ssize_t read;
    FILE *fp;

    if (!(fp = fopen("input.txt", "r"))) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&data_read, &len, fp)) != -1) {
        data_read[strcspn(data_read, "\r\n")] = '\0';
        _parse_line(data_read);
    }

    free(data_read);
    fclose(fp);
}

int main(void)
{
    _parse_input();

    uint32_t ans1 = 0;
    uint32_t ans2 = 0;

    for (uint32_t i = 0; i < count; i++) {
        if (i) {
            if (input[i] > input[i - 1]) {
                ans1++;
            }
        }

        if (i < (count - 2)) {
            if ((input[i + 1] + input[i + 2] + input[i + 3]) > (input[i] + input[i + 1] + input[i + 2])) {
                ans2++;
            }
        }
    }

    printf("ans1 = %u\n", ans1);
    printf("ans2 = %u\n", ans2);

    return EXIT_SUCCESS;
}
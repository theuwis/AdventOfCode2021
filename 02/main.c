#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef enum { DIR_UP, DIR_DOWN, DIR_FORWARD } directions_e;

typedef struct {
    directions_e dir;
    uint32_t nof_moves;
} directions_t;

#define MAX_NOF_ENTRIES (1000)

static directions_t input[MAX_NOF_ENTRIES];
static uint32_t count;

static void _parse_line(char *line)
{
    assert((count < MAX_NOF_ENTRIES) && "Too many entries in input");

    char *token = NULL;
    token = strtok(line, " ");

    directions_e dir;

    if (strncmp(token, "up", 2) == 0) {
        dir = DIR_UP;
    } else if (strncmp(token, "down", 4) == 0) {
        dir = DIR_DOWN;
    } else if (strncmp(token, "forward", 7) == 0) {
        dir = DIR_FORWARD;
    } else {
        assert(false && "Unknown direction");
    }

    input[count].dir = dir;

    token = strtok(NULL, " ");
    input[count].nof_moves = atoi(token);

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

    /* Part 1 */
    uint32_t horizontal_1 = 0;
    int32_t depth_1 = 0;

    /* Part 2 */
    uint32_t horizontal_2 = 0;
    uint32_t depth_2 = 0;
    int32_t aim_2 = 0;

    for (uint32_t i = 0; i < count; i++) {
        switch (input[i].dir) {
            case DIR_UP:
                depth_1 += input[i].nof_moves;
                aim_2 -= input[i].nof_moves;
                break;
            case DIR_DOWN:
                depth_1 -= input[i].nof_moves;
                aim_2 += input[i].nof_moves;
                break;
            case DIR_FORWARD:
                horizontal_1 += input[i].nof_moves;
                horizontal_2 += input[i].nof_moves;
                depth_2 += (aim_2 * input[i].nof_moves);
                break;
        }
    }

    printf("ans1 = %u\n", horizontal_1 * abs(depth_1));
    printf("ans2 = %u\n", horizontal_2 * abs(depth_2));

    return EXIT_SUCCESS;
}
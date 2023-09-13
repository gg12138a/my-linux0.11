#include <linux/blk.h>
#include <stddef.h>

struct request request[NR_REQUEST] = {0};

void blk_dev_init(void) {
    for (int i = 0; i < NR_REQUEST; i++) {
        request[i].dev = -1;
        request[i].next = NULL;
    }
}

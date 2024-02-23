#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
    long mtype;
    int mtext[10]; // Matching the array size of kirk.c to receive numbers

};

int compare(const void *a, const void *b) { // Comparator function for sorting
    return (*(int*)a - *(int*)b);
}

int main(void) {
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    if ((key = ftok("kirk.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, 0644)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("spock: ready to sort numbers.\n");

    for(;;) { // Infinite loop to continuously receive and sort numbers
        if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        qsort(buf.mtext, 10, sizeof(int), compare); // Sorting the numbers
        printf("spock: Sorted numbers: ");
        for (int i = 0; i < 10; i++) { // Printing the sorted numbers
            printf("%d ", buf.mtext[i]);
        }
        printf("\n");
    }

    return 0;
}

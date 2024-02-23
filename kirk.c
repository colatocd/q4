#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
    long mtype;
    int mtext[10]; // Changed from char[200] to int[10] to send numbers
};

int main(void) {
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    if ((key = ftok("kirk.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Enter numbers to sort, -1 to finish:\n");
    buf.mtype = 1; // Using a fixed message type

    int num, i = 0;
    while(scanf("%d", &num) && num != -1) { // Accepting numbers instead of text
        if (i < 10) {
            buf.mtext[i++] = num; // Storing numbers in the array
        } else {
            printf("Array full. Sending numbers...\n");
            break; // Break if the array is full
        }
    }

    if (msgsnd(msqid, &buf, sizeof(buf.mtext), 0) == -1) // Sending the array of numbers
        perror("msgsnd");

    return 0;
}

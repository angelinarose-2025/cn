#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define WINDOW_SIZE 4  
#define TOTAL_FRAMES 10  

void send_frame(int frame) {
    printf("Sender: Sending frame %d\n", frame);
}

bool receive_ack(int frame) {
    int random = rand() % 10;  
    return (random < 8);  
}

int main() {
    srand(time(NULL));  
    int base = 0, next_frame = 0;

    while (base < TOTAL_FRAMES) {
        while (next_frame < base + WINDOW_SIZE && next_frame < TOTAL_FRAMES) {
            send_frame(next_frame);
            next_frame++;
        }

        sleep(1);

        if (receive_ack(base)) {
            printf("Receiver: ACK received for frame %d\n", base);
            base++;  
        } else {
            printf("Receiver: ACK lost for frame %d! Resending from %d\n", base, base);
            next_frame = base;
        }
    }

    printf("All frames sent successfully!\n");
    return 0;
}

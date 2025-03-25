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
    bool received[TOTAL_FRAMES] = {false};
    int base = 0;

    while (base < TOTAL_FRAMES) {
        for (int i = base; i < base + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            if (!received[i]) {
                send_frame(i);
            }
        }

        sleep(1);

        for (int i = base; i < base + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            if (!received[i] && receive_ack(i)) {
                printf("Receiver: ACK received for frame %d\n", i);
                received[i] = true;
            }
        }

        while (base < TOTAL_FRAMES && received[base]) {
            base++;
        }
    }

    printf("All frames sent successfully!\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define TIMEOUT 2  

void send_frame(int frame) {
    printf("Sender: Sending frame %d\n", frame);
}

bool receive_ack() {
    int random = rand() % 10;  
    return (random < 8); 
}

int main() {
    srand(time(NULL));  
    int total_frames = 5;  
    int frame = 0;

    while (frame < total_frames) {
        send_frame(frame);
        sleep(1); 

        if (receive_ack()) {
            printf("Receiver: ACK received for frame %d\n\n", frame);
            frame++;  
        } else {
            printf("Receiver: ACK lost for frame %d! Resending...\n\n", frame);
        }
    }

    printf("All frames sent successfully!\n");
    return 0;
}

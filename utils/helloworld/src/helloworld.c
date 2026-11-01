#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t now;

    time(&now);

    printf("\n");
    printf("================================\n");
    printf(" Hello World from OpenWrt Feed!\n");
    printf("================================\n");
    printf("Current time: %s", ctime(&now));
    printf("Running on OpenWrt Linux\n\n");

    return 0;
}
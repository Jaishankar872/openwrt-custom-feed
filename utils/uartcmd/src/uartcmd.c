#define _DEFAULT_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define APP_NAME "uartcmd"
#define APP_VERSION "1.0"
#define UART_DEVICE "/dev/ttyACM0"

static void print_usage(FILE *stream)
{
    fprintf(stream,
            "Usage:\n"
            "  %s -en1             Turn Light 1 on\n"
            "  %s -dis1            Turn Light 1 off\n"
            "  %s -cus \"command\"   Send a custom command\n"
            "  %s -v               Print version and available commands\n",
            APP_NAME, APP_NAME, APP_NAME, APP_NAME);
}

static int write_all(int fd, const char *buffer, size_t length)
{
    size_t written = 0;

    while (written < length) {
        ssize_t result = write(fd, buffer + written, length - written);

        if (result < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (result == 0) {
            errno = EIO;
            return -1;
        }

        written += (size_t)result;
    }

    return 0;
}

static int send_command(const char *command)
{
    struct termios settings;
    size_t command_length = strlen(command);
    int fd = open(UART_DEVICE, O_WRONLY | O_NOCTTY);

    if (fd < 0) {
        fprintf(stderr, "%s: cannot open %s: %s\n",
                APP_NAME, UART_DEVICE, strerror(errno));
        return 1;
    }

    if (tcgetattr(fd, &settings) < 0) {
        fprintf(stderr, "%s: cannot read serial settings: %s\n",
                APP_NAME, strerror(errno));
        close(fd);
        return 1;
    }

    cfmakeraw(&settings);
    if (cfsetispeed(&settings, B115200) < 0 ||
        cfsetospeed(&settings, B115200) < 0) {
        fprintf(stderr, "%s: cannot set 115200 baud: %s\n",
                APP_NAME, strerror(errno));
        close(fd);
        return 1;
    }

    settings.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
#ifdef CRTSCTS
    settings.c_cflag &= ~CRTSCTS;
#endif
    settings.c_cflag |= CS8 | CLOCAL | CREAD;
    settings.c_iflag &= ~(IXON | IXOFF | IXANY);
    settings.c_cc[VMIN] = 1;
    settings.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &settings) < 0) {
        fprintf(stderr, "%s: cannot configure serial port: %s\n",
                APP_NAME, strerror(errno));
        close(fd);
        return 1;
    }

    if (write_all(fd, command, command_length) < 0 ||
        write_all(fd, "\n", 1) < 0) {
        fprintf(stderr, "%s: failed to write to %s: %s\n",
                APP_NAME, UART_DEVICE, strerror(errno));
        close(fd);
        return 1;
    }

    if (close(fd) < 0) {
        fprintf(stderr, "%s: failed to close %s: %s\n",
                APP_NAME, UART_DEVICE, strerror(errno));
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    const char *command;

    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        printf("%s %s\n", APP_NAME, APP_VERSION);
        print_usage(stdout);
        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "-en1") == 0) {
        command = "Light-1-ON";
    } else if (argc == 2 && strcmp(argv[1], "-dis1") == 0) {
        command = "Light-1-OFF";
    } else if (argc == 3 && strcmp(argv[1], "-cus") == 0) {
        if (argv[2][0] == '\0') {
            fprintf(stderr, "%s: custom command must not be empty\n", APP_NAME);
            return 2;
        }
        command = argv[2];
    } else {
        print_usage(stderr);
        return 2;
    }

    return send_command(command);
}

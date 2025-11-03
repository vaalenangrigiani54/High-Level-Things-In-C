#define _POSIX_C_SOURCE 200809L
#include <ioExtras.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>


/*===================================================================================================*/
/*--------------------------------------- AUXILIARY FUNCTIONS ---------------------------------------*/
/*===================================================================================================*/


static bool ioExtrasAux_readInteger(const char* inputText, long long* output, uint64_t bytes, FILE* stream) {
    if (inputText == NULL || output == NULL || stream == NULL)
        return false;
    
    char buffer[(bytes << 3) * sizeof(char)];
    if (!read_string(inputText, buffer, sizeof(buffer), stream))
        return false;

    *output = atoll(buffer);
    return true;
}


static bool ioExtrasAux_readFloat(const char* inputText, long double* output, uint64_t bytes, FILE* stream) {
    if (inputText == NULL || output == NULL || stream == NULL)
        return false;
    
    char buffer[(bytes << 3) * sizeof(char)];
    if (!read_string(inputText, buffer, sizeof(buffer), stream))
        return false;

    *output = strtold(buffer, NULL);
    return true;
}


static bool ioExtrasAux_initConsole(struct termios* oldt) {
	struct termios newt;

	if (tcgetattr(STDIN_FILENO, oldt) == -1) {
        perror("tcgetattr");
        return false;
    }

	newt = *oldt;
	newt.c_lflag &= ~(ICANON | ECHO);

	if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
        perror("tcsetattr");
        return false;
    }

	return true;
}


static bool ioExtrasAux_restoreConsole(struct termios* oldt) {
	bool ok = tcsetattr(STDIN_FILENO, TCSANOW, oldt) != -1;
    if (ok)
        perror("tcsetattr");

    return ok;
}


static int32_t ioExtrasAux_nextKey() {
	unsigned char c[20];
	ssize_t n = read(STDIN_FILENO, c, 20);
    if (n == -1) {
        return 0;
    }

    if (n >= 3 && c[0] == 27 && c[1] == 91) {
        switch (c[2]) {
            case 65: return UP_ARROW;
            case 66: return DOWN_ARROW;
            case 67: return RIGHT_ARROW;
            case 68: return LEFT_ARROW;
        }
    }

	return c[0];
}



/*====================================================================================================*/
/*----------------------------------------- HEADER FUNCTIONS -----------------------------------------*/
/*====================================================================================================*/


bool read_int8(const char* inputText, int8_t* output, FILE* stream) {
    long long n;
    if (!ioExtrasAux_readInteger(inputText, &n, sizeof(int8_t), stream))
        return false;

    *output = (int8_t) n;
    return true;
}


bool read_int16(const char* inputText, int16_t* output, FILE* stream) {
    long long n;
    if (!ioExtrasAux_readInteger(inputText, &n, sizeof(int16_t), stream))
        return false;

    *output = (int16_t) n;
    return true;
}


bool read_int32(const char* inputText, int32_t* output, FILE* stream) {
    long long n;
    if (!ioExtrasAux_readInteger(inputText, &n, sizeof(int32_t), stream))
        return false;

    *output = (int32_t) n;
    return true;
}


bool read_int64(const char* inputText, int64_t* output, FILE* stream) {
    long long n;
    if (!ioExtrasAux_readInteger(inputText, &n, sizeof(int64_t), stream))
        return false;

    *output = (int64_t) n;
    return true;
}


bool read_float(const char* inputText, float* output, FILE* stream) {
    long double x;
    if (!ioExtrasAux_readFloat(inputText, &x, sizeof(float), stream))
        return false;

    *output = (float) x;
    return true;
}


bool read_double64(const char* inputText, double* output, FILE* stream) {
    long double x;
    if (!ioExtrasAux_readFloat(inputText, &x, sizeof(double), stream))
        return false;

    *output = (double) x;
    return true;
}


bool read_double80(const char* inputText, long double* output, FILE* stream) {
    return ioExtrasAux_readFloat(inputText, output, sizeof(long double), stream);
}


bool read_character(const char* inputText, char* output, FILE* stream) {
    if (inputText == NULL || output == NULL || stream == NULL)
        return false;
    
    char buffer[2 * sizeof(char)];
    if (!read_string(inputText, buffer, sizeof(buffer), stream))
        return false;

    *output = *buffer;
    return true;
}


bool read_string(const char* inputText, char* buffer, int32_t bufferSize, FILE* stream) {
    if (inputText == NULL || buffer == NULL || bufferSize <= 0 || stream == NULL)
        return false;
    
    if (!fgets(buffer, bufferSize, stream))
        return false;

    uint64_t len = strlen(buffer);

    if (len > 0) {
        if (buffer[len - 1] != NEWLINE) {
            if (isatty(fileno(stdin))) {
                int c;
                while ((c = fgetc(stream)) != NEWLINE && c != EOF);
            }
        } else
            buffer[len - 1] = END_STRING;
    }

    return true;
}


void clearScreen() {
    printf("\x1b[2J\x1b[H");
    fflush(stdout);
}


void hideCursor() {
    printf("\x1b[?25l");
}


void showCursor() {
    printf("\x1b[?25h");
}


int32_t waitForKeyPressed() {
    struct termios oldt;
    if (!ioExtrasAux_initConsole(&oldt))
        return 0;

    int32_t key = getchar();

    if (!ioExtrasAux_restoreConsole(&oldt))
        return 0;

    return key;
}


bool callbackLoop(int32_t (*callback)(int32_t, __ptr_t), __ptr_t arg, uint32_t fps) {
    if (callback == NULL || fps == 0)
        return false;
    
    int32_t input = 0;
	struct termios oldt;

    if (!ioExtrasAux_initConsole(&oldt)) return;
    if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        return false;
    }

	do {
		usleep(1000000 / fps);
		input = ioExtrasAux_nextKey();
	} while (callback(input, arg) != 0);

	if (!ioExtrasAux_restoreConsole(&oldt))
        return false;

    if (fcntl(STDIN_FILENO, F_SETFL, 0) == -1) {
        perror("fcntl");
        return false;
    }

    return true;
}
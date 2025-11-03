#ifndef IO_EXTRAS_H
#define IO_EXTRAS_H

#include <moreTypes.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef UP_ARROW
    #define UP_ARROW 256
#endif

#ifndef DOWN_ARROW
    #define DOWN_ARROW 257
#endif

#ifndef LEFT_ARROW
    #define LEFT_ARROW 258
#endif

#ifndef RIGHT_ARROW
    #define RIGHT_ARROW 259
#endif


// Input for 8-bit integers.
bool read_int8(const char* inputText, int8_t* output, FILE* stream);
// Input for 16-bit integers.
bool read_int16(const char* inputText, int16_t* output, FILE* stream);
// Input for 32-bit integers.
bool read_int32(const char* inputText, int32_t* output, FILE* stream);
// Input for 64-bit integers.
bool read_int64(const char* inputText, int64_t* output, FILE* stream);
// Input for single precision floating point numbers.
bool read_float(const char* inputText, float* output, FILE* stream);
// Input for double precision floating point numbers.
bool read_double64(const char* inputText, double* output, FILE* stream);
// Input for extended precision floating point numbers.
bool read_double80(const char* inputText, long double* output, FILE* stream);
// Input for single characters.
bool read_character(const char* inputText, char* output, FILE* stream);
// Input for strings (wrapper of fgets).
bool read_string(const char* inputText, char* buffer, int32_t bufferSize, FILE* stream);

// Clears the terminal screen.
void clearScreen();

// Hides the cursor in terminal.
void hideCursor();

// Shows the cursor in terminal.
void showCursor();

/**
 * Waits until any key is pressed.
 * 
 * @returns The pressed key code. `0` if error.
 */
int32_t waitForKeyPressed();

/**
 * Executes a function in a loop at a specified framerate.
 * 
 * @param callback (NOT NULL) The callback function. Receives the pressed key code (`0` if no pressed key) at that frame and optional arguments.
 * @param arg A pointer to the optional arguments. Use a struct if you want to send more than one argument.
 * @param fps The amount of times per second in which `callback` is called.
 * 
 * @returns `true` if successful. Otherwise `false`.
 * 
 * @note `callback` must return `0` to stop the iteration.
 */
bool callbackLoop(int32_t (*callback)(int32_t, __ptr_t), __ptr_t arg, uint32_t fps);


/*
Example usage:
```c
    ...

    int main() {
        ...
        return 0;
    }

    EXECUTE_BEFORE_MAIN(setUp) {
        // Code for setting up things before main
    }
```
*/
#define EXECUTE_BEFORE_MAIN(func_name) \
    void __##func_name##_execute_before_main__() __attribute__ ((constructor)); \
    void __##func_name##_execute_before_main__()


/*
Example usage:
```c
    ...

    int main() {
        ...
        return 0;
    }

    EXECUTE_AFTER_MAIN(setUp) {
        // Code for tearing down things after main
    }
```
*/
#define EXECUTE_AFTER_MAIN(func_name) \
    void __##func_name##_execute_after_main__() __attribute__ ((destructor)); \
    void __##func_name##_execute_after_main__()


#endif // IO_EXTRAS_H
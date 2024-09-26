#include "check_abort.h"

// Example function to demonstrate the use of MC/DC (code coverage metric).
// See this youtube video for more information:
// https://www.youtube.com/watch?v=k0_PF8MtEEo
boolean CheckAbort(boolean off_course, boolean abort_commanded, boolean valid_abort_command) {
    if (off_course || (abort_commanded && valid_abort_command)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

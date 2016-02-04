#include <string.h>

#include "SystemLog.h"

#define SHELL_CONTEXT_PROMPT                            "\n\rwsacii-1.0> "
#define SHELL_CONTEXT_PROMPT_LEN                        (strlen(SHELL_CONTEXT_PROMPT))

BYTE printTerminal (BYTE* Ptr, boolean hasPrompt){

    BYTE buffer[500];
    
    memset(buffer, 0, 500);
    
    if (hasPrompt)
        memcpy(buffer, SHELL_CONTEXT_PROMPT, SHELL_CONTEXT_PROMPT_LEN);

    if (Ptr == NULL)
        return 0;
    
    strcat(buffer, Ptr);
    printf(buffer);
    return 0;
}

void SystemLog_Printf(BOOL hasPrompt, const char * format, ...){

    char buffer[SYSTEM_LOG_PRINT_MAX_BUFFER_SIZE];
    va_list argptr;

    va_start(argptr, format);
    vsprintf(buffer, format, argptr);
    va_end(argptr);

    printTerminal(buffer, hasPrompt);
}


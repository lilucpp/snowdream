#pragma once

#include <stdio.h>
#include <string>

namespace Helper {
    #ifdef WIN32
    #define POPEN _popen
    #define PCLOSE _pclose
    #else
    #define POPEN popen
    #define PCLOSE pclose
    #endif

    inline std::string ssystem(const char *cmd) 
    {
        char buffer[128];
        std::string result = "";
        FILE *pipe = POPEN(cmd, "r");
        if (!pipe)
            return result;

        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }

        PCLOSE(pipe);
        return result;
    }

    inline int ssystem(const char *cmd, std::string &result)
    {
        char buffer[128];
        FILE *pipe = POPEN(cmd, "r");
        if (!pipe)
            errno;

        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }

        PCLOSE(pipe);
        return 0;
    }
}
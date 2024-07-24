
#ifndef FSW_H
#define FSW_H

#include "SRDirectoryChangeHandler.h"
#include "libfswatch/c++/libfswatch_exception.hpp"
#include "libfswatch/c++/monitor_factory.hpp"
#include "libfswatch/c/libfswatch_log.h"

using namespace fsw;

#define FSW_EXIT_OK 0
#define FSW_EXIT_UNK_OPT 1
#define FSW_EXIT_USAGE 2
#define FSW_EXIT_LATENCY 3
#define FSW_EXIT_STREAM 4
#define FSW_EXIT_ERROR 5
#define FSW_EXIT_ENFILE 6
#define FSW_EXIT_OPT 7
#define FSW_EXIT_MONITOR_NAME 8
#define FSW_EXIT_FORMAT 9

#define RECOLL_ENV "/home/peanut/software/bin/recoll_env.sh"
#define RECOLL_INDEX "/home/peanut/software/bin/recollindex"
#define RECOLLQ "/home/peanut/software/bin/recollq"
#define RECOLL_CONF "/home/peanut/software/recoll_home/"

#define LOCK_MAX_TRY 20

class FSWatchImpl {
   public:
    FSWatchImpl();
    ~FSWatchImpl();

    void start_monitor();
    void conf_and_start_monitor();

    static void index_inc_dir( const std::string& path );
    static void index_exc_dir( const std::string& path );
    static void index_inc_file( const std::string& path );
    static void index_exc_file( const std::string& path );

    static void set_change_handle( CSRDirectoryChangeHandler* handle );

    static void process_events( const std::vector< event >& events, void* context );
    static CSRDirectoryChangeHandler* change_handle_;
};

#endif /* FSW_H */

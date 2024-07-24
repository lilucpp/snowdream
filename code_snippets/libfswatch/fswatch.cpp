#ifdef HAVE_CONFIG_H
#include "libfswatch_config.h"
#endif
#include <fcntl.h>
#include <sys/file.h>
#include <QString>
#include <boost/filesystem.hpp>
#include <csignal>
#include <exception>
#include <functional>
#include <map>
#include <thread>
#include "boost/bind.hpp"
#include "confighelp.h"
#include "fmt/format.h"
#include "fswatch.hpp"
#include "gettext.h"
#include "plog/Appenders/ConsoleAppender.h"
#include "plog/Log.h"
#include "threadpool.h"

#ifdef HAVE_GETOPT_LONG
#include <getopt.h>
#endif

extern plog::Severity g_log_level;

#define _( String ) gettext( String )

static FSW_EVENT_CALLBACK process_events;
static monitor* active_monitor = nullptr;

CSRDirectoryChangeHandler* FSWatchImpl::change_handle_ = nullptr;

static void close_monitor() {
    if ( active_monitor ) active_monitor->stop();
}

extern "C" void close_handler( int signal ) {
    FSW_ELOG( "Executing termination handler.\n" );
    close_monitor();
}

static void register_signal_handlers() {
    struct sigaction action;
    action.sa_handler = close_handler;
    sigemptyset( &action.sa_mask );
    action.sa_flags = 0;

    if ( sigaction( SIGTERM, &action, nullptr ) == 0 ) {
        FSW_ELOG( "SIGTERM handler registered.\n" );
    } else {
        std::cerr << "SIGTERM handler registration failed." << std::endl;
    }

    if ( sigaction( SIGABRT, &action, nullptr ) == 0 ) {
        FSW_ELOG( "SIGABRT handler registered.\n" );
    } else {
        std::cerr << "SIGABRT handler registration failed." << std::endl;
    }

    if ( sigaction( SIGINT, &action, nullptr ) == 0 ) {
        FSW_ELOG( "SIGINT handler registered.\n" );
    } else {
        std::cerr << "SIGINT handler registration failed" << std::endl;
    }
}

void FSWatchImpl::set_change_handle( CSRDirectoryChangeHandler* handle ) {
    change_handle_ = handle;
}

void FSWatchImpl::start_monitor() {
    ConfigHelp& conf = ConfigHelp::getInstance();
    // system_default_monitor_type
    active_monitor = monitor_factory::create_monitor( fsw_monitor_type::poll_monitor_type,
//    active_monitor = monitor_factory::create_monitor( fsw_monitor_type::inotify_monitor_type,
                                                      conf.getDirs(), process_events );

    // active_monitor->set_properties("");
    active_monitor->set_allow_overflow( true );
    // active_monitor->set_latency(lvalue);
    active_monitor->set_fire_idle_event( true );
    active_monitor->set_recursive( true );
    active_monitor->set_directory_only( false );
    active_monitor->set_event_type_filters( conf.getEventTypes() );
    active_monitor->set_filters( conf.getExcludeFilters() );
    // active_monitor->set_follow_symlinks(Lflag);
    active_monitor->set_watch_access( false );

    active_monitor->start();
}

FSWatchImpl::FSWatchImpl() {}

FSWatchImpl::~FSWatchImpl() { close_monitor(); }

static bool is_skip_by_name( const std::string& path ) {
    boost::filesystem::path p( path );

    // if ( !boost::filesystem::exists( p ) ) return false;

    if ( boost::filesystem::is_directory( p ) ) return false;

    ConfigHelp& conf = ConfigHelp::getInstance();
    std::unordered_set< std::string >&& container = conf.getFileTypes();
    if ( container.find( boost::filesystem::extension( p ) ) == container.end() ) return true;

    return false;
}

auto time_comp = []( const event& e1, const event& e2 ) -> bool {
    return ( e1.get_time() <= e2.get_time() );
};

void pre_process_events( const std::vector< event >& events,
                         std::set< event, decltype( time_comp ) >& time_events ) {
    //LOG_DEBUG << "pre_process_events";
    //合并事件，按文件名、时间
    std::map< std::string, event > path_events;
    for ( const event& evt : events ) {
        if ( is_skip_by_name( evt.get_path() ) ) {
           // LOG_DEBUG << "skip file:" << evt.get_path();
            continue;
        }
        auto itr = path_events.find( evt.get_path() );
        if ( itr == path_events.end() ) {
            path_events.insert(
                {evt.get_path(), {evt.get_path(), evt.get_time(), evt.get_flags()}} );
        } else {
            if ( itr->second.get_time() <= evt.get_time() ) {
                path_events.insert(
                    {evt.get_path(), {evt.get_path(), evt.get_time(), evt.get_flags()}} );
            }
        }
        // log_debug
        const auto& flags = evt.get_flags();
        if ( plog::debug <= g_log_level ) {
            std::vector< std::string > str_flags;
            for_each( flags.begin(), flags.end(), [&str_flags]( fsw_event_flag flag ) {
                str_flags.push_back( event::get_event_flag_name( flag ) );
            } );
           // LOG_DEBUG << fmt::format( "{}:{} {}", evt.get_time(), evt.get_path(),
             //                         fmt::join( str_flags, " " ) );
        }
    }

    //按时间排序
    for_each( path_events.begin(), path_events.end(),
              [&time_events]( std::pair< std::string, const event& > item ) {
                  time_events.insert(
                      {item.second.get_path(), item.second.get_time(), item.second.get_flags()} );
              } );
}

void FSWatchImpl::index_inc_dir( const std::string& path ) {
    if ( change_handle_ ) {
        // change_handle_->On_FileAdded( QString::fromUtf8( path.c_str() ) );
    }
}

void FSWatchImpl::index_exc_dir( const std::string& path ) {
    if ( change_handle_ ) {
        // change_handle_->On_FileRemoved( QString::fromUtf8( path.c_str() ) );
    }
}

void FSWatchImpl::index_inc_file( const std::string& path ) {
    try{
        int i = 0;
        for ( ; i < LOCK_MAX_TRY; ++i ) {
            int fd = open( path.c_str(), O_RDONLY );
            if ( fd != -1 ) {
                if ( flock( fd, LOCK_EX | LOCK_NB ) != -1 ) {
                    flock( fd, LOCK_UN );
                    close( fd );
                    //LOG_DEBUG << "index_inc_file get lock try times: " << i;
                    break;
                }
            }
            sleep( 1 );
        }
        if ( i == LOCK_MAX_TRY ) {
            LOG_ERROR << "index_inc_file get lock failed.";
            return;
        }

        if ( change_handle_ ) {
            change_handle_->On_FileAdded( QString::fromUtf8( path.c_str() ) );
        }
    }catch ( std::exception& e ) {
	     LOG_INFO << "exception: " << e.what();
    }catch ( ... ) {
	  LOG_INFO << ( "exception unknown" );
    }
}

void FSWatchImpl::index_exc_file( const std::string& path ) {
    if ( change_handle_ ) {
        change_handle_->On_FileRemoved( QString::fromUtf8( path.c_str() ) );
    }
}

void FSWatchImpl::process_events( const std::vector< event >& events, void* context ) {

//    for (const event &evt : events) {
//        LOG_DEBUG << "process_events 1:" << evt.get_path() << evt.get_time();
//        for (const fsw_event_flag& fg : evt.get_flags() ) {
//            LOG_DEBUG << "process_events 2:" << fg;
//        }
//    }

    //预处理事件
    std::set< event, decltype( time_comp ) > time_events( time_comp );
    pre_process_events( events, time_events );

    ThreadPool& pool = get_thread_pool();
    boost::system::error_code ec;
    for ( const event& evt : time_events ) {
        std::string path = evt.get_path();
        if ( path.find( ".kingsoft" ) != std::string::npos || path.find( ".local/share" ) != std::string::npos) {
	       continue;
        }//不监控office临时文件，防止崩溃问题
        if ( path.find( ".cache" ) != std::string::npos || path.find( ".config" ) != std::string::npos|| path.find( "tmpCheckToolPath" ) != std::string::npos) {
           continue;
        }//不监控系统临时文件，防止崩溃问题
        std::string inc_file;
        std::string exc_file;
        const auto& flags = evt.get_flags();
        for ( const auto& f : flags ) {
            switch ( f ) {
                case fsw_event_flag::Created:
                case fsw_event_flag::Updated:
                case fsw_event_flag::MovedTo:
                    inc_file = path;
                    break;
                case fsw_event_flag::Removed:
                case fsw_event_flag::Renamed:
                case fsw_event_flag::MovedFrom:
                    exc_file = path;
                    break;
                default:
                    LOG_WARNING << fmt::format( "未处理{}:{} {}", evt.get_time(), path,
                                                event::get_event_flag_name( f ) );
                    break;
            }
        }
        if ( boost::filesystem::is_directory( path, ec ) ) {
            if ( !inc_file.empty() ) {
                pool.post( boost::bind( &FSWatchImpl::index_inc_dir, path ) );
            }
            if ( !exc_file.empty() ) {
                pool.post( boost::bind( &FSWatchImpl::index_exc_dir, path ) );
            }
            continue;
        }
        if ( !inc_file.empty() ) {
            pool.post( boost::bind( &FSWatchImpl::index_inc_file, path ) );
        }
        if ( !exc_file.empty() ) {
            pool.post( boost::bind( &FSWatchImpl::index_exc_file, path ) );
        }
    }
}

void FSWatchImpl::conf_and_start_monitor() {
// Trigger gettext operations
#ifdef ENABLE_NLS
    setlocale( LC_ALL, "" );
    bindtextdomain( PACKAGE, LOCALEDIR );
    textdomain( PACKAGE );
#endif

    // configure and start the monitor
    try {
        // registering handlers to clean up resources
        register_signal_handlers();
        // atexit( boost::bind( &FSWatchImpl::close_monitor, shared_from_this() ) );

        // configure and start the monitor loop
        start_monitor();

        delete active_monitor;
        active_monitor = nullptr;
    } catch ( libfsw_exception& lex ) {
        LOG_INFO << lex.what() << "\n";
        LOG_INFO << "Status code: " << lex.error_code() << "\n";
    } catch ( std::invalid_argument& ex ) {
        LOG_INFO << ex.what() << "\n";
    } catch ( std::exception& conf ) {
        LOG_INFO << "An error occurred and the program will be terminated.\n";
        LOG_INFO << conf.what() << "\n";
    } catch ( ... ) {
        LOG_INFO << "An unknown error occurred and the program will be terminated.\n";
    }
}

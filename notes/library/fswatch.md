# fswatch 

## 命令行
```shell
Usage:
fswatch [OPTION] ... path ...
 -M, --list-monitors   List the available monitors.
 -m, --monitor=NAME    Use the specified monitor.
 -r, --recursive       Recurse subdirectories.
 -t, --timestamp       Print the event timestamp.
 -x, --event-flags     Print the event flags.
 ...

#常用：
fswatch -x --event Created --event Removed 
fswatch  -r -t -x   ./test_famonitor/
```

### Event Flags
```txt
NoOp:Idle event, optionally issued when no changes were detected.
PlatformSpecific:This event maps a platform-specific event that has no corresponding flag.
Created:The object has been created.
Updated:The object has been updated. The kind of update is monitor-dependent.
Removed:The object has been removed.
Renamed:The object has been renamed.
OwnerModified:The object’s owner has changed.
AttributeModified:An object’s attribute has changed.
MovedFrom:The object has moved from this location to a new location of the same file system.
MovedTo:The object has moved from another location in the same file system into this location.
IsFile:The object is a regular file.
IsDir:The object is a directory.  
IsSymLink:The object is a symbolic link. 
Link:The object link count has changed.
```
一个注意事项：使用libfswatch库时，可以用IsFile、IsDir来判断删除的路径文件or文件夹。

## libfswatch
```c++
void process_events(const std::vector<event> &events, void *context)
{
	// context可以穿个类指针供回调用
    FSWatchImpl *fswImpl = (FSWatchImpl *)context;
    // 预处理事件，合并event。
    std::set<event, decltype(time_comp)> time_events(time_comp);
    pre_process_events(events, time_events);
    
    std::error_code ec;
    for (const event &evt : events) {
        std::string path = evt.get_path();
        const auto &flags = evt.get_flags();

		// 打印flag日志
        std::vector<std::string> str_flags;
        for_each(flags.begin(), flags.end(), [&str_flags](fsw_event_flag flag)
                 { str_flags.push_back(event::get_event_flag_name(flag)); });
        SPDLOG_INFO("xxxx {}:{} {}", evt.get_time(), evt.get_path(), fmt::join(str_flags, " "));

        bool IsDir = fs::is_directory( path, ec );
        std::vector<event>::iterator iter;
        bool found = false;
        for (const auto &f : flags) {
            switch (f) {
            case fsw_event_flag::NoOp:
                break;
            case fsw_event_flag::Removed:
                if (std::find(flags.begin(), flags.end(), fsw_event_flag::IsDir) != flags.end()) IsDir = true;

            case fsw_event_flag::Created:
            case fsw_event_flag::Updated:
            case fsw_event_flag::Renamed:
                SPDLOG_INFO("处理...{} {} {}: {}", evt.get_time(), event::get_event_flag_name(f), evt.get_correlation_id(), path);
            case fsw_event_flag::MovedFrom:
            case fsw_event_flag::MovedTo:
            default:
                break;
            }
        }
    }
}


void start_monitor() {
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
	event_filters.push_back({fsw_event_flag::Created});
    event_filters.push_back({fsw_event_flag::Updated});
    event_filters.push_back({fsw_event_flag::Removed});
    event_filters.push_back({fsw_event_flag::Renamed});
    event_filters.push_back({fsw_event_flag::MovedFrom});
    event_filters.push_back({fsw_event_flag::MovedTo});
    event_filters.push_back({fsw_event_flag::IsFile});
    event_filters.push_back({fsw_event_flag::IsDir});
    active_monitor->set_event_type_filters(event_filters);
    active_monitor->set_filters( conf.getExcludeFilters() );
    // active_monitor->set_follow_symlinks(Lflag);
    active_monitor->set_watch_access( false );

    active_monitor->start();
}
```


## 参考文档
https://emcrisostomo.github.io/fswatch/doc/1.17.1/fswatch.html/  

## Windows操作系统版本号汇总

 确定 Windows 版本号最简单的方式便是使用 ver 命令。 

下表汇总列出了目前已知主要版本的 Windows 操作系统版本号。

|              操作系统名称              |       版本号       |
| :------------------------------------: | :----------------: |
| Windows 95 OEM Service Release 1 (95A) |      4.00.950      |
| Windows 95 OEM Service Release 2 (95B) |     4.00.1111      |
|   Windows 95 OEM Service Release 2.1   |   4.03.1212-1214   |
|  Windows 95 OEM Service Release 2.5 C  |     4.03.1214      |
|               Windows 98               |     4.10.1998      |
|     Windows 98 Second Edition (SE)     |    4.10.2222 A     |
|           Windows Millenium            |     4.90.3000      |
|             Windows NT 3.1             |      3.10.528      |
|             Windows NT 3.5             |      3.50.807      |
|            Windows NT 3.51             |     3.51.1057      |
|            Windows NT 4.00             |     4.00.1381      |
|              Windows 2000              |     5.00.2195      |
|               Windows XP               |      5.1.2600      |
|       Windows XP Service Pack 1        | 5.1.2600.1105-1106 |
|       Windows XP Service Pack 2        |   5.1.2600.2180    |
|       Windows XP Service Pack 3        |      5.1.2600      |
|          Windows Server 2003           |      5.2.3790      |
|   Windows Server 2003 Service Pack 1   |   5.2.3790.1180    |
|          Windows Server 2003           |   5.2.3790.1218    |
|          Windows Home Server           |      5.2.3790      |
|           Windows Vista RTM            |   6.0.6000.16386   |
|             Windows Vista              |      6.0.6000      |
|      Windows Vista Service Pack 2      |      6.0.6002      |
|          Windows Server 2008           |      6.0.6001      |
|             Windows 7 RTM              |      6.1.7601      |
|       Windows Server 2008 R2 RTM       |   6.1.7600.16385   |
|       Windows Server 2008 R2 SP1       |      6.1.7601      |
|        Windows Home Server 2011        |      6.1.8400      |
|          Windows Server 2012           |      6.2.9200      |
|               Windows 8                |      6.2.9200      |
|         Windows Server 2012 R2         |      6.3.9200      |
|              Windows 8.1               |      6.3.9200      |
|          Windows 8.1 Update 1          |      6.3.9600      |
|         Windows Server 2012 R2         |      6.3.9600      |
|               Windows 10               |     10.0.10240     |

以上来自： https://www.sysgeek.cn/windows-operating-system-version-numbers/ 



```cpp
//
// NTDDI version constants
//
#define NTDDI_WIN7                          0x06010000
#define NTDDI_WIN8                          0x06020000
#define NTDDI_WINBLUE                       0x06030000
#define NTDDI_WINTHRESHOLD                  0x0A000000  /* ABRACADABRA_THRESHOLD */
#define NTDDI_WIN10                         0x0A000000  /* ABRACADABRA_THRESHOLD */
#define NTDDI_WIN10_TH2                     0x0A000001  /* ABRACADABRA_WIN10_TH2 */
#define NTDDI_WIN10_RS1                     0x0A000002  /* ABRACADABRA_WIN10_RS1 */
#define NTDDI_WIN10_RS2                     0x0A000003  /* ABRACADABRA_WIN10_RS2 */
#define NTDDI_WIN10_RS3                     0x0A000004  /* ABRACADABRA_WIN10_RS3 */
#define NTDDI_WIN10_RS4                     0x0A000005  /* ABRACADABRA_WIN10_RS4 */
#define NTDDI_WIN10_RS5                     0x0A000006  /* ABRACADABRA_WIN10_RS5 */
#define NTDDI_WIN10_19H1                    0x0A000007  /* ABRACADABRA_WIN10_19H1*/

#define WDK_NTDDI_VERSION                   NTDDI_WIN10_19H1 /* ABRACADABRA_WIN10_19H1 */

//
// masks for version macros
//
#define OSVERSION_MASK      0xFFFF0000
#define SPVERSION_MASK      0x0000FF00
#define SUBVERSION_MASK     0x000000FF

//
// macros to extract various version fields from the NTDDI version
//
#define OSVER(Version)  ((Version) & OSVERSION_MASK)
#define SPVER(Version)  (((Version) & SPVERSION_MASK) >> 8)
#define SUBVER(Version) (((Version) & SUBVERSION_MASK) )
```

以上来着：https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-160
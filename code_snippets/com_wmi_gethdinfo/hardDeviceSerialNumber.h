
#ifndef _HDD_SERIAL_INFO_H_
#define _HDD_SERIAL_INFO_H_

#include <tchar.h>
#include <string.h>
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <vector>

#pragma warning(disable:4996) // strncpy GetVersionEx mbstowcs

#pragma pack(1)

#define IDENTIFY_BUFFER_SIZE 512

//  IOCTL commands
#define DFP_GET_VERSION 0x00074080
#define DFP_SEND_DRIVE_COMMAND 0x0007c084
#define DFP_RECEIVE_DRIVE_DATA 0x0007c088

#define FILE_DEVICE_SCSI 0x0000001b
#define IOCTL_SCSI_MINIPORT_IDENTIFY ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define IOCTL_SCSI_MINIPORT 0x0004D008 //  see NTDDSCSI.H for definition

#define SMART_GET_VERSION CTL_CODE(IOCTL_DISK_BASE, 0x0020, METHOD_BUFFERED, FILE_READ_ACCESS)
#define SMART_SEND_DRIVE_COMMAND \
    CTL_CODE(IOCTL_DISK_BASE, 0x0021, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define SMART_RCV_DRIVE_DATA CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

//  GETVERSIONOUTPARAMS contains the data returned from the
//  Get Driver Version function.
typedef struct _GETVERSIONOUTPARAMS
{
    BYTE bVersion;       // Binary driver version.
    BYTE bRevision;      // Binary driver revision.
    BYTE bReserved;      // Not used.
    BYTE bIDEDeviceMap;  // Bit map of IDE devices.
    DWORD fCapabilities; // Bit mask of driver capabilities.
    DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

//  Bits returned in the fCapabilities member of GETVERSIONOUTPARAMS
#define CAP_IDE_ID_FUNCTION 1            // ATA ID command supported
#define CAP_IDE_ATAPI_ID 2               // ATAPI ID command supported
#define CAP_IDE_EXECUTE_SMART_FUNCTION 4 // SMART commannds supported

//  Valid values for the bCommandReg member of IDEREGS.
#define IDE_ATAPI_IDENTIFY 0xA1 //  Returns ID sector for ATAPI.
#define IDE_ATA_IDENTIFY 0xEC   //  Returns ID sector for ATA.

// The following struct defines the interesting part of the IDENTIFY
// buffer:
typedef struct _IDSECTOR
{
    USHORT wGenConfig;
    USHORT wNumCyls;
    USHORT wReserved;
    USHORT wNumHeads;
    USHORT wBytesPerTrack;
    USHORT wBytesPerSector;
    USHORT wSectorsPerTrack;
    USHORT wVendorUnique[3];
    CHAR sSerialNumber[20];
    USHORT wBufferType;
    USHORT wBufferSize;
    USHORT wECCSize;
    CHAR sFirmwareRev[8];
    CHAR sModelNumber[40];
    USHORT wMoreVendorUnique;
    USHORT wDoubleWordIO;
    USHORT wCapabilities;
    USHORT wReserved1;
    USHORT wPIOTiming;
    USHORT wDMATiming;
    USHORT wBS;
    USHORT wNumCurrentCyls;
    USHORT wNumCurrentHeads;
    USHORT wNumCurrentSectorsPerTrack;
    ULONG ulCurrentSectorCapacity;
    USHORT wMultSectorStuff;
    ULONG ulTotalAddressableSectors;
    USHORT wSingleWordDMA;
    USHORT wMultiWordDMA;
    BYTE bReserved[128];
} IDSECTOR, *PIDSECTOR;

typedef struct _SRB_IO_CONTROL
{
    ULONG HeaderLength;
    UCHAR Signature[8];
    ULONG Timeout;
    ULONG ControlCode;
    ULONG ReturnCode;
    ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

//  Max number of drives assuming primary/secondary, master/slave topology
//	Modified to read only the master serial
#define MAX_IDE_DRIVES 1

//
// IDENTIFY data (from ATAPI driver source)
//

#pragma pack(1)

typedef struct _IDENTIFY_DATA
{
    USHORT GeneralConfiguration;       // 00 00
    USHORT NumberOfCylinders;          // 02  1
    USHORT Reserved1;                  // 04  2
    USHORT NumberOfHeads;              // 06  3
    USHORT UnformattedBytesPerTrack;   // 08  4
    USHORT UnformattedBytesPerSector;  // 0A  5
    USHORT SectorsPerTrack;            // 0C  6
    USHORT VendorUnique1[3];           // 0E  7-9
    USHORT SerialNumber[10];           // 14  10-19
    USHORT BufferType;                 // 28  20
    USHORT BufferSectorSize;           // 2A  21
    USHORT NumberOfEccBytes;           // 2C  22
    USHORT FirmwareRevision[4];        // 2E  23-26
    USHORT ModelNumber[20];            // 36  27-46
    UCHAR MaximumBlockTransfer;        // 5E  47
    UCHAR VendorUnique2;               // 5F
    USHORT DoubleWordIo;               // 60  48
    USHORT Capabilities;               // 62  49
    USHORT Reserved2;                  // 64  50
    UCHAR VendorUnique3;               // 66  51
    UCHAR PioCycleTimingMode;          // 67
    UCHAR VendorUnique4;               // 68  52
    UCHAR DmaCycleTimingMode;          // 69
    USHORT TranslationFieldsValid : 1; // 6A  53
    USHORT Reserved3 : 15;
    USHORT NumberOfCurrentCylinders;  // 6C  54
    USHORT NumberOfCurrentHeads;      // 6E  55
    USHORT CurrentSectorsPerTrack;    // 70  56
    ULONG CurrentSectorCapacity;      // 72  57-58
    USHORT CurrentMultiSectorSetting; //     59
    ULONG UserAddressableSectors;     //     60-61
    USHORT SingleWordDMASupport : 8;  //     62
    USHORT SingleWordDMAActive : 8;
    USHORT MultiWordDMASupport : 8; //     63
    USHORT MultiWordDMAActive : 8;
    USHORT AdvancedPIOModes : 8; //     64
    USHORT Reserved4 : 8;
    USHORT MinimumMWXferCycleTime;     //     65
    USHORT RecommendedMWXferCycleTime; //     66
    USHORT MinimumPIOCycleTime;        //     67
    USHORT MinimumPIOCycleTimeIORDY;   //     68
    USHORT Reserved5[2];               //     69-70
    USHORT ReleaseTimeOverlapped;      //     71
    USHORT ReleaseTimeServiceCommand;  //     72
    USHORT MajorRevision;              //     73
    USHORT MinorRevision;              //     74
    USHORT Reserved6[50];              //     75-126
    USHORT SpecialFunctionsEnabled;    //     127
    USHORT Reserved7[128];             //     128-255
} IDENTIFY_DATA, *PIDENTIFY_DATA;

#pragma pack()

//  Required to ensure correct PhysicalDrive IOCTL structure setup
#pragma pack(4)

#define IOCTL_STORAGE_QUERY_PROPERTY CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)


#pragma pack(4)

#define SENDIDLENGTH sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE
#define IOCTL_DISK_GET_DRIVE_GEOMETRY_EX CTL_CODE(IOCTL_DISK_BASE, 0x0028, METHOD_BUFFERED, FILE_ANY_ACCESS)

namespace HARDDISKSERIAL
{
    static char g_cszHardDriveSerialNumber[1024];
    static char g_cszHardDriveModelNumber[1024];
    static char g_cszErrorMessage[256];
    static BYTE g_byIdOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];

    inline BOOL DoIDENTIFY(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, PSENDCMDOUTPARAMS pSCOP,
                                          BYTE bIDCmd, BYTE bDriveNum, PDWORD lpcbBytesReturned)
    {
        // Set up data structures for IDENTIFY command.
        pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
        pSCIP->irDriveRegs.bFeaturesReg = 0;
        pSCIP->irDriveRegs.bSectorCountReg = 1;
        // pSCIP -> irDriveRegs.bSectorNumberReg = 1;
        pSCIP->irDriveRegs.bCylLowReg = 0;
        pSCIP->irDriveRegs.bCylHighReg = 0;

        // Compute the DriveIndex number.
        pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

        // The command can either be IDE identify or ATAPI identify.
        pSCIP->irDriveRegs.bCommandReg = bIDCmd;
        pSCIP->bDriveNumber = bDriveNum;
        pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

        return (DeviceIoControl(hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA, (LPVOID)pSCIP, sizeof(SENDCMDINPARAMS) - 1,
                                (LPVOID)pSCOP, sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1, lpcbBytesReturned,
                                NULL));
    }

    inline char* ConvertToString(DWORD dwDiskData[256], int iFirstIndex, int iLastIndex, char* pcszBuf)
    {
        int iIndex = 0;
        int iPosition = 0;

        //  each integer has two characters stored in it backwards

        // Removes the spaces from the serial no
        for (iIndex = iFirstIndex; iIndex <= iLastIndex; iIndex++)
        {
            //  get high byte for 1st character
            char ctemp = (char)(dwDiskData[iIndex] / 256);
            char cszmyspace[] = " ";
            if (!(ctemp == *cszmyspace))
            {
                pcszBuf[iPosition++] = ctemp;
            }
            //  get low byte for 2nd character
            char ctemp1 = (char)(dwDiskData[iIndex] % 256);
            if (!(ctemp1 == *cszmyspace))
            {
                pcszBuf[iPosition++] = ctemp1;
            }
        }

        //  end the string
        pcszBuf[iPosition] = '\0';

        //  cut off the trailing blanks
        for (iIndex = iPosition - 1; iIndex > 0 && isspace(pcszBuf[iIndex]); iIndex--)
            pcszBuf[iIndex] = '\0';

        return pcszBuf;
    }

    inline void PrintIdeInfo(int iDrive, DWORD dwDiskData[256])
    {
        char cszSerialNumber[1024];
        char cszModelNumber[1024];
        char cszRevisionNumber[1024];
        char bufferSize[32];

        __int64 i64Sectors = 0;
        __int64 i64Byte = 0;

        //  copy the hard iDrive serial number to the cszBuffer
        ConvertToString(dwDiskData, 10, 19, cszSerialNumber);
        ConvertToString(dwDiskData, 27, 46, cszModelNumber);
        ConvertToString(dwDiskData, 23, 26, cszRevisionNumber);
        sprintf_s(bufferSize, 32, "%u", dwDiskData[21] * 512);

        if (0 == g_cszHardDriveSerialNumber[0] &&
            //  serial number must be alphanumeric
            //  (but there can be leading spaces on IBM drives)
            (isalnum(cszSerialNumber[0]) || isalnum(cszSerialNumber[19])))
        {
            strcpy_s(g_cszHardDriveSerialNumber, 1024, cszSerialNumber);
            strcpy_s(g_cszHardDriveModelNumber, 1024, cszModelNumber);
        }
    }

    inline int ReadPhysicalDriveInNTWithAdminRights(int DriveIndex)
    {
        int iDone = FALSE;

        HANDLE hPhysicalDriveIOCTL = 0;

        //  Try to get a handle to PhysicalDrive IOCTL, report failure
        //  and exit if can't.
        char cszDriveName[256];

        sprintf_s(cszDriveName, 256, "\\\\.\\PhysicalDrive%d", DriveIndex);

        //  Windows NT, Windows 2000, must have admin rights
        hPhysicalDriveIOCTL = CreateFileA(cszDriveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                          NULL, OPEN_EXISTING, 0, NULL);
        if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
        {
            SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
            sprintf_s(g_cszErrorMessage, 256,
                      "%d ReadPhysicalDriveInNTWithAdminRights ERROR ,CreateFileA(%s) returned INVALID_HANDLE_VALUE",
                      __LINE__, cszDriveName);
        }
        else
        {
            GETVERSIONOUTPARAMS VersionParams;
            DWORD dwBytesReturned = 0;

            // Get the version, etc of PhysicalDrive IOCTL
            memset((void*)&VersionParams, 0, sizeof(VersionParams));

            if (!DeviceIoControl(hPhysicalDriveIOCTL, DFP_GET_VERSION, NULL, 0, &VersionParams, sizeof(VersionParams),
                                 &dwBytesReturned, NULL))
            {

                DWORD dwErr = GetLastError();
                SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
                sprintf_s(g_cszErrorMessage, 256,
                          "%d ReadPhysicalDriveInNTWithAdminRights ERROR DeviceIoControl() %d, DFP_GET_VERSION) returned "
                          "0, error is %d\n",
                          __LINE__, (int)hPhysicalDriveIOCTL, (int)dwErr);
            }

            // If there is a IDE device at number "iI" issue commands
            // to the device
            if (VersionParams.bIDEDeviceMap <= 0)
            {
                SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
                sprintf_s(g_cszErrorMessage, 256,
                          "%d ReadPhysicalDriveInNTWithAdminRights ERROR No device found at iPosition %d (%d)", __LINE__,
                          (int)DriveIndex, (int)VersionParams.bIDEDeviceMap);
            }
            else
            {
                BYTE bIDCmd = 0; // IDE or ATAPI IDENTIFY cmd
                SENDCMDINPARAMS scip;
                // SENDCMDOUTPARAMS OutCmd;

                // Now, get the ID sector for all IDE devices in the system.
                // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
                // otherwise use the IDE_ATA_IDENTIFY command
                bIDCmd = (VersionParams.bIDEDeviceMap >> DriveIndex & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

                memset(&scip, 0, sizeof(scip));
                memset(g_byIdOutCmd, 0, sizeof(g_byIdOutCmd));

                if (DoIDENTIFY(hPhysicalDriveIOCTL, &scip, (PSENDCMDOUTPARAMS)&g_byIdOutCmd, (BYTE)bIDCmd, (BYTE)DriveIndex,
                               &dwBytesReturned))
                {
                    DWORD dwDiskData[256];
                    int iIjk = 0;
                    USHORT* punIdSector = (USHORT*)((PSENDCMDOUTPARAMS)g_byIdOutCmd)->bBuffer;

                    for (iIjk = 0; iIjk < 256; iIjk++)
                        dwDiskData[iIjk] = punIdSector[iIjk];

                    PrintIdeInfo(DriveIndex, dwDiskData);

                    iDone = TRUE;
                }
            }

            CloseHandle(hPhysicalDriveIOCTL);
        }

        return iDone;
    }

    inline int ReadPhysicalDriveInNTUsingSmart(int DriveIndex)
    {
        int iDone = FALSE;

        HANDLE hPhysicalDriveIOCTL = 0;

        //  Try to get a handle to PhysicalDrive IOCTL, report failure
        //  and exit if can't.
        char cszDriveName[256];

        sprintf_s(cszDriveName, 256, "\\\\.\\PhysicalDrive%d", DriveIndex);

        //  Windows NT, Windows 2000, Windows Server 2003, Vista
        hPhysicalDriveIOCTL =
            CreateFileA(cszDriveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL, OPEN_EXISTING, 0, NULL);
        // if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
        //    printf ("Unable to open physical DriveIndex %d, error code: 0x%lX\n",
        //            DriveIndex, GetLastError ());

        if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
        {
            SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
            sprintf_s(
                g_cszErrorMessage, 256,
                "%d ReadPhysicalDriveInNTUsingSmart ERROR, CreateFileA(%s) returned INVALID_HANDLE_VALUE Error Code %d",
                __LINE__, cszDriveName, GetLastError());
        }
        else
        {
            GETVERSIONINPARAMS GetVersionParams;
            DWORD dwBytesReturned = 0;

            // Get the version, etc of PhysicalDrive IOCTL
            memset((void*)&GetVersionParams, 0, sizeof(GetVersionParams));

            if (!DeviceIoControl(hPhysicalDriveIOCTL, SMART_GET_VERSION, NULL, 0, &GetVersionParams,
                                 sizeof(GETVERSIONINPARAMS), &dwBytesReturned, NULL))
            {
                DWORD dwErr = GetLastError();
                SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
                sprintf_s(g_cszErrorMessage, 256,
                          "\n%d ReadPhysicalDriveInNTUsingSmart ERROR DeviceIoControl(%d, SMART_GET_VERSION) returned 0, "
                          "error is %d",
                          __LINE__, (int)hPhysicalDriveIOCTL, (int)dwErr);
            }
            else
            {
                // Print the SMART version
                // PrintVersion (& GetVersionParams);
                // Allocate the command cszBuffer
                ULONG CommandSize = sizeof(SENDCMDINPARAMS) + IDENTIFY_BUFFER_SIZE;
                PSENDCMDINPARAMS Command = (PSENDCMDINPARAMS)malloc(CommandSize);
                // Retrieve the IDENTIFY data
                // Prepare the command
    #define ID_CMD 0xEC // Returns ID sector for ATA
                Command->irDriveRegs.bCommandReg = ID_CMD;
                DWORD BytesReturned = 0;
                if (!DeviceIoControl(hPhysicalDriveIOCTL, SMART_RCV_DRIVE_DATA, Command, sizeof(SENDCMDINPARAMS), Command,
                                     CommandSize, &BytesReturned, NULL))
                {
                    SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
                    sprintf_s(g_cszErrorMessage, 256, "SMART_RCV_DRIVE_DATA IOCTL");

                    // Print the error
                    // PrintError ("SMART_RCV_DRIVE_DATA IOCTL", GetLastError());
                }
                else
                {
                    // Print the IDENTIFY data
                    DWORD dwDiskData[256];
                    USHORT* punIdSector = (USHORT*)(PIDENTIFY_DATA)((PSENDCMDOUTPARAMS)Command)->bBuffer;

                    for (int iIjk = 0; iIjk < 256; iIjk++)
                        dwDiskData[iIjk] = punIdSector[iIjk];

                    PrintIdeInfo(DriveIndex, dwDiskData);
                    iDone = TRUE;
                }
                // Done
                CloseHandle(hPhysicalDriveIOCTL);
                free(Command);
            }
        }

        return iDone;
    }

    inline char* flipAndCodeBytes(int iPos, int iFlip, const char* pcszStr, char* pcszBuf)
    {
        int iI;
        int iJ = 0;
        int iK = 0;

        pcszBuf[0] = '\0';
        if (iPos <= 0)
            return pcszBuf;

        if (!iJ)
        {
            char cP = 0;
            // First try to gather all characters representing hex digits only.
            iJ = 1;
            iK = 0;
            pcszBuf[iK] = 0;
            for (iI = iPos; iJ && !(pcszStr[iI] == '\0'); ++iI)
            {
                char cC = tolower(pcszStr[iI]);
                if (isspace(cC))
                    cC = '0';
                ++cP;
                pcszBuf[iK] <<= 4;

                if (cC >= '0' && cC <= '9')
                    pcszBuf[iK] |= (char)(cC - '0');
                else if (cC >= 'a' && cC <= 'f')
                    pcszBuf[iK] |= (char)(cC - 'a' + 10);
                else
                {
                    iJ = 0;
                    break;
                }

                if (cP == 2)
                {
                    if ((pcszBuf[iK] != '\0') && !isprint(pcszBuf[iK]))
                    {
                        iJ = 0;
                        break;
                    }
                    ++iK;
                    cP = 0;
                    pcszBuf[iK] = 0;
                }
            }
        }

        if (!iJ)
        {
            // There are non-digit characters, gather them as is.
            iJ = 1;
            iK = 0;
            for (iI = iPos; iJ && (pcszStr[iI] != '\0'); ++iI)
            {
                char cC = pcszStr[iI];

                if (!isprint(cC))
                {
                    iJ = 0;
                    break;
                }

                pcszBuf[iK++] = cC;
            }
        }

        if (!iJ)
        {
            // The characters are not there or are not printable.
            iK = 0;
        }

        pcszBuf[iK] = '\0';

        if (iFlip)
            // Flip adjacent characters
            for (iJ = 0; iJ < iK; iJ += 2)
            {
                char t = pcszBuf[iJ];
                pcszBuf[iJ] = pcszBuf[iJ + 1];
                pcszBuf[iJ + 1] = t;
            }

        // Trim any beginning and end space
        iI = iJ = -1;
        for (iK = 0; (pcszBuf[iK] != '\0'); ++iK)
        {
            if (!isspace(pcszBuf[iK]))
            {
                if (iI < 0)
                    iI = iK;
                iJ = iK;
            }
        }

        if ((iI >= 0) && (iJ >= 0))
        {
            for (iK = iI; (iK <= iJ) && (pcszBuf[iK] != '\0'); ++iK)
                pcszBuf[iK - iI] = pcszBuf[iK];
            pcszBuf[iK - iI] = '\0';
        }

        return pcszBuf;
    }

    inline int ReadPhysicalDriveInNTWithZeroRights(int DriveIndex)
    {
        int iDone = FALSE;

        HANDLE hPhysicalDriveIOCTL = 0;

        //  Try to get a handle to PhysicalDrive IOCTL, report failure
        //  and exit if can't.
        char cszDriveName[256];

        sprintf_s(cszDriveName, 256, "\\\\.\\PhysicalDrive%d", DriveIndex);

        //  Windows NT, Windows 2000, Windows XP - admin rights not required
        hPhysicalDriveIOCTL =
            CreateFileA(cszDriveName, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
        {
            SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
            sprintf_s(g_cszErrorMessage, 256,
                      "%d ReadPhysicalDriveInNTWithZeroRights ERROR CreateFileA(%s) returned INVALID_HANDLE_VALUE",
                      __LINE__, cszDriveName);
        }
        else
        {
            STORAGE_PROPERTY_QUERY query;
            DWORD dwBytesReturned = 0;
            char cszBuffer[10000];

            memset((void*)&query, 0, sizeof(query));
            query.PropertyId = StorageDeviceProperty;
            query.QueryType = PropertyStandardQuery;

            memset(cszBuffer, 0, sizeof(cszBuffer));

            if (DeviceIoControl(hPhysicalDriveIOCTL, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), &cszBuffer,
                                sizeof(cszBuffer), &dwBytesReturned, NULL))
            {
                STORAGE_DEVICE_DESCRIPTOR* descrip = (STORAGE_DEVICE_DESCRIPTOR*)&cszBuffer;
                char cszSerialNumber[1000];
                char cszModelNumber[1000];
                char cszVendorId[1000];
                char cszProductRevision[1000];

                flipAndCodeBytes(descrip->VendorIdOffset, 0, cszBuffer, cszVendorId);
                flipAndCodeBytes(descrip->ProductIdOffset, 0, cszBuffer, cszModelNumber);
                flipAndCodeBytes(descrip->ProductRevisionOffset, 0, cszBuffer, cszProductRevision);
                flipAndCodeBytes(descrip->SerialNumberOffset, 0, cszBuffer, cszSerialNumber);

                if (0 == g_cszHardDriveSerialNumber[0] &&
                    //  serial number must be alphanumeric
                    //  (but there can be leading spaces on IBM drives)
                    (iswalnum(cszSerialNumber[0]) || iswalnum(cszSerialNumber[19])))
                {
                    strcpy_s(g_cszHardDriveSerialNumber, 1024, cszSerialNumber);
                    strcpy_s(g_cszHardDriveModelNumber, 1024, cszModelNumber);
                    iDone = TRUE;
                }
                // Get the disk DriveIndex geometry.
                memset(cszBuffer, 0, sizeof(cszBuffer));
                if (!DeviceIoControl(hPhysicalDriveIOCTL, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, &cszBuffer,
                                     sizeof(cszBuffer), &dwBytesReturned, NULL))
                {
                    SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
                    sprintf_s(g_cszErrorMessage,
                              "%s ReadPhysicalDriveInNTWithZeroRights ERROR DeviceIoControl(), "
                              "IOCTL_DISK_GET_DRIVE_GEOMETRY_EX) returned 0",
                              cszDriveName);
                }
                else
                {
                    DISK_GEOMETRY_EX* geom = (DISK_GEOMETRY_EX*)&cszBuffer;
                    int iFixed = (geom->Geometry.MediaType == FixedMedia);
                    __int64 i64Size = geom->DiskSize.QuadPart;
                }
            }
            else
            {
                DWORD dwErr = GetLastError();
                SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
                sprintf_s(g_cszErrorMessage, "DeviceIOControl IOCTL_STORAGE_QUERY_PROPERTY error = %d\n", dwErr);
            }

            CloseHandle(hPhysicalDriveIOCTL);
        }

        return iDone;
    }



    inline int ReadIdeDriveAsScsiDriveInNT(void)
    {
        int iDone = FALSE;
        int iController = 0;

        for (iController = 0; iController < 2; iController++)
        {
            HANDLE hScsiDriveIOCTL = 0;
            char cszDriveName[256];

            //  Try to get a handle to PhysicalDrive IOCTL, report failure
            //  and exit if can't.
            sprintf_s(cszDriveName, "\\\\.\\Scsi%d:", iController);

            //  Windows NT, Windows 2000, any rights should do
            hScsiDriveIOCTL = CreateFileA(cszDriveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                          NULL, OPEN_EXISTING, 0, NULL);

            if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
            {
                int iDrive = 0;

                for (iDrive = 0; iDrive < 2; iDrive++)
                {
                    char cszBuffer[sizeof(SRB_IO_CONTROL) + SENDIDLENGTH];
                    SRB_IO_CONTROL* cP = (SRB_IO_CONTROL*)cszBuffer;
                    SENDCMDINPARAMS* pin = (SENDCMDINPARAMS*)(cszBuffer + sizeof(SRB_IO_CONTROL));
                    DWORD dwDummy;

                    memset(cszBuffer, 0, sizeof(cszBuffer));
                    cP->HeaderLength = sizeof(SRB_IO_CONTROL);
                    cP->Timeout = 10000;
                    cP->Length = SENDIDLENGTH;
                    cP->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
                    strncpy((char*)cP->Signature, "SCSIDISK", 8);

                    pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
                    pin->bDriveNumber = iDrive;

                    if (DeviceIoControl(hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, cszBuffer,
                                        sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDINPARAMS) - 1, cszBuffer,
                                        sizeof(SRB_IO_CONTROL) + SENDIDLENGTH, &dwDummy, NULL))
                    {
                        SENDCMDOUTPARAMS* pOut = (SENDCMDOUTPARAMS*)(cszBuffer + sizeof(SRB_IO_CONTROL));
                        IDSECTOR* pId = (IDSECTOR*)(pOut->bBuffer);
                        if (pId->sModelNumber[0])
                        {
                            DWORD dwDiskData[256];
                            int iIjk = 0;
                            USHORT* punIdSector = (USHORT*)pId;

                            for (iIjk = 0; iIjk < 256; iIjk++)
                                dwDiskData[iIjk] = punIdSector[iIjk];

                            PrintIdeInfo(iController * 2 + iDrive, dwDiskData);

                            iDone = TRUE;
                        }
                    }
                }
                CloseHandle(hScsiDriveIOCTL);
            }
        }

        return iDone;
    }



    inline long getHardDriveComputerID(int DriveIndex)
    {
        int iDone = FALSE;
        // char string [1024];
        __int64 i64Id = 0;
        OSVERSIONINFO version;

        strcpy_s(g_cszHardDriveSerialNumber, 1024, "");

        memset(&version, 0, sizeof(version));
        version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx(&version);
        if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)
        {
            //  this works under WinNT4 or Win2K if you have admin rights
            iDone = ReadPhysicalDriveInNTWithAdminRights(DriveIndex);
            //  this works under WinNT4 or Win2K or WinXP if you have any rights
            if (!iDone)
            {
                iDone = ReadPhysicalDriveInNTWithZeroRights(DriveIndex);
            }
            //  this works under WinNT4 or Win2K or WinXP or Windows Server 2003 or Vista if you have any rights
            if (!iDone)
            {
                iDone = ReadPhysicalDriveInNTUsingSmart(DriveIndex);
            }
            //  this should work in WinNT or Win2K if previous did not work
            //  this is kind of a backdoor via the SCSI mini port driver into
            //     the IDE drives
            if (!iDone)
            {
                iDone = ReadIdeDriveAsScsiDriveInNT();
            }
        }

        if (g_cszHardDriveSerialNumber[0] > 0)
        {
            char* cP = g_cszHardDriveSerialNumber;

            //  ignore first 5 characters from western digital hard drives if
            //  the first four characters are WD-W
            if (!strncmp(g_cszHardDriveSerialNumber, "WD-W", 4))
                cP += 5;
            for (; cP && *cP; cP++)
            {
                if ('-' == *cP)
                    continue;
                i64Id *= 10;
                switch (*cP)
                {
                    case '0':
                        i64Id += 0;
                        break;
                    case '1':
                        i64Id += 1;
                        break;
                    case '2':
                        i64Id += 2;
                        break;
                    case '3':
                        i64Id += 3;
                        break;
                    case '4':
                        i64Id += 4;
                        break;
                    case '5':
                        i64Id += 5;
                        break;
                    case '6':
                        i64Id += 6;
                        break;
                    case '7':
                        i64Id += 7;
                        break;
                    case '8':
                        i64Id += 8;
                        break;
                    case '9':
                        i64Id += 9;
                        break;
                    case 'a':
                    case 'A':
                        i64Id += 10;
                        break;
                    case 'b':
                    case 'B':
                        i64Id += 11;
                        break;
                    case 'c':
                    case 'C':
                        i64Id += 12;
                        break;
                    case 'd':
                    case 'D':
                        i64Id += 13;
                        break;
                    case 'e':
                    case 'E':
                        i64Id += 14;
                        break;
                    case 'f':
                    case 'F':
                        i64Id += 15;
                        break;
                    case 'g':
                    case 'G':
                        i64Id += 16;
                        break;
                    case 'h':
                    case 'H':
                        i64Id += 17;
                        break;
                    case 'i':
                    case 'I':
                        i64Id += 18;
                        break;
                    case 'j':
                    case 'J':
                        i64Id += 19;
                        break;
                    case 'k':
                    case 'K':
                        i64Id += 20;
                        break;
                    case 'l':
                    case 'L':
                        i64Id += 21;
                        break;
                    case 'm':
                    case 'M':
                        i64Id += 22;
                        break;
                    case 'n':
                    case 'N':
                        i64Id += 23;
                        break;
                    case 'o':
                    case 'O':
                        i64Id += 24;
                        break;
                    case 'p':
                    case 'P':
                        i64Id += 25;
                        break;
                    case 'q':
                    case 'Q':
                        i64Id += 26;
                        break;
                    case 'r':
                    case 'R':
                        i64Id += 27;
                        break;
                    case 's':
                    case 'S':
                        i64Id += 28;
                        break;
                    case 't':
                    case 'T':
                        i64Id += 29;
                        break;
                    case 'u':
                    case 'U':
                        i64Id += 30;
                        break;
                    case 'v':
                    case 'V':
                        i64Id += 31;
                        break;
                    case 'w':
                    case 'W':
                        i64Id += 32;
                        break;
                    case 'x':
                    case 'X':
                        i64Id += 33;
                        break;
                    case 'y':
                    case 'Y':
                        i64Id += 34;
                        break;
                    case 'z':
                    case 'Z':
                        i64Id += 35;
                        break;
                }
            }
        }

        i64Id %= 100000000;
        if (strstr(g_cszHardDriveModelNumber, "IBM-"))
            i64Id += 300000000;
        else if (strstr(g_cszHardDriveModelNumber, "MAXTOR") || strstr(g_cszHardDriveModelNumber, "Maxtor"))
            i64Id += 400000000;
        else if (strstr(g_cszHardDriveModelNumber, "WDC "))
            i64Id += 500000000;
        else
            i64Id += 600000000;

        return (long)i64Id;
    }

    inline int GetSerialNo(int DriveIndex, char* SerialNumber)
    {
        SecureZeroMemory(g_cszErrorMessage, sizeof(g_cszErrorMessage));
        SecureZeroMemory(g_cszHardDriveModelNumber, sizeof(g_cszHardDriveModelNumber));
        SecureZeroMemory(g_cszHardDriveSerialNumber, sizeof(g_cszHardDriveSerialNumber));

        if (DriveIndex >= 0)
        {
            getHardDriveComputerID(DriveIndex);
            size_t numberLength = strlen(g_cszHardDriveSerialNumber);
            if (numberLength == 0)
                return -1;
            memcpy(SerialNumber, g_cszHardDriveSerialNumber, numberLength);
        }
        return 0;
    }


    inline int GetErrorMessage(TCHAR* tszErrorMessage)
    {
        if (strlen(g_cszErrorMessage) != 0)
        {
            mbstowcs((wchar_t*)tszErrorMessage, g_cszErrorMessage, sizeof(g_cszErrorMessage));
            return 0;
        }
        else
            return -1;
    }

};
#pragma pack()

#endif // _HDD_SERIAL_INFO_H_

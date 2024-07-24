

## usb-1

There are at least three programs I know of that list device mount points:

1. [`mount`](http://manpages.ubuntu.com/mount.8) - mount a filesystem (used for general mount info too):

   ```
   $ mount
   /dev/sda3 on / type ext4 (rw,errors=remount-ro)
   proc on /proc type proc (rw,noexec,nosuid,nodev)
   sysfs on /sys type sysfs (rw,noexec,nosuid,nodev)
   ...
   /dev/mapper/lvmg-homelvm on /home type btrfs (rw,relatime,compress=lzo,space_cache)
   /dev/sda5 on /home/muru/arch type btrfs (rw,relatime,compress=lzo,space_cache)
   binfmt_misc on /proc/sys/fs/binfmt_misc type binfmt_misc (rw,noexec,nosuid,nodev)
   systemd on /sys/fs/cgroup/systemd type cgroup (rw,noexec,nosuid,nodev,none,name=systemd)
   ```

2. [`df`](http://manpages.ubuntu.com/df.1) - report file system disk space usage

   ```
   $ df       
   Filesystem                1K-blocks      Used Available Use% Mounted on
   /dev/sda3                  30832636  11993480  17249912  42% /
   none                              4         0         4   0% /sys/fs/cgroup
   ...
   /dev/sda5                  31457280   3948600  25396496  14% /home/bro3886/arch
   ```

3. [`lsblk`](http://manpages.ubuntu.com/lsblk.8) - list block devices

   ```
   $ lsblk
   NAME                    MAJ:MIN RM   SIZE RO TYPE MOUNTPOINT
   sda                       8:0    0 465.8G  0 disk 
   ├─sda1                    8:1    0   100M  0 part 
   ├─sda2                    8:2    0  58.5G  0 part 
   ├─sda3                    8:3    0    30G  0 part /
   ├─sda4                    8:4    0     1K  0 part 
   ├─sda5                    8:5    0    30G  0 part 
   ├─sda6                    8:6    0 339.2G  0 part 
   │ └─lvmg-homelvm (dm-0) 252:0    0   1.2T  0 lvm  
   └─sda7                    8:7    0     8G  0 part [SWAP]
   sdb                       8:16   0 931.5G  0 disk 
   └─sdb1                    8:17   0 931.5G  0 part 
     └─lvmg-homelvm (dm-0) 252:0    0   1.2T  0 lvm  
   ```

Of these three, `mount` lists all the mountpoints, AFAICT. The others have their weaknesses.

------

1. `findmnt` suggested by [@webwurst](https://askubuntu.com/a/1035085/158442) is now my favourite tool for the job. It's a Swiss Army knife when it comes to output control (newer versions can output in JSON too):

   ```
   $ findmnt /
   TARGET SOURCE    FSTYPE OPTIONS
   /      /dev/sda1 ext4   rw,relatime,errors=remount-ro,data=ordered
   $ findmnt / -no source
   /dev/sda1
   $ findmnt / --json
   {
      "filesystems": [
         {"target": "/", "source": "/dev/sda1", "fstype": "ext4", "options": "rw,relatime,errors=remount-ro,data=ordered"}
      ]
   }
   $ findmnt / --df
   SOURCE    FSTYPE  SIZE  USED AVAIL USE% TARGET
   /dev/sda1 ext4   40.2G 25.8G 12.5G  64% /
   ```

## usb-2

The following commands show you information about mounted volumes:

- The well-known Unix `mount`, showing e.g. `/dev/disk5s3` mounted at `/Volumes/Foo`
- `diskutil list` shows an overview of all disks and volumes
- `diskutil info /dev/disk5s3` shows information about that volume, including a `Volume UUID` that can be used to uniquely identify that volume.

You can query `diskutil info` by using the volume's UUID:

```bsh
$ diskutil info DEC8759E-F77D-3EAE-B3EB-B6438F1AA428 | grep 'Mount Point'
   Mount Point:              /Volumes/DroboOne
```

------

Sample command output on my system:**(/etc/mtab  /etc/fstab)**

```bsh
$ mount
/dev/disk1 on / (hfs, local, journaled)
devfs on /dev (devfs, local, nobrowse)
map -hosts on /net (autofs, nosuid, automounted, nobrowse)
map auto_home on /home (autofs, automounted, nobrowse)
localhost:/bNqIvVr1ZdFBdf19Io81Q4 on /Volumes/MobileBackups (mtmfs, nosuid, read-only, nobrowse)
/dev/disk4 on /Volumes/MyBook (hfs, local, nodev, nosuid, journaled)
/dev/disk5s3 on /Volumes/DroboOne (hfs, local, nodev, nosuid, journaled, noowners)
/dev/disk7s3 on /Volumes/DroboTwo (hfs, local, nodev, nosuid, journaled, noowners)
/dev/disk6s3 on /Volumes/DroboThree (hfs, local, nodev, nosuid, journaled, noowners)
```

------

```bsh
$ diskutil list
/dev/disk0
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      GUID_partition_scheme                        *256.1 GB   disk0
   1:                        EFI                         209.7 MB   disk0s1
   2:          Apple_CoreStorage                         240.0 GB   disk0s2
   3:                 Apple_Boot Recovery HD             650.0 MB   disk0s3
/dev/disk1
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:                  Apple_HFS Servus10 HD            *239.7 GB   disk1
/dev/disk2
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      GUID_partition_scheme                        *3.0 TB     disk2
   1:                        EFI                         209.7 MB   disk2s1
   2:          Apple_CoreStorage                         3.0 TB     disk2s2
   3:                 Apple_Boot Boot OS X               134.2 MB   disk2s3
/dev/disk4
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:                  Apple_HFS MyBook                 *3.0 TB     disk4
/dev/disk5
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:     Apple_partition_scheme                        *2.2 TB     disk5
   1:        Apple_partition_map                         32.3 KB    disk5s1
   2:                  Apple_HFS DroboOne                2.2 TB     disk5s3
/dev/disk6
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:     Apple_partition_scheme                        *2.2 TB     disk6
   1:        Apple_partition_map                         32.3 KB    disk6s1
   2:                  Apple_HFS DroboThree              2.2 TB     disk6s3
/dev/disk7
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:     Apple_partition_scheme                        *2.2 TB     disk7
   1:        Apple_partition_map                         32.3 KB    disk7s1
   2:                  Apple_HFS DroboTwo                2.2 TB     disk7s3
```

------

```bsh
$ diskutil info /dev/disk5s3
   Device Identifier:        disk5s3
   Device Node:              /dev/disk5s3
   Part of Whole:            disk5
   Device / Media Name:      Untitled

   Volume Name:              DroboOne
   Escaped with Unicode:     DroboOne

   Mounted:                  Yes
   Mount Point:              /Volumes/DroboOne
   Escaped with Unicode:     /Volumes/DroboOne

   File System Personality:  Journaled HFS+
   Type (Bundle):            hfs
   Name (User Visible):      Mac OS Extended (Journaled)
   Journal:                  Journal size 172032 KB at offset 0x4001000
   Owners:                   Disabled

   Partition Type:           Apple_HFS
   OS Can Be Installed:      No
   Media Type:               Generic
   Protocol:                 FireWire
   SMART Status:             Not Supported
   Volume UUID:              DEC8759E-F77D-3EAE-B3EB-B6438F1AA428

   Total Size:               2.2 TB (2198888927232 Bytes) (exactly 4294704936 512-Byte-Blocks)
   Volume Free Space:        169.4 GB (169412173824 Bytes) (exactly 330883152 512-Byte-Blocks)
   Device Block Size:        512 Bytes

   Read-Only Media:          No
   Read-Only Volume:         No
   Ejectable:                Yes
```

## usb-3 code

```c++
#include <mntent.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <scsi/scsi.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>

int getSerialSCSI(int fd, void *buf, size_t buf_len) {
    // we shall retrieve page 0x80 as per http://en.wikipedia.org/wiki/SCSI_Inquiry_Command
    unsigned char inq_cmd[] = {INQUIRY, 1, 0x80, 0, buf_len, 0};
    unsigned char sense[32];
    struct sg_io_hdr io_hdr;
            int result;

    memset(&io_hdr, 0, sizeof (io_hdr));
    io_hdr.interface_id = 'S';
    io_hdr.cmdp = inq_cmd;
    io_hdr.cmd_len = sizeof (inq_cmd);
    io_hdr.dxferp = buf;
    io_hdr.dxfer_len = buf_len;
    io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
    io_hdr.sbp = sense;
    io_hdr.mx_sb_len = sizeof (sense);
    io_hdr.timeout = 5000;

    result = ioctl(fd, SG_IO, &io_hdr);
    if (result < 0)
        return result;

    if ((io_hdr.info & SG_INFO_OK_MASK) != SG_INFO_OK)
        return 1;

    return 0;
}

std::string getSerial(const std::string& devPath) {
    std::string serialNum;

    const char *dev = devPath.c_str();
    char scsi_serial[255];
    int rc;
    int fd;

    fd = open(dev, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror(dev);
    }

    memset(scsi_serial, 0, sizeof (scsi_serial));
    rc = getSerialSCSI(fd, scsi_serial, 255);
    // scsi_serial[3] is the length of the serial number
    // scsi_serial[4] is serial number (raw, NOT null terminated)
    if (rc < 0) {
        printf("FAIL, rc=%d, errno=%d\n", rc, errno);
    } else
    if (rc == 1) {
        printf("FAIL, rc=%d, drive doesn't report serial number\n", rc);
    } else {
        if (!scsi_serial[3]) {
            printf("Failed to retrieve serial for %s\n", dev);
            return serialNum;
        }
//        printf("Serial Number: %.*s\n", (size_t) scsi_serial[3], (char *) & scsi_serial[4]);
        serialNum=(char *) & scsi_serial[4];
    }
    close(fd);
   
  return serialNum;

}

std::string getDevPathByMountPath(const std::string& mountPath) {
  std::string devPath;

  struct mntent *m;
  FILE *f = NULL;
  f = setmntent("/etc/mtab","r"); //open file for describing the mounted filesystems
  if(!f) {
    printf("error:%s\n",strerror(errno));
    return devPath;
  }

  while ((m = getmntent(f))) {//read next line
    //printf("Drive %s, name %s,type  %s,opt  %s\n", m->mnt_dir, m->mnt_fsname,
    //        m->mnt_type,m->mnt_opts );
    if(strncmp(mountPath.c_str(), m->mnt_dir, mountPath.size()) == 0) {
        devPath = m->mnt_fsname;
        break;
    }
  } 
    
  endmntent(f);   //close file for describing the mounted filesystems
  
  return devPath;
}

int main(int argc, char** argv)
{
   std::cout << "mount path:" << argv[1] << std::endl;

   std::string devPath=getDevPathByMountPath(argv[1]);
   
   std::cout << "dev path:" << devPath << std::endl;

   std::string serialNum = getSerial(devPath);
   std::cout << "serial num:" << serialNum << std::endl;
   
   return 0;
}

```

输出

```shell
peanut@peanut-vm:~/test$ sudo ./a.out /media/peanut/BCF8EC48F8EC028E
[sudo] peanut 的密码： 
mount path:/media/peanut/BCF8EC48F8EC028E
dev path:/dev/sdb1
serial num:8EF156813090

```



资料

1. [scsi_get_serial] https://stackoverflow.com/a/2666504/8330816
2. [getmntent] https://stackoverflow.com/a/9290177/8330816  https://blog.csdn.net/hepeng597/article/details/8925506
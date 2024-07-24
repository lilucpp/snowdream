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

static std::string GetFileContent(const std::string &filename) {
  std::ifstream ifs(filename);
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                     (std::istreambuf_iterator<char>()    ) );

  return content;
}

std::string getsnfrom990() {
    // maybe huawei 990 pc 
    // https://consumer.huawei.com/cn/support/content/zh-cn15957983/
    std::string hdcode = GetFileContent("/proc/bootdevice/cid");

    return hdcode;
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

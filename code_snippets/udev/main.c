#include <libudev.h>
#include <stdio.h>

#define BLOCK_SIZE 512

static struct udev_device*
get_child(struct udev* udev, struct udev_device* parent, const char* subsystem)
{
    struct udev_device* child = NULL;
    struct udev_enumerate *enumerate = udev_enumerate_new(udev);

    udev_enumerate_add_match_parent(enumerate, parent);
    udev_enumerate_add_match_subsystem(enumerate, subsystem);
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    udev_list_entry_foreach(entry, devices) {
        const char *path = udev_list_entry_get_name(entry);
        child = udev_device_new_from_syspath(udev, path);
        break;
    }

    udev_enumerate_unref(enumerate);
    return child;
}

static void enumerate_usb_mass_storage(struct udev* udev)
{
    struct udev_enumerate* enumerate = udev_enumerate_new(udev);

    udev_enumerate_add_match_subsystem(enumerate, "scsi");
    udev_enumerate_add_match_property(enumerate, "DEVTYPE", "scsi_device");
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    udev_list_entry_foreach(entry, devices) {
        const char *path, *tmp;
		unsigned long long disk_size = 0;
		unsigned short int block_size = BLOCK_SIZE;

        path = udev_list_entry_get_name(entry);
        struct udev_device* scsi = udev_device_new_from_syspath(udev, path);

        struct udev_device* block = get_child(udev, scsi, "block");
        struct udev_device* scsi_disk = get_child(udev, scsi, "scsi_disk");

        struct udev_device* usb
            = udev_device_get_parent_with_subsystem_devtype(scsi, "usb", "usb_device");

        if (block && scsi_disk && usb) {
            printf("I: VID=%s\n", udev_device_get_sysattr_value(usb, "idVendor"));
            printf("I: PID=%s\n", udev_device_get_sysattr_value(usb, "idProduct"));
            printf("I: SERIAL=%s\n", udev_device_get_sysattr_value(usb, "serial"));
            printf("I: VENDOR=%s\n",  udev_device_get_sysattr_value(scsi, "vendor"));
            printf("I: DEVNODE=%s\n", udev_device_get_devnode(block));
			printf("I: KERNEL=%s\n", udev_device_get_sysname(block));
			printf("I: DEVPATH=%s\n", udev_device_get_devpath(block));
			printf("I: DEVTYPE=%s\n", udev_device_get_devtype(block));

			tmp = udev_device_get_sysattr_value(block, "size");
			if (tmp)
				disk_size = atoll(tmp, NULL, 10);

			tmp = udev_device_get_sysattr_value(block, "queue/logical_block_size");
			if (tmp)
				block_size = atoi(tmp);

			printf("I: DEVSIZE=");
			if (strncmp(udev_device_get_sysname(block), "sr", 2) != 0)
				printf("%lld GB\n", (disk_size * block_size) / 1000000000);
			else
				printf("n/a\n");
        }

        if (block) {
            udev_device_unref(block);
        }

        if (scsi_disk) {
            udev_device_unref(scsi_disk);
        }

        udev_device_unref(scsi);
    }

    udev_enumerate_unref(enumerate);
}

int main()
{
    struct udev* udev = udev_new();

    enumerate_usb_mass_storage(udev);

    udev_unref(udev);
    return 0;
}
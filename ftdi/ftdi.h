/***************************************************************************
                          ftdi.h  -  description
                             -------------------
    begin                : Fri Apr 4 2003
    copyright            : (C) 2003 by Intra2net AG
    email                : opensource@intra2net.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation;             *
 *                                                                         *
 ***************************************************************************/

#ifndef __libftdi_h__
#define __libftdi_h__

/* libusb header */
#include <usb.h>

/* Kernel USB headers */
#include <linux/usb.h>
#include <linux/usbdevice_fs.h>

/* Keep this in sync with libusb */
struct usb_dev_handle {
  int fd;

  struct usb_bus *bus;
  struct usb_device *device;

  int config;
  int interface;
  int altsetting;

  /* Added by RMT so implementations can store other per-open-device data */
  void *impl_info;
};


enum ftdi_chip_type { TYPE_AM=0, TYPE_BM=1, TYPE_2232C=2 };

struct ftdi_context {
    // USB specific
    struct usb_dev_handle *usb_dev;
    int usb_read_timeout;
    int usb_write_timeout;
    struct usbdevfs_urb *urb;

    // FTDI specific
    enum ftdi_chip_type type;
    int baudrate;
    unsigned char bitbang_enabled;
    unsigned char *readbuffer;
    unsigned int readbuffer_offset;
    unsigned int readbuffer_remaining;
    unsigned int readbuffer_chunksize;
    unsigned int writebuffer_chunksize;

    // FTDI FT2232C requirecments
    int interface;   // 0 or 1
    int index;       // 1 or 2
    // Endpoints
    int in_ep;
    int out_ep;      // 1 or 2
    
    /* 1: (default) Normal bitbang mode, 2: FT2232C SPI bitbang mode */
    unsigned char bitbang_mode;

    // misc
    char *error_str;
};

struct ftdi_eeprom {
    int vendor_id;
    int product_id;

    int self_powered;
    int remote_wakeup;
    int BM_type_chip;

    int in_is_isochronous;
    int out_is_isochronous;
    int suspend_pull_downs;

    int use_serial;
    int change_usb_version;
    int usb_version;
    int max_power;

    char *manufacturer;
    char *product;
    char *serial;
};

#ifdef __cplusplus
extern "C" {
#endif

    int ftdi_init(struct ftdi_context *ftdi);
    void ftdi_deinit(struct ftdi_context *ftdi);
    void ftdi_set_usbdev (struct ftdi_context *ftdi, usb_dev_handle *usbdev);
    int ftdi_usb_open(struct ftdi_context *ftdi, int vendor, int product);
    int ftdi_usb_close(struct ftdi_context *ftdi);
    int ftdi_usb_reset(struct ftdi_context *ftdi);
    int ftdi_usb_purge_buffers(struct ftdi_context *ftdi);

    int ftdi_set_baudrate(struct ftdi_context *ftdi, int baudrate);

    int ftdi_read_data(struct ftdi_context *ftdi, unsigned char *buf, int size);
    int ftdi_read_data_set_chunksize(struct ftdi_context *ftdi, unsigned int chunksize);
    int ftdi_read_data_get_chunksize(struct ftdi_context *ftdi, unsigned int *chunksize);

    int ftdi_write_data(struct ftdi_context *ftdi, unsigned char *buf, int size);
    int ftdi_write_data_set_chunksize(struct ftdi_context *ftdi, unsigned int chunksize);
    int ftdi_write_data_get_chunksize(struct ftdi_context *ftdi, unsigned int *chunksize);

    int ftdi_enable_bitbang(struct ftdi_context *ftdi, unsigned char bitmask);
    int ftdi_disable_bitbang(struct ftdi_context *ftdi);
    int ftdi_read_pins(struct ftdi_context *ftdi, unsigned char *pins);

    int ftdi_set_latency_timer(struct ftdi_context *ftdi, unsigned char latency);
    int ftdi_get_latency_timer(struct ftdi_context *ftdi, unsigned char *latency);

    /* init and build eeprom from ftdi_eeprom structure */
    void ftdi_eeprom_initdefaults(struct ftdi_eeprom *eeprom);
    int  ftdi_eeprom_build(struct ftdi_eeprom *eeprom, unsigned char *output);

    /* "eeprom" needs to be valid 128 byte eeprom (generated by the eeprom generator)
       the checksum of the eeprom is valided */
    int ftdi_read_eeprom(struct ftdi_context *ftdi, unsigned char *eeprom);
    int ftdi_write_eeprom(struct ftdi_context *ftdi, unsigned char *eeprom);
    int ftdi_erase_eeprom(struct ftdi_context *ftdi);

#ifdef __cplusplus
}
#endif

#endif /* __libftdi_h__ */

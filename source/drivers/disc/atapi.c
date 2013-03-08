#include <kernel.h>

#define ATAPI_STATUS_CHECK(status, check) (((status) & (check)))

#define ATA_TIMEOUT 100000
#define ATAPI_SECTOR_SIZE       2048

#define ATA_PRIMARY_BUS         0x1F0
#define ATA_SECONDARY_BUS       0x170
#define ATA_PRIMARY_BUS2_BASE   0x1E8
#define ATA_SECONDARY_BUS2_BASE 0x168

#define ATA_DRIVE_MASTER        0xA0
#define ATA_DRIVE_SLAVE         0xB0
/*
#define ATA_DATA          0x0000 // Read/Write
#define ATA_ERROR         0x0001 // Read Only
#define ATA_FEATURES      0x0001 // Write Only
#define ATA_COUNT         0x0002 // Read/Write
#define ATA_LOW           0x0003 // Read/Write
#define ATA_MID           0x0004 // Read/Write
#define ATA_HIGH          0x0005 // Read/Write
#define ATA_DEVICE        0x0006 // Read/Write*/
#define ATA_STATUS        0x0007 // Read
/*
#define ATA_COMMAND       0x0007 // Write
//#define ATA_DCR           0x0008
#define ATA_ALT_STATUS    0x0206 // Read
#define ATA_DEVICE_CTRL   0x0206 // Write
*/
#define ATA_RESET         0x04

#define ATA_BSY           0x80
#define ATA_DRQ           0x08

typedef struct ata_device {
  short channel;
  short drive;

  short buffer[1024]; //a 1 megabyte buffer (transfer area / cache)

  char serial_number[21];
  char firmware_revision[9];
  char model[41];

  bool atapi; // Bool for atapi or not
  bool exists;
  bool dma;
  bool interrupts; // Are we using interrupts

  struct ata_device *next;

} ata_device_t;

ata_device_t ata_devices[4];

typedef struct cdrom_capacity {
    uint32_t lba;
    uint32_t block_length;
} cdrom_capacity_t;


typedef struct pata_identify_info {
    uint16_t configuration;
    uint16_t cylinders;
    uint16_t reserved1;
    uint16_t heads;
    uint16_t track_bytes;
    uint16_t sector_bytes;
    uint16_t sectors;
    uint16_t reserved2[ 3 ];
    uint8_t serial_number[ 20 ];
    uint16_t buf_type;
    uint16_t buf_size;
    uint16_t ecc_bytes;
    uint8_t revision[ 8 ];
    uint8_t model_id[ 40 ];
    uint8_t sectors_per_rw_long;
    uint8_t reserved3;
    uint16_t reserved4;
    uint8_t reserved5;
    uint8_t capabilities;
    uint16_t reserved6;
    uint8_t reserved7;
    uint8_t pio_cycle_time;
    uint8_t reserved8;
    uint8_t dma;
    uint16_t valid;
    uint16_t current_cylinders;
    uint16_t current_heads;
    uint16_t current_sectors;
    uint16_t current_capacity0;
    uint16_t current_capacity1;
    uint8_t sectors_per_rw_irq;
    uint8_t sectors_per_rw_irq_valid;
    uint32_t lba_sectors;
    uint16_t single_word_dma_info;
    uint16_t multi_word_dma_info;
    uint16_t eide_pio_modes;
    uint16_t eide_dma_min;
    uint16_t eide_dma_time;
    uint16_t eide_pio;
    uint16_t eide_pio_iordy;
    uint16_t reserved9[ 2 ];
    uint16_t reserved10[ 4 ];
    uint16_t command_queue_depth;
    uint16_t reserved11[ 4 ];
    uint16_t major;
    uint16_t minor;
    uint16_t command_set_1;
    uint16_t command_set_2;
    uint16_t command_set_features_extensions;
    uint16_t command_set_features_enable_1;
    uint16_t command_set_features_enable_2;
    uint16_t command_set_features_default;
    uint16_t ultra_dma_modes;
    uint16_t reserved12[ 2 ];
    uint16_t advanced_power_management;
    uint16_t reserved13;
    uint16_t hardware_config;
    uint16_t acoustic;
    uint16_t reserved14[ 5 ];
    uint64_t lba_capacity_48;
    uint16_t reserved15[ 22 ];
    uint16_t last_lun;
    uint16_t reserved16;
    uint16_t device_lock_functions;
    uint16_t current_set_features_options;
    uint16_t reserved17[ 26 ];
    uint16_t reserved18;
    uint16_t reserved19[ 3 ];
    uint16_t reserved20[ 96 ];
} pata_identify_info_t;

uint32_t atapi_get_capacity()
{
  int error;
  uint8_t packet[12];
  cdrom_capacity_t cdc;

  memset(packet, 0, sizeof(packet));
  packet[0] = 0x25;

  //error = atapi_do_packet()
}


#define ATA_DATA(x)         (x)
  #define ATA_FEATURES(x)     (x+1)
  #define ATA_SECTOR_COUNT(x) (x+2)
  #define ATA_ADDRESS1(x)     (x+3)
  #define ATA_ADDRESS2(x)     (x+4)
  #define ATA_ADDRESS3(x)     (x+5)
  #define ATA_DRIVE_SELECT(x) (x+6)
  #define ATA_COMMAND(x)      (x+7)
  #define ATA_DCR(x)          (x+0x206)
#define ATA_SELECT_DELAY(bus) \
    {inb(ATA_DCR(bus));inb(ATA_DCR(bus));inb(ATA_DCR(bus));inb(ATA_DCR(bus));}

int atapi_drive_read_sector (uint32_t bus, uint32_t drive, uint32_t lba, uint8_t *buffer)
  {
    /* 0xA8 is READ SECTORS command byte. */
    uint8_t read_cmd[12] = { 0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t status;
    int size;
    /* Tell the scheduler that this process is using the ATA subsystem. */
    //ata_grab ();
    /* Select drive (only the slave-bit is set) */
    outb (drive & (1 << 4), ATA_DRIVE_SELECT (bus));
    ATA_SELECT_DELAY (bus);       /* 400ns delay */
    outb (0x0, ATA_FEATURES (bus));       /* PIO mode */
    outb (ATAPI_SECTOR_SIZE & 0xFF, ATA_ADDRESS2 (bus));
    outb (ATAPI_SECTOR_SIZE >> 8, ATA_ADDRESS3 (bus));
    outb (0xA0, ATA_COMMAND (bus));       /* ATA PACKET command */
    while ((status = inb (ATA_COMMAND (bus))) & 0x80)     /* BUSY */
      asm volatile ("pause");
    while (!((status = inb (ATA_COMMAND (bus))) & 0x8) && !(status & 0x1))
      asm volatile ("pause");
    /* DRQ or ERROR set */
    if (status & 0x1) {
      size = -1;
      goto cleanup;
    }
    read_cmd[9] = 1;              /* 1 sector */
    read_cmd[2] = (lba >> 0x18) & 0xFF;   /* most sig. byte of LBA */
    read_cmd[3] = (lba >> 0x10) & 0xFF;
    read_cmd[4] = (lba >> 0x08) & 0xFF;
    read_cmd[5] = (lba >> 0x00) & 0xFF;   /* least sig. byte of LBA */
    /* Send ATAPI/SCSI command */
    outws (ATA_DATA (bus), (uint16_t *) read_cmd, 6);
    /* Wait for IRQ that says the data is ready. */
    //schedule ();
    /* Read actual size */
    size =
      (((int) inb (ATA_ADDRESS3 (bus))) << 8) |
      (int) (inb (ATA_ADDRESS2 (bus)));
    /* This example code only supports the case where the data transfer
     * of one sector is done in one step. */
    //ASSERT (size == ATAPI_SECTOR_SIZE);
    /* Read data. */
    kprintf("about to read data");
    inws (ATA_DATA (bus), buffer, size / 2);
    /* The controller will send another IRQ even though we've read all
     * the data we want.  Wait for it -- so it doesn't interfere with
     * subsequent operations: */
    //schedule ();
    /* Wait for BSY and DRQ to clear, indicating Command Finished */
    while ((status = inb (ATA_COMMAND (bus))) & 0x88)
      asm volatile ("pause");
   cleanup:
    /* Exit the ATA subsystem */
    //ata_release ();
    return size;
  }



void pata_send_command(uint8_t cmd, uint8_t* regs, uint8_t reg_mask)
{
  // port->cmd_base == 0x170
  // port->ctrl_base == 0x376

  // Select
  outb(0xE0, 0x170 + 6);
  inb(0x374);

  // Wait
  int i = 0;
  for(i = 0; i < 4; i++)
    inb(0x374);

  uint8_t status = inb(0x374);
  kprintf("status = %d", status);

  for (i = 1; i < 7; i++)
    if ( (reg_mask & ( 1 << i ) ) != 0)
      outb(regs[i], 0x170 + i);

  outb(cmd, 0x170 + 7);

  status = inb(0x374);
  kprintf("status = %d", status);

  return 0;
}

void pata_read_pio(void* buffer, size_t size)
{
  if ((size & 2) != 0)
  {
    inws((uint16_t*)buffer, size, 0x170 + 0);
  }
  else
  {
    inls((uint32_t*)buffer, size / 2, 0x170 + 0);
  }
}

void ata_detect()
{
  ata_devices[0].channel = ATA_PRIMARY_BUS;
  ata_devices[0].drive = ATA_DRIVE_MASTER;

  ata_devices[1].channel = ATA_PRIMARY_BUS;
  ata_devices[1].drive = ATA_DRIVE_SLAVE;

  ata_devices[2].channel = ATA_SECONDARY_BUS;
  ata_devices[2].drive = ATA_DRIVE_MASTER;

  ata_devices[3].channel = ATA_SECONDARY_BUS;
  ata_devices[3].drive = ATA_DRIVE_SLAVE;

  uint32_t i = 0;
  for (i = 0; i < 4; i++)
  {
    outb(ata_devices[i].channel + 0x0006, ata_devices[i].drive);
    char status = inb(ata_devices[i].channel + ATA_STATUS);

    if (!status || status == 0xFF)
    {
      ata_devices[i].exists = 0;
    }
    else
    {
      ata_devices[i].exists = 1;
    }
  }
}

void init_atapi()
{
  ata_detect();
  uint32_t i;

  for (i = 0; i < 4; i++)
  {
    if (ata_devices[i].exists)
    {
      kprintf("Found Device: %x, %x", ata_devices[i].channel, ata_devices[i].drive);

      if (ata_devices[i].atapi)
        kprintf(", ATAPI: YES");

      kprintf("\n");
    }
  }

  // Try to read a few sectors
  //char* buf = kmalloc(2048);
  //int read = atapi_drive_read_sector(0x170, 0xa0, 0, buf);

  pata_send_command(0xA1, 0, 0);
  pata_identify_info_t ident;
  pata_read_pio(&ident, sizeof( pata_identify_info_t ) / 2);

  char model[40];
  memcpy(&model, (char*)ident.model_id, 40);
  kprintf("model %s\n", model);

  kprintf("sectors %d", ident.sectors);
}

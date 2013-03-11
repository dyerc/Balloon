#ifndef FS_ISO9660_H
#define FS_ISO9660_H

#define BLOCK_SIZE 2048

typedef struct iso9660_directory_entry
{
    uint8_t record_length;
    uint8_t ext_attribute_length;
    uint32_t location_le;
    uint32_t location_be;
    uint32_t length_le;
    uint32_t length_be;
    uint8_t datetime[ 7 ];
    uint8_t flags;
    uint8_t interleaved_unit;
    uint8_t interleaved_gap;
    uint16_t seq_le;
    uint16_t seq_be;
    uint8_t name_length;
} __attribute__(( packed )) iso9660_directory_entry_t;

typedef struct iso9660_volume_descriptor
{
  uint8_t type;
  uint8_t identifier[ 5 ];
  uint8_t version;
  uint8_t zero1;
  uint8_t system_identifier[ 32 ];
  uint8_t volume_identifier[ 32 ];
  uint8_t zero2[ 8 ];
  uint32_t sectors_le;
  uint32_t sectors_be;
  uint8_t zero3[ 32 ];
  uint16_t setsize_le;
  uint16_t setsize_be;
  uint16_t seq_number_le;
  uint16_t seq_number_be;
  uint16_t sector_size_le;
  uint16_t sector_size_be;
  uint8_t dunno1[ 8 ];
  uint8_t dunno2[ 4 ];
  uint8_t dunno3[ 4 ];
  uint8_t dunno4[ 4 ];
  uint8_t dunno5[ 4 ];
  uint8_t root_dir_record[ 32 ];

} __attribute__(( packed )) iso9660_volume_descriptor_t;

#endif

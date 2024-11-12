#pragma once

#ifndef _MAIN_H_
#define _MAIN_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define PACKET_VERSION_NUMBER       3
#define PACKET_TYPE                 1
#define SECONDARY_HEADER_FLAG       1
#define APID                       11
#define SEQUENCE_FLAGS              2
#define PACKET_SEQUENCE_COUNT      14


typedef struct PacketIdentification {
    uint8_t         packet_type;
    uint8_t         secondary_header_flag;
    uint16_t        apid;
} PacketIdentification;


typedef struct PacketSequenceControl {
    uint8_t         sequence_flags;
    uint16_t        packet_sequence_count;
} PacketSequenceControl;


typedef struct PrimaryHeader {
    uint8_t                 packet_version_number;
    PacketIdentification    packet_identification;
    PacketSequenceControl   packet_sequence_control;
    uint16_t                packet_data_length;
} PrimaryHeader;


int bitshift(int shift_direction, int shift, uint32_t *value);




#endif 
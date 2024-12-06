#pragma once

#ifndef _MAIN_H_
#define _MAIN_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

#define PRIMARY_HEADER_SIZE           6
#define MAX_CCSDS_PACKET_SIZE     65535

#define PACKET_VERSION_NUMBER_B       3
#define PACKET_TYPE_B                 1
#define SECONDARY_HEADER_FLAG_B       1
#define APID_B                       11
#define SEQUENCE_FLAGS_B              2
#define PACKET_SEQUENCE_COUNT_B      14
#define PACKET_DATA_LENGTH_B         16

#define PACKET_VERSION_NUMBER_H       0x07        //   3 bit
#define PACKET_TYPE_H                 0x01        //   1 bit
#define SECONDARY_HEADER_FLAG_H       0x01        //   1 bit
#define APID_H                        0x07FF      //  11 bit
#define SEQUENCE_FLAGS_H              0x03        //   2 bit
#define PACKET_SEQUENCE_COUNT_H       0x3FFF      //  14 bit
#define PACKET_DATA_LENGTH_H          0xFFFF      //  16 bit

#define BYTE_MASK                     0xFF


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

typedef struct CCSDSPackedHeader {
    uint16_t                packed_primary_header_word1;
    uint16_t                packed_primary_header_word2;
    uint16_t                packed_primary_header_word3;
} CCSDSPackedHeader;


int bitshift(int shift_direction, int shift, uint32_t *value);
int pack_primary_header(struct PrimaryHeader * p_header, struct CCSDSPackedHeader * c_packed_header);
std::vector <uint8_t> packetize(struct PrimaryHeader * p_header, struct CCSDSPackedHeader * c_packed_header, std::vector <uint8_t> * payload);
int depacketize(std::vector <uint8_t> * packet, struct CCSDSPackedHeader * c_packed_header, std::vector <uint8_t> * payload);
int unpack_primary_header(struct PrimaryHeader * p_header, struct CCSDSPackedHeader * c_packed_header);
int write_to_file(std::vector <uint8_t> * ccsds_packet);
int parse_primary_header(std::vector<uint8_t> * packet, struct CCSDSPackedHeader * c_packed_header);



#endif 
#include "../inc/main.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    printf("Hello, world!\n");
    
    int shift_dir;
    int shift_displ;
    uint32_t value_m;

    if (argc < 3) {
        std::cout << "Improper use of ccsds\n" <<
            "./ccsds <shift direction> <shift displacement> <value>\n" <<
            "shift direction: 0 for left or 1 for right\n" <<
            "shift displacement: how many bits to shift\n" <<
            "value: original value to be shifted\n" << std::endl;
        return 1;
    }

    sscanf(argv[1], "%d", &shift_dir);
    sscanf(argv[2], "%d", &shift_displ);
    sscanf(argv[3], "%u", &value_m);

    printf("value_m at initialization: %u\n", value_m);

    bitshift(shift_dir, shift_displ, &value_m);

    printf("value_m after bitshift left by %d: %u\n", shift_displ, value_m);
    
    struct PrimaryHeader p_header = {};
    p_header.packet_version_number = 001;
    p_header.packet_identification.packet_type = 1;
    p_header.packet_identification.secondary_header_flag = 0;
    p_header.packet_identification.apid = 713;
    p_header.packet_sequence_control.sequence_flags = 3;
    p_header.packet_sequence_control.packet_sequence_count = 5973;
    p_header.packet_data_length = 37119;

    pack_primary_header(&p_header);
    
    return 0;
}


int bitshift(int shift_direction, int shift, uint32_t *value) {
    switch (shift_direction)
    {
    case 0:
        *value = *value << shift;
        break;
    
    case 1:
        *value = *value >> shift;
        break;

    default:
        break;
    }
    return 0;
}


/**
 * @brief 
 * 
 * typedef struct PrimaryHeader {
    uint8_t                 packet_version_number;
    PacketIdentification    packet_identification;
    PacketSequenceControl   packet_sequence_control;
    uint16_t                packet_data_length;
 * 
 * @param prime_header 
 * @return int 
 */

int pack_primary_header(struct PrimaryHeader *prime_header) {

    if(!prime_header) {
        return 1;
    }

    prime_header->packet_version_number &= PACKET_VERSION_NUMBER_H;
    prime_header->packet_identification.packet_type &= PACKET_TYPE_H;
    prime_header->packet_identification.secondary_header_flag &= SECONDARY_HEADER_FLAG_H;
    prime_header->packet_identification.apid &= APID_H;
    prime_header->packet_sequence_control.sequence_flags &= SEQUENCE_FLAGS_H;
    prime_header->packet_sequence_control.packet_sequence_count &= PACKET_SEQUENCE_COUNT_H;
    prime_header->packet_data_length &= PACKET_DATA_LENGTH_H;

    uint16_t    primary_header_w1 = 0;
    uint16_t    primary_header_w2 = 0;
    uint16_t    primary_header_w3 = 0;

    constexpr int WORD_LENGTH = 16;

    primary_header_w1 = (prime_header->packet_version_number << (WORD_LENGTH - PACKET_VERSION_NUMBER_B)) |
                        (prime_header->packet_identification.packet_type << (WORD_LENGTH - (PACKET_VERSION_NUMBER_B + PACKET_TYPE_B))) | 
                        (prime_header->packet_identification.secondary_header_flag << (WORD_LENGTH - (PACKET_VERSION_NUMBER_B + PACKET_TYPE_B + SECONDARY_HEADER_FLAG_B))) | 
                        (prime_header->packet_identification.apid & APID_H);

    primary_header_w2 = (prime_header->packet_sequence_control.sequence_flags << (WORD_LENGTH - SEQUENCE_FLAGS_B)) | 
                        (prime_header->packet_sequence_control.packet_sequence_count & PACKET_SEQUENCE_COUNT_H);

    primary_header_w3 = prime_header->packet_data_length;

    // debug output summary:
    std::cout << "primary_header_w1: 0x" << std::hex << primary_header_w1 << std::endl;
    std::cout << "primary_header_w2: 0x" << std::hex << primary_header_w2 << std::endl;
    std::cout << "primary_header_w3: 0x" << std::hex << primary_header_w3 << std::endl;
    
    // format packed header as bytes:
    uint8_t packed_header[6];
    packed_header[0] = (primary_header_w1 >> 8) & BYTE_MASK;
    packed_header[1] = primary_header_w1 & BYTE_MASK;
    packed_header[2] = (primary_header_w2 >> 8) & BYTE_MASK;
    packed_header[3] = primary_header_w2 & BYTE_MASK;
    packed_header[4] = (primary_header_w3 >> 8) & BYTE_MASK;
    packed_header[5] = primary_header_w3 & BYTE_MASK;

    // display packed header: 
    std::cout << "Packed Header: ";
    for(int i = 0; i < 6; i++) {
        printf("%02X", packed_header[i]);
    }
    std::cout << std::endl;

    return 0;
}


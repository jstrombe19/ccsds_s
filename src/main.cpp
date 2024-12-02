#include "../inc/main.hpp"


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
    
    std::vector <uint8_t> payload = {0x3b, 0x44, 0x19, 0x2c, 0xaf};
    struct CCSDSPackedHeader c_packed_header = {};
    struct PrimaryHeader p_header = {};
    p_header.packet_version_number = 001;
    p_header.packet_identification.packet_type = 1;
    p_header.packet_identification.secondary_header_flag = 0;
    p_header.packet_identification.apid = 713;
    p_header.packet_sequence_control.sequence_flags = 3;
    p_header.packet_sequence_control.packet_sequence_count = 5973;
    p_header.packet_data_length = 37119;

    pack_primary_header(&p_header, &c_packed_header);

    packetize(&p_header, &c_packed_header, &payload);
    
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

int pack_primary_header(struct PrimaryHeader *prime_header, struct CCSDSPackedHeader *c_packed_header) {

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

    c_packed_header->packed_primary_header_word1 = 0;
    c_packed_header->packed_primary_header_word2 = 0;
    c_packed_header->packed_primary_header_word3 = 0;

    constexpr int WORD_LENGTH = 16;

    c_packed_header->packed_primary_header_word1 = (prime_header->packet_version_number << (WORD_LENGTH - PACKET_VERSION_NUMBER_B)) |
                        (prime_header->packet_identification.packet_type << (WORD_LENGTH - (PACKET_VERSION_NUMBER_B + PACKET_TYPE_B))) | 
                        (prime_header->packet_identification.secondary_header_flag << (WORD_LENGTH - (PACKET_VERSION_NUMBER_B + PACKET_TYPE_B + SECONDARY_HEADER_FLAG_B))) | 
                        (prime_header->packet_identification.apid & APID_H);

    c_packed_header->packed_primary_header_word2 = (prime_header->packet_sequence_control.sequence_flags << (WORD_LENGTH - SEQUENCE_FLAGS_B)) | 
                        (prime_header->packet_sequence_control.packet_sequence_count & PACKET_SEQUENCE_COUNT_H);

    c_packed_header->packed_primary_header_word3 = prime_header->packet_data_length;

    // debug output summary:
    std::cout << "c_packed_header->packed_primary_header_word1: 0x" << std::hex << c_packed_header->packed_primary_header_word1 << std::endl;
    std::cout << "c_packed_header->packed_primary_header_word2: 0x" << std::hex << c_packed_header->packed_primary_header_word2 << std::endl;
    std::cout << "c_packed_header->packed_primary_header_word3: 0x" << std::hex << c_packed_header->packed_primary_header_word3 << std::endl;
    
    // format packed header as bytes:
    uint8_t packed_header[6];
    packed_header[0] = (c_packed_header->packed_primary_header_word1 >> 8) & BYTE_MASK;
    packed_header[1] = c_packed_header->packed_primary_header_word1 & BYTE_MASK;
    packed_header[2] = (c_packed_header->packed_primary_header_word2 >> 8) & BYTE_MASK;
    packed_header[3] = c_packed_header->packed_primary_header_word2 & BYTE_MASK;
    packed_header[4] = (c_packed_header->packed_primary_header_word3 >> 8) & BYTE_MASK;
    packed_header[5] = c_packed_header->packed_primary_header_word3 & BYTE_MASK;

    // display packed header: 
    std::cout << "Packed Header: ";
    for(int i = 0; i < 6; i++) {
        printf("%02X", packed_header[i]);
    }
    std::cout << std::endl;

    return 0;
}


std::vector <uint8_t> packetize(struct PrimaryHeader * p_header, struct CCSDSPackedHeader * c_packed_header, std::vector <uint8_t> * payload) {
    if ((c_packed_header->packed_primary_header_word1 | c_packed_header->packed_primary_header_word2 | c_packed_header->packed_primary_header_word3) == 0) {
        pack_primary_header(p_header, c_packed_header);
    }

    std::vector <uint8_t> packet(PRIMARY_HEADER_SIZE + payload->size());
    memcpy(packet.data(), c_packed_header, PRIMARY_HEADER_SIZE);

    memcpy(packet.data() + PRIMARY_HEADER_SIZE, payload->data(), payload->size());

    return packet;
};


int depacketize(struct PrimaryHeader * up_header, std::vector <uint8_t> * payload) {
    return 0;
};

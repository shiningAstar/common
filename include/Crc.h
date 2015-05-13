#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED

int modbus_crc16(const unsigned char *puchMsg,int usDataLen, unsigned char *crc);

int make_crc16_code(const unsigned char *src, int len, unsigned char *crc);

int make_crc32_code(const unsigned char *src, int len, unsigned char *crc);

bool verify_crc16_data(unsigned char *src, int len, const unsigned char *crc);

bool verify_crc32_data(unsigned char *src, int len, const unsigned char *crc);


#endif // CRC_H_INCLUDED

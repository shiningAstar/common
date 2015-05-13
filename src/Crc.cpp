
#include "crc.h"
#include "string.h"

const unsigned char crc16_1[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
/* CRC低位字节值表*/
const unsigned char crc16_2[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;

const unsigned char crc32_1[] ={
0x00, 0x96, 0x2c, 0xba, 0x19, 0x8f, 0x35, 0xa3, 0x32, 0xa4,
0x1e, 0x88, 0x2b, 0xbd, 0x07, 0x91, 0x64, 0xf2, 0x48, 0xde,
0x7d, 0xeb, 0x51, 0xc7, 0x56, 0xc0, 0x7a, 0xec, 0x4f, 0xd9,
0x63, 0xf5, 0xc8, 0x5e, 0xe4, 0x72, 0xd1, 0x47, 0xfd, 0x6b,
0xfa, 0x6c, 0xd6, 0x40, 0xe3, 0x75, 0xcf, 0x59, 0xac, 0x3a,
0x80, 0x16, 0xb5, 0x23, 0x99, 0x0f, 0x9e, 0x08, 0xb2, 0x24,
0x87, 0x11, 0xab, 0x3d, 0x90, 0x06, 0xbc, 0x2a, 0x89, 0x1f,
0xa5, 0x33, 0xa2, 0x34, 0x8e, 0x18, 0xbb, 0x2d, 0x97, 0x01,
0xf4, 0x62, 0xd8, 0x4e, 0xed, 0x7b, 0xc1, 0x57, 0xc6, 0x50,
0xea, 0x7c, 0xdf, 0x49, 0xf3, 0x65, 0x58, 0xce, 0x74, 0xe2,
0x41, 0xd7, 0x6d, 0xfb, 0x6a, 0xfc, 0x46, 0xd0, 0x73, 0xe5,
0x5f, 0xc9, 0x3c, 0xaa, 0x10, 0x86, 0x25, 0xb3, 0x09, 0x9f,
0x0e, 0x98, 0x22, 0xb4, 0x17, 0x81, 0x3b, 0xad, 0x20, 0xb6,
0x0c, 0x9a, 0x39, 0xaf, 0x15, 0x83, 0x12, 0x84, 0x3e, 0xa8,
0x0b, 0x9d, 0x27, 0xb1, 0x44, 0xd2, 0x68, 0xfe, 0x5d, 0xcb,
0x71, 0xe7, 0x76, 0xe0, 0x5a, 0xcc, 0x6f, 0xf9, 0x43, 0xd5,
0xe8, 0x7e, 0xc4, 0x52, 0xf1, 0x67, 0xdd, 0x4b, 0xda, 0x4c,
0xf6, 0x60, 0xc3, 0x55, 0xef, 0x79, 0x8c, 0x1a, 0xa0, 0x36,
0x95, 0x03, 0xb9, 0x2f, 0xbe, 0x28, 0x92, 0x04, 0xa7, 0x31,
0x8b, 0x1d, 0xb0, 0x26, 0x9c, 0x0a, 0xa9, 0x3f, 0x85, 0x13,
0x82, 0x14, 0xae, 0x38, 0x9b, 0x0d, 0xb7, 0x21, 0xd4, 0x42,
0xf8, 0x6e, 0xcd, 0x5b, 0xe1, 0x77, 0xe6, 0x70, 0xca, 0x5c,
0xff, 0x69, 0xd3, 0x45, 0x78, 0xee, 0x54, 0xc2, 0x61, 0xf7,
0x4d, 0xdb, 0x4a, 0xdc, 0x66, 0xf0, 0x53, 0xc5, 0x7f, 0xe9,
0x1c, 0x8a, 0x30, 0xa6, 0x05, 0x93, 0x29, 0xbf, 0x2e, 0xb8,
0x02, 0x94, 0x37, 0xa1, 0x1b, 0x8d};

const unsigned char crc32_2[] = {
0x00, 0x30, 0x61, 0x51, 0xc4, 0xf4, 0xa5, 0x95, 0x88, 0xb8,
0xe9, 0xd9, 0x4c, 0x7c, 0x2d, 0x1d, 0x10, 0x20, 0x71, 0x41,
0xd4, 0xe4, 0xb5, 0x85, 0x98, 0xa8, 0xf9, 0xc9, 0x5c, 0x6c,
0x3d, 0x0d, 0x20, 0x10, 0x41, 0x71, 0xe4, 0xd4, 0x85, 0xb5,
0xa8, 0x98, 0xc9, 0xf9, 0x6c, 0x5c, 0x0d, 0x3d, 0x30, 0x00,
0x51, 0x61, 0xf4, 0xc4, 0x95, 0xa5, 0xb8, 0x88, 0xd9, 0xe9,
0x7c, 0x4c, 0x1d, 0x2d, 0x41, 0x71, 0x20, 0x10, 0x85, 0xb5,
0xe4, 0xd4, 0xc9, 0xf9, 0xa8, 0x98, 0x0d, 0x3d, 0x6c, 0x5c,
0x51, 0x61, 0x30, 0x00, 0x95, 0xa5, 0xf4, 0xc4, 0xd9, 0xe9,
0xb8, 0x88, 0x1d, 0x2d, 0x7c, 0x4c, 0x61, 0x51, 0x00, 0x30,
0xa5, 0x95, 0xc4, 0xf4, 0xe9, 0xd9, 0x88, 0xb8, 0x2d, 0x1d,
0x4c, 0x7c, 0x71, 0x41, 0x10, 0x20, 0xb5, 0x85, 0xd4, 0xe4,
0xf9, 0xc9, 0x98, 0xa8, 0x3d, 0x0d, 0x5c, 0x6c, 0x83, 0xb3,
0xe2, 0xd2, 0x47, 0x77, 0x26, 0x16, 0x0b, 0x3b, 0x6a, 0x5a,
0xcf, 0xff, 0xae, 0x9e, 0x93, 0xa3, 0xf2, 0xc2, 0x57, 0x67,
0x36, 0x06, 0x1b, 0x2b, 0x7a, 0x4a, 0xdf, 0xef, 0xbe, 0x8e,
0xa3, 0x93, 0xc2, 0xf2, 0x67, 0x57, 0x06, 0x36, 0x2b, 0x1b,
0x4a, 0x7a, 0xef, 0xdf, 0x8e, 0xbe, 0xb3, 0x83, 0xd2, 0xe2,
0x77, 0x47, 0x16, 0x26, 0x3b, 0x0b, 0x5a, 0x6a, 0xff, 0xcf,
0x9e, 0xae, 0xc2, 0xf2, 0xa3, 0x93, 0x06, 0x36, 0x67, 0x57,
0x4a, 0x7a, 0x2b, 0x1b, 0x8e, 0xbe, 0xef, 0xdf, 0xd2, 0xe2,
0xb3, 0x83, 0x16, 0x26, 0x77, 0x47, 0x5a, 0x6a, 0x3b, 0x0b,
0x9e, 0xae, 0xff, 0xcf, 0xe2, 0xd2, 0x83, 0xb3, 0x26, 0x16,
0x47, 0x77, 0x6a, 0x5a, 0x0b, 0x3b, 0xae, 0x9e, 0xcf, 0xff,
0xf2, 0xc2, 0x93, 0xa3, 0x36, 0x06, 0x57, 0x67, 0x7a, 0x4a,
0x1b, 0x2b, 0xbe, 0x8e, 0xdf, 0xef};

const unsigned char crc32_3[] = {
0x00, 0x07, 0x0e, 0x09, 0x6d, 0x6a, 0x63, 0x64, 0xdb, 0xdc,
0xd5, 0xd2, 0xb6, 0xb1, 0xb8, 0xbf, 0xb7, 0xb0, 0xb9, 0xbe,
0xda, 0xdd, 0xd4, 0xd3, 0x6c, 0x6b, 0x62, 0x65, 0x01, 0x06,
0x0f, 0x08, 0x6e, 0x69, 0x60, 0x67, 0x03, 0x04, 0x0d, 0x0a,
0xb5, 0xb2, 0xbb, 0xbc, 0xd8, 0xdf, 0xd6, 0xd1, 0xd9, 0xde,
0xd7, 0xd0, 0xb4, 0xb3, 0xba, 0xbd, 0x02, 0x05, 0x0c, 0x0b,
0x6f, 0x68, 0x61, 0x66, 0xdc, 0xdb, 0xd2, 0xd5, 0xb1, 0xb6,
0xbf, 0xb8, 0x07, 0x00, 0x09, 0x0e, 0x6a, 0x6d, 0x64, 0x63,
0x6b, 0x6c, 0x65, 0x62, 0x06, 0x01, 0x08, 0x0f, 0xb0, 0xb7,
0xbe, 0xb9, 0xdd, 0xda, 0xd3, 0xd4, 0xb2, 0xb5, 0xbc, 0xbb,
0xdf, 0xd8, 0xd1, 0xd6, 0x69, 0x6e, 0x67, 0x60, 0x04, 0x03,
0x0a, 0x0d, 0x05, 0x02, 0x0b, 0x0c, 0x68, 0x6f, 0x66, 0x61,
0xde, 0xd9, 0xd0, 0xd7, 0xb3, 0xb4, 0xbd, 0xba, 0xb8, 0xbf,
0xb6, 0xb1, 0xd5, 0xd2, 0xdb, 0xdc, 0x63, 0x64, 0x6d, 0x6a,
0x0e, 0x09, 0x00, 0x07, 0x0f, 0x08, 0x01, 0x06, 0x62, 0x65,
0x6c, 0x6b, 0xd4, 0xd3, 0xda, 0xdd, 0xb9, 0xbe, 0xb7, 0xb0,
0xd6, 0xd1, 0xd8, 0xdf, 0xbb, 0xbc, 0xb5, 0xb2, 0x0d, 0x0a,
0x03, 0x04, 0x60, 0x67, 0x6e, 0x69, 0x61, 0x66, 0x6f, 0x68,
0x0c, 0x0b, 0x02, 0x05, 0xba, 0xbd, 0xb4, 0xb3, 0xd7, 0xd0,
0xd9, 0xde, 0x64, 0x63, 0x6a, 0x6d, 0x09, 0x0e, 0x07, 0x00,
0xbf, 0xb8, 0xb1, 0xb6, 0xd2, 0xd5, 0xdc, 0xdb, 0xd3, 0xd4,
0xdd, 0xda, 0xbe, 0xb9, 0xb0, 0xb7, 0x08, 0x0f, 0x06, 0x01,
0x65, 0x62, 0x6b, 0x6c, 0x0a, 0x0d, 0x04, 0x03, 0x67, 0x60,
0x69, 0x6e, 0xd1, 0xd6, 0xdf, 0xd8, 0xbc, 0xbb, 0xb2, 0xb5,
0xbd, 0xba, 0xb3, 0xb4, 0xd0, 0xd7, 0xde, 0xd9, 0x66, 0x61,
0x68, 0x6f, 0x0b, 0x0c, 0x05, 0x02};

const unsigned char crc32_4[] = {
0x00, 0x77, 0xee, 0x99, 0x07, 0x70, 0xe9, 0x9e, 0x0e, 0x79,
0xe0, 0x97, 0x09, 0x7e, 0xe7, 0x90, 0x1d, 0x6a, 0xf3, 0x84,
0x1a, 0x6d, 0xf4, 0x83, 0x13, 0x64, 0xfd, 0x8a, 0x14, 0x63,
0xfa, 0x8d, 0x3b, 0x4c, 0xd5, 0xa2, 0x3c, 0x4b, 0xd2, 0xa5,
0x35, 0x42, 0xdb, 0xac, 0x32, 0x45, 0xdc, 0xab, 0x26, 0x51,
0xc8, 0xbf, 0x21, 0x56, 0xcf, 0xb8, 0x28, 0x5f, 0xc6, 0xb1,
0x2f, 0x58, 0xc1, 0xb6, 0x76, 0x01, 0x98, 0xef, 0x71, 0x06,
0x9f, 0xe8, 0x78, 0x0f, 0x96, 0xe1, 0x7f, 0x08, 0x91, 0xe6,
0x6b, 0x1c, 0x85, 0xf2, 0x6c, 0x1b, 0x82, 0xf5, 0x65, 0x12,
0x8b, 0xfc, 0x62, 0x15, 0x8c, 0xfb, 0x4d, 0x3a, 0xa3, 0xd4,
0x4a, 0x3d, 0xa4, 0xd3, 0x43, 0x34, 0xad, 0xda, 0x44, 0x33,
0xaa, 0xdd, 0x50, 0x27, 0xbe, 0xc9, 0x57, 0x20, 0xb9, 0xce,
0x5e, 0x29, 0xb0, 0xc7, 0x59, 0x2e, 0xb7, 0xc0, 0xed, 0x9a,
0x03, 0x74, 0xea, 0x9d, 0x04, 0x73, 0xe3, 0x94, 0x0d, 0x7a,
0xe4, 0x93, 0x0a, 0x7d, 0xf0, 0x87, 0x1e, 0x69, 0xf7, 0x80,
0x19, 0x6e, 0xfe, 0x89, 0x10, 0x67, 0xf9, 0x8e, 0x17, 0x60,
0xd6, 0xa1, 0x38, 0x4f, 0xd1, 0xa6, 0x3f, 0x48, 0xd8, 0xaf,
0x36, 0x41, 0xdf, 0xa8, 0x31, 0x46, 0xcb, 0xbc, 0x25, 0x52,
0xcc, 0xbb, 0x22, 0x55, 0xc5, 0xb2, 0x2b, 0x5c, 0xc2, 0xb5,
0x2c, 0x5b, 0x9b, 0xec, 0x75, 0x02, 0x9c, 0xeb, 0x72, 0x05,
0x95, 0xe2, 0x7b, 0x0c, 0x92, 0xe5, 0x7c, 0x0b, 0x86, 0xf1,
0x68, 0x1f, 0x81, 0xf6, 0x6f, 0x18, 0x88, 0xff, 0x66, 0x11,
0x8f, 0xf8, 0x61, 0x16, 0xa0, 0xd7, 0x4e, 0x39, 0xa7, 0xd0,
0x49, 0x3e, 0xae, 0xd9, 0x40, 0x37, 0xa9, 0xde, 0x47, 0x30,
0xbd, 0xca, 0x53, 0x24, 0xba, 0xcd, 0x54, 0x23, 0xb3, 0xc4,
0x5d, 0x2a, 0xb4, 0xc3, 0x5a, 0x2d};

//void create_crc_table()
//{
//	int i, j;
//	//unsigned short crc16_t;
//	unsigned char crc16[2];
//	unsigned char crc32[4];
//
//	for (i = 0; i < 256; i++)
//	{
//		//crc16 = i;
//		//crc16_t = i;
//		crc16[0] = i;
//		crc16[1] = 0;
//		for (j = 0; j < 8; j++)
//		{
//			if(crc16[0] & 0x01)
//			{
//				//crc16 = (crc16 >> 1) ^ 0xA001;
//				//crc16_t = (crc16_t >> 1) ^ 0xA001;
//				crc16[0] = (crc16[0] >> 1) ^ 0x01;
//				crc16[1] = crc16[1] ^ 0xa0;
//			}
//			else
//			{
//				//crc16 >>= 1;
//				//crc16_t >>= 1;
//				//crc16[0] = ((crc16[0] >> 1) | (crc16[1] << 7));
//				crc16[0] = crc16[0] >> 1;
//			}
//		}
//		memcpy(crc16_table[i], crc16, 2);
//		//crc16_table_t[i] = crc16_t;
//	}
//	for (i = 0; i < 256; i++)
//	{
//		//crc32 = i;
//		crc32[0] = i & 0x000000FF;
//		crc32[1] = (i >> 8) & 0x000000FF;
//		crc32[2] = crc32[3] = 0;
//		for (j = 0; j < 8; j++)
//		{
//			if (crc32[0] & 0x1)
//			{
//				//crc32 = (crc32 >> 1) ^ 0xEDB88320;
//				crc32[0] = ((crc32[0] >> 1) | (crc32[1] << 7)) ^ 0x20;
//				crc32[1] = ((crc32[1] >> 1) | (crc32[2] << 7)) ^ 0x83;
//				crc32[2] = ((crc32[2] >> 1) | (crc32[3] << 7)) ^ 0xB8;
//				crc32[3] = (crc32[3] >> 1) ^ 0xED;
//			}
//			else
//			{
//				//crc32 >>= 1;
//				crc32[0] = ((crc32[0] >> 1) | (crc32[1] << 7));
//				crc32[1] = ((crc32[1] >> 1) | (crc32[2] << 7));
//				crc32[2] = ((crc32[2] >> 1) | (crc32[3] << 7));
//				crc32[3] = (crc32[3] >> 1);
//			}
//		}
//		//crc32_table[i] = crc32;
//		memcpy(crc32_table[i], crc32, 4);
//	}
//	//printf("crc16 %x crc16_t %x\n", *(short*)crc16_table[2], crc16_table_t[2]);
//	table_inited = 1;
//
//}

//short get_crc16_code(char *src, int len)
//{
//	unsigned short crc16;
//	unsigned int i;
//
//	if(src == NULL || len <= 0)
//	{
//		return 0;
//	}
//
//	if(table_inited == 0)
//		create_crc_table();
//
//	crc16 = 0x0000;
//    for(i=0; i<len; i++)
//	{
//		printf("get %x crc16>>8 %x table[%x] %x\n", crc16, (crc16 >> 8), (crc16 & 0xFF), crc16_table_t[(crc16 & 0xFF) ^ src[i]]);
//		crc16 = (crc16 >> 8) ^ crc16_table_t[(crc16 & 0xFF) ^ src[i]];
//
//	}
//
//	return crc16;
//
//}

int modbus_crc16(const unsigned char *puchMsg,int usDataLen, unsigned char *crc)
{
    if(puchMsg == NULL || usDataLen <= 0 || crc == NULL)
    {
        return -1;
    }
	crc[0] = crc[1] = 0xFF ; /* 高CRC字节初始化 */ /* 低CRC 字节初始化 */
	int uIndex ; /* CRC循环中的索引 */
    while (usDataLen--) /* 传输消息缓冲区 */
    {
                uIndex = crc[0] ^ *puchMsg++ ; /* 计算CRC */
                crc[0] = crc[1] ^ crc16_1[uIndex] ;
                crc[1] = crc16_2[uIndex] ;
    }
    return 0;
}

int make_crc16_code(const unsigned char *data,int len, unsigned char *crc16)
{
    if(data == NULL || len <= 0 || crc16 == NULL)
    {
        return -1;
    }
	crc16[0] = crc16[1] = 0x00; /* 高CRC字节初始化 */ /* 低CRC 字节初始化 */
	int index; /* CRC循环中的索引 */
    while (len--) /* 传输消息缓冲区 */
    {
                index = crc16[0] ^ *data++ ; /* 计算CRC */
                crc16[0] = crc16[1] ^ crc16_1[index] ;
                crc16[1] = crc16_2[index];
    }
    return 0;
}

//int get_crc32_code(char *src, int len)
//{
//	int crc32;
//	int i;
//
//	if(src == NULL || len <= 0)
//	{
//		return 0;
//	}
//
//	if(table_inited == 0)
//		create_crc_table();
//
//	crc32 = 0xffffffff;
//
//    for(i=0; i<len; i++)
//	{
//		crc32 = (crc32 >> 8) ^ crc32_table[(crc32 & 0xFF) ^ src[i]];
//	}
//	crc32 ^= 0xFFFFFFFF;
//
//	return crc32;
//}

int make_crc32_code(const unsigned char *data, int len, unsigned char *crc32)
{
    if(data == NULL || len <= 0 || crc32 == NULL)
    {
        return -1;
    }
    crc32[0] = crc32[1] = crc32[2] = crc32[3] = 0xFF;
    int index;
    while(len--)
    {
        index = crc32[0] ^ *data++;
        crc32[0] = crc32[1] ^ crc32_1[index];
        crc32[1] = crc32[2] ^ crc32_2[index];
        crc32[2] = crc32[3] ^ crc32_3[index];
        crc32[3] = crc32_4[index];
    }
    crc32[0] ^= 0xFF;
    crc32[1] ^= 0xFF;
    crc32[2] ^= 0xFF;
    crc32[3] ^= 0xFF;
    return 0;
}
//int check_crc16_encode_data(char *src_with_crc, int len)
//{
//	short crc16, crc_src;
//	int i;
//
//	if(src_with_crc == NULL || len <= 2)
//	{
//		return -1;
//	}
//
//	crc16 = get_crc16_code(src_with_crc, len - 2);
//
//	crc_src = *((short*)(src_with_crc + len - 2));
//
//	return crc16 == crc_src ? 0 : -1;
//
//}

bool verify_crc16_data(unsigned char *src, int len, const unsigned char *crc)
{
	char crc16[2];
	int i;

	if(src == NULL || len <= 0 || crc == NULL)
	{
		return false;
	}

	if(make_crc16_code(src, len, (unsigned char *)crc16) < 0)
	{
		return false;
	}

	if(memcpy(crc16, crc, 2) != 0)
	{
		return false;
	}

	return true;

}

//int check_crc32_encode_data(char *src_with_crc, int len)
//{
//	int crc32, crc_src;
//	int i;
//
//	if(src_with_crc == NULL || len <= 4)
//	{
//		return -1;
//	}
//
//	crc32 = get_crc32_code(src_with_crc, len - 4);
//
//	crc_src = *((int*)(src_with_crc + len - 4));
//
//	return crc32 == crc_src ? 0 : -1;
//}

bool verify_crc32_data(unsigned char *src, int len, const unsigned char *crc)
{
	char crc32[4];
	int i;

	if(src == NULL || len <= 0 || crc == NULL)
	{
		return false;
	}

	if(make_crc32_code(src, len, (unsigned char *)crc32) < 0)
	{
		return false;
	}

	if(memcmp(crc32, crc, 4) != 0)
	{
		return false;
	}

	return true;
}

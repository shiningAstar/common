
#include "crc.h"
#include "string.h"

unsigned char crc16_table[256][2];
//unsigned short crc16_table_t[256];
unsigned char crc32_table[256][4];
int table_inited = 0;

void create_crc_table()
{
	int i, j;
	//unsigned short crc16_t;
	unsigned char crc16[2];
	unsigned char crc32[4];

	for (i = 0; i < 256; i++)
	{
		//crc16 = i;
		//crc16_t = i;
		crc16[0] = i;
		crc16[1] = 0;
		for (j = 0; j < 8; j++)
		{
			if(crc16[0] & 0x01)
			{
				//crc16 = (crc16 >> 1) ^ 0xA001;
				//crc16_t = (crc16_t >> 1) ^ 0xA001;
				crc16[0] = (crc16[0] >> 1) ^ 0x01;
				crc16[1] = crc16[1] ^ 0xa0;
			}
			else
			{
				//crc16 >>= 1;
				//crc16_t >>= 1;
				//crc16[0] = ((crc16[0] >> 1) | (crc16[1] << 7));
				crc16[0] = crc16[0] >> 1;
			}
		}
		memcpy(crc16_table[i], crc16, 2);
		//crc16_table_t[i] = crc16_t;
	}
	for (i = 0; i < 256; i++)
	{
		//crc32 = i;
		crc32[0] = i & 0x000000FF;
		crc32[1] = (i >> 8) & 0x000000FF;
		crc32[2] = crc32[3] = 0;
		for (j = 0; j < 8; j++)
		{
			if (crc32[0] & 0x1)
			{
				//crc32 = (crc32 >> 1) ^ 0xEDB88320;
				crc32[0] = ((crc32[0] >> 1) | (crc32[1] << 7)) ^ 0x20;
				crc32[1] = ((crc32[1] >> 1) | (crc32[2] << 7)) ^ 0x83;
				crc32[2] = ((crc32[2] >> 1) | (crc32[3] << 7)) ^ 0xB8;
				crc32[3] = (crc32[3] >> 1) ^ 0xED;
			}
			else
			{
				//crc32 >>= 1;
				crc32[0] = ((crc32[0] >> 1) | (crc32[1] << 7));
				crc32[1] = ((crc32[1] >> 1) | (crc32[2] << 7));
				crc32[2] = ((crc32[2] >> 1) | (crc32[3] << 7));
				crc32[3] = (crc32[3] >> 1);
			}
		}
		//crc32_table[i] = crc32;
		memcpy(crc32_table[i], crc32, 4);
	}
	//printf("crc16 %x crc16_t %x\n", *(short*)crc16_table[2], crc16_table_t[2]);
	table_inited = 1;

}

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

bool make_crc16_code(const unsigned char *src, int len, unsigned char *crc)
{
	int i;
	unsigned char c[2];

	if(src == NULL || len <= 0 || crc == NULL)
	{
		return false;
	}

	if(table_inited == 0)
		create_crc_table();

	memset(crc, 0, 2);
    for(i=0; i<len; i++)
	{
		memcpy(c, crc, 2);
		//printf("make %x table[%x] %x\n", *(short*)crc, (unsigned char)(c[0]), *(short*)crc16_table[c[0] ^ src[i]]);
		crc[0] = c[1] ^ crc16_table[(unsigned char)(c[0] ^ src[i])][0];
		crc[1] = 0x00 ^ crc16_table[(unsigned char)(c[0] ^ src[i])][1];

		//crc16 = (crc16 >> 8) ^ crc16_table[(crc16 & 0xFF) ^ src[i]];
	}

	return true;
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

bool make_crc32_code(const unsigned char *src, int len, unsigned char *crc)
{
	int i;
	unsigned char c[4];

	if(src == NULL || len <= 0 || crc == NULL)
	{
		return false;
	}

	if(table_inited == 0)
		create_crc_table();

	memset(crc, 0, 4);
    for(i=0; i<len; i++)
	{
		memcpy(c, crc, 4);
		//printf("make %x table[%x] %x\n", *(short*)crc, (unsigned char)(c[0]), *(short*)crc16_table[c[0] ^ src[i]]);
		crc[0] = c[1] ^ crc32_table[(unsigned char)(c[0] ^ src[i])][0];
		crc[1] = c[2] ^ crc32_table[(unsigned char)(c[0] ^ src[i])][1];
		crc[2] = c[3] ^ crc32_table[(unsigned char)(c[0] ^ src[i])][2];
		crc[3] = 0x00 ^ crc32_table[(unsigned char)(c[0] ^ src[i])][3];

		//crc16 = (crc16 >> 8) ^ crc16_table[(crc16 & 0xFF) ^ src[i]];
	}

	return true;
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

	if(!make_crc16_code(src, len, (unsigned char *)crc16))
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

	if(!make_crc32_code(src, len, (unsigned char *)crc32))
	{
		return false;
	}

	if(memcmp(crc32, crc, 4) != 0)
	{
		return false;
	}

	return true;
}

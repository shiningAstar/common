#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED

/**

    所有相关函数的参数说明，同名参数功能相同，注释中所说字符串包含字符或者是其他数据类型

    *src:待校验字符串
    *len:待校验的字符长度,例如，要校验"1234567"，len=7
    *crc:生成的校验码
    *注意：make_crc16_code生成最终的校验码，vertify_crc16_data根据字符串直接生成最终校验校验传入的crc，二者配合使用
*/

/** */
int modbus_crc16(const unsigned char *puchMsg,int usDataLen, unsigned char *crc);



/**直接生成最终的crc 校验码*/
int make_crc16_code(const unsigned char *src, int len, unsigned char *crc);

/**直接生成生成crc 校验码*/
int make_crc32_code(const unsigned char *src, int len, unsigned char *crc);

/** 用于验证crc校验码是否正确，
    传入的src字符串应当为可生成最终crc校验码的全长字符串或最终字符串，en与src相关
    */
bool verify_crc16_data(unsigned char *src, int len, const unsigned char *crc);

/** 用于验证crc校验码是否正确*/
bool verify_crc32_data(unsigned char *src, int len, const unsigned char *crc);



/**
    *函数使用说明（以16位为例）：
    *   先用inti_crc16初始化crc16校验码，然后调用update_crc16_by_string，调用次数其情况而定，
        直到检验完所有数据，调用时，将初始化后的crc16传入到参数。校验完所有的数据后所得到的crc16就是最终的校验码
    *32crc 注意：在依照16位crc完成其生成步骤后，需要调用get_final_crc32来获取最终的crc32

    使用流程：crc16:  init_crc16  -->     update_crc16_by_string
                                            直到校验完所有数据
                crc32:  init_crc32  --> update_crc32_by_string  -->     get_final_crc32

*/

/** 初始化crc16,将其各个位设为0*/
bool init_crc16(unsigned char *crc16);

/** 初始化crc32，将其各个位设为1*/
bool init_crc32(unsigned char *crc32);

/**
    *功能：用于在原来CRC校验的的基础上跟新为新的校验码
    *描述：本函数用于在初始化并生成crc校验码后，源待校验字符串在其原来的基础上又增加了字符，
    此时，传入源字符串后拼接的后续字符串，拼接的字符串长度，已经生成的校验码，在此基础上生成
    新的校验码。
 */
int update_crc16_by_string(const unsigned char *src,int len,unsigned char * crc);

/** 功能及用途同上*/
int update_crc32_by_string(const unsigned char *src,int len,unsigned char * crc);


/**
    *功能：获取最后的32位Crc校验码
    *描述：16crc在计算完成后不需要其他处理，但是32位crc在最终生成时，需要取反，这里用来获得取反后的最终结果。
 */
bool get_final_crc32(unsigned char *crc32);

#endif // CRC_H_INCLUDED

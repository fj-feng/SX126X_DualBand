//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "uartString.h"

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------

//----------------------------------------------------------------
//                     Private Macros
//----------------------------------------------------------------

//----------------------------------------------------------------
//                    Private Typedefs
//----------------------------------------------------------------

//----------------------------------------------------------------
//                   Private Variables
//----------------------------------------------------------------
static S_CharData s_cmdData = { 0 };    // 字符串缓存

//----------------------------------------------------------------
//              Private Function Declarations
//----------------------------------------------------------------
static bool CheckNewlineSign( void );

//----------------------------------------------------------------
//                   Function Definitions
//----------------------------------------------------------------
/**
 * @brief 拷贝串口数据到字符串缓存
 * @param  data 数据首地址
 * @param  len 数据长度
 */
void CopyUartData( uint8_t* data, uint16_t len )
{
    uint16_t i       = 0;
    s_cmdData.length = 0;
    for( i = 0; i < len && i < CHARDATA_MAX_LEN; i++ )
    {
        if( data == NULL )
            return;
        s_cmdData.data[ i ] = (char) ( *data );
        s_cmdData.length++;
        data++;
    }
}

/**
 * @brief  检查回车换行标志
 * @return true 有回车换行标志
 * @return false 无回车换行标志
 */
static bool CheckNewlineSign( void )
{
    if( s_cmdData.data[ s_cmdData.length - 2 ] == '\r' && s_cmdData.data[ s_cmdData.length - 1 ] == '\n' )
        return true;
    else
        return false;
}

/**
 * @brief  指令解析,并返回指令类型，同时返回指令附带的数据内容和数据个数
 * @param  value 指令附带的数据内容首地址
 * @param  len 指令附带的数据内容个数
 * @return T_CommandList 指令类型
 */
T_CommandList DecodeCommand( uint32_t* value, uint16_t* len )
{
    T_CommandList command = C_None;

    *len                  = 0;

    /*检查回车换行*/
    if( CheckNewlineSign() == false )
        return command;

    /*数据长度为0则返回*/
    if( s_cmdData.length == 0 )
        return command;

    if( DecodeChar( value, len, "freq", false ) == true )
    {
        if( *len == 0 )
            command = C_readFreq;
        else
            command = C_setFreq;
    }
    else if( DecodeChar( value, len, "bw", false ) == true )
    {
        if( *len == 0 )
            command = C_readBw;
        else
            command = C_setBw;
    }
    else if( DecodeChar( value, len, "sf", false ) == true )
    {
        if( *len == 0 )
            command = C_readSf;
        else
            command = C_setSf;
    }
    else if( DecodeChar( value, len, "cr", true ) == true )
    {
        if( *len == 0 )
            command = C_readCr;
        else
            command = C_setCr;
    }
    else if( DecodeChar( value, len, "power", false ) == true )
    {
        if( *len == 0 )
            command = C_readPower;
        else
            command = C_setPower;
    }
    else if( DecodeChar( value, len, "set base params", false ) == true )
    {
        if( *len != 0 )
        {
            command = C_setBaseParams;
        }
    }
    else if( DecodeChar( value, len, "send preamble", false ) == true )
    {
        command = C_sendInfinitePreamble;
    }
    else if( DecodeChar( value, len, "preamble", false ) == true )
    {
        if( *len == 0 )
            command = C_readPreamble;
        else
            command = C_setPreamble;
    }
    else if( DecodeChar( value, len, "cad time", false ) == true )
    {
        if( *len == 0 )
            command = C_readCadTime;
        else
            command = C_setCadTime;
    }
    else if( DecodeChar( value, len, "enter p2p tx", false ) == true )
    {
        command = C_enterP2pTx;
    }
    else if( DecodeChar( value, len, "enter p2p rx", false ) == true )
    {
        command = C_enterP2pRx;
    }
    else if( DecodeChar( value, len, "send wave", false ) == true )
    {
        command = C_sendWave;
    }
    else if( DecodeChar( value, len, "enter cad", false ) == true )
    {
        command = C_enterCad;
    }
    else if( DecodeChar( value, len, "send wave sleep", false ) == true )
    {
        command = C_sendWaveWithMcuSleep;
    }
    else if( DecodeChar( value, len, "enter rx sleep", false ) == true )
    {
        command = C_enterRxWithMcuSleep;
    }
    else if( DecodeChar( value, len, "lora sleep", false ) == true )
    {
        command = C_LoRaEnterSleep;
    }
    else if( DecodeChar( value, len, "mcu sleep", false ) == true )
    {
        command = C_mcuSleep;
    }
    else if( DecodeChar( value, len, "send data", true ) == true )
    {
        command = C_sendSingleData;
    }
    else if( DecodeChar( value, len, "enter rx", false ) == true )
    {
        command = C_enterRx;
    }
    else if( DecodeChar( value, len, "enter standby", false ) == true )
    {
        command = C_enterStandby;
    }
    else if( DecodeChar( value, len, "set p2p tx params", false ) == true )
    {
        command = C_setP2pTxParams;
    }
    else if( DecodeChar( value, len, "enter per", false ) == true )
    {
        command = C_setPER;
    }
    else if( DecodeChar( value, len, "set lowdropt", false ) == true )
    {
        command = C_setLowDrOpt;
    }
    else if( DecodeChar( value, len, "cap", false ) == true )
    {
        command = C_Cap;
    }

    return command;
}

/**
 * @brief  解析字符串，并返回解析结果
 * @param  value 指令附带的数据内容首地址
 * @param  len 指令附带的数据内容个数
 * @param  cmd 解析的指令内容
 * @param  hexData 附带数据是否是HEX格式书写
 * @return true 指令正确
 * @return false 指令错误
 */
bool DecodeChar( uint32_t* value, uint16_t* len, char* cmd, bool hexData )
{
    char*    p;
    char     str[ 20 ] = { 0 };    // 用于临时存放数据
    uint8_t  i         = 0;
    uint32_t data      = 0;
    uint16_t dataLen   = s_cmdData.length;

    /*如果字符串长度小于指令长度，则返回错误*/
    if( s_cmdData.length < strlen( cmd ) )
        return false;

    /*获取字符串中对应指令的起始位置*/
    p = strstr( s_cmdData.data, cmd );

    /*空指针表示字符串中没有找到对应的指令*/
    if( p == NULL )
        return false;

    /*将位置便宜到对应指令后，同时计算剩余字符串的长度*/
    p += ( strlen( cmd ) );
    dataLen -= ( strlen( cmd ) );

    /*解析指令后附带的数据内容，如果最后剩余回车换行或者剩余字节为0则停止解析*/
    while( ( *p != '\r' && *( p + 1 ) != '\n' ) && dataLen != 0 )
    {
        if( *p == ' ' )
        {
            p++;
            dataLen--;
            continue;
        }
        else
        {
            str[ i++ ] = *p++;
            dataLen--;
        }

        /*空格用于分隔数据*/
        if( *p == ' ' || ( *p == '\r' && *( p + 1 ) == '\n' ) )
        {
            /*根据附带数据是否是HEX书写方式进行对应的转换*/
            if( hexData == true )
                data = strtol( str, NULL, 16 );
            else
                data = atoi( str );
            /*若数据内容溢出则返回错误，反之保存*/
            if( data <= 0xFFFFFFFF )
            {
                *value++ = data;
                ( *len )++;
            }
            else
                return false;
            i = 0;
            memset( str, 0, 20 );
        }
    }
    return true;
}

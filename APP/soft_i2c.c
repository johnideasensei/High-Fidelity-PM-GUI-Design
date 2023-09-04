
#include "soft_i2c.h"
#include "main.h"


/*Ӳ��I2C*/
void I2CMstInit(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTA, PIN1, PORTA_PIN1_I2C0_SCL, 1);
	PORTN->OPEND |= (1 << PIN1);	// ��©
	PORTN->PULLU |= (1 << PIN1);	// ����
	PORT_Init(PORTA, PIN0, PORTA_PIN0_I2C0_SDA, 1);
	PORTN->OPEND |= (1 << PIN0);
	PORTN->PULLU |= (1 << PIN0);
	
	I2C_initStruct.Master = 1;
	I2C_initStruct.MstClk = 100000;
	I2C_initStruct.Addr10b = 0;
	I2C_initStruct.TXEmptyIEn = 0;
	I2C_initStruct.RXNotEmptyIEn = 0;
	I2C_Init(I2C0, &I2C_initStruct);
	
	I2C_Open(I2C0);
}


void IIC_Init(void)
{
#if SOFT
	GPIO_Init(GPIOA, PIN0, 1, 1, 0, 1);//����
	GPIO_Init(GPIOA, PIN1, 1, 1, 0, 1);
#else
	I2CMstInit();
#endif
}
	

	
	
/**
 * @brief ����start�ź�
 * 
 */
void i2c_start(void)
{
    CT_SDA_H;
    CT_SCL_H;
    swm_delay_us(I2C_DELAY);
    CT_SDA_L;
    swm_delay_us(I2C_DELAY);
    CT_SCL_L;
    swm_delay_us(I2C_DELAY);
}

/**
 * @brief ����stop�ź�
 * 
 */
void i2c_stop(void)
{
    CT_SDA_L;
    swm_delay_us(I2C_DELAY);
    CT_SCL_H;
    swm_delay_us(I2C_DELAY);
    CT_SDA_H;
    swm_delay_us(I2C_DELAY);

}

/**
 * @brief ����ackӦ���ź�
 * 
 * @param ack 0:��ack�ź� 1:��ack�ź�
 */
void i2c_ack(uint8_t ack)
{
    if (ack)
    {
        CT_SDA_H;
    }
    else
    {
        CT_SDA_L;
    }
    swm_delay_us(I2C_DELAY);
    CT_SCL_H;
    swm_delay_us(I2C_DELAY);
    CT_SCL_L;
    swm_delay_us(I2C_DELAY);
}

/**
 * @brief �ȴ�������ackӦ���ź�
 * 
 * @return uint8_t 0:��ack�ź� 1:��ack�ź�
 */
uint8_t i2c_waitack(void)
{
    uint8_t err_time = 0;

    SET_SDA_READ;
    while (READ_SDA)
    {
        err_time++;
        if (err_time > 200)
        {
					SET_SDA_WRITE;
            return 1;
        }
    }
    CT_SCL_H;
    swm_delay_us(I2C_DELAY);
    CT_SCL_L;
    swm_delay_us(I2C_DELAY);
    SET_SDA_WRITE;
    
    return 0;
}

/**
 * @brief д����
 * 
 * @param data ����
 */
void i2c_write_byte(uint8_t data)
{
    for (uint32_t i = 0; i < 8; i++) //д�ֽ�
    {
        CT_SCL_L;
        swm_delay_us(I2C_DELAY);
        if (data & 0x80)
        {
            CT_SDA_H;
        }
        else
        {
            CT_SDA_L;
        }
        swm_delay_us(I2C_DELAY);
        CT_SCL_H;
        swm_delay_us(I2C_DELAY);
        data <<= 1;
    }
    CT_SCL_L;
    swm_delay_us(I2C_DELAY);
}

/**
 * @brief ������
 * 
 * @return uint8_t ����
 */
uint8_t i2c_read_byte(void)
{
    uint8_t value = 0;

    SET_SDA_READ;
    for (uint32_t i = 0; i < 8; i++)
    {
        value <<= 1;
        CT_SCL_H;
        swm_delay_us(I2C_DELAY);
        if (READ_SDA)
            value++;
        CT_SCL_L;
        swm_delay_us(I2C_DELAY);
    }
    SET_SDA_WRITE;
    return value;
}






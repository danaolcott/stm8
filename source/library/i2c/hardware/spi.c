/*
4/5/20 - 7/20
SPI Controller File - modify for use with the
STM standard library.  
Pinout: 
PA6 - SPI1_CS
PC6 - SPI1_SCK
PA3 - SPI1_MOSI
Note: SPI1 Miso pin not needed or defined.



*/

#include "main.h"
#include "spi.h"


///////////////////////////////////////////////
//Configure SPI1 on PA6, PC6, PA3.  Assumes
//pins are remapped and SPI1 clock is enabled.
void spi_init(void)
{
    //PA6 - SPI1_CS - pushpull output, initially high
    GPIO_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);

    //PC6 - SPI1_SCK - output push-pull
    GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_SetBits(GPIOC, GPIO_Pin_6);
        
    //PA3 - SPI1_MOSI - output push pull
    GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
    
    //configure the spi as master, msb first....
    SPI_DeInit(SPI1);
    
    SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_8, SPI_Mode_Master, SPI_CPOL_Low,
        SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0);
        
    SPI_Cmd(SPI1, ENABLE);
}

void spi_select(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_6);
}

void spi_deselect(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
}

uint8_t spi_tx(uint8_t data)
{
    uint8_t result = 0x00;
    
    SPI_SendData(SPI1, data);    
    while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE));
    while (!SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE));
    result = SPI_ReceiveData(SPI1);
    
    return result;
}

uint8_t spi_writeByte(uint8_t data)
{
    uint8_t result = 0x00;
    spi_select();
    result = spi_tx(data);
    spi_deselect();
    return result;
}

void spi_writeArray(uint8_t *data, uint16_t length)
{
    uint16_t i = 0;
    uint8_t result = 0;

    spi_select();
    for (i = 0 ; i < length ; i++)
    {
        result = spi_tx(data[i]);
    }

    spi_deselect();

}



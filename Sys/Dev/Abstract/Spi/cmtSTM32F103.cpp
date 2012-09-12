//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#ifdef CMT_SPI2_MAP
CmtSpiBase*
CmtSpiBase::GetPort2() {
  static CmtSpiBase *port = 0;
  if( port == 0 ) {
    //Настроить
#if CMT_SPI2_MAP == 0
    //Разрешить необходимые устройства
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
    //Ножки SPI
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = MBIT13 | MBIT15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = MBIT14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //SPI для доступа к драйверам
    /* SPIy Config -------------------------------------------------------------*/
    SPI_InitTypeDef   SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init( SPI2, &SPI_InitStructure);
  
    /* Enable SPIy */
    SPI_Cmd(SPI2, ENABLE);

    port = new CmtSpi( SPI2 );
#elif CMT_SPI2_MAP == 1
    //Разрешить необходимые устройства
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
    //Ножки SPI
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = MBIT13 | MBIT15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = MBIT14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //SPI для доступа к драйверам
    /* SPIy Config -------------------------------------------------------------*/
    SPI_InitTypeDef   SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init( SPI2, &SPI_InitStructure);
  
    /* Enable SPIy */
    SPI_Cmd(SPI2, ENABLE);

    port = new CmtSpi( SPI2 );
#else
  #error "CMT: map not defined for this CPU"
#endif
    }
  return port;
  } 

#endif


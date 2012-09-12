//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

uint_8
CmtI2cMaster::WriteBlock(cpvoid src, uint32 count, uint32 *pBreak) {
  uint8 *ptr = (uint8*)src;
  /*!< While the bus is busy */
  uint32 sEETimeout = 10000;
  while(I2C_GetFlagStatus(mI2c, I2C_FLAG_BUSY))
    if((sEETimeout--) == 0) return CMTE_TIME_OUT;

  /*!< Send START condition */
  I2C_GenerateSTART( mI2c, ENABLE );

  /*!< Test on EV5 and clear it */
  sEETimeout = 0x1000;
  while(!I2C_CheckEvent(mI2c, I2C_EVENT_MASTER_MODE_SELECT))
    if((sEETimeout--) == 0) return CMTE_TIME_OUT;
  
  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress( mI2c, mMasterAddr << 1, I2C_Direction_Transmitter );

  /*!< Test on EV6 and clear it */
  sEETimeout = 0x1000;
  while(!I2C_CheckEvent(mI2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    if((sEETimeout--) == 0) return CMTE_TIME_OUT;
  
  while( count-- ) {
    /*!< Send the EEPROM's internal address to write to : only one byte Address */
    I2C_SendData( mI2c, *ptr++ );

    /*!< Test on EV8 and clear it */
    sEETimeout = 0x1000; 
    while(!I2C_CheckEvent(mI2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
      if((sEETimeout--) == 0) return CMTE_TIME_OUT;
      }  
    }
  
  /*!< STOP condition */    
  I2C_GenerateSTOP(mI2c, ENABLE);

  return CMTE_OK;
  }

uint16 i2cDummi;

uint_8
CmtI2cMaster::ReadBlock(pvoid dest, uint32 count, uint32 *pBreak) {
  if( dest == 0 || count == 0 ) return CMTE_BAD_PARAM;
  uint8 *ptr = (uint8*)dest;
  /*!< While the bus is busy */
  uint32 sEETimeout = 10000;
  while(I2C_GetFlagStatus(mI2c, I2C_FLAG_BUSY))
    if((sEETimeout--) == 0) return CMTE_TIME_OUT;

  /*!< Send START condition */
  I2C_GenerateSTART( mI2c, ENABLE );

  /*!< Test on EV5 and clear it */
  sEETimeout = 0x1000;
  while(!I2C_CheckEvent( mI2c, I2C_EVENT_MASTER_MODE_SELECT))
    if((sEETimeout--) == 0) return CMTE_TIME_OUT;
  

  /*!< Send EEPROM address for read */
  I2C_Send7bitAddress( mI2c, mMasterAddr << 1, I2C_Direction_Receiver);  

  /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
  sEETimeout = 0x1000;
  while(I2C_GetFlagStatus( mI2c, I2C_FLAG_ADDR) == RESET)
    if((sEETimeout--) == 0) return CMTE_TIME_OUT;

  /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
  i2cDummi = mI2c->SR2;

  while( count > 1 ) {
    
    /* Wait for the byte to be received */
    sEETimeout = 0x1000;
    while( I2C_GetFlagStatus( mI2c, I2C_FLAG_RXNE) == RESET)
      if((sEETimeout--) == 0) return CMTE_TIME_OUT;
    
    /*!< Read the byte received from the EEPROM */
    *ptr++ = I2C_ReceiveData( mI2c );
    
    /*!< Decrement the read bytes counter */
    count--;

    }
  
  /*!< Disable Acknowledgement */
  I2C_AcknowledgeConfig( mI2c, DISABLE);   

  /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
//  (void)mI2c->SR2;
  
  /*!< Send STOP Condition */
  I2C_GenerateSTOP( mI2c, ENABLE);
 
  /* Wait for the byte to be received */
  sEETimeout = 0x1000;
  while( I2C_GetFlagStatus( mI2c, I2C_FLAG_RXNE) == RESET)
    if((sEETimeout--) == 0) return CMTE_TIME_OUT;
  
  /*!< Read the byte received from the EEPROM */
  *ptr++ = I2C_ReceiveData( mI2c );
  
  /*!< Decrement the read bytes counter */
  count--;
  
  /* Wait to make sure that STOP control bit has been cleared */
  sEETimeout = 0x1000;
  while( mI2c->CR1 & I2C_CR1_STOP )
    if((sEETimeout--) == 0) return CMTE_TIME_OUT;
  
  /*!< Re-Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig( mI2c, ENABLE);    
  return CMTE_OK;
  }

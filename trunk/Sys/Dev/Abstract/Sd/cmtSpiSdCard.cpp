//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#define MMC_DATA_SIZE 512 /* 16-bit in size, 512 bytes */
#define MAX_TIMEOUT 0xFF
#define IDLE_STATE_TIMEOUT 1
#define OP_COND_TIMEOUT 2
#define SET_BLOCKLEN_TIMEOUT 3
#define WRITE_BLOCK_TIMEOUT 4
#define WRITE_BLOCK_FAIL 5
#define READ_BLOCK_TIMEOUT 6
#define READ_BLOCK_DATA_TOKEN_MISSING 7
#define DATA_TOKEN_TIMEOUT 8
#define SELECT_CARD_TIMEOUT 9
#define SET_RELATIVE_ADDR_TIMEOUT 10

CmtSpiSdCard::CmtSpiSdCard( CmtSpiPipeBase *spi, CmtSdIsConnect isConnect, int sdFactor, uint32 blockSize, uint32 blockCount ) :
  CmtBlockDevice( blockSize, blockCount ),
  mSpi(spi),
  fnIsConnect(isConnect),
  mSdFactor(sdFactor) {
    //Устройство должно быть инициализировано
    mStatus = CMTE_OK;
    }

uint_8 
CmtSpiSdCard::Read( uint32 *dest, uint32 block, uint32 count ) {
  uint_8 res = CMTE_OK;
  checkLock( CMTE_BD_NO_DEV );
  while( count-- ) {
    res = ReadBlock( dest, block );
    if( res != CMTE_OK ) break;
    //Переходим к следующему блоку
    dest += mBlockSize >> 2;
    block++;
    }
  devUnLock();
  return res;
  }

uint_8 
CmtSpiSdCard::Write( const uint32 *src, uint32 block, uint32 count ) {
  uint_8 res = CMTE_OK;
  checkLock( CMTE_BD_NO_DEV );
  while( count-- ) {
    res = WriteBlock( src, block );
    if( res != CMTE_OK ) break;
    //Переходим к следующему блоку
    src += mBlockSize >> 2;
    block++;
    }
  devUnLock();
  return res;
  }

uint_8 
CmtSpiSdCard::ReadBlock( uint32 *dest, uint32 block ) {
  uint_16 Checksum;
  CCHLOCK(mSpi,CMTE_FAIL);
  mSpi->Select(); /* clear SPI SSEL */
  block <<= mSdFactor; // sector <<= 9;
  MMCCmd[0] = 0x51;
  MMCCmd[1] = block >> 24;
  MMCCmd[2] = block >> 16;
  MMCCmd[3] = block >> 8;
  MMCCmd[4] = block;

  /* checksum is no longer required but we always send 0xFF */
  MMCCmd[5] = 0xFF;
  mSpi->WriteBlock( MMCCmd, MMC_CMD_SIZE, mBreak );
  /* if mmc_response returns 1 then we failed to get a 0x00 response */
  if( Response( mSpi, 0x00 ) != CMTE_OK ) {
    mSpi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(mSpi);
    return CMTE_SD_READ_BLOCK_TIMEOUT;
    }
  /* wait for data token */
  if( Response( mSpi, 0xFE ) != CMTE_OK ) {
    mSpi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(mSpi);
    return CMTE_SD_BLOCK_DATA_TOKEN_MISSING;
    }
  /* Get the block of data based on the length */
  memset( dest, 0xff, MMC_DATA_SIZE );
  mSpi->ReadBlock( dest, MMC_DATA_SIZE, mBreak );
  /* CRC bytes that are not needed */
  Checksum = 0xffff;
  mSpi->Transfer( &Checksum, &Checksum, 2, mBreak );
  mSpi->UnSelect(); /* set SPI SSEL */
  Checksum = 0xffff;
  mSpi->WriteBlock( &Checksum, 1, mBreak );
  CUNLOCK(mSpi);
  if( mBreak && mBreak->IsSignaled() ) return CMTE_BREAK;
  return fnIsConnect();
  }

uint_8 
CmtSpiSdCard::WriteBlock( const uint32 *src, uint32 block ) {
  CCHLOCK(mSpi,CMTE_FAIL);
  mSpi->Select(); /* clear SPI SSEL */
  block <<= mSdFactor; // sector <<= 9;
  /* block size has been set in mmc_init() */
  MMCCmd[0] = 0x58;
  MMCCmd[1] = block >> 24;
  MMCCmd[2] = block >> 16;
  MMCCmd[3] = block >> 8;
  MMCCmd[4] = block;

  /* checksum is no longer required but we always send 0xFF */
  MMCCmd[5] = 0xFF;
  mSpi->WriteBlock( MMCCmd, MMC_CMD_SIZE, mBreak );
  /* if mmc_response returns 1 then we failed to get a 0x00 response */
  if( Response( mSpi, 0x00 ) != CMTE_OK ) {
    mSpi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(mSpi);
    return CMTE_SD_WRITE_BLOCK_TIMEOUT;
    }

  /* Set bit 0 to 0 which indicates the beginning of the data block */
  MMCCmd[0] = 0xfe;
  mSpi->Transfer( MMCCmd, 0, 1, mBreak );
  //MMCCmd[0] = 0xFE;
  //cmtSpiWriteBlock( CMT_SD->mSpi, MMCCmd, 1 );

  /* send data, pattern as 0x00,0x01,0x02,0x03,0x04,0x05 ...*/
  mSpi->WriteBlock( src, MMC_DATA_SIZE, mBreak );

  /* Send dummy checksum */
  /* when the last check sum is sent, the response should come back
  immediately. So, check the SPI FIFO MISO and make sure the status
  return 0xX5, the bit 3 through 0 should be 0x05 */
  MMCCmd[0] = 0xFF;
  MMCCmd[1] = 0xFF;
  mSpi->WriteBlock( MMCCmd, 2, mBreak );
  MMCCmd[0] = 0;
  mSpi->Transfer( MMCCmd, MMCCmd, 1, mBreak );
  if( (MMCCmd[0] & 0x0F) != 0x05 ) {
    mSpi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(mSpi);
    return CMTE_SD_WRITE_BLOCK_FAIL;
    }
  /* if the status is already zero, the write hasn't finished
  yet and card is busy */

  if( WaitForWriteFinish() != CMTE_OK ) {
    mSpi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(mSpi);
    return CMTE_SD_WRITE_BLOCK_FAIL;
    }
  mSpi->UnSelect(); /* set SPI SSEL */
  MMCCmd[0] = 0xff;
  mSpi->WriteBlock( MMCCmd, 1, mBreak );
  CUNLOCK(mSpi);
  return fnIsConnect();
  }

uint_8
CmtSpiSdCard::WaitForWriteFinish() {
  uint_16 count = 0xFFFF; /* The delay is set to maximum considering the longest data block length to handle */
  uint8 dummi = 0;
  while( dummi == 0 && count ) {
    dummi = 0xff;
    mSpi->Transfer( &dummi, &dummi, 1, mBreak );
    if( mBreak && mBreak->IsSignaled() ) return CMTE_BREAK;
    count--;
    }
  if( count == 0 )
    return CMTE_FAIL; /* Failure, loop was exited due to timeout */
  else
    return CMTE_OK; /* Normal, loop was exited before timeout */
  }  

uint_8 
CmtSpiSdCard::Response( CmtSpiBase *spi, uint8 response ) {
  uint_16 count = 0xff;
  uint8 dummi[2];
  dummi[0] = 0xff;
  dummi[1] = response + 1;
  while( count-- && (dummi[1] != response) ) {
    spi->Transfer( dummi, dummi+1, 1, 0 );
    }
  if( dummi[1] != response )
    return CMTE_FAIL; /* Failure, loop was exited due to timeout */
  else
    return CMTE_OK; /* Normal, loop was exited before timeout */
  }  
  
CmtSpiSdCard* 
CmtSpiSdCard::CreateCard( CmtSpiPipeBase *spi, CmtSdIsConnect isConnect ) {
  if( isConnect() != CMTE_OK ) return 0; //Карта не подключена
  CCHLOCK(spi,0);  //SPI не доступно
  uint32 i;
  uint8 MMCCmd[16];
  uint8 dummi[2];

  spi->UnSelect(); /* set SPI SSEL */
  /* initialise the MMC card into SPI mode by sending 80 clks on */
  /* Use MMCRDData as a temporary buffer for SPI_Send() */
  for(i=0; i<10; i++) {
    MMCCmd[i] = 0xFF;
    }
  spi->WriteBlock( MMCCmd, 10, 0 );
  spi->Select(); /* clear SPI SSEL */

  /* send CMD0(RESET or GO_IDLE_STATE) command, all the arguments
  are 0x00 for the reset command, precalculated checksum */
  MMCCmd[0] = 0x40;
  MMCCmd[1] = 0x00;
  MMCCmd[2] = 0x00;
  MMCCmd[3] = 0x00;
  MMCCmd[4] = 0x00;
  MMCCmd[5] = 0x95;
  spi->WriteBlock( MMCCmd, MMC_CMD_SIZE, 0 );

  /* if = 1 then there was a timeout waiting for 0x01 from the MMC */
  if( Response(spi,0x01) != CMTE_OK ) {
//strcpy( (char*)gpsGeo, "            A" );
    spi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(spi);
    return 0;
    }
//strcpy( (char*)gpsGeo, "            U" );

  /* Send some dummy clocks after GO_IDLE_STATE */
  spi->UnSelect(); /* set SPI SSEL */
  MMCCmd[0] = 0xff;
  spi->WriteBlock( MMCCmd, 1, 0 );

  spi->Select(); /* clear SPI SSEL */

  /* must keep sending command until zero response ia back. */
  i = MAX_TIMEOUT;
  do {
    /* send mmc CMD1(SEND_OP_COND) to bring out of idle state */
    /* all the arguments are 0x00 for command one */
    MMCCmd[0] = 0x41;
    MMCCmd[1] = 0x00;
    MMCCmd[2] = 0x00;
    MMCCmd[3] = 0x00;
    MMCCmd[4] = 0x00;
    /* checksum is no longer required but we always send 0xFF */
    MMCCmd[5] = 0xFF;
    spi->WriteBlock( MMCCmd, MMC_CMD_SIZE, 0 );
    i--;
    } while ( (Response(spi,0x00) != CMTE_OK) && (i>0) );

  /* timeout waiting for 0x00 from the MMC */
  if ( i == 0 ) {
    //MMCStatus = OP_COND_TIMEOUT;
//strcpy( (char*)gpsGeo, "            B" );
    spi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(spi);
    return 0;
    }

  /* Send some dummy clocks after SEND_OP_COND */
  spi->UnSelect(); /* set SPI SSEL */
  MMCCmd[0] = 0xff;
  spi->WriteBlock( MMCCmd, 1, 0 );

  //Команда чтения CSD
  spi->Select(); /* clear SPI SSEL */

  /* send CMD9(SEND_CSD) command */
  MMCCmd[0] = 0x49;
  MMCCmd[1] = 0x00;
  MMCCmd[2] = 0x00;
  MMCCmd[3] = 0x00;
  MMCCmd[4] = 0x00;
  MMCCmd[5] = 0xFF;
  spi->WriteBlock( MMCCmd, MMC_CMD_SIZE, 0 );

  /* if = 1 then there was a timeout waiting for 0x01 from the MMC */
  if( Response(spi,0x00) != CMTE_OK ) {
//strcpy( (char*)gpsGeo, "            D" );
    spi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(spi);
    return 0;
    }
  
  /* wait for data token */
  if( Response( spi, 0xFE ) != CMTE_OK ) {
//strcpy( (char*)gpsGeo, "            E" );
    spi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(spi);
    return 0;
    }

  for(i=0; i<16; i++) {
    MMCCmd[i] = 0xff;
    }
  /* Get the block of data based on the length */
  
  spi->ReadBlock( MMCCmd, 16, 0 );
  /* CRC bytes that are not needed */
  dummi[0] = 0xff;
  dummi[1] = 0xff;
  spi->WriteBlock( dummi, 2, 0 );
//GPIO_SetBits( GPIOA, MBIT3 );
  spi->UnSelect(); /* set SPI SSEL */
  dummi[0] = 0;
  spi->WriteBlock( dummi, 1, 0 );

  //Анализ размера карты (определение количества блоков)
  uint32 blockCount; //Количество блоков по 512 байт
  int factor;
  //Регистр CSD MSB битами вперед
  //Биты  127-120  119-112 111-104  103-96  95-88  87-80  79-72  71-64  63-56  55-48  47-40  39-32  31-24  23-16  15-8  7-0
  //Байты    0        1       2        3      4      5      6      7      8      9      10     11     12     13     14   15
  if( (MMCCmd[0] & 0xc0) == 0 ) {
    //SD 1.0
    uint32 c_size;
    c_size = MMCCmd[6] & 0x3; //[73:62]
    c_size <<= 8;
    c_size += MMCCmd[7];
    c_size <<= 2;
    c_size += (MMCCmd[8] >> 6) & 0x3;
    
    uint32 c_size_mult; 
    c_size_mult = MMCCmd[9] & 0x3; //[49:47]
    c_size_mult <<= 1;
    c_size_mult += (MMCCmd[10] >> 7) & 1;
    
    uint32 read_bl_len;
    read_bl_len = MMCCmd[5] & 0xf; //[83:80]
    
    read_bl_len -= 9; //Перейти к блокам
    
    blockCount = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << read_bl_len);
    factor = 9;
    }
  else if( (MMCCmd[0] & 0xc0) == 1 ) {
    //SD 2.0
    uint32 c_size;
    c_size = MMCCmd[7] & 0x3f; //[69:48]
    c_size <<= 8;
    c_size += MMCCmd[8];
    c_size <<= 8;
    c_size += MMCCmd[9];
    
    //Количество блоков
    blockCount = c_size << 10;
    factor = 0;
    }
  else {
/*strcpy( (char*)gpsGeo, "             " );
cmtSPrintHexUpper( (char*)(gpsGeo + 8), MMCCmd[0], 2 );
cmtSPrintHexUpper( (char*)(gpsGeo + 10), MMCCmd[1], 2 );
cmtSPrintHexUpper( (char*)(gpsGeo + 12), MMCCmd[2], 2 );
cmtSPrintHexUpper( (char*)(gpsGeo + 14), MMCCmd[3], 2 );*/
    //Ошибка определения размера диска
    CUNLOCK(spi);
    return 0;
    }
  
  spi->Select(); /* clear SPI SSEL */

  /* send MMC CMD16(SET_BLOCKLEN) to set the block length */
  MMCCmd[0] = 0x50;
  MMCCmd[1] = 0x00; /* 4 bytes from here is the block length */
                    /* LSB is first */
                    /* 00 00 00 10 set to 16 bytes */
                    /* 00 00 02 00 set to 512 bytes */
  MMCCmd[2] = 0x00;

  /* high block length bits - 512 bytes */
  MMCCmd[3] = 0x02;

  /* low block length bits */
  MMCCmd[4] = 0x00;

  /* checksum is no longer required but we always send 0xFF */
  MMCCmd[5] = 0xFF;
  spi->WriteBlock( MMCCmd, MMC_CMD_SIZE, 0 );

  if( Response(spi,0x00) != CMTE_OK ) {
//strcpy( (char*)gpsGeo, "            F" );
    //MMCStatus = SET_BLOCKLEN_TIMEOUT;
    spi->UnSelect(); /* set SPI SSEL */
    CUNLOCK(spi);
    return 0;
    }
  /* Send some dummy clocks after SET_BLOCKLEN */
  spi->UnSelect(); /* set SPI SSEL */
  dummi[0] = 0xff;
  spi->WriteBlock( dummi, 1, 0 );
  CUNLOCK(spi);

  //Построить объект карты  
  return new CmtSpiSdCard( spi, isConnect, factor, 512, blockCount );
  }


CmtSpiSdDevPolling::CmtSpiSdDevPolling( CmtSpiPipeBase *spi, CmtSdIsConnect  isConnect ) :
  CmtDevPolling(),
  mSpi(spi),
  fnIsConnect(isConnect),
  mSdCard(0) {
  
  }

void
CmtSpiSdDevPolling::Poll() {
  if( fnIsConnect() == CMTE_OK ) {
    //Карта подключена, проверить структуру SD
    if( mSdCard == 0 ) {
      //Структуры нет, формируем
      cmtWaitTick( 100 );
      if( fnIsConnect() != CMTE_OK ) return;
      mSdCard = CmtSpiSdCard::CreateCard( mSpi, fnIsConnect );
      if( mSdCard ) OnConnect();
      }
    }
  else {
    //Подключения нет, проверить наличие структуры SD
    if( mSdCard ) {
      OnDisconnect();
      CDECREF( mSdCard );
      mSdCard = 0;
      }
    }
  }

#ifdef CMT_FS
//! Событие, выполняемое при подключении карты (карта mSdCard)
void
CmtSpiSdFsDevPolling::OnConnect() {
  //Карта создана, формировать логические ФС
  CmtFileSystemBase::CreateFileSystem( mSdCard, 0 );
  }
  
  //! Событие, выполняемое при отключении карты
void
CmtSpiSdFsDevPolling::OnDisconnect() {
  //Убрать файловые системы, ассоциированные с диском
  int i;
  for( i = 0; i < CMT_FS; i++ )
    if( cmtLogDrives[i] && cmtLogDrives[i]->mDisk == mSdCard ) {
      //Файловая система от данного диска
      CDECREF( cmtLogDrives[i] );
      cmtLogDrives[i] = 0;
      }
  }
#endif

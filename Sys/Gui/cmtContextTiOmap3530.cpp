//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

uint32 CmtContextTiOmap3530::dmaIndex;
bool   CmtContextTiOmap3530::dmaActive;

void 
CmtContextTiOmap3530::FrameComplete() {
  //Установить для отображения данные контекста
  __raw_writel( (uint32)(GetData()), DISPC_GFX_BA0 );
  
  //Применить изменения (установка начала буфера отображения)
  __raw_setl( DISPC_CONTROL_GOLCD, DISPC_CONTROL );
  }

void 
CmtContextTiOmap3530::SetDmaChannel( uint32 index ) {
  //Предварительная настройка dma
  dmaIndex = index;
  dmaActive = false;

  //Запретить канал
  __raw_clearl( MBIT7, DMA4_CCR(dmaIndex) );

  //Сбросить флаг завершения транзакции
  __raw_writel( 0x1ffe, DMA4_CSR(dmaIndex) );
  
  __raw_writel( 0, DMA4_CLNK_CTRL(dmaIndex) );
  __raw_writel( 0, DMA4_CICR(dmaIndex) );

  //Настроить канал
  __raw_writel(
        MBIT(0,26) | //WRITE_PRIORITY Channel priority on the Write side
                     //  0x0: Channel has low priority on the Write side during the arbitration process
                     //  0x1: Channel has high priority on Write sided during the arbitration process
        MBIT(1,25) | //BUFFERING_DISABLE This bit allows to disable the default buffering functionality when transfer is source synchronized.
                     //  0x0: buffering is enable across element/packet when source is
                     //       synchronized to element, packet, frame or blocks
                     //  0x1: buffering is disabled across element/packet when source is
                     //       synchronized to element, packet, frame or blocks
        MBIT(0,24) | //SEL_SRC_DST_SYNC Specifies that element, packet, frame or block transfer (depending on CCR.bs and CCR.fs) 
                     //       is triggered by the source or the destination on the DMA request
                     //  0x0: Transfer is triggered by the destination. If synch on packet the
                     //       packet element number is specified in the CDFI register
                     //  0x1: Transfer is triggered by the source. If synchronized on packet
                     //       the packet element number is specified in the CSFI register
        MBIT(0,23) | //PREFETCH Enables the prefetch mode
                     //  0x0: Prefetch mode is disabled. When Sel_Src_Dst_Sync=1
                     //       transfers are buffered and pipelined between DMA requests
                     //  0x1: Prefetch mode is enabled. Prefetch mode is active only when
                     //       destination is synchronized. It is SW user responsibility not to have
                     //       at the same time Prefetch=1 when Sel_Src_Dst_Sync=1. This mode is not supported
        MBIT(0,22) | //SUPERVISOR Enables the supervisor mode
                     //  0x0: Supervisor mode is disabled
                     //  0x1: Supervisor mode is enabled
        MBIT(0,18) | //BS Block synchronization
                     //  This bit used with the fs to see how the DMA request is serviced in a synchronized transfer
        MBIT(0,17) | //TRANSPARENT_COPY_ENABLE Transparent copy enable
                     //  0x0: Transparent copy mode is disabled
                     //  0x1: Transparent copy mode is enabled
        MBIT(1,16) | //CONST_FILL_ENABLE Constant fill enable
                     //  0x0: Constant fill mode is disabled
                     //  0x1: Constant fill mode is enabled
        MBIT(3,14) | //DST_AMODE Selects the addressing mode on the Write Port of a channel.
                     //  0x0: Constant address mode
                     //  0x1: Post-incremented address mode
                     //  0x2: Single index address mode
                     //  0x3: Double index address mode
        MBIT(3,12) | //SRC_AMODE Selects the addressing mode on the Read Port of a channel.
                     //  0x0: Constant address mode
                     //  0x1: Post-incremented address mode
                     //  0x2: Single index address mode
                     //  0x3: Double index address mode
        MBIT(0,8) | //SUSPEND_SENSITIVE Logical channel suspend enable bit
                    //  0x0: The channel ignores the MSuspend even if EMUFree is set to 0.
                    //  0x1: If EMUFree is set to 0 and MSuspend comes in then all
                    //       current OCP services (single transaction or burst transaction as
                    //       specified in the corresponding CSDP register) have to be
                    //       completed before stopping processing any more transactions
        MBIT(0,7) | //ENABLE Logical channel enable. It is SW responsibility to clear the CSR
                    //       register and the IRQSTATUS bit for the different interrupt lines before enabling the channel.
                    //  0x0: The logical channel is disabled
                    //  0x1: The logical channel is enabled
        MBIT(0,6) | //READ_PRIORITY Channel priority on the read side
                    //  0x0: Channel has low priority on the Read side during the arbitration process
                    //  0x1: Channel has high priority on read sided during the arbitration process
        MBIT(0,5) | //FS Frame synchronization
                    //  This bit used with the BS to see how the DMA request is serviced in a synchronized transfer
                    //  FS=0 and BS=0: An element is transferred once a DMA request is made.
                    //  FS=0 and BS=1: An entire block is transferred once a DMA request is made.
                    //  FS=1 and BS=0: An entire frame is transferred once a DMA request is made.
                    //  FS=1 and BS=1: A packet is transferred once a DMA request is made.
                    //  All these different transfers can be interleaved on the port with other DMA requests.
        DMA4_CHANNEL_FIELD(0), //software sync
        DMA4_CCR(dmaIndex)
        );
  
  __raw_writel( 0, DMA4_CSR(dmaIndex) );
  
  __raw_writel(
        MBIT(0,21) | //SRC_ENDIAN Channel source endianness control
                     //  0x0: Source has Little Endian type
                     //  0x1: Source has Big Endian type
        MBIT(1,20) | //SRC_ENDIAN_LOCK Endianness Lock
                     //  0x0: Endianness adapt
                     //  0x1: Endianness lock
        MBIT(0,19) | //DST_ENDIAN Channel Destination endianness control
                     //  0x0: Destination has Little Endian type
                     //  0x1: Destination has Big Endian type
        MBIT(1,18) | //DST_ENDIAN_LOCK Endianness Lock
                     //  0x0: Endianness adapt
                     //  0x1: Endianness lock
        MBIT(0,16) | //WRITE_MODE Used to enable writing mode without posting or with posting
                     //  0x0: Write nonposted (WRNP)
                     //  0x1: Write (Posted)
                     //  0x2: All transaction are mapped on the Write command as posted
                     //       except for the last transaction in the transfer mapped on a Write nonposted
                     //  0x3: Undefined
        MBIT(3,14) | //DST_BURST_EN Used to enable bursting on the Write Port. Smaller burst size than RW the programmed burst size is also allowed
                     //  0x0: Single access
                     //  0x1: 16 bytes or 4x32-bit/2x64-bit burst access
                     //  0x2: 32 bytes or 8x32-bit/4x64-bit burst access
                     //  0x3: 64 bytes or 16x32-bit/8x64-bit burst access
        MBIT(1,13) | //DST_PACKED Destination receives packed data.
                     //  0x0: The destination target is nonpacked
                     //  0x1: The destination target is packed
        MBIT(3,7) | //SRC_BURST_EN Used to enable bursting on the Read Port. Smaller burst size than the programmed burst size is also allowed
                    //  0x0: Single access
                    //  0x1: 16 bytes or 4x32-bit/2x64-bit burst access
                    //  0x2: 32 bytes or 8x32-bit/4x64-bit burst access
                    //  0x3: 64 bytes or 16x32-bit/8x64-bit burst access
        MBIT(1,6) | //SRC_PACKED Source provides packed data.
                    //  0x0: The source target is nonpacked
                    //  0x1: The source target is packed
        MBIT(2,0),  //DATA_TYPE Defines the type of the data moved in the channel.
                    //  0x0: 8 bits scalar
                    //  0x1: 16 bits scalar
                    //  0x2: 32 bits scalar
                    //  0x3: Undefined
        DMA4_CSDP(dmaIndex)
        );
        
    //Сбросить флаг завершения транзакции
    __raw_writel( 0x1ffe, DMA4_CSR(dmaIndex) );

  }

//uint32 dma_status;
//uint32 dma_stat2;
void     
CmtContextTiOmap3530::WaitAccel() {
  if( dmaActive ) {
/*    do {
dma_status = __raw_readl( DMA4_CCR(dmaIndex) );
    } while( dma_status & MBIT7 );
dma_stat2 = __raw_readl( DMA4_CSR(dmaIndex) ); */
//    CmtEvSetBit32 ev( (uint32*)(DMA4_CSR(dmaIndex)), MBIT5 | MBIT11 | MBIT10 | MBIT8 );
    CmtEvClrBit32 ev( (uint32*)(DMA4_CCR(dmaIndex)), MBIT7 );
    ev.Wait();
//    cmtWaitTick(100);
//    dma_status = ev.Mask();
//    cmtWaitBreakSetBit( DMA4_CSR(dmaIndex), MBIT7 | MBIT5, 0 );
//    cmtWaitBreakSetBit( DMA4_CSR(dmaIndex), MBIT5, 0 );
//    dmaActive = false;
//dmaIndex = 0;
dmaActive = false;
//return;
    }
  }

void     
CmtContextTiOmap3530::Clear( uint32 color, bool set ) {
  FilledRect( CmtPoint(), mSize, color, set );
  }

void     
CmtContextTiOmap3530::FilledRect( CmtPoint a, CmtPoint b, uint32 color, bool set ) {
  if( dmaIndex && set && !dmaActive ) {
    //Вариант с ускорением

    //Ограничения
    Clip( a );
    Clip( b );
    int32 x,y;
    if( a.y > b.y ) {
      //Обмен координат
      y   = a.y;
      a.y = b.y;
      b.y = y;
      }
    if( a.x > b.x ) {
      //Обмен координат
      x = a.x;
      a.x = b.x;
      b.x = x;
      }
    
    uint32 dstAddr = (uint32)GetData(a);
    if( dstAddr == 0 || b.x == 0 || b.y == 0 || a.x == b.x || a.y == b.y ) return;
    
    //Запретить канал
    __raw_clearl( MBIT7, DMA4_CCR(dmaIndex) );
    
    //Сбросить флаг завершения транзакции
    __raw_writel( 0x1ffe, DMA4_CSR(dmaIndex) );
    
    __raw_writel(
          MBIT(0,26) | //WRITE_PRIORITY Channel priority on the Write side
                       //  0x0: Channel has low priority on the Write side during the arbitration process
                       //  0x1: Channel has high priority on Write sided during the arbitration process
          MBIT(0,25) | //BUFFERING_DISABLE This bit allows to disable the default buffering functionality when transfer is source synchronized.
                       //  0x0: buffering is enable across element/packet when source is
                       //       synchronized to element, packet, frame or blocks
                       //  0x1: buffering is disabled across element/packet when source is
                       //       synchronized to element, packet, frame or blocks
          MBIT(0,24) | //SEL_SRC_DST_SYNC Specifies that element, packet, frame or block transfer (depending on CCR.bs and CCR.fs) 
                       //       is triggered by the source or the destination on the DMA request
                       //  0x0: Transfer is triggered by the destination. If synch on packet the
                       //       packet element number is specified in the CDFI register
                       //  0x1: Transfer is triggered by the source. If synchronized on packet
                       //       the packet element number is specified in the CSFI register
          MBIT(0,23) | //PREFETCH Enables the prefetch mode
                       //  0x0: Prefetch mode is disabled. When Sel_Src_Dst_Sync=1
                       //       transfers are buffered and pipelined between DMA requests
                       //  0x1: Prefetch mode is enabled. Prefetch mode is active only when
                       //       destination is synchronized. It is SW user responsibility not to have
                       //       at the same time Prefetch=1 when Sel_Src_Dst_Sync=1. This mode is not supported
          MBIT(0,22) | //SUPERVISOR Enables the supervisor mode
                       //  0x0: Supervisor mode is disabled
                       //  0x1: Supervisor mode is enabled
          MBIT(1,18) | //BS Block synchronization
                       //  This bit used with the fs to see how the DMA request is serviced in a synchronized transfer
          MBIT(0,17) | //TRANSPARENT_COPY_ENABLE Transparent copy enable
                       //  0x0: Transparent copy mode is disabled
                       //  0x1: Transparent copy mode is enabled
          MBIT(1,16) | //CONST_FILL_ENABLE Constant fill enable
                       //  0x0: Constant fill mode is disabled
                       //  0x1: Constant fill mode is enabled
          MBIT(3,14) | //DST_AMODE Selects the addressing mode on the Write Port of a channel.
                       //  0x0: Constant address mode
                       //  0x1: Post-incremented address mode
                       //  0x2: Single index address mode
                       //  0x3: Double index address mode
          MBIT(0,12) | //SRC_AMODE Selects the addressing mode on the Read Port of a channel.
                       //  0x0: Constant address mode
                       //  0x1: Post-incremented address mode
                       //  0x2: Single index address mode
                       //  0x3: Double index address mode
          MBIT(0,8) | //SUSPEND_SENSITIVE Logical channel suspend enable bit
                      //  0x0: The channel ignores the MSuspend even if EMUFree is set to 0.
                      //  0x1: If EMUFree is set to 0 and MSuspend comes in then all
                      //       current OCP services (single transaction or burst transaction as
                      //       specified in the corresponding CSDP register) have to be
                      //       completed before stopping processing any more transactions
          MBIT(0,7) | //ENABLE Logical channel enable. It is SW responsibility to clear the CSR
                      //       register and the IRQSTATUS bit for the different interrupt lines before enabling the channel.
                      //  0x0: The logical channel is disabled
                      //  0x1: The logical channel is enabled
          MBIT(0,6) | //READ_PRIORITY Channel priority on the read side
                      //  0x0: Channel has low priority on the Read side during the arbitration process
                      //  0x1: Channel has high priority on read sided during the arbitration process
          MBIT(0,5) | //FS Frame synchronization
                      //  This bit used with the BS to see how the DMA request is serviced in a synchronized transfer
                      //  FS=0 and BS=0: An element is transferred once a DMA request is made.
                      //  FS=0 and BS=1: An entire block is transferred once a DMA request is made.
                      //  FS=1 and BS=0: An entire frame is transferred once a DMA request is made.
                      //  FS=1 and BS=1: A packet is transferred once a DMA request is made.
                      //  All these different transfers can be interleaved on the port with other DMA requests.
          DMA4_CHANNEL_FIELD(0), //software sync
          DMA4_CCR(dmaIndex)
          );
    //Запрограммировать адрес и количество
    __raw_writel( dstAddr, DMA4_CDSA(dmaIndex) );
    __raw_writel( dstAddr, DMA4_CSSA(dmaIndex) );
    __raw_writel( b.x - a.x, DMA4_CEN(dmaIndex) );  //Количество элементов во фрейме
    __raw_writel( b.y - a.y, DMA4_CFN(dmaIndex) ); //Количество фреймов
    
    //Индексы
    __raw_writel( 1, DMA4_CDEI(dmaIndex) ); //Индекс элементов
    __raw_writel( ((mSize.x - (b.x-a.x)) << 2) + 1, DMA4_CDFI(dmaIndex) );

    __raw_writel( 1, DMA4_CSEI(dmaIndex) );
    __raw_writel( 1, DMA4_CSFI(dmaIndex) );
    
    //Цвет заполнения
    __raw_writel( color, DMA4_COLOR(dmaIndex) );
    
    //Все готово, разрешить обмен
    __raw_setl( MBIT7, DMA4_CCR(dmaIndex) );

    dmaActive = true;

    WaitAccel();
    }
  else CmtContext::FilledRect( a, b, color, set );  
  }

void 
CmtContextTiOmap3530::CopyRect( CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest, bool set ) {
  if( dmaIndex && set && !dmaActive ) {
    //Вариант с ускорением
    uint32 dstAddr = (uint32)GetData(dest);
    
    if( size.x == 0 || size.y == 0 || srcSizeX == 0 ) return;
    
    //Запретить канал
    __raw_clearl( MBIT7, DMA4_CCR(dmaIndex) );
    
    //Сбросить флаг завершения транзакции
    __raw_writel( 0x1ffe, DMA4_CSR(dmaIndex) );
    
    __raw_writel(
          MBIT(0,26) | //WRITE_PRIORITY Channel priority on the Write side
                       //  0x0: Channel has low priority on the Write side during the arbitration process
                       //  0x1: Channel has high priority on Write sided during the arbitration process
          MBIT(1,25) | //BUFFERING_DISABLE This bit allows to disable the default buffering functionality when transfer is source synchronized.
                       //  0x0: buffering is enable across element/packet when source is
                       //       synchronized to element, packet, frame or blocks
                       //  0x1: buffering is disabled across element/packet when source is
                       //       synchronized to element, packet, frame or blocks
          MBIT(0,24) | //SEL_SRC_DST_SYNC Specifies that element, packet, frame or block transfer (depending on CCR.bs and CCR.fs) 
                       //       is triggered by the source or the destination on the DMA request
                       //  0x0: Transfer is triggered by the destination. If synch on packet the
                       //       packet element number is specified in the CDFI register
                       //  0x1: Transfer is triggered by the source. If synchronized on packet
                       //       the packet element number is specified in the CSFI register
          MBIT(0,23) | //PREFETCH Enables the prefetch mode
                       //  0x0: Prefetch mode is disabled. When Sel_Src_Dst_Sync=1
                       //       transfers are buffered and pipelined between DMA requests
                       //  0x1: Prefetch mode is enabled. Prefetch mode is active only when
                       //       destination is synchronized. It is SW user responsibility not to have
                       //       at the same time Prefetch=1 when Sel_Src_Dst_Sync=1. This mode is not supported
          MBIT(0,22) | //SUPERVISOR Enables the supervisor mode
                       //  0x0: Supervisor mode is disabled
                       //  0x1: Supervisor mode is enabled
          MBIT(0,18) | //BS Block synchronization
                       //  This bit used with the fs to see how the DMA request is serviced in a synchronized transfer
          MBIT(0,17) | //TRANSPARENT_COPY_ENABLE Transparent copy enable
                       //  0x0: Transparent copy mode is disabled
                       //  0x1: Transparent copy mode is enabled
          MBIT(0,16) | //CONST_FILL_ENABLE Constant fill enable
                       //  0x0: Constant fill mode is disabled
                       //  0x1: Constant fill mode is enabled
          MBIT(3,14) | //DST_AMODE Selects the addressing mode on the Write Port of a channel.
                       //  0x0: Constant address mode
                       //  0x1: Post-incremented address mode
                       //  0x2: Single index address mode
                       //  0x3: Double index address mode
          MBIT(3,12) | //SRC_AMODE Selects the addressing mode on the Read Port of a channel.
                       //  0x0: Constant address mode
                       //  0x1: Post-incremented address mode
                       //  0x2: Single index address mode
                       //  0x3: Double index address mode
          MBIT(0,8) | //SUSPEND_SENSITIVE Logical channel suspend enable bit
                      //  0x0: The channel ignores the MSuspend even if EMUFree is set to 0.
                      //  0x1: If EMUFree is set to 0 and MSuspend comes in then all
                      //       current OCP services (single transaction or burst transaction as
                      //       specified in the corresponding CSDP register) have to be
                      //       completed before stopping processing any more transactions
          MBIT(0,7) | //ENABLE Logical channel enable. It is SW responsibility to clear the CSR
                      //       register and the IRQSTATUS bit for the different interrupt lines before enabling the channel.
                      //  0x0: The logical channel is disabled
                      //  0x1: The logical channel is enabled
          MBIT(0,6) | //READ_PRIORITY Channel priority on the read side
                      //  0x0: Channel has low priority on the Read side during the arbitration process
                      //  0x1: Channel has high priority on read sided during the arbitration process
          MBIT(0,5) | //FS Frame synchronization
                      //  This bit used with the BS to see how the DMA request is serviced in a synchronized transfer
                      //  FS=0 and BS=0: An element is transferred once a DMA request is made.
                      //  FS=0 and BS=1: An entire block is transferred once a DMA request is made.
                      //  FS=1 and BS=0: An entire frame is transferred once a DMA request is made.
                      //  FS=1 and BS=1: A packet is transferred once a DMA request is made.
                      //  All these different transfers can be interleaved on the port with other DMA requests.
          DMA4_CHANNEL_FIELD(0), //software sync
          DMA4_CCR(dmaIndex)
          );
    //Запрограммировать адрес и количество
    __raw_writel( dstAddr, DMA4_CDSA(dmaIndex) );
    __raw_writel( (uint32)srcData, DMA4_CSSA(dmaIndex) );
    __raw_writel( size.x, DMA4_CEN(dmaIndex) );  //Количество элементов во фрейме
    __raw_writel( size.y, DMA4_CFN(dmaIndex) ); //Количество фреймов
    
    //Индексы
    __raw_writel( 1, DMA4_CDEI(dmaIndex) ); //Индекс элементов
    __raw_writel( ((mSize.x - (size.x)) << 2) + 1, DMA4_CDFI(dmaIndex) );
    
    __raw_writel( 1, DMA4_CSEI(dmaIndex) );
    __raw_writel( ((srcSizeX - (size.x)) << 2) + 1, DMA4_CSFI(dmaIndex) );
    
    //Все готово, разрешить обмен
    __raw_setl( MBIT7, DMA4_CCR(dmaIndex) );
    dmaActive = true;

    WaitAccel();
    }
  else CmtContext::CopyRect( size, srcData, srcSizeX, dest, set )  ;
  }

void
CmtContextTiOmap3530::CopyTranspRect( uint32 transpColor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest ) {
  if( dmaIndex && !dmaActive ) {
    //Вариант с ускорением
    uint32 dstAddr = (uint32)GetData(dest);

    if( size.x == 0 || size.y == 0 || srcSizeX == 0 ) return;

    //Запретить канал
    __raw_clearl( MBIT7, DMA4_CCR(dmaIndex) );

    //Сбросить флаг завершения транзакции
    __raw_writel( 0x1ffe, DMA4_CSR(dmaIndex) );

    __raw_writel(
          MBIT(0,26) | //WRITE_PRIORITY Channel priority on the Write side
                       //  0x0: Channel has low priority on the Write side during the arbitration process
                       //  0x1: Channel has high priority on Write sided during the arbitration process
          MBIT(1,25) | //BUFFERING_DISABLE This bit allows to disable the default buffering functionality when transfer is source synchronized.
                       //  0x0: buffering is enable across element/packet when source is
                       //       synchronized to element, packet, frame or blocks
                       //  0x1: buffering is disabled across element/packet when source is
                       //       synchronized to element, packet, frame or blocks
          MBIT(0,24) | //SEL_SRC_DST_SYNC Specifies that element, packet, frame or block transfer (depending on CCR.bs and CCR.fs)
                       //       is triggered by the source or the destination on the DMA request
                       //  0x0: Transfer is triggered by the destination. If synch on packet the
                       //       packet element number is specified in the CDFI register
                       //  0x1: Transfer is triggered by the source. If synchronized on packet
                       //       the packet element number is specified in the CSFI register
          MBIT(0,23) | //PREFETCH Enables the prefetch mode
                       //  0x0: Prefetch mode is disabled. When Sel_Src_Dst_Sync=1
                       //       transfers are buffered and pipelined between DMA requests
                       //  0x1: Prefetch mode is enabled. Prefetch mode is active only when
                       //       destination is synchronized. It is SW user responsibility not to have
                       //       at the same time Prefetch=1 when Sel_Src_Dst_Sync=1. This mode is not supported
          MBIT(0,22) | //SUPERVISOR Enables the supervisor mode
                       //  0x0: Supervisor mode is disabled
                       //  0x1: Supervisor mode is enabled
          MBIT(0,18) | //BS Block synchronization
                       //  This bit used with the fs to see how the DMA request is serviced in a synchronized transfer
          MBIT(1,17) | //TRANSPARENT_COPY_ENABLE Transparent copy enable
                       //  0x0: Transparent copy mode is disabled
                       //  0x1: Transparent copy mode is enabled
          MBIT(0,16) | //CONST_FILL_ENABLE Constant fill enable
                       //  0x0: Constant fill mode is disabled
                       //  0x1: Constant fill mode is enabled
          MBIT(3,14) | //DST_AMODE Selects the addressing mode on the Write Port of a channel.
                       //  0x0: Constant address mode
                       //  0x1: Post-incremented address mode
                       //  0x2: Single index address mode
                       //  0x3: Double index address mode
          MBIT(3,12) | //SRC_AMODE Selects the addressing mode on the Read Port of a channel.
                       //  0x0: Constant address mode
                       //  0x1: Post-incremented address mode
                       //  0x2: Single index address mode
                       //  0x3: Double index address mode
          MBIT(0,8) | //SUSPEND_SENSITIVE Logical channel suspend enable bit
                      //  0x0: The channel ignores the MSuspend even if EMUFree is set to 0.
                      //  0x1: If EMUFree is set to 0 and MSuspend comes in then all
                      //       current OCP services (single transaction or burst transaction as
                      //       specified in the corresponding CSDP register) have to be
                      //       completed before stopping processing any more transactions
          MBIT(0,7) | //ENABLE Logical channel enable. It is SW responsibility to clear the CSR
                      //       register and the IRQSTATUS bit for the different interrupt lines before enabling the channel.
                      //  0x0: The logical channel is disabled
                      //  0x1: The logical channel is enabled
          MBIT(0,6) | //READ_PRIORITY Channel priority on the read side
                      //  0x0: Channel has low priority on the Read side during the arbitration process
                      //  0x1: Channel has high priority on read sided during the arbitration process
          MBIT(0,5) | //FS Frame synchronization
                      //  This bit used with the BS to see how the DMA request is serviced in a synchronized transfer
                      //  FS=0 and BS=0: An element is transferred once a DMA request is made.
                      //  FS=0 and BS=1: An entire block is transferred once a DMA request is made.
                      //  FS=1 and BS=0: An entire frame is transferred once a DMA request is made.
                      //  FS=1 and BS=1: A packet is transferred once a DMA request is made.
                      //  All these different transfers can be interleaved on the port with other DMA requests.
          DMA4_CHANNEL_FIELD(0), //software sync
          DMA4_CCR(dmaIndex)
          );
    //Запрограммировать адрес и количество
    __raw_writel( dstAddr, DMA4_CDSA(dmaIndex) );
    __raw_writel( (uint32)srcData, DMA4_CSSA(dmaIndex) );
    __raw_writel( size.x, DMA4_CEN(dmaIndex) );  //Количество элементов во фрейме
    __raw_writel( size.y, DMA4_CFN(dmaIndex) ); //Количество фреймов

    //Цвет прозрачности
    __raw_writel( transpColor, DMA4_COLOR(dmaIndex) );

    //Индексы
    __raw_writel( 1, DMA4_CDEI(dmaIndex) ); //Индекс элементов
    __raw_writel( ((mSize.x - (size.x)) << 2) + 1, DMA4_CDFI(dmaIndex) );

    __raw_writel( 1, DMA4_CSEI(dmaIndex) );
    __raw_writel( ((srcSizeX - (size.x)) << 2) + 1, DMA4_CSFI(dmaIndex) );

    //Все готово, разрешить обмен
    __raw_setl( MBIT7, DMA4_CCR(dmaIndex) );
    dmaActive = true;

    WaitAccel();
    }
  else CmtContext::CopyTranspRect( transpColor, size, srcData, srcSizeX, dest )  ;
  }


void     
CmtContextTiOmap3530::HorzLine( int32 x1, int32 x2, int32 y, uint32 color, bool set ) {
  if( dmaIndex && set && !dmaActive ) {
    //Вариант с ускорением

    if( x2 < 0 || x1 >= mSize.x || y < 0 || y >= mSize.y ) return; //Линия вне окна
    //Отсечение
    if( x1 < 0 ) x1 = 0;
    if( x2 >= mSize.x ) x2 = mSize.x - 1;
    //Указатель
    uint32 dstAddr = (uint32)(mData + mSize.x * y + x1);
    
    //Запретить канал
    __raw_clearl( MBIT7, DMA4_CCR(dmaIndex) );
    
    //Сбросить флаг завершения транзакции
    __raw_writel( 0x1ffe, DMA4_CSR(dmaIndex) );
    
    __raw_writel(
          MBIT(0,26) | //WRITE_PRIORITY Channel priority on the Write side
                       //  0x0: Channel has low priority on the Write side during the arbitration process
                       //  0x1: Channel has high priority on Write sided during the arbitration process
          MBIT(1,25) | //BUFFERING_DISABLE This bit allows to disable the default buffering functionality when transfer is source synchronized.
                       //  0x0: buffering is enable across element/packet when source is
                       //       synchronized to element, packet, frame or blocks
                       //  0x1: buffering is disabled across element/packet when source is
                       //       synchronized to element, packet, frame or blocks
          MBIT(0,24) | //SEL_SRC_DST_SYNC Specifies that element, packet, frame or block transfer (depending on CCR.bs and CCR.fs) 
                       //       is triggered by the source or the destination on the DMA request
                       //  0x0: Transfer is triggered by the destination. If synch on packet the
                       //       packet element number is specified in the CDFI register
                       //  0x1: Transfer is triggered by the source. If synchronized on packet
                       //       the packet element number is specified in the CSFI register
          MBIT(0,23) | //PREFETCH Enables the prefetch mode
                       //  0x0: Prefetch mode is disabled. When Sel_Src_Dst_Sync=1
                       //       transfers are buffered and pipelined between DMA requests
                       //  0x1: Prefetch mode is enabled. Prefetch mode is active only when
                       //       destination is synchronized. It is SW user responsibility not to have
                       //       at the same time Prefetch=1 when Sel_Src_Dst_Sync=1. This mode is not supported
          MBIT(0,22) | //SUPERVISOR Enables the supervisor mode
                       //  0x0: Supervisor mode is disabled
                       //  0x1: Supervisor mode is enabled
          MBIT(0,18) | //BS Block synchronization
                       //  This bit used with the fs to see how the DMA request is serviced in a synchronized transfer
          MBIT(0,17) | //TRANSPARENT_COPY_ENABLE Transparent copy enable
                       //  0x0: Transparent copy mode is disabled
                       //  0x1: Transparent copy mode is enabled
          MBIT(1,16) | //CONST_FILL_ENABLE Constant fill enable
                       //  0x0: Constant fill mode is disabled
                       //  0x1: Constant fill mode is enabled
          MBIT(1,14) | //DST_AMODE Selects the addressing mode on the Write Port of a channel.
                       //  0x0: Constant address mode
                       //  0x1: Post-incremented address mode
                       //  0x2: Single index address mode
                       //  0x3: Double index address mode
          MBIT(0,12) | //SRC_AMODE Selects the addressing mode on the Read Port of a channel.
                       //  0x0: Constant address mode
                       //  0x1: Post-incremented address mode
                       //  0x2: Single index address mode
                       //  0x3: Double index address mode
          MBIT(0,8) | //SUSPEND_SENSITIVE Logical channel suspend enable bit
                      //  0x0: The channel ignores the MSuspend even if EMUFree is set to 0.
                      //  0x1: If EMUFree is set to 0 and MSuspend comes in then all
                      //       current OCP services (single transaction or burst transaction as
                      //       specified in the corresponding CSDP register) have to be
                      //       completed before stopping processing any more transactions
          MBIT(0,7) | //ENABLE Logical channel enable. It is SW responsibility to clear the CSR
                      //       register and the IRQSTATUS bit for the different interrupt lines before enabling the channel.
                      //  0x0: The logical channel is disabled
                      //  0x1: The logical channel is enabled
          MBIT(0,6) | //READ_PRIORITY Channel priority on the read side
                      //  0x0: Channel has low priority on the Read side during the arbitration process
                      //  0x1: Channel has high priority on read sided during the arbitration process
          MBIT(0,5) | //FS Frame synchronization
                      //  This bit used with the BS to see how the DMA request is serviced in a synchronized transfer
                      //  FS=0 and BS=0: An element is transferred once a DMA request is made.
                      //  FS=0 and BS=1: An entire block is transferred once a DMA request is made.
                      //  FS=1 and BS=0: An entire frame is transferred once a DMA request is made.
                      //  FS=1 and BS=1: A packet is transferred once a DMA request is made.
                      //  All these different transfers can be interleaved on the port with other DMA requests.
          DMA4_CHANNEL_FIELD(0), //software sync
          DMA4_CCR(dmaIndex)
          );
    //Запрограммировать адрес и количество
    __raw_writel( dstAddr, DMA4_CDSA(dmaIndex) );
    __raw_writel( x2 - x1 + 1, DMA4_CEN(dmaIndex) );  //Количество элементов во фрейме
    __raw_writel( 1, DMA4_CFN(dmaIndex) ); //Количество фреймов
    
    //Цвет заполнения
    __raw_writel( color, DMA4_COLOR(dmaIndex) );
    
    //Все готово, разрешить обмен
    __raw_setl( MBIT7, DMA4_CCR(dmaIndex) );
    dmaActive = true;

    WaitAccel();
    }
  else CmtContext::HorzLine( x1, x2, y, color, set );  
  }

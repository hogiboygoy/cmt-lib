//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Debug1
//==============================================================================

// Define MMC states
#define MMC_STATE_IDLE      0
#define MMC_STATE_READY     1
#define MMC_STATE_IDENT     2
#define MMC_STATE_STBY      3
#define MMC_STATE_TRAN      4
#define MMC_STATE_DATA      5
#define MMC_STATE_RCV       6
#define MMC_STATE_PRG       7
#define MMC_STATE_DIS       8

#define CARDTYPE_MMC        1
#define CARDTYPE_SD         2
#define CARDTYPE_SDHC       3


/* MMC command definitions */

#define    GO_IDLE_STATE            0 
#define    SEND_OP_COND             1 
#define    ALL_SEND_CID             2 
#define    SET_RELATIVE_ADDR        3 
#define    SEND_RELATIVE_ADDRESS    3   // not same number as above -- why didn't they just use a different command number !!
#define    SET_DSR                  4 
#define    SELECT_DESELECT_CARD     7 
#define    SEND_CSD                 9 
#define    SEND_CID                 10
#define    READ_DAT_UNTIL_STOP      11
#define    STOP_TRANSMISSION        12
#define    SEND_STATUS              13
#define    GO_INACTIVE_STATE        15
#define    SET_BLOCKLEN             16
#define    READ_SINGLE_BLOCK        17
#define    READ_MULTIPLE_BLOCK      18
#define    WRITE_DAT_UNTIL_STOP     20
#define    WRITE_BLOCK              24
#define    WRITE_MULTIPLE_BLOCK     25
#define    PROGRAM_CID              26
#define    PROGRAM_CSD              27
#define    SET_WRITE_PROT           28
#define    CLR_WRITE_PROT           29
#define    SEND_WRITE_PROT          30
#define    TAG_SECTOR_START         32
#define    TAG_SECTOR_END           33
#define    UNTAG_SECTOR             34
#define    TAG_ERASE_GROUP_START    35
#define    TAG_ERASE_GROUP_END      36
#define    UNTAG_ERASE_GROUP        37
#define    ERASE                    38
#define    LOCK_UNLOCK              42
#define    APP_CMD                  55
#define    GEN_CMD                  56

/* SD application specific commands -- must follow APP_CMD */
#define    SET_BUS_WIDTH            6
#define    SD_STATUS                13
#define    SEND_NUM_WR_BLOCKS       22
#define    SET_WR_BLK_ERASE_COUNT   23
#define    SD_SEND_OP_CODE          41
#define    SET_CLR_CARD_DETECT      42
#define    SEND_SCR                 51

/* SD Security commands -- must follow APP_CMD */
#define    GET_MKB                  43
#define    GET_MID                  44
#define    SET_CER_RN1              45
#define    GET_CER_RN2              46
#define    SET_CER_RS2              47
#define    GET_CER_RS2              48
#define    SECURE_READ_MULTI_BLOCK  18
#define    SECURE_WRITE_MULTI_BLOCK 25
#define    SECURE_ERASE             38
#define    CHANGE_SECURE_AREA       49
#define    SECURE_WRITE_MKB         26

/* SD 2.0 (SDHC) commands, arg 11:8 = VHS (supply voltage), 7:0 = check pattern, response R7 */
#define    SD_SEND_IF_COND          8

/* state encoding in MMC card status register */
#define MMC_STATUS_STATE_IDLE       0
#define MMC_STATUS_STATE_READY      1
#define MMC_STATUS_STATE_IDENT      2
#define MMC_STATUS_STATE_STBY       3
#define MMC_STATUS_STATE_TRAN       4
#define MMC_STATUS_STATE_DATA       5
#define MMC_STATUS_STATE_RCV        6
#define MMC_STATUS_STATE_PRG        7
#define MMC_STATUS_STATE_DIS        8

/* Responses */
#define RSP_TYPE_NONE   (RSP_TYPE_NORSP   | CCCE_NOCHECK | CICE_NOCHECK)
#define RSP_TYPE_R1      (RSP_TYPE_LGHT48  | CCCE_CHECK   | CICE_CHECK)
#define RSP_TYPE_R1B   (RSP_TYPE_LGHT48B | CCCE_CHECK   | CICE_CHECK)
#define RSP_TYPE_R2      (RSP_TYPE_LGHT136 | CCCE_CHECK   | CICE_NOCHECK)
#define RSP_TYPE_R3      (RSP_TYPE_LGHT48  | CCCE_NOCHECK | CICE_NOCHECK)
#define RSP_TYPE_R4      (RSP_TYPE_LGHT48  | CCCE_NOCHECK | CICE_NOCHECK)
#define RSP_TYPE_R5      (RSP_TYPE_LGHT48  | CCCE_CHECK   | CICE_CHECK)
#define RSP_TYPE_R6      (RSP_TYPE_LGHT48  | CCCE_CHECK   | CICE_CHECK)
#define RSP_TYPE_R7     (RSP_TYPE_LGHT48  | CCCE_CHECK   | CICE_CHECK)

/* All supported commands */
#define MMC_CMD0   (INDEX(0)  | RSP_TYPE_NONE | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD1   (INDEX(1)  | RSP_TYPE_R3   | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD2   (INDEX(2)  | RSP_TYPE_R2   | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD3   (INDEX(3)  | RSP_TYPE_R1   | DP_NO_DATA | DDIR_WRITE)
#define MMC_SDCMD3   (INDEX(3)  | RSP_TYPE_R6   | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD4   (INDEX(4)  | RSP_TYPE_NONE | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD6   (INDEX(6)  | RSP_TYPE_R1B  | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD7_SELECT   (INDEX(7)  | RSP_TYPE_R1B  | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD7_DESELECT \
   (INDEX(7)  | RSP_TYPE_NONE | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD8   (INDEX(8)  | RSP_TYPE_R1   | DP_DATA    | DDIR_READ)
#define MMC_SDCMD8   (INDEX(8)  | RSP_TYPE_R7   | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD9   (INDEX(9)  | RSP_TYPE_R2   | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD12   (INDEX(12) | RSP_TYPE_R1B  | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD13   (INDEX(13) | RSP_TYPE_R1   | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD15   (INDEX(15) | RSP_TYPE_NONE | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD16   (INDEX(16) | RSP_TYPE_R1   | DP_NO_DATA | DDIR_WRITE)
#define MMC_CMD17   (INDEX(17) | RSP_TYPE_R1   | DP_DATA    | DDIR_READ)
#define MMC_CMD18   (INDEX(18) | RSP_TYPE_R1   | DP_DATA    | DDIR_READ | MMCHS_CMD_MSBS | MMCHS_CMD_DE | MMCHS_CMD_BCE)
#define MMC_CMD24   (INDEX(24) | RSP_TYPE_R1   | DP_DATA    | DDIR_WRITE)
#define MMC_ACMD6   (INDEX(6)  | RSP_TYPE_R1   | DP_NO_DATA | DDIR_WRITE)
#define MMC_ACMD41   (INDEX(41) | RSP_TYPE_R3   | DP_NO_DATA | DDIR_WRITE)
#define MMC_ACMD51   (INDEX(51) | RSP_TYPE_R1   | DP_DATA    | DDIR_READ)
#define MMC_CMD55   (INDEX(55) | RSP_TYPE_R1   | DP_NO_DATA | DDIR_WRITE)

#define MMC_AC_CMD_RCA_MASK     (unsigned int)(0xFFFF << 16)
#define MMC_BC_CMD_DSR_MASK     (unsigned int)(0xFFFF << 16)
#define MMC_DSR_DEFAULT         (0x0404)
#define SD_CMD8_CHECK_PATTERN       (0xAA)
#define SD_CMD8_2_7_3_6_V_RANGE     (0x01 << 8)

/* Clock Configurations and Macros */

#define MMC_CLOCK_REFERENCE      (96)
#define MMC_RELATIVE_CARD_ADDRESS   (0x1234)
#define MMC_INIT_SEQ_CLK      (MMC_CLOCK_REFERENCE * 1000 / 80)
#define MMC_400kHz_CLK         (MMC_CLOCK_REFERENCE * 1000 / 400)
#define CLKDR(r, f, u)         ((((r)*100) / ((f)*(u))) + 1)
#define CLKD(f, u)         (CLKDR(MMC_CLOCK_REFERENCE, f, u))

#define MMC_OCR_REG_ACCESS_MODE_MASK   (0x3 << 29)
#define MMC_OCR_REG_ACCESS_MODE_BYTE    (0x0 << 29)
#define MMC_OCR_REG_ACCESS_MODE_SECTOR  (0x2 << 29)

#define MMC_OCR_REG_HOST_CAPACITY_SUPPORT_MASK      (0x1 << 30)
#define MMC_OCR_REG_HOST_CAPACITY_SUPPORT_BYTE      (0x0 << 30)
#define MMC_OCR_REG_HOST_CAPACITY_SUPPORT_SECTOR   (0x1 << 30)

#define MMC_SD2_CSD_C_SIZE_LSB_MASK         (0xFFFF)
#define MMC_SD2_CSD_C_SIZE_MSB_MASK         (0x003F)
#define MMC_SD2_CSD_C_SIZE_MSB_OFFSET       (16)
#define MMC_CSD_C_SIZE_LSB_MASK             (0x0003)
#define MMC_CSD_C_SIZE_MSB_MASK             (0x03FF)
#define MMC_CSD_C_SIZE_MSB_OFFSET           (2)

#define MMC_CSD_TRAN_SPEED_UNIT_MASK        (0x07 << 0)
#define MMC_CSD_TRAN_SPEED_FACTOR_MASK      (0x0F << 3)
#define MMC_CSD_TRAN_SPEED_UNIT_100MHZ      (0x3 << 0)
#define MMC_CSD_TRAN_SPEED_FACTOR_1_0       (0x01 << 3)
#define MMC_CSD_TRAN_SPEED_FACTOR_8_0       (0x0F << 3)

#define CLK_INITSEQ            0
#define CLK_400KHZ            1
#define CLK_MISC            2

static const unsigned int tran_exp[] = {
   10000,          100000,         1000000,        10000000,
   0,              0,              0,              0
};

static const unsigned char tran_mant[] = {
   0,      10,     12,     13,     15,     20,     25,     30,
   35,     40,     45,     50,     55,     60,     70,     80,
};

typedef struct {
   unsigned not_used:1;
   unsigned crc:7;
   unsigned ecc:2;
   unsigned file_format:2;
   unsigned tmp_write_protect:1;
   unsigned perm_write_protect:1;
   unsigned copy:1;
   unsigned file_format_grp:1;
   unsigned content_prot_app:1;
   unsigned reserved_1:4;
   unsigned write_bl_partial:1;
   unsigned write_bl_len:4;
   unsigned r2w_factor:3;
   unsigned default_ecc:2;
   unsigned wp_grp_enable:1;
   unsigned wp_grp_size:5;
   unsigned erase_grp_mult:5;
   unsigned erase_grp_size:5;
   unsigned c_size_mult:3;
   unsigned vdd_w_curr_max:3;
   unsigned vdd_w_curr_min:3;
   unsigned vdd_r_curr_max:3;
   unsigned vdd_r_curr_min:3;
   unsigned c_size_lsb:2;
   unsigned c_size_msb:10;
   unsigned reserved_2:2;
   unsigned dsr_imp:1;
   unsigned read_blk_misalign:1;
   unsigned write_blk_misalign:1;
   unsigned read_bl_partial:1;
   unsigned read_bl_len:4;
   unsigned ccc:12;
   unsigned tran_speed:8;
   unsigned nsac:8;
   unsigned taac:8;
   unsigned reserved_3:2;
   unsigned spec_vers:4;
   unsigned csd_structure:2;
} mmc_csd_reg_t;

/* csd for sd2.0 */
typedef struct {
   unsigned not_used:1;
   unsigned crc:7;
   unsigned reserved_1:2;
   unsigned file_format:2;
   unsigned tmp_write_protect:1;
   unsigned perm_write_protect:1;
   unsigned copy:1;
   unsigned file_format_grp:1;
   unsigned reserved_2:5;
   unsigned write_bl_partial:1;
   unsigned write_bl_len:4;
   unsigned r2w_factor:3;
   unsigned reserved_3:2;
   unsigned wp_grp_enable:1;
   unsigned wp_grp_size:7;
   unsigned sector_size:7;
   unsigned erase_blk_len:1;
   unsigned reserved_4:1;
   unsigned c_size_lsb:16;
   unsigned c_size_msb:6;
   unsigned reserved_5:6;
   unsigned dsr_imp:1;
   unsigned read_blk_misalign:1;
   unsigned write_blk_misalign:1;
   unsigned read_bl_partial:1;
   unsigned read_bl_len:4;
   unsigned ccc:12;
   unsigned tran_speed:8;
   unsigned nsac:8;
   unsigned taac:8;
   unsigned reserved_6:6;
   unsigned csd_structure:2;
} mmc_sd2_csd_reg_t;

/* extended csd - 512 bytes long */
typedef struct {
   unsigned char reserved_1[181];
   unsigned char erasedmemorycontent;
   unsigned char reserved_2;
   unsigned char buswidthmode;
   unsigned char reserved_3;
   unsigned char highspeedinterfacetiming;
   unsigned char reserved_4;
   unsigned char powerclass;
   unsigned char reserved_5;
   unsigned char commandsetrevision;
   unsigned char reserved_6;
   unsigned char commandset;
   unsigned char extendedcsdrevision;
   unsigned char reserved_7;
   unsigned char csdstructureversion;
   unsigned char reserved_8;
   unsigned char cardtype;
   unsigned char reserved_9[3];
   unsigned char powerclass_52mhz_1_95v;
   unsigned char powerclass_26mhz_1_95v;
   unsigned char powerclass_52mhz_3_6v;
   unsigned char powerclass_26mhz_3_6v;
   unsigned char reserved_10;
   unsigned char minreadperf_4b_26mhz;
   unsigned char minwriteperf_4b_26mhz;
   unsigned char minreadperf_8b_26mhz_4b_52mhz;
   unsigned char minwriteperf_8b_26mhz_4b_52mhz;
   unsigned char minreadperf_8b_52mhz;
   unsigned char minwriteperf_8b_52mhz;
   unsigned char reserved_11;
   unsigned int sectorcount;
   unsigned char reserved_12[288];
   unsigned char supportedcommandsets;
   unsigned char reserved_13[7];
} mmc_extended_csd_reg_t;

/* mmc sd responce */
typedef struct {
   unsigned int ocr;
} mmc_resp_r3;

typedef struct {
   unsigned short cardstatus;
   unsigned short newpublishedrca;
} mmc_resp_r6;

#ifdef CMT_DMA
CmtSdCard::CmtSdCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect isConnect, int sdFactor, uint32 blockSize, uint32 blockCount, CmtDmaBase *dmaRx, CmtDmaBase *dmaTx ) :
  CmtBlockDevice( blockSize, blockCount ),
  mMmc(mmc),
  fnSdIsConnect(isConnect),
  mSdFactor(sdFactor),
  mDmaRx(dmaRx),
  mDmaTx(dmaTx) {
    //Устройство должно быть инициализировано
    mStatus = CMTE_OK;
    }
#else
CmtSdCard::CmtSdCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect isConnect, int sdFactor, uint32 blockSize, uint32 blockCount ) :
  CmtBlockDevice( blockSize, blockCount ),
  mMmc(mmc),
  fnSdIsConnect(isConnect),
  mSdFactor(sdFactor) {
    //Устройство должно быть инициализировано
    mStatus = CMTE_OK;
    }
#endif

uint_8 
CmtSdCard::Read( uint32 *dest, uint32 block, uint32 count ) {
  uint_8 res = CMTE_OK;
#ifdef CMT_DEV_ARBITR
  if( !Lock() ) return CMTE_BD_NO_DEV;
#endif
#ifdef CMT_DMA
  if( mDmaRx ) {
    block <<= mSdFactor; // sector <<= 9;
    //Настроить DMA
    mDmaRx->InitTransfer( dest, 512 / 4, count );
    //Отправить команду
    if( SendCommand( mMmc, MMC_CMD18, block, 0, count ) ) {
      //Команда ушла, принять данные
      mDmaRx->WaitTransfer(0);
      mDmaRx->StopTransfer();
      return fnSdIsConnect();
      }
    mDmaRx->StopTransfer();
    cmtDebug1( 16, 3, "ReadBlock fail cmd", 0 );
    return CMTE_SD_READ_BLOCK_TIMEOUT;
    }
  else {
    while( count-- ) {
      res = ReadBlock( dest, block );
      if( res != CMTE_OK ) break;
      //Переходим к следующему блоку
      dest += mBlockSize >> 2;
      block++;
      }
    }
#else
  while( count-- ) {
    res = ReadBlock( dest, block );
    if( res != CMTE_OK ) break;
    //Переходим к следующему блоку
    dest += mBlockSize >> 2;
    block++;
    }
#endif
#ifdef CMT_DEV_ARBITR
  UnLock();
#endif
  return res;
  }

uint_8 
CmtSdCard::Write( const uint32 *src, uint32 block, uint32 count ) {
  uint_8 res = CMTE_OK;
#ifdef CMT_DEV_ARBITR
  if( !Lock() ) return CMTE_BD_NO_DEV;
#endif
  while( count-- ) {
    res = WriteBlock( src, block );
    if( res != CMTE_OK ) break;
    //Переходим к следующему блоку
    src += mBlockSize >> 2;
    block++;
    }
#ifdef CMT_DEV_ARBITR
  UnLock();
#endif
  return res;
  }

#if 0
uint_8 
CmtSpiSdCard::Response( CmtSpiBase *spi, uint8 response ) {
  uint_16 count = 0xff;
  while( (spi->Transfer(0xff) != response) && count ) count--;
  if( count == 0 )
    return CMTE_FAIL; /* Failure, loop was exited due to timeout */
  else
    return CMTE_OK; /* Normal, loop was exited before timeout */
  }  
#endif

uint_8 
CmtSdCard::ReadBlock( uint32 *dest, uint32 block ) {
  cmtDebugInfo( CMT_DBGM_SD, 1, 1, "Enter in ReadBlock %1", block, 0, 0, 0 );
  block <<= mSdFactor; // sector <<= 9;
  //Отправить команду
  cmtDebug1( 16, 1, "ReadBlock block = %1", block );
  if( SendCommand( mMmc, MMC_CMD17, block, 0 ) ) {
    //Команда ушла, принять данные
    if( GetData( mMmc, dest ) )
      return fnSdIsConnect();
    cmtDebug1( 16, 2, "ReadBlock fail GetData", 0 );
    return CMTE_SD_BLOCK_DATA_TOKEN_MISSING;
    }
  cmtDebug1( 16, 3, "ReadBlock fail cmd", 0 );
  return CMTE_SD_READ_BLOCK_TIMEOUT;
  }

uint_8 
CmtSdCard::WriteBlock( const uint32 *src, uint32 block ) {
  block <<= mSdFactor; // sector <<= 9;
  //Отправить команду
  if( SendCommand( mMmc, MMC_CMD24, block, 0 ) ) {
    //Команда ушла, передать данные
    if( PutData( mMmc, src ) )
      return fnSdIsConnect();
    return CMTE_SD_WRITE_BLOCK_FAIL;
    }
  return CMTE_SD_WRITE_BLOCK_TIMEOUT;
  }
  
#if 0
uint_8
CmtSdCard::WaitForWriteFinish() {
  uint_16 count = 0xFFFF; /* The delay is set to maximum considering the longest data block length to handle */
  while( (mSpi->Transfer(0xff) == 0) && count ) count--;
  if( count == 0 )
    return CMTE_FAIL; /* Failure, loop was exited due to timeout */
  else
    return CMTE_OK; /* Normal, loop was exited before timeout */
  }  
#endif

void
CmtSdCard::BoardInit( OMAP_MMCHS_REGS *mmc ) {
#if 0
  unsigned int value = 0;

  value = CONTROL_PBIAS_LITE;
  CONTROL_PBIAS_LITE = value | (1 << 2) | (1 << 1) | (1 << 9);

  value = CONTROL_DEV_CONF0;
  CONTROL_DEV_CONF0 = value | (1 << 24);
  return 1;
#endif
  }

void 
CmtSdCard::InitStream( OMAP_MMCHS_REGS *mmc ) {
  cmtDebug1( 1, 1, "InitStream() first command", 0 );
  mmc->MMCHS_CON |= INIT_INITSTREAM;

  mmc->MMCHS_CMD = MMC_CMD0;
  while( !(__raw_readl( &(mmc->MMCHS_STAT) ) & CC_MASK) );

  mmc->MMCHS_STAT = CC_MASK;

  cmtDebug1( 1, 2, "InitStream() second command", 0 );
  mmc->MMCHS_CMD = MMC_CMD0;
  while( !(__raw_readl( &(mmc->MMCHS_STAT) ) & CC_MASK) );

  mmc->MMCHS_STAT = mmc->MMCHS_STAT;
  mmc->MMCHS_CON &= ~INIT_INITSTREAM;
  cmtDebug1( 1, 32, "InitStream() complete", 0 );
  }

#define mmc_reg_out(addr, mask, val) \
   (addr) = (((addr)) & (~(mask)) ) | ( (val) & (mask));

void
CmtSdCard::ClockConfig( OMAP_MMCHS_REGS *mmc, uint32 iclk, uint32 clk_div ) {
  uint32 val;

  mmc_reg_out( mmc->MMCHS_SYSCTL, (ICE_MASK | DTO_MASK | CEN_MASK),   (ICE_STOP | DTO_15THDTO | CEN_DISABLE) );

  switch (iclk) {
  case CLK_INITSEQ:
     val = MMC_INIT_SEQ_CLK / 2;
     break;
  case CLK_400KHZ:
     val = MMC_400kHz_CLK;
     break;
  case CLK_MISC:
     val = clk_div;
     break;
  default:
     return;
  }
  mmc_reg_out( mmc->MMCHS_SYSCTL, ICE_MASK | CLKD_MASK, (val << CLKD_OFFSET) | ICE_OSCILLATE );

  while( (__raw_readl( &(mmc->MMCHS_SYSCTL) ) & ICS_MASK) == ICS_NOTREADY );

  mmc->MMCHS_SYSCTL |= CEN_ENABLE;
  }

void
CmtSdCard::InitSetup( OMAP_MMCHS_REGS *mmc ) {
  uint32 reg_val;

  cmtDebug1( 4, 1, "InitSetup SoftReset", 0 );
  BoardInit( mmc );

  mmc->MMCHS_SYSCONFIG |= MMC_SOFTRESET;
  while( (__raw_readl( &(mmc->MMCHS_SYSSTATUS) ) & RESETDONE) == 0 );

  cmtDebug1( 4, 2, "...done. InitSetup AllReset", 0 );
  mmc->MMCHS_SYSCTL |= SOFTRESETALL;
  while( (__raw_readl( &(mmc->MMCHS_SYSCTL) ) & SOFTRESETALL) != 0x0 );

  cmtDebug1( 4, 3, "...done. InitSetup Clock Config", 0 );
  mmc->MMCHS_HCTL = DTW_1_BITMODE | SDBP_PWROFF | SDVS_3V0;
  mmc->MMCHS_CAPA |= VS30_3V0SUP | VS18_1V8SUP;

  reg_val = mmc->MMCHS_CON & RESERVED_MASK;

  mmc->MMCHS_CON = CTPL_MMC_SD | reg_val | WPP_ACTIVEHIGH |
      CDP_ACTIVEHIGH | MIT_CTO | DW8_1_4BITMODE | MODE_FUNC |
      STR_BLOCK | HR_NOHOSTRESP | INIT_NOINIT | NOOPENDRAIN;

  ClockConfig( mmc, CLK_INITSEQ, 0 );
  mmc->MMCHS_HCTL |= SDBP_PWRON;

  mmc->MMCHS_IE = 0x307f0033;
  cmtDebug1( 4, 4, "...done.", 0 );

  InitStream( mmc );
  }

bool
CmtSdCard::SendCommand( OMAP_MMCHS_REGS *mmc, uint32 cmd, uint32 arg, uint32 *response, uint32 blkCount ) {
  uint32 mmc_stat;

  while( (__raw_readl( &(mmc->MMCHS_PSTATE) ) & DATI_MASK) == DATI_CMDDIS );
  cmtDebug1( 6, 1, "SendCommand ready", 0 );

  mmc->MMCHS_BLK = BLEN_512BYTESLEN | (blkCount << 16);
  mmc->MMCHS_STAT = 0xFFFFFFFF;
  mmc->MMCHS_ARG = arg;
  mmc->MMCHS_CMD = cmd | CMD_TYPE_NORMAL | CICE_NOCHECK |
      CCCE_NOCHECK | MSBS_SGLEBLK | ACEN_DISABLE | BCE_DISABLE |
      DE_DISABLE;

  while (1) {
    do {
      mmc_stat = __raw_readl( &(mmc->MMCHS_STAT) );
      } 
    while( mmc_stat == 0 );

    if( (mmc_stat & ERRI_MASK) != 0 ) {
      cmtDebug1( 6, 2, "SendCommand error %1", mmc_stat );
      return false;
      }

    if( mmc_stat & CC_MASK ) {
      cmtDebug1( 6, 3, "SendCommand completed", 0 );
      mmc->MMCHS_STAT = CC_MASK;
      if( response ) {
        response[0] = mmc->MMCHS_RSP10;
        if( (cmd & RSP_TYPE_MASK) == RSP_TYPE_LGHT136) {
          response[1] = mmc->MMCHS_RSP32;
          response[2] = mmc->MMCHS_RSP54;
          response[3] = mmc->MMCHS_RSP76;
          }
        }
      break;
      }
    }
  return true;
  }

bool
CmtSdCard::GetData( OMAP_MMCHS_REGS *mmc, uint32 *output_buf ) {
  uint32 mmc_stat;

  /*
   * Start Polled Read
   */
  cmtDebug1( 7, 1, "GetData start polling", 0 );
  while(1) {
    do {
      mmc_stat = __raw_readl( &(mmc->MMCHS_STAT) );
      } 
    while( mmc_stat == 0 );

    if( (mmc_stat & ERRI_MASK) != 0 ) {
      cmtDebug1( 7, 2, "GetData err = %1", mmc_stat );
      return false;
      }

    if( mmc_stat & BRR_MASK ) {
      //Чтение данных
      unsigned int k;

      mmc->MMCHS_STAT |= BRR_MASK;
      for( k = 0; k < 512 / 4; k++ ) {
        *output_buf = mmc->MMCHS_DATA;
        output_buf++;
        //read_count += 4;
        }
      }

    if( mmc_stat & BWR_MASK )
      mmc->MMCHS_STAT |= BWR_MASK;

    if( mmc_stat & TC_MASK ) {
      mmc->MMCHS_STAT |= TC_MASK;
      break;
      }
    }
  cmtDebug1( 7, 8, "GetData success", 0 );
  return true;
  }

bool
CmtSdCard::PutData( OMAP_MMCHS_REGS *mmc, const uint32 *src ) {
  uint32 mmc_stat;
  
  while(1) {
    do {
      mmc_stat = __raw_readl( &(mmc->MMCHS_STAT) );
      } 
    while( mmc_stat == 0 );

    if( (mmc_stat & ERRI_MASK) != 0 ) {
      return false;
      }

    if( mmc_stat & BWR_MASK ) {
      //Чтение данных
      unsigned int k;

      mmc->MMCHS_STAT |= BWR_MASK;
      for( k = 0; k < 512 / 4; k++ ) {
        mmc->MMCHS_DATA = src[0];
        src++;
        }
      }

    if( mmc_stat & BRR_MASK )
      mmc->MMCHS_STAT |= BWR_MASK;

    if( mmc_stat & TC_MASK ) {
      mmc->MMCHS_STAT |= TC_MASK;
      break;
      }
    }
  return true;
  }

bool
CmtSdCard::DetectCard( OMAP_MMCHS_REGS *mmc, bool *sdCardType, uint32 *factor, uint32 *RCA ) {
  uint32 argument = 0;
  uint32 ocr_value, ocr_recvd, ret_cmd41, hcs_val;
  uint32 resp[4];
  uint32 retry_cnt = 2000;

  /* Set to Initialization Clock */
  ClockConfig( mmc, CLK_400KHZ, 0 );

  argument = 0x00000000;

  ocr_value = (0x1FF << 15);
  cmtDebug1( 8, 1, "DetectCard CMD0", 0 );
  if( !SendCommand( mmc, MMC_CMD0, argument, resp ) ) {
    cmtDebug1( 8, 2, "DetectCard CMD0 fail", 0 );
    return false;
    }

  cmtDebug1( 8, 3, "DetectCard CMD8", 0 );
  argument = SD_CMD8_CHECK_PATTERN | SD_CMD8_2_7_3_6_V_RANGE;
  if( SendCommand( mmc,MMC_SDCMD8, argument, resp) ) {
    cmtDebug1( 8, 4, "DetectCard CMD8 success", 0 );
    *factor = 0;
    hcs_val = MMC_OCR_REG_HOST_CAPACITY_SUPPORT_SECTOR;
    }
  else {
    cmtDebug1( 8, 5, "DetectCard CMD8 fail", 0 );
    *factor = 9;
    hcs_val = MMC_OCR_REG_HOST_CAPACITY_SUPPORT_BYTE;
    }

  argument = 0x0000 << 16;
  cmtDebug1( 8, 7, "DetectCard CMD55", 0 );
  if( SendCommand( mmc, MMC_CMD55, argument, resp) ) {
    cmtDebug1( 8, 8, "DetectCard CMD55 success (SD)", 0 );
    *sdCardType = true;    //mmc_card_cur->card_type = SD_CARD;
    ocr_value |= hcs_val;
    ret_cmd41 = MMC_ACMD41;
    } 
  else {
    cmtDebug1( 8, 9, "DetectCard CMD55 fail (MMC)", 0 );
    *sdCardType = false; //mmc_card_cur->card_type = MMC_CARD;
    ocr_value |= MMC_OCR_REG_ACCESS_MODE_SECTOR;
    ret_cmd41 = MMC_CMD1;
    mmc->MMCHS_CON &= ~OD;
    mmc->MMCHS_CON |= OPENDRAIN;
    }

  argument = ocr_value;
  cmtDebug1( 8, 11, "DetectCard cmd41 (ACMD41 for SD, CMD1 for MMC)", 0 );
  if( !SendCommand( mmc, ret_cmd41, argument, resp ) ) {
    cmtDebug1( 8, 12, "DetectCard cmd41 fail", 0 );
    return false;
    }

  cmtDebug1( 8, 11, "DetectCard cmd41 success", 0 );
  
  ocr_recvd = ((mmc_resp_r3 *) resp)->ocr;

  while( !(ocr_recvd & MBIT31) && (retry_cnt > 0) ) {
    retry_cnt--;
    if( *sdCardType ) {
      argument = 0x0000 << 16;
      SendCommand( mmc, MMC_CMD55, argument, resp );
      }

    argument = ocr_value;
    if( !SendCommand( mmc, ret_cmd41, argument, resp ) ) {
      cmtDebug1( 8, 12, "DetectCard Loop cmd41 fail", 0 );
      return false;
      }
    ocr_recvd = ((mmc_resp_r3 *) resp)->ocr;
    }

  if( !(ocr_recvd & MBIT31) ) {
    cmtDebug1( 8, 14, "DetectCard ocr_recvd bit 31", 0 );
    return false;
    }

  if( !*sdCardType ) { //MMC_CARD
    if( (ocr_recvd & MMC_OCR_REG_ACCESS_MODE_MASK) == MMC_OCR_REG_ACCESS_MODE_SECTOR) {
      *factor = 0; //mmc_card_cur->mode = SECTOR_MODE;
      } 
    else {
      *factor = 9; //mmc_card_cur->mode = BYTE_MODE;
      }

    ocr_recvd &= ~MMC_OCR_REG_ACCESS_MODE_MASK;
    } 
  else {
    if( (ocr_recvd & MMC_OCR_REG_HOST_CAPACITY_SUPPORT_MASK) == MMC_OCR_REG_HOST_CAPACITY_SUPPORT_SECTOR ) {
      *factor = 0; //mmc_card_cur->mode = SECTOR_MODE;
      } 
    else {
      *factor = 9; //mmc_card_cur->mode = BYTE_MODE;
      }
    ocr_recvd &= ~MMC_OCR_REG_HOST_CAPACITY_SUPPORT_MASK;
    }

  ocr_recvd &= ~(0x1 << 31);
  if( !(ocr_recvd & ocr_value) ) {
    cmtDebug1( 8, 15, "DetectCard ocr_recvd bit ocr_value", 0 );
    return false;
    }

  cmtDebug1( 8, 16, "DetectCard CMD2", 0 );
  if( !SendCommand( mmc, MMC_CMD2, argument, resp ) ) {
    cmtDebug1( 8, 17, "DetectCard CMD2 fail", 0 );
    return false;
    }

  if( !*sdCardType ) { //mmc_card_cur->card_type == MMC_CARD
    argument = *RCA << 16;
    cmtDebug1( 8, 20, "DetectCard CMD3", 0 );
    if( !SendCommand( mmc, MMC_CMD3, argument, resp ) ) {
      cmtDebug1( 8, 21, "DetectCard CMD3 fail", 0 );
      return false;
      }
    } 
  else {
    argument = 0x00000000;
    cmtDebug1( 8, 22, "DetectCard SDCMD3", 0 );
    if( !SendCommand( mmc, MMC_SDCMD3, argument, resp ) ) {
      cmtDebug1( 8, 23, "DetectCard SDCMD3 fail", 0 );
      return false;
      }
    *RCA = ((mmc_resp_r6 *) resp)->newpublishedrca;
    }

  mmc->MMCHS_CON &= ~OD;
  mmc->MMCHS_CON |= NOOPENDRAIN;
  return true;
  }

#ifdef CMT_DMA
CmtSdCard* 
CmtSdCard::CreateCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect, CmtDmaBase *dmaRx, CmtDmaBase *dmaTx ) {
#else
CmtSdCard*
CmtSdCard::CreateCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect ) {
#endif
  uint8 MMCCmd[16];
  
  if( isConnect() != CMTE_OK ) return 0; //Карта не подключена

  bool   ret_val;
  bool   sdCardType;
  uint32 factor;
  uint32 RCA = 1;
  uint32 argument;
  uint32 resp[4];
  unsigned int trans_fact, trans_unit, retries = 2;
  unsigned int max_dtr;
  int dsor;
  volatile mmc_csd_reg_t Card_CSD;
  unsigned char trans_speed;

  cmtDebug1( 10, 1, "CreateCard init setup", 0 );
  InitSetup( mmc );

  do {
    cmtDebug1( 10, 2, "CreateCard detect card", 0 );
    ret_val = DetectCard( mmc, &sdCardType, &factor, &RCA );
    retries--;
    } 
  while( (retries > 0) && !ret_val );

  cmtDebug1( 10, 3, "CreateCard CMD9 RCA=%1", RCA );
  argument = RCA << 16;
  if( !SendCommand( mmc, MMC_CMD9, argument, resp ) ) {
    cmtDebug1( 10, 3, "CreateCard CMD9 fail", 0 );
    return 0;
    }

  memcpy( MMCCmd, resp, 16 );
  ((unsigned int *)&Card_CSD)[3] = resp[3];
  ((unsigned int *)&Card_CSD)[2] = resp[2];
  ((unsigned int *)&Card_CSD)[1] = resp[1];
  ((unsigned int *)&Card_CSD)[0] = resp[0];

//  if (mmc_card_cur->card_type == MMC_CARD)
//     mmc_card_cur->version = Card_CSD.spec_vers;

  trans_speed = Card_CSD.tran_speed;

  cmtDebug1( 10, 5, "CreateCard trans_speed %1", trans_speed );

  cmtDebug1( 10, 5, "CreateCard CMD4", 0 );
  if( !SendCommand( mmc, MMC_CMD4, MMC_DSR_DEFAULT << 16, resp ) ) {
    cmtDebug1( 10, 6, "CreateCard CMD4 fail", 0 );
    return 0;
    }
#if 1

  trans_unit = trans_speed & MMC_CSD_TRAN_SPEED_UNIT_MASK;
  trans_fact = trans_speed & MMC_CSD_TRAN_SPEED_FACTOR_MASK;

  if( trans_unit > MMC_CSD_TRAN_SPEED_UNIT_100MHZ )
    return 0;

  if( (trans_fact < MMC_CSD_TRAN_SPEED_FACTOR_1_0) ||
      (trans_fact > MMC_CSD_TRAN_SPEED_FACTOR_8_0) )
    return 0;

  trans_unit >>= 0;
  trans_fact >>= 3;

  max_dtr = tran_exp[trans_unit] * tran_mant[trans_fact];
  dsor = 96000000 / max_dtr;

  if( dsor == 4 )
    dsor = 5;
  if( dsor == 3 )
    dsor = 4;

  cmtDebug1( 10, 10, "CreateCard config dsor = %1", dsor );
  ClockConfig( mmc, CLK_MISC, 7 ); //dsor );
#endif

  cmtDebug1( 10, 11, "CreateCard CMD7", 0 );
  argument = RCA << 16;
  if( !SendCommand( mmc, MMC_CMD7_SELECT, argument, resp ) ) {
    cmtDebug1( 10, 12, "CreateCard CMD7 fail", 0 );
    return 0;
    }

  /* Configure the block length to 512 bytes */
  cmtDebug1( 10, 14, "CreateCard CMD16", 0 );
  if( !SendCommand( mmc, MMC_CMD16, 512, resp ) ) {
    cmtDebug1( 10, 15, "CreateCard CMD16 fail", 0 );
    return 0;
    }

  //Анализ размера карты (определение количества блоков)
  uint32 blockCount; //Количество блоков по 512 байт
  if( factor == 0 ) { //mmc_dev_data->mode == SECTOR_MODE
    if( sdCardType ) { //mmc_dev_data->card_type == SD_CARD
      blockCount = (((mmc_sd2_csd_reg_t *) &Card_CSD)->c_size_lsb & MMC_SD2_CSD_C_SIZE_LSB_MASK) |
            ((((mmc_sd2_csd_reg_t *) &Card_CSD)->c_size_msb & MMC_SD2_CSD_C_SIZE_MSB_MASK) << MMC_SD2_CSD_C_SIZE_MSB_OFFSET);
      blockCount *= 1024;
      } 
    else {
      mmc_extended_csd_reg_t ext_csd;
      argument = 0x00000000;
      cmtDebug1( 10, 16, "CreateCard CMD8", 0 );
      if( !SendCommand( mmc, MMC_CMD8, argument, resp ) ) {
        cmtDebug1( 10, 15, "CreateCard CMD8 fail", 0 );
        return 0;
        }
      if( !GetData( mmc, (uint32*)(&ext_csd) ) ) {
        cmtDebug1( 10, 16, "CreateCard CMD8 GetData fail", 0 );
        return 0;
        }
      blockCount = ext_csd.sectorcount;
      if( blockCount == 0 ) blockCount = 8388608;
      }
    }
  else {
    if( Card_CSD.c_size_mult >= 8 )
      return 0;

    if( Card_CSD.read_bl_len >= 12 )
      return 0;

     /* Compute size */
     uint32 count = 1 << (Card_CSD.c_size_mult + 2);
     uint32 card_size = (Card_CSD.c_size_lsb & MMC_CSD_C_SIZE_LSB_MASK) |
         ((Card_CSD.c_size_msb & MMC_CSD_C_SIZE_MSB_MASK) << MMC_CSD_C_SIZE_MSB_OFFSET);

     uint32 blk_no = (card_size + 1) * count;
     uint32 blk_len = 1 << Card_CSD.read_bl_len;
     uint32 size = blk_no * blk_len;

     blockCount = size >> 9;
     }

  cmtDebug1( 10, 21, "card blocks %1", blockCount );
  cmtDebug1( 10, 22, "card factor %1", factor );
  
  //Построить объект карты
#ifdef CMT_DMA
  return new CmtSdCard( mmc, isConnect, factor, 512, blockCount, dmaRx, dmaTx );
#else
  return new CmtSdCard( mmc, isConnect, factor, 512, blockCount );
#endif
  }

#if 0 
CmtSdDevPolling::CmtSdDevPolling( CmtSpiBase *spi, CmtSPIss cs, CmtSdIsConnect  isConnect ) :
  CmtDevPolling(),
  mSpi(spi),
  fnIsConnect(isConnect),
  mSdCard(0),
  fnCS(cs) {
  
  }

void
CmtSdDevPolling::Poll() {
  if( fnIsConnect() == CMTE_OK ) {
    //Карта подключена, проверить структуру SD
    if( mSdCard == 0 ) {
      //Структуры нет, формируем
      cmtWaitTick( 100 );
      if( fnIsConnect() != CMTE_OK ) return;
      mSdCard = CmtSpiSdCard::CreateCard( mSpi, fnCS, fnIsConnect );
      if( mSdCard ) OnConnect();
      }
    }
  else {
    //Подключения нет, проверить наличие структуры SD
    if( mSdCard ) {
      OnDisconnect();
      mSdCard->DecRef();
      mSdCard = 0;
      }
    }
  }

#ifdef CMT_FS
//! Событие, выполняемое при подключении карты (карта mSdCard)
void
CmtSdDevPolling::OnConnect() {
  //Карта создана, формировать логические ФС
  CmtFileSystemBase::CreateFileSystem( mSdCard, 0 );
  }
  
  //! Событие, выполняемое при отключении карты
void
CmtSdDevPolling::OnDisconnect() {
  //Убрать файловые системы, ассоциированные с диском
  int i;
  for( i = 0; i < CMT_FS; i++ )
    if( cmtLogDrives[i] && cmtLogDrives[i]->mDisk == mSdCard ) {
      //Файловая система от данного диска
      cmtLogDrives[i]->DecRef();
      cmtLogDrives[i] = 0;
      }
  }
#endif
#endif

//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   DMA
//==============================================================================
#ifndef TI_DMA_H
#define TI_DMA_H

#define DMA4_BASE    0x48056000

#define DMA4_IRQSTATUS_L(j)   (DMA4_BASE + 0x08 + (j) * 4)
#define DMA4_IRQENABLE_L(j)   (DMA4_BASE + 0x18 + (j) * 4)
#define DMA4_SYSSTATUS        (DMA4_BASE + 0x28)
#define DMA4_OCP_SYSCONFIG    (DMA4_BASE + 0x2C)
#define DMA4_CAPS_0           (DMA4_BASE + 0x64)
#define DMA4_CAPS_2           (DMA4_BASE + 0x6C)
#define DMA4_CAPS_3           (DMA4_BASE + 0x70)
#define DMA4_CAPS_4           (DMA4_BASE + 0x74)
#define DMA4_GCR              (DMA4_BASE + 0x78)

#define DMA4_REG( offset, i ) (DMA4_BASE + offset + (i) * 0x60)

#define DMA4_CCR(i)           DMA4_REG( 0x00000080, i )
#define DMA4_CLNK_CTRL(i)     DMA4_REG( 0x00000084, i )
#define DMA4_CICR(i)          DMA4_REG( 0x00000088, i )
#define DMA4_CSR(i)           DMA4_REG( 0x0000008C, i )
#define DMA4_CSDP(i)          DMA4_REG( 0x00000090, i )
#define DMA4_CEN(i)           DMA4_REG( 0x00000094, i )
#define DMA4_CFN(i)           DMA4_REG( 0x00000098, i )
#define DMA4_CSSA(i)          DMA4_REG( 0x0000009C, i )
#define DMA4_CDSA(i)          DMA4_REG( 0x000000A0, i )
#define DMA4_CSEI(i)          DMA4_REG( 0x000000A4, i )
#define DMA4_CSFI(i)          DMA4_REG( 0x000000A8, i )
#define DMA4_CDEI(i)          DMA4_REG( 0x000000AC, i )
#define DMA4_CDFI(i)          DMA4_REG( 0x000000B0, i )
#define DMA4_CSAC(i)          DMA4_REG( 0x000000B4, i )
#define DMA4_CDAC(i)          DMA4_REG( 0x000000B8, i )
#define DMA4_CCEN(i)          DMA4_REG( 0x000000BC, i )
#define DMA4_CCFN(i)          DMA4_REG( 0x000000C0, i )
#define DMA4_COLOR(i)         DMA4_REG( 0x000000C4, i )


#define DMA4_CHANNEL_FIELD(ch) MBIT( ((ch) >> 5), 19 ) | (ch & 0x1f)


#define S_DMA_SYS_DMA_REQ0     2 // External DMA request 0 (system expansion)
#define S_DMA_SYS_DMA_REQ1     3 // External DMA request 1 (system expansion)
#define S_DMA_GPMC_DMA GPMC    4 // request from prefetch engine
//#define S_DMA_4 Reserved Reserved
#define S_DMA_DSS_LINE_TRIGGER 6 // Display subsystem—frame update request
#define S_DMA__SYS_DMA_REQ2    7 // External DMA request 2 (system expansion)
//S_DMA_7 Reserved Reserved
//S_DMA_8 Reserved Reserved
//S_DMA_9 Reserved Reserved
//S_DMA_10 Reserved Reserved
//S_DMA_11 Reserved Reserved
//S_DMA_12 Reserved Reserved
//S_DMA_13 Reserved Reserved
#define S_DMA_SPI3_DMA_TX0    15 //McSPI module 3—transmit request channel 0
#define S_DMA_SPI3_DMA_RX0    16 //McSPI module 3—receive request channel 0
#define S_DMA_MCBSP3_DMA_TX   17 //MCBSP module 3—transmit request
#define S_DMA_MCBSP3_DMA_RX   18 //MCBSP module 3—receive request
#define S_DMA_MCBSP4_DMA_TX   19 //MCBSP module 4—transmit request
#define S_DMA_MCBSP4_DMA_RX   20 //MCBSP module 4—receive request
#define S_DMA_MCBSP5_DMA_TX   21 //MCBSP module 5—transmit request
#define S_DMA_MCBSP5_DMA_RX   22 //MCBSP module 5—receive request
#define S_DMA_SPI3_DMA_TX1    23 //McSPI module 3—transmit request channel 1
#define S_DMA_SPI3_DMA_RX1    24 //McSPI module 3—receive request channel 1
#define S_DMA_I2C3_DMA_TX     25 //I2C module 3—transmit request
#define S_DMA_I2C3_DMA_RX     26 //I2C module 3—receive request
#define S_DMA_I2C1_DMA_TX     27 //I2C module 1—transmit request
#define S_DMA_I2C1_DMA_RX     28 //I2C module 1—receive request
#define S_DMA_I2C2_DMA_TX     29 //I2C module 2—transmit request
#define S_DMA_I2C2_DMA_RX     30 //I2C module 2—receive request
#define S_DMA_MCBSP1_DMA_TX   31 //MCBSP module 1—transmit request
#define S_DMA_MCBSP1_DMA_RX   32 //MCBSP module 1—receive request
#define S_DMA_MCBSP2_DMA_TX   33 //MCBSP module 2—transmit request
#define S_DMA_MCBSP2_DMA_RX   34 //MCBSP module 2—receive request
#define S_DMA_SPI1_DMA_TX0    35 //McSPI module 1—transmit request channel 0
#define S_DMA_SPI1_DMA_RX0    36 //McSPI module 1—receive request channel 0
#define S_DMA_SPI1_DMA_TX1    37 //McSPI module 1—transmit request channel 1
#define S_DMA_SPI1_DMA_RX1    38 //McSPI module 1—receive request channel 1
#define S_DMA_SPI1_DMA_TX2    39 //McSPI module 1—transmit request channel 2
#define S_DMA_SPI1_DMA_RX2    40 //McSPI module 1—receive request channel 2
#define S_DMA_SPI1_DMA_TX3    41 //McSPI module 1—transmit request channel 3
#define S_DMA_SPI1_DMA_RX3    42 //McSPI module 1—receive request channel 3
#define S_DMA_SPI2_DMA_TX0    43 //McSPI module 2—transmit request channel 0
#define S_DMA_SPI2_DMA_RX0    44 //McSPI module 2—receive request channel 0
#define S_DMA_SPI2_DMA_TX1    45 //McSPI module 2—transmit request channel 1
#define S_DMA_SPI2_DMA_RX1    46 //McSPI module 2—receive request channel 1
#define S_DMA_MMC2_DMA_TX     47 //MMC/SD2 transmit request
#define S_DMA_MMC2_DMA_RX     48 //MMC/SD2 receive request
#define S_DMA_UART1_DMA_TX    49 //UART module 1—transmit request
#define S_DMA_UART1_DMA_RX    50 //UART module 1—receive request
#define S_DMA_UART2_DMA_TX    51 //UART module 2—transmit request
#define S_DMA_UART2_DMA_RX    52 //UART module 2—receive request
#define S_DMA_UART3_DMA_TX    53 //UART module 3—transmit request
#define S_DMA_UART3_DMA_RX    54 //UART module 3—receive request
//S_DMA_54 Reserved Reserved
//S_DMA_55 Reserved Reserved
//S_DMA_56 Reserved Reserved
//S_DMA_57 Reserved Reserved
//S_DMA_58 Reserved Reserved
//S_DMA_59 Reserved Reserved
#define S_DMA_MMC1_DMA_TX     61 //MMC/SD1 transmit request
#define S_DMA_MMC1_DMA_RX     62 //MMC/SD1 receive request
//S_DMA_62 Reserved Reserved
#define S_DMA_SYS_DMA_REQ3    64 //External DMA request 3 (system expansion)
//S_DMA_64 Reserved Reserved
//S_DMA_65 Reserved Reserved
//S_DMA_66 Reserved Reserved
//S_DMA_67 Reserved Reserved
//S_DMA_68 Reserved Reserved
#define S_DMA_SPI4_DMA_TX0    70 //McSPI module 4—transmit request channel 0
#define S_DMA_SPI4_DMA_RX0    71 //McSPI module 4—receive request channel 0
#define S_DMA_DSS_DMA0        72 //Display subsystem DMA request 0 (DSI)
#define S_DMA_DSS_DMA1        73 //Display subsystem DMA request 1 (DSI)
#define S_DMA_DSS_DMA2        74 //Display subsystem DMA request 2 (DSI)
#define S_DMA_DSS_DMA3        75 //Display subsystem DMA request 3 (DSI or RFBI)
//S_DMA_75 Reserved Reserved
#define S_DMA_MMC3_DMA_TX     77 //MMC/SD3 transmit request
#define S_DMA_MMC3_DMA_RX     78 //MMC/SD3 receive request
#endif


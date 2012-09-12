//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   UART/IrDA/CIR
//==============================================================================
#ifndef TI_UART_H
#define TI_UART_H

#define UART1_BASE 0x4806A000
#define UART2_BASE 0x4806C000
#define UART3_BASE 0x49020000

#define UART_DLL_REG        0x000
#define UART_RHR_REG        0x000
#define UART_THR_REG        0x000
#define UART_IER_REG        0x004
#define UART_DLH_REG        0x004
#define UART_FCR_REG        0x008
#define UART_IIR_REG        0x008
#define UART_EFR_REG        0x008
#define UART_LCR_REG        0x00c
#define UART_MCR_REG        0x010
#define UART_XON1_ADDR1_REG 0x010
#define UART_LSR_REG        0x014
#define UART_XON2_ADDR2_REG 0x014
#define UART_XOFF1_REG      0x018
#define UART_TCR_REG        0x018
#define UART_MSR_REG        0x018
#define UART_SPR_REG        0x01c
#define UART_XOFF2_REG      0x01c
#define UART_TLR_REG        0x01c
#define UART_MDR1_REG       0x020
#define UART_MDR2_REG       0x024
#define UART_TXFLL_REG      0x028
#define UART_SFLSR_REG      0x028
#define UART_RESUME_REG     0x02c
#define UART_TXFLH_REG      0x02c
#define UART_RXFLL_REG      0x030
#define UART_SFREGL_REG     0x030
#define UART_SFREGH_REG     0x034
#define UART_RXFLH_REG      0x034
#define UART_BLR_REG        0x038
#define UART_UASR_REG       0x038
#define UART_ACREG_REG      0x03c
#define UART_SCR_REG        0x040
#define UART_SSR_REG        0x044
#define UART_EBLR_REG       0x048
#define UART_MVR_REG        0x050
#define UART_SYSC_REG       0x054
#define UART_SYSS_REG       0x058
#define UART_WER_REG        0x05c
#define UART_CFPS_REG       0x060
 
#endif


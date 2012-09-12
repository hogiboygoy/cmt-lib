//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   General-Purpose Interface
//==============================================================================
#include <Sys/Core/Cpu/cmtCortexA8.h>
#include <Sys/Core/cmtType.h>
#include <Sys/Core/Support/cmtSupportTiOmap3530.h>

struct PRCM_AssocBases {
  uint32 mBase;
  uint32 FCLKEN;
  uint32 ICLKEN;
  uint32 mBit;
  };

static const PRCM_AssocBases prcmAssocBases[] = {
  // base aadr     fclken reg     iclken reg   bitmask
  { McBSP2_BASE,   CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT0   },
  { McBSP3_BASE,   CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT1   },
  { McBSP4_BASE,   CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT2   },
  { GPTIMER2_BASE, CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT3   },
  { GPTIMER3_BASE, CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT4   },
  { GPTIMER4_BASE, CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT5   },
  { GPTIMER5_BASE, CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT6   },
  { GPTIMER6_BASE, CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT7   },
  { GPTIMER7_BASE, CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT8   },
  { GPTIMER8_BASE, CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT9   },
  { GPTIMER9_BASE, CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT10   },
  { UART3_BASE,    CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT11   },
  { WD3_BASE,      CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT12   },
  { GPIO2_BASE,    CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT13   },
  { GPIO3_BASE,    CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT14   },
  { GPIO4_BASE,    CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT15   },
  { GPIO5_BASE,    CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT16   },
  { GPIO6_BASE,    CM_FCLKEN_PER, CM_ICLKEN_PER, MBIT17   },
  
  { GPTIMER1_BASE, CM_FCLKEN_WKUP, CM_ICLKEN_WKUP, MBIT0   },
  { GPIO1_BASE,    CM_FCLKEN_WKUP, CM_ICLKEN_WKUP, MBIT3   },
  { WD2_BASE,      CM_FCLKEN_WKUP, CM_ICLKEN_WKUP, MBIT5    },
  
  { McBSP1_BASE,    CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT9   },
  { McBSP5_BASE,    CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT10   },
  { GPTIMER10_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT11   },
  { GPTIMER11_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT12   },
  { UART1_BASE,     CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT13   },
  { UART2_BASE,     CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT14   },
  { I2C1_BASE,      CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT15   },
  { I2C2_BASE,      CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT16   },
  { I2C3_BASE,      CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT17   },
  { MCSPI1_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT18   },
  { MCSPI2_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT19   },
  { MCSPI3_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT20   },
  { MCSPI4_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT21   },
  { HDQ_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT22   },
  { MMCHS1_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT24    },
  { MMCHS2_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT25   },
  //{ , 0, CM_ICLKEN1_CORE,    },
  { MMCHS3_BASE, CM_FCLKEN1_CORE, CM_ICLKEN1_CORE, MBIT30   },
  { 0,0,0,0 } };


void 
PRCM_Clock( uint32 base, bool enable ) {
  for( int i = 0; prcmAssocBases[i].mBase; i++ ) {
    if( prcmAssocBases[i].mBase == base ) {
      if( enable ) {
        __raw_setl( prcmAssocBases[i].mBit, prcmAssocBases[i].FCLKEN );
        __raw_setl( prcmAssocBases[i].mBit, prcmAssocBases[i].ICLKEN );
        }
      else {
        __raw_clearl( prcmAssocBases[i].mBit, prcmAssocBases[i].FCLKEN );
        __raw_clearl( prcmAssocBases[i].mBit, prcmAssocBases[i].ICLKEN );
        }
      return;
      }
    }
  }

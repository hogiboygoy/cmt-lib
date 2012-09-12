//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

void
cmtInitSystem() {
  ErrorStatus HSEStartUpStatus;
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if( HSEStartUpStatus == SUCCESS ) {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable );

    /* Flash 2 wait state */
    FLASH_SetLatency( FLASH_Latency_2 );
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig( RCC_SYSCLK_Div1 ); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config( RCC_HCLK_Div1 ); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config( RCC_HCLK_Div2 );

    /* Configure PLLs *********************************************************/
    /* PLL2 configuration: PLL2CLK = (HSE / 1) * 8 = 40 MHz */
    RCC_PREDIV2Config( RCC_PREDIV2_Div1 );
    RCC_PLL2Config( RCC_PLL2Mul_8 );

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET);

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
    RCC_PREDIV1Config( RCC_PREDIV1_Source_HSE, RCC_PREDIV1_Div1 );
    RCC_PLLConfig( RCC_PLLSource_PREDIV1, RCC_PLLMul_9 );

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);
    }
  else {
    /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1);
    }

  }

//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
static uint32 cmtSmMoveUp;        //Количество шагов шагового двигателя при разгоне
static uint32 cmtSmMoveDown;      //Количество шагов шагового двигателя при торможении
static uint32 cmtSmMove;          //Количество шагов шагового двигателя при рабочем ходе
static uint32 cmtSmPeriodDelta;   //Изменение периода на один шаг при разгоне и торможении
static uint32 cmtSmPeriod;        //Рабочий период
static uint32 cmtSmMinPeriod;     //Минимальный период шага (для рабочего хода)
static uint32 cmtSmMoveCount;     //Общее количество шагов
static uint32 cmtSmDriveMask;     //битовая маска двигателей (обеспечивает одновременное управление двигателями)
static uint32 cmtSmCheckMask;     //битовая маска датчиков контроля
static uint_8 cmtSmResult;

#define STF 10000
#define STFA 1000   //1000
// steps - количество шагов
// stepsUpDown - количество шагов на разгон и торможение
// speed - максимальная скорость подачи импульсов (в Гц)
// pulse - функция, подающая один импульс
uint_8 
cmtStepMotor( uint32 steps, uint32 speed, uint32 stepsUpDown, uint32 driveMask, uint32 checkMask ) {
  TIM_SetCounter( CMT_STEP_MOTOR, 0 ); //Избежать прерывания, пока идет расчет параметров
  cmtSmMinPeriod = 400000 / speed;
  if( stepsUpDown )  cmtSmPeriodDelta = ((STFA - cmtSmMinPeriod) << 8) / stepsUpDown;
#ifdef MMM  
  cmtSmMoveUp = stepsUpDown;
  cmtSmMove = 0;
  cmtSmMoveDown = 0;
#endif
  if( steps < stepsUpDown * 2 ) {
    cmtSmMoveUp = steps >> 1;            //Количество шагов на разгон
    cmtSmMoveDown = steps - cmtSmMoveUp; //Количество шагов на торможение
    cmtSmMove = 0;
    }
  else {
    cmtSmMoveUp = cmtSmMoveDown = stepsUpDown;
    cmtSmMove = steps - 2 * stepsUpDown;
    }
  cmtSmPeriod = STFA << 8;
  cmtSmMoveCount = steps;
  cmtSmDriveMask = driveMask;
  cmtSmCheckMask = checkMask;
  cmtSmResult = CMTE_OK;
  
  cmtWaitClearUInt32( &cmtSmMoveCount );
  
  return cmtSmResult;
  }

#ifdef __cplusplus
extern "C" {
#endif

//Прерывание таймера формирующего управление двигателями
void CMT_STEP_MOTOR_IRQ_HANDLER(void) {
  static uint32 divider;
  if( cmtSmMoveUp ) {
    if( divider ) {
      cmtSmMoveUp--;
      cmtSmMoveCount--;
      TIM_SetCounter( CMT_STEP_MOTOR, STF - (cmtSmPeriod >> 8) );
      cmtSmPeriod -= cmtSmPeriodDelta;
      cmtSmResult = cmtStepMotorPulseCallback( cmtSmDriveMask, cmtSmCheckMask );
      }
    else TIM_SetCounter( CMT_STEP_MOTOR, STF - (cmtSmPeriod >> 8) );
    }
  else if( cmtSmMove ) {
    if( divider ) {
      cmtSmMove--;
      cmtSmMoveCount--;
      cmtSmResult = cmtStepMotorPulseCallback( cmtSmDriveMask, cmtSmCheckMask );
      }
    TIM_SetCounter( CMT_STEP_MOTOR, STF - cmtSmMinPeriod );
    }
  else if( cmtSmMoveDown ) {
    if( divider ) {
      cmtSmMoveDown--;
      cmtSmMoveCount--;
      TIM_SetCounter( CMT_STEP_MOTOR, STF - (cmtSmPeriod >> 8) );
      cmtSmPeriod += cmtSmPeriodDelta;
      cmtSmResult = cmtStepMotorPulseCallback( cmtSmDriveMask, cmtSmCheckMask );
      }
    else TIM_SetCounter( CMT_STEP_MOTOR, STF - (cmtSmPeriod >> 8) );
    }
  if( !divider )
    cmtSmResult = cmtStepMotorPulseCallback( 0, 0 );

  divider = !divider;
  TIM_ClearITPendingBit(CMT_STEP_MOTOR,TIM_IT_Update);
  }

#ifdef __cplusplus
 };
#endif

void 
cmtStepMotorInit() {
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  //Базовая инициализация таймера 
  TIM_TimeBaseStructure.TIM_Prescaler = 90 - 1; //400 кГц 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = STF; //Максимальный период шага
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(CMT_STEP_MOTOR, &TIM_TimeBaseStructure);
  
  /* TIM IT enable */
  TIM_ITConfig(CMT_STEP_MOTOR, TIM_IT_Update, ENABLE);
  
  //Разрешим таймер
  TIM_Cmd(CMT_STEP_MOTOR, ENABLE);
  
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = CMT_STEP_MOTOR_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  }

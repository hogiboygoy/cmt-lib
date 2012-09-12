//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
static uint32 cmtSmMoveUp;        //���������� ����� �������� ��������� ��� �������
static uint32 cmtSmMoveDown;      //���������� ����� �������� ��������� ��� ����������
static uint32 cmtSmMove;          //���������� ����� �������� ��������� ��� ������� ����
static uint32 cmtSmPeriodDelta;   //��������� ������� �� ���� ��� ��� ������� � ����������
static uint32 cmtSmPeriod;        //������� ������
static uint32 cmtSmMinPeriod;     //����������� ������ ���� (��� �������� ����)
static uint32 cmtSmMoveCount;     //����� ���������� �����
static uint32 cmtSmDriveMask;     //������� ����� ���������� (������������ ������������� ���������� �����������)
static uint32 cmtSmCheckMask;     //������� ����� �������� ��������
static uint_8 cmtSmResult;

#define STF 10000
#define STFA 1000   //1000
// steps - ���������� �����
// stepsUpDown - ���������� ����� �� ������ � ����������
// speed - ������������ �������� ������ ��������� (� ��)
// pulse - �������, �������� ���� �������
uint_8 
cmtStepMotor( uint32 steps, uint32 speed, uint32 stepsUpDown, uint32 driveMask, uint32 checkMask ) {
  TIM_SetCounter( CMT_STEP_MOTOR, 0 ); //�������� ����������, ���� ���� ������ ����������
  cmtSmMinPeriod = 400000 / speed;
  if( stepsUpDown )  cmtSmPeriodDelta = ((STFA - cmtSmMinPeriod) << 8) / stepsUpDown;
#ifdef MMM  
  cmtSmMoveUp = stepsUpDown;
  cmtSmMove = 0;
  cmtSmMoveDown = 0;
#endif
  if( steps < stepsUpDown * 2 ) {
    cmtSmMoveUp = steps >> 1;            //���������� ����� �� ������
    cmtSmMoveDown = steps - cmtSmMoveUp; //���������� ����� �� ����������
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

//���������� ������� ������������ ���������� �����������
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

  //������� ������������� ������� 
  TIM_TimeBaseStructure.TIM_Prescaler = 90 - 1; //400 ��� 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = STF; //������������ ������ ����
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(CMT_STEP_MOTOR, &TIM_TimeBaseStructure);
  
  /* TIM IT enable */
  TIM_ITConfig(CMT_STEP_MOTOR, TIM_IT_Update, ENABLE);
  
  //�������� ������
  TIM_Cmd(CMT_STEP_MOTOR, ENABLE);
  
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = CMT_STEP_MOTOR_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  }

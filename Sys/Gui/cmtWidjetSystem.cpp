//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

/*Функция устанавливает главный (корневой) виджет в систему управления виджетами.
  При этом предыдущий главный виджет может удаляться (осуществляя замещение) или
  не удаляться и возвращаться для хранения. Тем самым можно организовать вызов
  и возврат из вызова.
  */
void
CmtWidjetSystem::SetWidjet(CmtWidjet *widjet, bool prevDelete) {
  //Проверить необходимость удаления
  if( prevDelete ) {
    if( widjet && mWidjet && widjet != mWidjet->Calling() )
      widjet->SetCalling( mWidjet->Calling() );
    //Проверить возможность удаления и выполнить удаление
    if( mWidjet && mWidjet != widjet ) {
      if( mDelete ) delete mDelete;
      mDelete = mWidjet;
      }
    }
  else {
    if( mWidjet )
      widjet->SetCalling( mWidjet );
    }
  //Назначить новый виджет
  mWidjet = widjet;
  
  if( mWidjet ) {
    //Для вновь-назначенного виджета выполнить назначение фокуса ввода и перерисовку
    mWidjet->SetDirty();
    }
  }

void
CmtWidjetSystem::Loop() {
  if( mScreen ) {
    //Ожидать завершения фрейма
    mScreen->WaitEndOfFrame();
    
    if( mWidjet ) {
      //Проверить наличие виджета для удаления
      if( mDelete ) {
        delete mDelete;
        mDelete = 0;
        }

      //Оповестить о смене фрейма
      mWidjet->NextFrame();

      //Проверить наличие виджета для удаления
      if( mDelete ) {
        delete mDelete;
        mDelete = 0;
        }

      //Если требуется перерисовка - перерисовать
      if( mWidjet->Flags() & CMT_WF_DIRTY ) {
        //Виджет должен нарисовать на экране
        mWidjet->Paint( mScreen );
        //Известить экран о завершении рисования.
        //Здесь экран производит смену буферов или осуществляет передачу из буфера экрана в контроллер
        mScreen->FrameComplete();
        }
      }
    }
  }


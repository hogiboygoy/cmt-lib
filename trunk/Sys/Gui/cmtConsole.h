//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_CONSOLE_H
#define CMT_CONSOLE_H

class CmtConsole {
    CmtContext *mScreen;  //!< �����
    char       *mConsole; //!< ������ ������ ��� �������� ��������
    CmtPoint    mSize;    //!< ������ ������ � ��������
  public:
    
    CmtConsole( CmtContext *screen, CmtPoint sizeInSymbols );
    
    //! ������ ������ ����� ������
    void PrintS_W( cpchar str );
    
    //! ������ ���������� �����
    void PrintS( cpchar str );
    
    //! ���������� �����
    void Scroll();
    
    //! �������� �����
    void Update();
    
    void PrintAndUpdate( cpchar str );
  };

#endif

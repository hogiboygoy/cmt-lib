//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_IMAGE_H
#define CMT_IMAGE_H

#define CIS(x) ((uint32*)(x))

class CmtImage {
public:
  bool      mDinamic;    //!< false - ���������� ��������������
  uint32    mFrameCount; //!< ���������� ������ ��� �������������� �����������
  uint32   *mFrames;     //!< ����� ������ (������ ����� - ����� ���������� � ��)
  uint32    mFrameSize;  //!< ������ ������ ����� (� uint32) (������ ����� - ����� ���������� � ��)
  CmtPoint  mSize;       //!< ����������� ������ ��������

  CmtImage( uint32 *src = 0 );
  CmtImage( CmtImage *src );
  ~CmtImage();

  void    Setup( uint32 *src );
  
  void    Alloc( CmtPoint size, uint32 frameCount = 1 );
  
  CmtPoint Size() const { return mSize; }
  
  uint32   ExpositionTime( uint32 frameIndex ) const { return frameIndex < mFrameCount ? mFrames[mFrameSize * frameIndex] : 0; }
  
#ifdef CMT_SYS_TIMER_MAP
  uint32   ExpositionTickTime(uint32 frameIndex) const {
    //�������� ����� ����������
    frameIndex = ExpositionTime( frameIndex );
    //������� ��������� �����
    return cmtTickCount + frameIndex == 0 ? 0x70000000 : frameIndex;
    }
#endif
  
  uint32   FrameSize() const { return mFrameSize; }
  
  uint32   FrameCount() const { return mFrameCount; }
  
  uint32   NextFrameIndex( uint32 frameIndex ) const { return frameIndex + 1 >= mFrameCount ? 0 : frameIndex + 1; }
  
  uint32*  Frame( uint32 frameIndex = 0 ) { return frameIndex < mFrameCount ? mFrames + 1 + mFrameSize * frameIndex : 0; }
  
  uint32*  FrameData( CmtPoint pos, uint32 frameIndex = 0 );

  bool LoadBMP( CmtFile &is );
  bool LoadGIF( CmtFile &is );
  bool LoadJPG( CmtFile &is );
  bool LoadCmt( CmtFile &is );

#ifdef CMT_IMAGE_SAVE
  bool SaveCmt( CmtFile &os );
#endif

  bool Load( cpchar fileName );
  void CopyFrom( CmtImage *src, bool noRef = false ); //�� �������� ������ ������ ���������� ��������
  void Free(); //���������� �������, ���������� ���������
  bool Empty() const { return mFrames == 0; }
  void CreateShadow(); //������� �� �������� ����������� ����� ������
  bool GetFromClipBoard();
  bool PutOnClipBoard( uint32 frame = 0 );

  CmtImage* DefLoad( cpchar fileName ); //��������� �� ����� �� ��������� �������� � ������� ��������� �� ����

  static CmtPath mDefPath;
  };

typedef CmtImage *CmtImagePtr;
#endif

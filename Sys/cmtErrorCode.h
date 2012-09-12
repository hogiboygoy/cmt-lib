//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
/**
 * \defgroup errorCode ���� ������ �������
 * @{
 */
//! ����� ������
#define CMTE_OK                                            0 //!< �������� ����������
#define CMTE_FAIL                                          1 //!< ����� ������ ��������
#define CMTE_NOT_RELEASED                                  2 //!< ��������� ������� �� �����������
#define CMTE_NOT_INIT                                      3 //!< ���������� �� ����������������
#define CMTE_BAD_PARAM                                     4 //!< ������������ �������� ��� ������
#define CMTE_TIME_OUT                                      5 //!< ����-��� ��������
#define CMTE_BREAK                                         6 //!< ���������� �� �������� �������
#define CMTE_EOF                                           7 //!< ����� �����
#define CMTE_WRONG_VERSION                                 8 //!< ���������������� ������
#define CMTE_BAD_TYPE                                      9 //!< �������� (����������������) ���
#define CMTE_BAD_STRUCT                                   10 //!< ��������� ��������� ������

//! ����������� ��� ���������
#define CMTE_BD_NO_DEV                                    30 //!< ���������� �����������
#define CMTE_BD_READ                                      31 //!< ������ ������ ����������

//! ��� �������� ������
//! ����������� ���� ������ �������� ������
#define CMTE_FS_NOT_IDENTIFICAL                           40 //!< �������������� �������� ������ �� ����� �������� �������, � ����� ������
#define CMTE_FS_STRUCT                                    41 //!< ��������� �������� ���������
#define CMTE_FS_DISK_FULL                                 42 //!< ���� ��������
#define CMTE_FS_FILE_STRUCT                               43 //!< ��������� ��������� �����
#define CMTE_FS_COUNT_OVER                                44 //!< ���������� ���������� ���������� ���������� ���������
#define CMTE_FS_UNSUPPORTED                               45 //!< ���������������� �������� �������
#define CMTE_FS_MBR_READ                                  46 //!< ������ ������ ������� MBR
#define CMTE_FS_MBR_FAIL                                  47 //!< ������ ������� MBR
#define CMTE_FS_BOOT_FAIL                                 48 //!< ��� ��������� BOOT �������
#define CMTE_FS_DIR_PRESENT                               49 //!< ���������� � �������� ������� ��� �������
#define CMTE_FS_NO_FILE                                   50 //!< ��� ����� � �������� ������
#define CMTE_FS_FILE_ACCESS                               51 //!< ������ ������� � �����

//! ���� ��� SD ����
#define CMTE_SD_READ_BLOCK_TIMEOUT                        60 //!< ����-��� �������� ������ ����� ����� �������� ������� ������ �����
#define CMTE_SD_BLOCK_DATA_TOKEN_MISSING                  61 //!< ��� ������� ������
#define CMTE_SD_WRITE_BLOCK_TIMEOUT                       62 //!< ����-��� �������� ������ ����� ����� �������� ������� ������ �����
#define CMTE_SD_WRITE_BLOCK_FAIL                          63 //!< ������ ������ �����

//������������ ���������������� ����� (�� ������������ � �������)
//���� � �������� ���������� ����� �������������� ���������� �������� ���
//��������� ���������� ��������
#define CMTE_USER                                        128 //!< ������������ ���������������� �����

/**
 * @}
 */

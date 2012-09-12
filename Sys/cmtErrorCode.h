//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
/**
 * \defgroup errorCode Коды ошибок системы
 * @{
 */
//! Общие ошибки
#define CMTE_OK                                            0 //!< Успешное завершение
#define CMTE_FAIL                                          1 //!< Общая ошибка операции
#define CMTE_NOT_RELEASED                                  2 //!< Требуемая функция не реализована
#define CMTE_NOT_INIT                                      3 //!< Устройство не инициализировано
#define CMTE_BAD_PARAM                                     4 //!< Недопустимый параметр при вызове
#define CMTE_TIME_OUT                                      5 //!< Тайм-аут ожидания
#define CMTE_BREAK                                         6 //!< Прекращено по внешнему событию
#define CMTE_EOF                                           7 //!< Конец файла
#define CMTE_WRONG_VERSION                                 8 //!< Неподдерживаемая версия
#define CMTE_BAD_TYPE                                      9 //!< Неверный (неподдерживаемый) тип
#define CMTE_BAD_STRUCT                                   10 //!< Нарушение структуры данных

//! Специфичные для устройств
#define CMTE_BD_NO_DEV                                    30 //!< Устройство отсутствует
#define CMTE_BD_READ                                      31 //!< Ошибка чтения устройства

//! Для файловых систем
//! Специфичные коды ошибок файловых систем
#define CMTE_FS_NOT_IDENTIFICAL                           40 //!< Переименование возможно только на одной файловой системе, а здесь разные
#define CMTE_FS_STRUCT                                    41 //!< Нарушение файловой структуры
#define CMTE_FS_DISK_FULL                                 42 //!< Диск заполнен
#define CMTE_FS_FILE_STRUCT                               43 //!< Нарушение структуры файла
#define CMTE_FS_COUNT_OVER                                44 //!< Превышение количества допустимых логических устройств
#define CMTE_FS_UNSUPPORTED                               45 //!< Неподдерживаемая файловая система
#define CMTE_FS_MBR_READ                                  46 //!< Ошибка чтения сектора MBR
#define CMTE_FS_MBR_FAIL                                  47 //!< Ошибка разбора MBR
#define CMTE_FS_BOOT_FAIL                                 48 //!< Нет сигнатуры BOOT сектора
#define CMTE_FS_DIR_PRESENT                               49 //!< Директорий с заданным имененм уже имеется
#define CMTE_FS_NO_FILE                                   50 //!< Нет файла с заданным именем
#define CMTE_FS_FILE_ACCESS                               51 //!< Ошибка доступа к файлу

//! Коды для SD карт
#define CMTE_SD_READ_BLOCK_TIMEOUT                        60 //!< Тайм-аут ожидания ответа карты после отправки команды чтения блока
#define CMTE_SD_BLOCK_DATA_TOKEN_MISSING                  61 //!< Нет маркера данных
#define CMTE_SD_WRITE_BLOCK_TIMEOUT                       62 //!< Тайм-аут ожидания ответа карты после отправки команды записи блока
#define CMTE_SD_WRITE_BLOCK_FAIL                          63 //!< Ошибка записи блока

//Пространство пользовательских кодов (не используются в системе)
//Коды с большими значениями могут использоваться различными задачами для
//индикации завершения процедур
#define CMTE_USER                                        128 //!< Пространство пользовательских кодов

/**
 * @}
 */

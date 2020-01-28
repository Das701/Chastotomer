#pragma once
#include "stm32f4xx_hal_def.h"


#define FLASH_LATENCY_0    0   /*!< FLASH Zero Latency cycle      */
#define FLASH_LATENCY_1    0   /*!< FLASH One Latency cycle       */
#define FLASH_LATENCY_2    0   /*!< FLASH Two Latency cycles      */
#define FLASH_LATENCY_3    0   /*!< FLASH Three Latency cycles    */
#define FLASH_LATENCY_4    0   /*!< FLASH Four Latency cycles     */
#define FLASH_LATENCY_5    0   /*!< FLASH Five Latency cycles     */
#define FLASH_LATENCY_6    0   /*!< FLASH Six Latency cycles      */
#define FLASH_LATENCY_7    0   /*!< FLASH Seven Latency cycles    */
#define FLASH_LATENCY_8    0   /*!< FLASH Eight Latency cycles    */
#define FLASH_LATENCY_9    0   /*!< FLASH Nine Latency cycles     */
#define FLASH_LATENCY_10   0   /*!< FLASH Ten Latency cycles      */
#define FLASH_LATENCY_11   0   /*!< FLASH Eleven Latency cycles   */
#define FLASH_LATENCY_12   0   /*!< FLASH Twelve Latency cycles   */
#define FLASH_LATENCY_13   0   /*!< FLASH Thirteen Latency cycles */
#define FLASH_LATENCY_14   0   /*!< FLASH Fourteen Latency cycles */
#define FLASH_LATENCY_15   0   /*!< FLASH Fifteen Latency cycles  */

#define FLASH_SECTOR_0     0U  /*!< Sector Number 0   */
#define FLASH_SECTOR_1     1U  /*!< Sector Number 1   */
#define FLASH_SECTOR_2     2U  /*!< Sector Number 2   */
#define FLASH_SECTOR_3     3U  /*!< Sector Number 3   */
#define FLASH_SECTOR_4     4U  /*!< Sector Number 4   */
#define FLASH_SECTOR_5     5U  /*!< Sector Number 5   */
#define FLASH_SECTOR_6     6U  /*!< Sector Number 6   */
#define FLASH_SECTOR_7     7U  /*!< Sector Number 7   */
#define FLASH_SECTOR_8     8U  /*!< Sector Number 8   */
#define FLASH_SECTOR_9     9U  /*!< Sector Number 9   */
#define FLASH_SECTOR_10    10U /*!< Sector Number 10  */
#define FLASH_SECTOR_11    11U /*!< Sector Number 11  */
#define FLASH_SECTOR_12    12U /*!< Sector Number 12  */
#define FLASH_SECTOR_13    13U /*!< Sector Number 13  */
#define FLASH_SECTOR_14    14U /*!< Sector Number 14  */
#define FLASH_SECTOR_15    15U /*!< Sector Number 15  */
#define FLASH_SECTOR_16    16U /*!< Sector Number 16  */
#define FLASH_SECTOR_17    17U /*!< Sector Number 17  */
#define FLASH_SECTOR_18    18U /*!< Sector Number 18  */
#define FLASH_SECTOR_19    19U /*!< Sector Number 19  */
#define FLASH_SECTOR_20    20U /*!< Sector Number 20  */
#define FLASH_SECTOR_21    21U /*!< Sector Number 21  */
#define FLASH_SECTOR_22    22U /*!< Sector Number 22  */
#define FLASH_SECTOR_23    23U /*!< Sector Number 23  */


#define FLASH_TYPEERASE_SECTORS         0x00000000U  /*!< Sectors erase only          */

#define FLASH_VOLTAGE_RANGE_1        0x00000000U  /*!< Device operating range: 1.8V to 2.1V                */
#define FLASH_VOLTAGE_RANGE_2        0x00000001U  /*!< Device operating range: 2.1V to 2.7V                */
#define FLASH_VOLTAGE_RANGE_3        0x00000002U  /*!< Device operating range: 2.7V to 3.6V                */
#define FLASH_VOLTAGE_RANGE_4        0x00000003U  /*!< Device operating range: 2.7V to 3.6V + External Vpp */


typedef struct
{
    uint32_t TypeErase;   /*!< Mass erase or sector Erase.
                          This parameter can be a value of @ref FLASHEx_Type_Erase */

    uint32_t Banks;       /*!< Select banks to erase when Mass erase is enabled.
                          This parameter must be a value of @ref FLASHEx_Banks */

    uint32_t Sector;      /*!< Initial FLASH sector to erase when Mass erase is disabled
                          This parameter must be a value of @ref FLASHEx_Sectors */

    uint32_t NbSectors;   /*!< Number of sectors to be erased.
                          This parameter must be a value between 1 and (max number of sectors - value of Initial sector)*/

    uint32_t VoltageRange;/*!< The device voltage range which defines the erase parallelism
                          This parameter must be a value of @ref FLASHEx_Voltage_Range */

} FLASH_EraseInitTypeDef;


HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError);

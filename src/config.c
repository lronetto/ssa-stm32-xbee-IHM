#include "stm32f10x_conf.h"
#include "config.h"
#include <string.h>

#define FLASH_PAGE_COUNT 128

#define FLASH_PAGE_SIZE                 ((uint16_t)0x200)

// use the last KB for sensor config storage
#define FLASH_WRITE_ADDR  (0x08000000 + (uint32_t)FLASH_PAGE_SIZE * (FLASH_PAGE_COUNT - 1))



void EE_Write(EE_t EEP){
	FLASH_Status status;
	uint32_t i;

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	if (FLASH_ErasePage(FLASH_WRITE_ADDR) == FLASH_COMPLETE) {
		for (i = 0; i < sizeof(EE_t); i += 4) {
			status = FLASH_ProgramWord(FLASH_WRITE_ADDR + i, *(uint32_t *) ((char *) &EEP + i));
			if (status != FLASH_COMPLETE)
				break;          // TODO: fail
	        }
	    }

	    FLASH_Lock();
}
void EE_Read(EE_t *EEP){
	 memcpy(EEP, (char *) FLASH_WRITE_ADDR, sizeof(EE_t));
}



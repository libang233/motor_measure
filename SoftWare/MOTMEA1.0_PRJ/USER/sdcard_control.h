#ifndef __SDCARD_CONTROL_H
#define __SDCARD_CONTROL_H

#include "config.h"
#include "newtype.h"

//
//index文件数据定义
//
#define INDEX_DATA_MAX_BYTE								1115

#define INDEX_DATA_HEAD_DEF								0xaa			
#define INDEX_DATA_HEAD_ADDRESS							0
#define INDEX_DATA_ADJUST_CUR_BYTE_NUM_ADDRESS			1
#define INDEX_DATA_ADJUST_CUR_ADDRESS_HIGH				2
#define INDEX_DATA_PASSWORD_BYTE_NUM_ADDRESS			22
#define INDEX_DATA_PASSWORD_ADDRESS_HIGH				23
#define INDEX_DATA_PASSWORD_TIP_BYTE_NUM_ADDRESS		43
#define INDEX_DATA_PASSWORD_TIP_ADDRESS_HIGH			44
#define INDEX_DATA_PN_NUM_QUANT_ADDRESS					64
#define INDEX_DATA_PN_NUM_BYTE_NUM_ADDRESS				65
#define INDEX_DATA_PN_NUM_ADDRESS_HIGH					66

#define INDEX_DATA_ONE_PN_SIZE							21

//
//料号文件数据定义
//
#define PN_FILE_MAX_BYTE								236

#define	PN_FILE_PN_IDX_ADDRESS							0		
#define	PN_FILE_PN_NUM_ADDRESS_HIGH						1
#define	PN_FILE_PN_DES_IDX_ADDRESS						21
#define PN_FILE_PN_DES_ADDRESS_HIGH						22
#define	PN_FILE_PART_NUM_ADDRESS						42
#define	PN_FILE_POLES_ADDRESS							43
#define	PN_FILE_CURRENT_ADDRESS_HIGH					44
#define	PN_FILE_CURRENT_ADDRESS_LOW						45
#define PN_FILE_PWM_FREQUN_ADDRESS_HIGH					46
#define PN_FILE_PWM_FREQUN_ADDRESS_LOW					47	
#define	PN_FILE_POWER_ADDRESS							48
#define	PN_FILE_TRIGGER_ADDERSS							49
#define	PN_FILE_PART1_EXPLAIN_IDX_ADDRESS				50
#define	PN_FILE_PART1_EXPLAIN_ADDRESS_HIGH				51
#define	PN_FILE_PART1_PWM_ADDRESS						71
#define	PN_FILE_PART1_TIME_ADDRESS						72
#define PN_FILE_PART1_SPEED_UP_ADDRESS_HIGH				73	
#define PN_FILE_PART1_SPEED_UP_ADDRESS_LOW				74
#define PN_FILE_PART1_SPEED_DOWN_ADDRESS_HIGH			75
#define PN_FILE_PART1_SPEED_DOWN_ADDRESS_LOW			76
#define PN_FILE_PART1_CURE_UP_ADDRESS_HIGH				77	
#define PN_FILE_PART1_CURE_UP_ADDRESS_LOW				78
#define PN_FILE_PART1_CURE_DOWN_ADDRESS_HIGH			79
#define PN_FILE_PART1_CURE_DOWN_ADDRESS_LOW				80


#define PN_FILE_ONE_PART_SIZE							31

#define PN_FILE_NAME_SIZE_MAX							10							

//
//料号模板
//
#define MODLE_PN_POSITION								0

#define MODLE_PN_NUM_BYTE_NUM							18
#define MODLE_PN_NUM 									"modle pwm fan text"
#define MODLE_PN_DESCRIP_BYTE_NUM						7
#define MODLE_PN_DESCRIP								"pwm fan"
#define	MODLE_PN_POLES_NUM								4
#define MODLE_PN_PART_NUM								4
#define MODLE_PN_TEST_POWER								2
#define	MODLE_PN_TRIGGER								1
#define MODLE_PN_PWM_FREQUEN							25000
#define MODLE_PN_CUR									5

#define MODLE_PN_PART1_IDX								4
#define MODLE_PN_PART1_EXPLAIN							"HAHA"
#define MODLE_PN_PART1_PWM								50
#define MODLE_PN_PART1_TIME								10
#define MODLE_PN_PART1_SPEED_UP							20000
#define MODLE_PN_PART1_SPEED_DOWN						10000
#define MODLE_PN_PART1_CURE_UP							2000
#define MODLE_PN_PART1_CURE_DOWN						1000

#define MODLE_PN_PART2_IDX								4
#define MODLE_PN_PART2_EXPLAIN							"HAHA"
#define MODLE_PN_PART2_PWM								50
#define MODLE_PN_PART2_TIME								10
#define MODLE_PN_PART2_SPEED_UP							20000
#define MODLE_PN_PART2_SPEED_DOWN						10000
#define MODLE_PN_PART2_CURE_UP							2000
#define MODLE_PN_PART2_CURE_DOWN						1000

#define MODLE_PN_PART3_IDX								4
#define MODLE_PN_PART3_EXPLAIN							"HAHA"
#define MODLE_PN_PART3_PWM								50
#define MODLE_PN_PART3_TIME								10
#define MODLE_PN_PART3_SPEED_UP							20000
#define MODLE_PN_PART3_SPEED_DOWN						10000
#define MODLE_PN_PART3_CURE_UP							2000
#define MODLE_PN_PART3_CURE_DOWN						1000

#define MODLE_PN_PART4_IDX								4
#define MODLE_PN_PART4_EXPLAIN							"HAHA"
#define MODLE_PN_PART4_PWM								50
#define MODLE_PN_PART4_TIME								10
#define MODLE_PN_PART4_SPEED_UP							20000
#define MODLE_PN_PART4_SPEED_DOWN						10000
#define MODLE_PN_PART4_CURE_UP							2000
#define MODLE_PN_PART4_CURE_DOWN						1000

//
//初始密码设置
//
#define INIT_PASSWORD_IDX								8
#define INIT_PASSWORD									"12345678"
#define INIT_PASSWORD_TIP_IDX							4
#define INIT_PASSWORD_TIP								"init"


typedef struct
{
	u8 sdcardStatus;		//SD卡状态
	u8 fileStatus;			//SD卡文件状态
	
	bool PNFileStatus[MAX_PN_NUM_MAX_QUANTITY];
	
}sdcard_Information_TypeDef;

extern sdcard_Information_TypeDef sdcardInformation;

void sdcard_PN_file_load(void);
void sdcard_PN_file_edit(void);
void sdcard_PN_file_delete(void);
void sdcard_index_file_password_edit(void);
void sdcard_init(void);
void sdcard_control_handle(void);

#endif




/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : sdcard_control.c
 * Author    : hlb
 * Brief     : sd卡用户层程序
 * Date      : 2017.07.22
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
#include "sdcard_control.h"
#include "sys_manager.h"
#include "message_send.h"
#include "stdio.h"
#include "exfuns.h"
#include "sdio.h"
#include "ff.h"
 
//实例化sd卡数据
sdcard_Information_TypeDef sdcardInformation;

/**
* @ Function Name : sdcard_index_file_load
* @ Author        : hlb
* @ Brief         : SD卡数据下载
* @ Date          : 2017.07.23
* @ Modify        : ...
**/
void sdcard_index_file_load(void)
{
	u16 i;
	u16 j;
	u8 res;
	u8 buffer[INDEX_DATA_MAX_BYTE];
	
	f_open(file, "0:/index.txt", FA_OPEN_EXISTING | FA_READ);
	br = 1;
	
	//清空缓冲区
	memset(&buffer, 0, INDEX_DATA_MAX_BYTE);
	
	while(1)
	{
		res = f_read(file, buffer, INDEX_DATA_MAX_BYTE, &br);
		if(res || br == 0)
		{
			break;
		}
	}
	f_close(file);
	

	
	adjustInformation.inputCurrentStringIdx = buffer[INDEX_DATA_ADJUST_CUR_BYTE_NUM_ADDRESS];
	for(i = 0; i < adjustInformation.inputCurrentStringIdx; i++)
	{
		adjustInformation.inputCurrent[i] = buffer[INDEX_DATA_ADJUST_CUR_ADDRESS_HIGH + i];
	}
	
	passwordInformation.realPasswordStringIdx = buffer[INDEX_DATA_PASSWORD_BYTE_NUM_ADDRESS];
	for(i = 0; i < passwordInformation.realPasswordStringIdx; i++)
	{
		passwordInformation.realPassword[i] = buffer[INDEX_DATA_PASSWORD_ADDRESS_HIGH + i];
	}
	
	passwordInformation.oldPasswordTipStringIdx = buffer[INDEX_DATA_PASSWORD_TIP_BYTE_NUM_ADDRESS];
	for(i = 0; i < passwordInformation.oldPasswordTipStringIdx; i++)
	{
		passwordInformation.oldPasswordTip[i] = buffer[INDEX_DATA_PASSWORD_TIP_ADDRESS_HIGH + i];
	}
	
	configInformation.PNNumQuantity = 0;
	for(i = 0; i < MAX_PN_NUM_MAX_QUANTITY; i++)
	{
		
		if(buffer[INDEX_DATA_PN_NUM_BYTE_NUM_ADDRESS + i * INDEX_DATA_ONE_PN_SIZE] != 0)
		{
			sdcardInformation.PNFileStatus[i] = true;
			
			//读取PN号字符串索引
			configInformation.PNNumStringIdx[configInformation.PNNumQuantity] = \
				buffer[INDEX_DATA_PN_NUM_BYTE_NUM_ADDRESS + i * INDEX_DATA_ONE_PN_SIZE];
			//根据偏移读取每一项PN
			for(j = 0; j < configInformation.PNNumStringIdx[i]; j++)
			{
				configInformation.PNNum[configInformation.PNNumQuantity][j] = \
					buffer[INDEX_DATA_PN_NUM_ADDRESS_HIGH + i * INDEX_DATA_ONE_PN_SIZE + j];
			}
			configInformation.PNNumQuantity++;
		}
		else
		{
			sdcardInformation.PNFileStatus[i] = false;
		}
		
	}

	
}

/**
* @ Function Name : sdcard_index_file_pn_edit
* @ Author        : hlb
* @ Brief         : SD卡引导文件PN号编辑
* @ Date          : 2017.07.25
* @ Input         : u8  pos   	index文件内PN号的序号
					u8 *PNNum	pn号
					u8 size		pn号大小
* @ Modify        : ...
**/
void sdcard_index_file_pn_edit(u8 pos, u8 *PNNum, u8 size)
{
	u8 i;
	u8 res;
	u8 buffer[INDEX_DATA_MAX_BYTE];
	
	f_open(file, "0:/index.txt", FA_OPEN_EXISTING | FA_READ);
	
	br = 1;
	
	//清空缓冲区
	memset(&buffer, 0, INDEX_DATA_MAX_BYTE);
	
	while(1)
	{
		res = f_read(file, buffer, INDEX_DATA_MAX_BYTE, &br);
		if(res || br == 0)
		{
			break;
		}
	}
	f_close(file);
	
	buffer[INDEX_DATA_PN_NUM_BYTE_NUM_ADDRESS + pos * INDEX_DATA_ONE_PN_SIZE] = size;
	for(i = 0; i < size; i++)
	{
		buffer[INDEX_DATA_PN_NUM_ADDRESS_HIGH + pos * INDEX_DATA_ONE_PN_SIZE + i] = \
			PNNum[i];
	}	
	
	f_open(file, "0:/index.txt", FA_OPEN_EXISTING | FA_WRITE);
	f_write(file, buffer, sizeof(buffer), &bw);
	f_close(file);
	
}

/**
* @ Function Name : sdcard_PN_file_load
* @ Author        : hlb
* @ Brief         : SD卡料号下载
* @ Date          : 2017.07.25
* @ Modify        : ...
**/
void sdcard_PN_file_load(void)
{
	u16 i;
	u8 j;
	u8 res;
	u8 PNIdx = 0;
	u8 fileName[PN_FILE_NAME_SIZE_MAX];
	u8 buffer[PN_FILE_MAX_BYTE];
	
	memset(fileName, 0, PN_FILE_NAME_SIZE_MAX);
	
	for(i = 0; i < MAX_PN_NUM_MAX_QUANTITY; i++)
	{
		if(sdcardInformation.PNFileStatus[i] == true)
		{
			if(PNIdx == configInformation.PNNumSelect)
			{
				sprintf((char*)fileName, "0:/%d.txt", i);
				break;
			}
			PNIdx++;
		}
	}

	
	
	f_open(file, (char*)fileName, FA_OPEN_EXISTING | FA_READ);
	br = 1;
	
	//清空缓冲区
	memset(&buffer, 0, PN_FILE_MAX_BYTE);
	
	while(1)
	{
		res = f_read(file, buffer, PN_FILE_MAX_BYTE, &br);
		if(res || br == 0)
		{
			break;
		}
	}
	f_close(file);
	

	
	configInformation.PNNumDisplayIdx = buffer[PN_FILE_PN_IDX_ADDRESS];
	for(i = 0; i < configInformation.PNNumDisplayIdx; i++)
	{
		configInformation.PNNumDisplay[i] = buffer[PN_FILE_PN_NUM_ADDRESS_HIGH + i];
	}
	
	configInformation.descriptionStringIdx = buffer[PN_FILE_PN_DES_IDX_ADDRESS];
	for(i = 0; i < configInformation.descriptionStringIdx; i++)
	{
		configInformation.PNNumDescription[i] = buffer[PN_FILE_PN_DES_ADDRESS_HIGH + i];
	}
	
	configInformation.partNum		 = buffer[PN_FILE_PART_NUM_ADDRESS];
	configInformation.polesNum		 = buffer[PN_FILE_POLES_ADDRESS];
	configInformation.testCurrent	 = (buffer[PN_FILE_CURRENT_ADDRESS_HIGH] << 8) + \
		buffer[PN_FILE_CURRENT_ADDRESS_LOW];
	configInformation.PWMSignFrequen = (buffer[PN_FILE_PWM_FREQUN_ADDRESS_HIGH] << 8) + \
		buffer[PN_FILE_PWM_FREQUN_ADDRESS_LOW];
	configInformation.powerSelect	 = buffer[PN_FILE_POWER_ADDRESS];
	configInformation.trigger    	 = buffer[PN_FILE_TRIGGER_ADDERSS];
	
	for(i = 0; i < configInformation.partNum; i++)
	{
		configInformation.partInformation[i].explainStringIdx = \
			buffer[PN_FILE_PART1_EXPLAIN_IDX_ADDRESS + i * PN_FILE_ONE_PART_SIZE];
		for(j = 0; j < configInformation.partInformation[i].explainStringIdx; j++)
		{
			configInformation.partInformation[i].explain[j] = \
				buffer[PN_FILE_PART1_EXPLAIN_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE + j];
		}
		
		configInformation.partInformation[i].testPWM = \
			buffer[PN_FILE_PART1_PWM_ADDRESS + i * PN_FILE_ONE_PART_SIZE];
		configInformation.partInformation[i].testTime = \
			buffer[PN_FILE_PART1_TIME_ADDRESS + i * PN_FILE_ONE_PART_SIZE];
		configInformation.partInformation[i].testSpeedUpLimit = \
			(buffer[PN_FILE_PART1_SPEED_UP_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE] << 8) + \
			buffer[PN_FILE_PART1_SPEED_UP_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE];
		configInformation.partInformation[i].testSpeedDownLimit = \
			(buffer[PN_FILE_PART1_SPEED_DOWN_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE] << 8) + \
			buffer[PN_FILE_PART1_SPEED_DOWN_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE];
		configInformation.partInformation[i].testCurrentUpLimit = \
			(buffer[PN_FILE_PART1_CURE_UP_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE] << 8) + \
			buffer[PN_FILE_PART1_CURE_UP_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE];
		configInformation.partInformation[i].testCurrentDownLimit = \
			(buffer[PN_FILE_PART1_CURE_DOWN_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE] << 8) + \
			buffer[PN_FILE_PART1_CURE_DOWN_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE];		
	}
}
/**
* @ Function Name : sdcard_PN_file_delete
* @ Author        : hlb
* @ Brief         : SD卡料号删除
* @ Date          : 2017.07.25
* @ Modify        : ...
**/
void sdcard_PN_file_delete(void)
{
	u8 i;
	u8 PNIdx = 0;
	u8 size = 0;
	u8 *PNNum;
	
	for(i = 0; i < MAX_PN_NUM_MAX_QUANTITY; i++)
	{
		if(sdcardInformation.PNFileStatus[i] == true)
		{
			if(PNIdx == configInformation.PNNumSelect)
			{
				if(i == 0)
				{
					//系统模板不可删除
					return;
				}
				sdcard_index_file_pn_edit(i,PNNum,size);
			}
			PNIdx ++;
		}
	}
	
	sdcard_index_file_load();
	
	//如果下一个料号不存在 否则不变
	if(configInformation.PNNumSelect == configInformation.PNNumQuantity )
	{
		configInformation.PNNumSelect--;
	}

	
}

/**
* @ Function Name : sdcard_PN_file_edit
* @ Author        : hlb
* @ Brief         : SD卡料号编辑
* @ Date          : 2017.07.25
* @ Modify        : ...
**/
void sdcard_PN_file_edit(void)
{
	u8 i;
	u8 j;
	u8 PNIdx = 0;
	u8 PNFile[PN_FILE_MAX_BYTE];
	u8 fileName[PN_FILE_NAME_SIZE_MAX];
	
	for(i = 0; i < MAX_PN_NUM_MAX_QUANTITY; i++)
	{
		if(sdcardInformation.PNFileStatus[i] == true)
		{
			//编辑已有的料号
			if(strcmp((char*)configInformation.PNNumDisplay, (char*)configInformation.PNNum[PNIdx]) == 0)
			{
				//不可编辑系统模板料号
				if(i == 0)
				{
					return;
				}
				//得到相应PN文件
				sprintf((char*)fileName, "0:/%d.txt", i);
				break;
			}
			PNIdx++;
		}
	}
	
	//没有同名的料号
	if(i == MAX_PN_NUM_MAX_QUANTITY)
	{
		for(i = 0; i < MAX_PN_NUM_MAX_QUANTITY; i++)
		{
			//新建料号
			if(sdcardInformation.PNFileStatus[i] == false)
			{
				sprintf((char*)fileName, "0:/%d.txt", i);
				//修改引导文件
				sdcard_index_file_pn_edit(i, configInformation.PNNumDisplay, configInformation.PNNumDisplayIdx);
				//修改料号列表
				configInformation.PNNumSelect = PNIdx;
				configInformation.PNNumQuantity++;
				configInformation.PNNumStringIdx[PNIdx] = configInformation.PNNumDisplayIdx;
				for(j = 0; j < configInformation.PNNumDisplayIdx; j++)
				{
					configInformation.PNNum[PNIdx][j] = configInformation.PNNumDisplay[j];
				}
				
				sdcardInformation.PNFileStatus[i] = true;
				
				break;
			}
		}
		//料号已满
		return;
	}


	memset(&PNFile, 0, sizeof(PNFile));
	memset(&fileName, 0, sizeof(fileName));
	
	PNFile[PN_FILE_PN_IDX_ADDRESS] = configInformation.PNNumDisplayIdx;
	for(i = 0; i < MODLE_PN_NUM_BYTE_NUM; i++)
	{
		PNFile[PN_FILE_PN_NUM_ADDRESS_HIGH + i] = configInformation.PNNumDisplay[i];
	}
	
	PNFile[PN_FILE_PN_DES_IDX_ADDRESS] = configInformation.descriptionStringIdx;
	for(i = 0; i < MODLE_PN_DESCRIP_BYTE_NUM; i++)
	{
		PNFile[PN_FILE_PN_DES_ADDRESS_HIGH + i] = configInformation.PNNumDescription[i];
	}
	
	PNFile[PN_FILE_PART_NUM_ADDRESS] 	 	 = configInformation.partNum;
	PNFile[PN_FILE_POLES_ADDRESS]	  		 = configInformation.polesNum;
	PNFile[PN_FILE_CURRENT_ADDRESS_LOW]		 = configInformation.testCurrent;
	PNFile[PN_FILE_PWM_FREQUN_ADDRESS_HIGH]  = (configInformation.PWMSignFrequen >> 8);
	PNFile[PN_FILE_PWM_FREQUN_ADDRESS_LOW]   = configInformation.PWMSignFrequen;
	PNFile[PN_FILE_POWER_ADDRESS] 			 = configInformation.powerSelect;
	PNFile[PN_FILE_TRIGGER_ADDERSS]			 = configInformation.trigger;

	for(i = 0; i < MODLE_PN_PART_NUM; i++)
	{
		PNFile[PN_FILE_PART1_EXPLAIN_IDX_ADDRESS + i * PN_FILE_ONE_PART_SIZE]		= \
			configInformation.partInformation[i].explainStringIdx;
		PNFile[PN_FILE_PART1_PWM_ADDRESS + i * PN_FILE_ONE_PART_SIZE]    	  		= \
			configInformation.partInformation[i].testPWM;
		PNFile[PN_FILE_PART1_TIME_ADDRESS + i * PN_FILE_ONE_PART_SIZE]				= \
			configInformation.partInformation[i].testTime;
		PNFile[PN_FILE_PART1_SPEED_UP_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE]		= \
			(configInformation.partInformation[i].testSpeedUpLimit >> 8);
		PNFile[PN_FILE_PART1_SPEED_UP_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE]      = \
			configInformation.partInformation[i].testSpeedUpLimit;
		PNFile[PN_FILE_PART1_SPEED_DOWN_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE]   = \
			(configInformation.partInformation[i].testSpeedDownLimit >> 8);
		PNFile[PN_FILE_PART1_SPEED_DOWN_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE]	= \
			configInformation.partInformation[i].testSpeedDownLimit;
		PNFile[PN_FILE_PART1_CURE_UP_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE]      = \
			(configInformation.partInformation[i].testCurrentUpLimit >> 8);
		PNFile[PN_FILE_PART1_CURE_UP_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE]       = \
			configInformation.partInformation[i].testCurrentUpLimit;
		PNFile[PN_FILE_PART1_CURE_DOWN_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE]    = \
			(configInformation.partInformation[i].testCurrentDownLimit >> 8);
		PNFile[PN_FILE_PART1_CURE_DOWN_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE]     = \
			configInformation.partInformation[i].testCurrentDownLimit;
		
		for(j = 0; j < configInformation.partInformation[i].explainStringIdx; j++)
		{
			PNFile[PN_FILE_PART1_EXPLAIN_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE + j] =
				configInformation.partInformation[i].explain[j];
		}
		
	}
	
	f_open(file, (char*)fileName, FA_OPEN_EXISTING | FA_WRITE);
	f_write(file, PNFile, PN_FILE_MAX_BYTE, &bw);
	f_close(file);
}

/**
* @ Function Name : sdcard_creat_modle_PN_file 
* @ Author        : hlb
* @ Brief         : SD卡创建模板料号
* @ Date          : 2017.07.24
* @ Modify        : ...
**/
void sdcard_creat_modle_PN_file(void)
{
	u8 i;
	u8 j;
	u8 modlePNFile[PN_FILE_MAX_BYTE];
	
	u8 modlePN[] = MODLE_PN_NUM;
	u8 modlePNDescrip[] = MODLE_PN_DESCRIP;
	u16 modlePNPWMFrequen = MODLE_PN_PWM_FREQUEN;
	
	u8  partExplainIdx[MODLE_PN_PART_NUM];
	u8  partPWM[MODLE_PN_PART_NUM];
	u8  partTime[MODLE_PN_PART_NUM];
	u16 partSpeedUp[MODLE_PN_PART_NUM];
	u16 partSpeedDOWN[MODLE_PN_PART_NUM];
	u16 partCurrentUp[MODLE_PN_PART_NUM];
	u16 partCurrentDown[MODLE_PN_PART_NUM];
	
	u8  partExplain[MODLE_PN_PART_NUM][MAX_BYTE_NUM_SPECIF_EXPLAIN] = \
			{MODLE_PN_PART1_EXPLAIN, \
			MODLE_PN_PART2_EXPLAIN, \
			MODLE_PN_PART3_EXPLAIN, \
			MODLE_PN_PART4_EXPLAIN};
	
	partExplainIdx[TEST_PART1] = MODLE_PN_PART1_IDX;
	partExplainIdx[TEST_PART2] = MODLE_PN_PART2_IDX;
	partExplainIdx[TEST_PART3] = MODLE_PN_PART3_IDX;
	partExplainIdx[TEST_PART4] = MODLE_PN_PART4_IDX;
	
	partPWM[TEST_PART1] = MODLE_PN_PART1_PWM;
	partPWM[TEST_PART2] = MODLE_PN_PART2_PWM;
	partPWM[TEST_PART3] = MODLE_PN_PART3_PWM;
	partPWM[TEST_PART4] = MODLE_PN_PART4_PWM;
	
	partTime[TEST_PART1] = MODLE_PN_PART1_TIME;
	partTime[TEST_PART2] = MODLE_PN_PART2_TIME;
	partTime[TEST_PART3] = MODLE_PN_PART3_TIME;
	partTime[TEST_PART4] = MODLE_PN_PART4_TIME;
	
	partSpeedUp[TEST_PART1] = MODLE_PN_PART1_SPEED_UP;
	partSpeedUp[TEST_PART2] = MODLE_PN_PART2_SPEED_UP;
	partSpeedUp[TEST_PART3] = MODLE_PN_PART3_SPEED_UP;
	partSpeedUp[TEST_PART4] = MODLE_PN_PART4_SPEED_UP;
	
	partSpeedDOWN[TEST_PART1] = MODLE_PN_PART1_SPEED_DOWN;
	partSpeedDOWN[TEST_PART2] = MODLE_PN_PART2_SPEED_DOWN;
	partSpeedDOWN[TEST_PART3] = MODLE_PN_PART3_SPEED_DOWN;
	partSpeedDOWN[TEST_PART4] = MODLE_PN_PART4_SPEED_DOWN;
	
	partCurrentUp[TEST_PART1] = MODLE_PN_PART1_CURE_UP;
	partCurrentUp[TEST_PART2] = MODLE_PN_PART2_CURE_UP;
	partCurrentUp[TEST_PART3] = MODLE_PN_PART3_CURE_UP;
	partCurrentUp[TEST_PART4] = MODLE_PN_PART4_CURE_UP;
	
	partCurrentDown[TEST_PART1] = MODLE_PN_PART1_CURE_DOWN;
	partCurrentDown[TEST_PART2] = MODLE_PN_PART2_CURE_DOWN;
	partCurrentDown[TEST_PART3] = MODLE_PN_PART3_CURE_DOWN;
	partCurrentDown[TEST_PART4] = MODLE_PN_PART4_CURE_DOWN;

	memset(&modlePNFile, 0, sizeof(modlePNFile));
	
	modlePNFile[PN_FILE_PN_IDX_ADDRESS] = MODLE_PN_NUM_BYTE_NUM;
	for(i = 0; i < MODLE_PN_NUM_BYTE_NUM; i++)
	{
		modlePNFile[PN_FILE_PN_NUM_ADDRESS_HIGH + i] = modlePN[i];
	}
	
	modlePNFile[PN_FILE_PN_DES_IDX_ADDRESS] = MODLE_PN_DESCRIP_BYTE_NUM;
	for(i = 0; i < MODLE_PN_DESCRIP_BYTE_NUM; i++)
	{
		modlePNFile[PN_FILE_PN_DES_ADDRESS_HIGH + i] = modlePNDescrip[i];
	}
	
	modlePNFile[PN_FILE_PART_NUM_ADDRESS] 	 	 = MODLE_PN_PART_NUM;
	modlePNFile[PN_FILE_POLES_ADDRESS]	  		 = MODLE_PN_POLES_NUM;
	modlePNFile[PN_FILE_CURRENT_ADDRESS_LOW]	 = MODLE_PN_CUR;
	modlePNFile[PN_FILE_PWM_FREQUN_ADDRESS_HIGH] = (modlePNPWMFrequen >> 8);
	modlePNFile[PN_FILE_PWM_FREQUN_ADDRESS_LOW]  = modlePNPWMFrequen;
	modlePNFile[PN_FILE_POWER_ADDRESS] 			 = MODLE_PN_TEST_POWER;
	modlePNFile[PN_FILE_TRIGGER_ADDERSS]		 = MODLE_PN_TRIGGER;

	for(i = 0; i < MODLE_PN_PART_NUM; i++)
	{
		modlePNFile[PN_FILE_PART1_EXPLAIN_IDX_ADDRESS + i * PN_FILE_ONE_PART_SIZE]		= partExplainIdx[i];
		modlePNFile[PN_FILE_PART1_PWM_ADDRESS + i * PN_FILE_ONE_PART_SIZE]    	  		= partPWM[i];
		modlePNFile[PN_FILE_PART1_TIME_ADDRESS + i * PN_FILE_ONE_PART_SIZE]				= partTime[i];
		modlePNFile[PN_FILE_PART1_SPEED_UP_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE]	= (partSpeedUp[i] >> 8);
		modlePNFile[PN_FILE_PART1_SPEED_UP_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE]     = partSpeedUp[i];
		modlePNFile[PN_FILE_PART1_SPEED_DOWN_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE]  = (partSpeedDOWN[i] >> 8);
		modlePNFile[PN_FILE_PART1_SPEED_DOWN_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE]	= partSpeedDOWN[i];
		modlePNFile[PN_FILE_PART1_CURE_UP_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE]     = (partCurrentUp[i] >> 8);
		modlePNFile[PN_FILE_PART1_CURE_UP_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE]      = partCurrentUp[i];
		modlePNFile[PN_FILE_PART1_CURE_DOWN_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE]   = (partCurrentDown[i] >> 8);
		modlePNFile[PN_FILE_PART1_CURE_DOWN_ADDRESS_LOW + i * PN_FILE_ONE_PART_SIZE]    = partCurrentDown[i];
		
		for(j = 0; j < partExplainIdx[i]; j++)
		{
			modlePNFile[PN_FILE_PART1_EXPLAIN_ADDRESS_HIGH + i * PN_FILE_ONE_PART_SIZE + j] = partExplain[i][j];
		}
		
	}
	

	f_open(file, "0:/0.txt", FA_CREATE_NEW | FA_WRITE);
	f_write(file, modlePNFile, PN_FILE_MAX_BYTE, &bw);
	f_close(file);
	
}

/**
* @ Function Name : sdcard_index_file_creat
* @ Author        : hlb
* @ Brief         : SD卡创建引导
* @ Date          : 2017.07.24
* @ Modify        : ...
**/
void sdcard_index_file_creat(void)
{
	u8 i;

	u8 modlePN[] = MODLE_PN_NUM;
	u8 sdInformaIndex[INDEX_DATA_MAX_BYTE];
	memset(&sdInformaIndex, 0, sizeof(sdInformaIndex));
	
	sdInformaIndex[INDEX_DATA_HEAD_ADDRESS] = INDEX_DATA_HEAD_DEF;
	sdInformaIndex[INDEX_DATA_PN_NUM_QUANT_ADDRESS] = 1;
	sdInformaIndex[INDEX_DATA_PN_NUM_BYTE_NUM_ADDRESS] = MODLE_PN_NUM_BYTE_NUM;
	
	for(i = 0; i < MODLE_PN_NUM_BYTE_NUM; i++)
	{
		sdInformaIndex[INDEX_DATA_PN_NUM_ADDRESS_HIGH + i] = modlePN[i];
	}
	
	f_write(file, sdInformaIndex, INDEX_DATA_MAX_BYTE, &bw);
	f_close(file);
	
}



/**
* @ Function Name : sdcard_init_file_creat
* @ Author        : hlb
* @ Brief         : SD卡数据创建
* @ Date          : 2017.07.24
* @ Modify        : ...
**/
void sdcard_init_file_creat(void)
{

	//创建引导文件
	sdcard_index_file_creat();
	
	//创建模板PN文件
	sdcard_creat_modle_PN_file();
}


 
/**
* @ Function Name : sdcard_init
* @ Author        : hlb
* @ Brief         : SD卡初始化
* @ Date          : 2017.07.22
* @ Modify        : ...
**/ 
void sdcard_init(void)
{
	memset(&sdcardInformation, 0, sizeof(sdcardInformation));
	
	//初始化SD卡
	sdcardInformation.sdcardStatus = SD_Init();
	
	if(sdcardInformation.sdcardStatus == FR_OK)
	{
		//为fatfs相关变量申请内存
		exfuns_init();
		
		//挂载SD卡
		sdcardInformation.sdcardStatus =f_mount(fs[0], "0:", 1);
		
		if(sdcardInformation.sdcardStatus == FR_OK)
		{
			//打开引导文件
			sdcardInformation.sdcardStatus = f_open(file, "0:/index.txt", FA_CREATE_NEW | FA_WRITE);
			
			if(sdcardInformation.sdcardStatus == FR_OK)
			{
				//创建新的文件
				sdcard_init_file_creat();
				//下载数据
				sdcard_index_file_load();
			}
			else if(sdcardInformation.sdcardStatus == FR_EXIST)
			{
				//下载数据
				sdcard_index_file_load();	
				sdcardInformation.sdcardStatus = FR_OK;		
			}
		}
	}
}

/**
* @ Function Name : sdcard_control_handle
* @ Author        : hlb
* @ Brief         : SD卡控制线程
* @ Date          : 2017.07.22
* @ Modify        : ...
**/
void sdcard_control_handle(void)
{
	u8 readOK[]= "readOK";
	u8 readError[] = "error";
	if(sdcardInformation.sdcardStatus == FR_OK)
	{	
		//usart_tx(readOK, 6);
	}
	else
	{
		//SD卡初始化
		sdcard_init();
		
		usart_tx(readError, 5);
		
	}
}



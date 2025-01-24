#ifndef __UAV_LIGHTSPEAKER_H__
#define __UAV_LIGHTSPEAKER_H__

#include "uav_platform.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum
{
	UAV_MODULE_ID_LIGHT = 1,
	UAV_MODULE_ID_SPEAKER = 2,
}E_UAVModuleID;
typedef enum
{
	UAV_LIGHT_STATE_OPEN = 1,
	UAV_LIGHT_STATE_CLOSE = 2,
}E_UAVLightState;

typedef enum
{
	UAV_SPEAK_STATE_OPEN = 1,
	UAV_SPEAK_STATE_CLOSE = 2,
}E_UAVSpeakState;

typedef enum
{
	UAV_LIGHT_BRILLIANCE_GEAR_INVALID = -1,
	UAV_LIGHT_BRILLIANCE_GEAR_1 = 1,
	UAV_LIGHT_BRILLIANCE_GEAR_2 = 2,
	UAV_LIGHT_BRILLIANCE_GEAR_3 = 3,
}E_UAVLightBrillianceGear;

typedef enum
{
	UAV_GIMBAL_LINKAGE_STATE_ENABLE = 1,
	UAV_GIMBAL_LINKAGE_STATE_DISABLE = 2,
}E_UAVGimbalLinkageState;

typedef struct __lightSpeakerState
{
	E_UAVLightState lightState;				//灯状态
	E_UAVLightBrillianceGear brillianceGear;	//亮度档位
	int32_t pitchAngle;							//俯仰角
	E_UAVGimbalLinkageState gimbalLinkageState;//云台联动状态

	E_UAVSpeakState speakState;				//喇叭状态
	int32_t volume;								//音量 -1:无效， 0-100:音量


}T_UAVLightSpeakerState;

typedef struct __lightSpeaker
{
	char *product_alias;					//产品别名
	uint8_t module_id; 						//模块ID 1-light, 2-speaker

	int32_t (*get_state)(T_UAVLightSpeakerState **state);	//获取状态
	void (*light_open)(void);				//开灯
	void (*light_close)(void);				//关灯
	void (*set_brilliance)(uint8_t gear);	//设置亮度
	void (*manul_set_pitch_angle)(float angle);	//手动设置俯仰角
	void (*auto_set_pitch_angle)(float angle);	//自动设置俯仰角
	void (*GimbalLinkage_enable)(void);		//云台联动开
	void (*GimbalLinkage_disable)(void);	//云台联动关
	void (*set_rgb)(uint8_t color);			//设置RGB颜色, 0-白，1-红，2-蓝
	void (*get_rgb)(uint8_t *color);		//获取RGB颜色
	void (*set_flash)(uint8_t mode);		//设置爆闪模式, 0-关闭，1-红光闪烁, 2-警示灯闪烁
	void (*get_flash)(uint8_t *mode);		//获取爆闪模式

	void (*speaker_open)(void);				//开喇叭
	void (*speaker_close)(void);				//关喇叭
	void (*set_volume)(uint8_t volume);		//设置音量

	void (*audio_play)(void);				//播放音频
	void (*audio_pause)(void);				//暂停音频
	void (*audio_stop)(void);				//停止音频
	void (*next_audio)(void);				//下一首音频
	void (*prev_audio)(void);				//上一首音频

	void (*opus_play)(void);				//播放opus
	void (*get_opus_state)(uint8_t *state);	//获取opus状态

	void (*set_tone)(uint8_t tone);			//设置音色 0-无效,1-女声，2-男声
	void (*set_speed)(uint8_t speed);		//设置语速 0-无效,1-100
	void (*start_text_transfer)(uint8_t param);		//开始传输文本, param=0;
	void (*stop_text_transfer)(uint8_t param);		//停止传输文本, param=0;

	void (*set_loop_play)(uint8_t param);	//设置循环播放开关, param=0-关闭，1-开启
	void (*get_loop_play)(uint8_t *param);	//获取循环播放开关
	//打开媒体资源文件上传
	void (*media_file_uploade_enable)(char *file, uint32_t crc32);
	//关闭媒体资源文件上传
	void (*media_file_uploade_disable)(char *file);
	//更新列表完成
	void (*update_list_complete)(uint8_t param);	//param=0
	//更新列表状态
	void (*update_list_state)(uint8_t *state);	
	//设置循环播放间隔
	void (*set_loop_play_interval)(uint8_t interval);	//interval=0-255
	//获取喊话器版本
	void (*get_version)(char *version);
	
	/**
	 * @brief 语音文件上传开关
	 * 
	 * @param file 文件名
	 * @param crc32 文件crc32
	 * @return void
	*/
	void (*voice_file_uploade_enable)(char *file, uint32_t crc32);
	/**
	 * @brief 语音文件上传关
	 * 
	 * @param file 文件名
	 * @return void
	*/
	void (*voice_file_uploade_disable)(char *file);
	/**
	 * @brief 语音文件播放
	 * 
	 * @param file 文件名
	 * @return void
	*/
	void (*voice_file_play)(char *file);	
	/**
	 * @brief 语音文件停止
	 * 
	 * @param file 文件名
	 * @return void
	*/
	void (*delete_voice_file)(char *file);	//删除语音文件，文件名
	/**
	 * @brief 删除录音文件
	 * 
	 * @param file 文件名
	 * @return void
	*/
	void (*delete_record_file)(char *file);	//删除录音文件, 文件名
	/**
	 * @brief 获取文件数量
	 * 
	 * @param none
	 * @return int32_t 文件数量
	*/
	int32_t (*get_file_num)(void);		
	/**
	 * @brief 查询文件列表: 根据pageIndex和num获取文件列表
	 * 
	 * @param pageIndex 页码
	 * @param num 文件数量指针
	 * @param file_list 文件列表指针, 指向一个二维数组a[n][128]，n=5, 
	 * @return int 0-成功，其他-失败
	*/
	int (*QueryRspCallback)(int32_t pageIndex, uint8_t num, char (*file_list)[128]);	
	/**
	 * @brief 语音文件处理回调
	 * 
	 * @param data 语音数据
	 * @param len 数据长度
	 * @return int 0-成功，其他-失败
	*/
	int (*speaker_run_time_data_process)(uint8_t *data, uint16_t len);	
}T_UAVLightSpeaker;


T_UAVReturnCode UAV_RegLightSpeaker(T_UAVLightSpeaker *lightSpeaker);

#ifdef __cplusplus
}
#endif


#endif // __UAV_LIGHTSPEAKER_H__

#include "test_widget_lightSpeaker.h"
#include "uav_lightSpeaker.h"
#include "uav_core.h"
#include "uav_error.h"
#include "uav_logger.h"

static T_UAVLightSpeakerState lightSpeakerState ={0};

static int32_t getLightSpeakerState(T_UAVLightSpeakerState **state)
{
    *state = &lightSpeakerState;
    return 0;
}
static void light_open(void)
{
    UAV_LOG_INFO("light open");    
}
static void light_close(void)
{
   UAV_LOG_INFO("light close"); 
}
static void set_brilliance(uint8_t gear)
{
   UAV_LOG_INFO("set brilliance gear:%d",gear); 
}
static void manul_set_pitch_angle(float angle)
{
   UAV_LOG_INFO("manul set pitch angle:%f",angle); 
}
static void auto_set_pitch_angle(float angle)
{
   UAV_LOG_INFO("auto set pitch angle:%f",angle); 
}
static void GimbalLinkage_enable(void)
{
   UAV_LOG_INFO("GimbalLinkage_enable"); 
}
static void GimbalLinkage_disable(void)
{
   UAV_LOG_INFO("GimbalLinkage_disable"); 
}
static void set_rgb(uint8_t color)
{
   UAV_LOG_INFO("set rgb color:%d",color); 
}
static void get_rgb(uint8_t *color)
{
   UAV_LOG_INFO("get rgb color:%d",*color); 
}
static void set_flash(uint8_t mode)
{
   UAV_LOG_INFO("set flash mode:%d",mode); 
}
static void get_flash(uint8_t *mode)
{
   UAV_LOG_INFO("get flash mode:%d",*mode); 
}

static void speaker_open(void)
{
   UAV_LOG_INFO("speak open"); 
}
static void speaker_close(void)
{
   UAV_LOG_INFO("speak close"); 
}
static void set_volume(int32_t volume)
{
   UAV_LOG_INFO("set volume:%d",volume); 
}

void voice_file_uploade_enable(char *file_name, uint32_t crc32)//语音文件上传开
{
    UAV_LOG_INFO("voice_file_uploade_enable, %s, crc32:%d",file_name, crc32);
}
void voice_file_uploade_disable(char *file_name)//语音文件上传关
{
    UAV_LOG_INFO("voice_file_uploade_disable, %s",file_name);
}
void voice_file_play(char *file)	//播放语音文件
{
    UAV_LOG_INFO("voice_file_play:%s",file);
}
void audio_play(void)				//播放音频
{
    UAV_LOG_INFO("audio_play");
}
void audio_pause(void)				//暂停音频
{
    UAV_LOG_INFO("audio_pause");
}
void audio_stop(void)				//停止音频
{
    UAV_LOG_INFO("audio_stop"); 
}
void next_audio(void)				//下一首音频
{
    UAV_LOG_INFO("next_audio");
}
void prev_audio(void)				//上一首音频
{
    UAV_LOG_INFO("prev_audio");
}
void delete_voice_file(char *file)	//删除语音文件
{
    UAV_LOG_INFO("delete_voice_file:%s",file);
}
void delete_record_file(char *file)	//删除录音文件
{
    UAV_LOG_INFO("delete_record_file:%s",file);
}



void opus_play(void)				//播放opus
{
    UAV_LOG_INFO("opus_play");
}
void get_opus_state(uint8_t *state)	//获取opus状态
{
    UAV_LOG_INFO("get_opus_state:%d",*state);
}

void set_tone(uint8_t tone)			//设置音色 0-无效,1-女声，2-男声
{
    UAV_LOG_INFO("set_tone:%d",tone);
}
void set_speed(uint8_t speed)		//设置语速 0-无效,1-100
{
    UAV_LOG_INFO("set_speed:%d",speed);
}
void start_text_transfer(uint8_t param)		//开始传输文本, param=0;
{
    UAV_LOG_INFO("start_text_transfer");
}
void stop_text_transfer(uint8_t param)		//停止传输文本, param=0;
{
    UAV_LOG_INFO("stop_text_transfer");
}
void set_loop_play(uint8_t param)	//设置循环播放开关, param=0-关闭，1-开启
{
    UAV_LOG_INFO("set_loop_play:%d",param);
}
void get_loop_play(uint8_t *param)	//获取循环播放开关
{
    UAV_LOG_INFO("get_loop_play:%d",*param);
}
//打开媒体资源文件上传
void media_file_uploade_enable(char *file, uint32_t crc32)
{
    UAV_LOG_INFO("media_file_uploade_enable, %s, crc32:%d",file, crc32);

}
//关闭媒体资源文件上传
void media_file_uploade_disable(char *file)
{
    UAV_LOG_INFO("media_file_uploade_disable, %s",file);
}
//更新列表完成
void update_list_complete(uint8_t param)	//param=0
{
    UAV_LOG_INFO("update_list_complete:%d",param);
}
/**
 * @brief 获取文件数量
 * 
 * @param none
 * @return int32_t 文件数量
*/
int32_t get_file_num(void)
{
    UAV_LOG_DEBUG("get_file_num");
    return 5;

}
/**
 * @brief 查询文件列表: 根据pageIndex和num获取文件列表
 * 
 * @param pageIndex 页码
 * @param num 文件数量指针
 * @param file_list 文件列表指针, 指向一个二维数组a[n][128]，n=5, 
 * @return int 0-成功，其他-失败
*/
int QueryRspCallback(int32_t pageIndex, uint8_t num, char (*file_list)[128])
{
    static char a[10][64] = {
        "file1",
        "file2",
        "file3",
        "file4",
        "file5",
        "file6",
        "file7",
        "file8",
        "file9",
        "file10",
    };  
    for(int i=0; i<num; i++)
    {
        strcpy(file_list[i], a[i+pageIndex*5]);
    }
    UAV_LOG_DEBUG("QueryRspCallback");

    return 0;
}
/**
 * @brief 语音文件处理回调
 * 
 * @param data 语音数据
 * @param len 数据长度
 * @return int 0-成功，其他-失败
*/
int speaker_run_time_data_process(uint8_t *data, uint16_t len)	//喇叭运行时消息处理
{
    UAV_LOG_DEBUG("speaker_run_time_data_process");
    return 0;
}

/**
 * @brief 返回更新列表状态
 * 
 * @param *state 更新列表状态返回：0-成功，1-失败
 * @return void
 */
void update_list_state(uint8_t *state)
{
    UAV_LOG_DEBUG("update list state");
}

/**
 * @brief 获取喊话器版本
 * 
 * @param version 版本信息字符串
 * @return void
 */
void get_version(char *version)
{
    UAV_LOG_DEBUG("get version:%s",version);
}
/**
 * @brief 设置循环播放时间间隔
 * 
 * @param interval 循环播放时间间隔
 * @return void
 */
void set_loop_play_interval(uint32_t interval)
{
    UAV_LOG_DEBUG("set loop play interval");
}
T_UAVLightSpeaker Uav_lightSpeaker={
    .module_id          = UAV_MODULE_ID_LIGHT,
    .get_state          = getLightSpeakerState,
    .light_open         = light_open,
    .light_close        = light_close,
    .set_brilliance     = set_brilliance,
    .manul_set_pitch_angle    = manul_set_pitch_angle,
    .auto_set_pitch_angle    = auto_set_pitch_angle,
    .GimbalLinkage_enable = GimbalLinkage_enable,
    .GimbalLinkage_disable = GimbalLinkage_disable,
    .set_rgb            = set_rgb,
    .get_rgb            = get_rgb,
    .set_flash          = set_flash,
    .get_flash          = get_flash,

    .speaker_open       = speaker_open,
    .speaker_close      = speaker_close,
    .set_volume         = set_volume,
    .voice_file_uploade_enable = voice_file_uploade_enable,
    .voice_file_uploade_disable = voice_file_uploade_disable,
    .voice_file_play    = voice_file_play,
    .audio_play         = audio_play,
    .audio_pause        = audio_pause,
    .audio_stop         = audio_stop,
    .next_audio         = next_audio,
    .prev_audio         = prev_audio,
    .delete_voice_file  = delete_voice_file,
    .delete_record_file = delete_record_file,
    .opus_play          = opus_play,
    .get_opus_state     = get_opus_state,
    .get_file_num       = get_file_num,
    .QueryRspCallback   = QueryRspCallback,
    .speaker_run_time_data_process = speaker_run_time_data_process,

    .set_tone           = set_tone,
    .set_speed          = set_speed,
    .start_text_transfer = start_text_transfer,
    .stop_text_transfer = stop_text_transfer,

    .set_loop_play      = set_loop_play,
    .get_loop_play      = get_loop_play,

    .media_file_uploade_enable = media_file_uploade_enable,
    .media_file_uploade_disable = media_file_uploade_disable,
    .update_list_complete = update_list_complete,

    .update_list_state = update_list_state,
    .set_loop_play_interval = set_loop_play_interval,
    .get_version = get_version,
};


T_UAVReturnCode UAV_Test_LightSpeakerStartService(void)
{
    T_UAVReturnCode returnCode ;
    returnCode = UAV_RegLightSpeaker(&Uav_lightSpeaker);
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
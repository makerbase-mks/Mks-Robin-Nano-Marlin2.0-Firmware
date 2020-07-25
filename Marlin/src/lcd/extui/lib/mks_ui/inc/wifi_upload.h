#ifndef _WIFI_UPLOAD_H_
#define _WIFI_UPLOAD_H_

#define ESP_FIRMWARE_FILE		"1:/MksWifi.bin"
#define ESP_WEB_FIRMWARE_FILE		"1:/MksWifi_Web.bin"
#define ESP_WEB_FILE		"1:/MksWifi_WebView.bin"



typedef enum
{
	upload_idle,
	resetting,
	connecting,
	erasing,
	uploading,
	done
} UploadState;

typedef enum
{
	success = 0,
	timeout,
	connected,
	badReply,
	fileRead,
	emptyFile,
	respHeader,
	slipFrame,
	slipState,
	slipData,
} EspUploadResult;

typedef struct
{
	//FIL  uploadFile;
	uint32_t fileSize;

	uint32_t uploadAddress;
	UploadState state;
	uint32_t retriesPerBaudRate;
	uint32_t connectAttemptNumber;
	uint32_t lastAttemptTime;
	uint32_t lastResetTime;
	uint32_t uploadBlockNumber;
	uint32_t uploadNextPercentToReport;
	EspUploadResult uploadResult;
} UPLOAD_STRUCT;


extern UPLOAD_STRUCT esp_upload;

#ifdef __cplusplus
extern "C" {
#endif
	int32_t wifi_upload(int type);
#ifdef __cplusplus
}
#endif

#endif


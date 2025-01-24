#Python环境搭建
#1、安装python3.6
#2、命令行: python -m pip install pycryptodome

import os
import sys
import time
from Crypto.Cipher import AES

#配置数据,小端模式存储时,XX[0]存放在最高字节,XX[3]存放在最低字节,XX[0]表示版本号最高位
MAGIC = 0xAA55AA02
DEVICE_ID = 0x30
HW_VERSION = [1, 0, 1, 0]
BOOT_VERSION = [1, 0, 0, 0]
REV1 = [0, 0, 0, 0]
REV2 = [0, 0, 0, 0]
REV3 = [0, 0, 0, 0]
APP_VERSION = [1, 0, 0, 0]
APP_SIZE = 0
APP_CRC32 = 0
MIN_HW_VERSION = [1, 0, 0, 0]
MAX_HW_VERSION = [5, 0, 0, 0]
FIRM = b"PN:ModelX_Throw-MCU_01_HDSC32F460"
SN = b"1234567890"

#UPG文件头大小
APP_UPG_HEAD_SIZE = 240
#生成文件名的格式
IMG_HEAD = "ModelX.MODEXTHROWV2-"
#IMG_HEAD = "ModelH.MODEHTHROWV2-"
#IMG_HEAD = "ModelM.MODEMTHROWV2-"


#flash分布
'''
		-----------------------------------------------------------------------
area	|bootloader| boot_config  | application1 | application2 | parameter   |
		|----------|--------------|--------------|--------------|-------------|
size	|  32K     |  8K          |  232K        |  232K        |  8K         |
		|----------|--------------|--------------|--------------|-------------|
addr	|  0x0000  |  0x8000      |  0xA000      |  0x44000     |  0x7E000    |
		-----------------------------------------------------------------------
'''
#flash分布
BOOT_BIN = ".\\boot.bin"
CFG_OFFSET = 0x8000
CFG_SIZE = 0x2000  #BOOT CFG AND APP CFG
APP_BIN = ".\\app.bin"
APP_OFFSET = 0xA000

UPG_NAME = "MODEX"
IMG_NAME = "MODEX"


#加密设置
USE_AES_UPG = 1 
AES_key = bytearray([0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c])


def add_to_16(text):
    add = 0
    if(len(text) % 16):
        add = 16 - len(text) % 16
    text += bytearray(add)
    return text

def encrypt(text):
    text = add_to_16(text)
    cryptos = AES.new(AES_key, AES.MODE_ECB)
    return cryptos.encrypt(text)

def decrypt(text):
    cryptor = AES.new(AES_key, AES.MODE_ECB)
    plain_text = cryptor.decrypt(text)
    return plain_text

def cal_crc16(data, len):
	crc = 0xffff
	for i in range(0, len):
		crc ^= data[i]
		for _ in range(8):
			if crc & 0x0001:
				crc >>= 1 
				crc ^= 0x8408 
			else: 
				crc=(crc>>1) 
	return crc

def calc_app_crc32(data):
	size = len(data)
	crc = 0
	uiCount = (int)(size / 4)
	index = 0
	uiTemp = 0
	
	for i in range(0, uiCount):
		uiTemp = int.from_bytes(data[index:index+4], byteorder = 'little', signed = False)
		for j in range(0, 32):
			if ((crc^uiTemp) & 0x80000000):
				crc = 0x04C11DB7^((crc<<1) & 0xFFFFFFFF)
			else:
				crc = (crc << 1) & 0xFFFFFFFF
			uiTemp = (uiTemp << 1) & 0xFFFFFFFF
		index += 4
	
	n = size%4
	temp = bytearray(4)
	if(n):
		for i in range(0, 4):
			if (i<n):
				temp[i] = data[index + i]
			else:
				temp[i] = 0xff
		uiTemp = int.from_bytes(data[index:index+4], byteorder = 'little', signed = False)
		for j in range(0, 32):
			if ((crc^uiTemp) & 0x80000000 & 0xFFFFFFFF):
				crc = 0x04C11DB7^((crc<<1) & 0xFFFFFFFF)
			else:
				crc = (crc << 1) & 0xFFFFFFFF
			uiTemp = (uiTemp << 1) & 0xFFFFFFFF
			
	return crc
	
def make_cfg(app_size, app_crc32, cfg_size):
	data = bytearray(cfg_size)
	for i in range(0, cfg_size):
		data[i] = 0
	
	#MAGIC
	tmp = MAGIC.to_bytes(length = 4, byteorder = 'little', signed = False)
	for i in range(0, 4):
		data[0 + i] = tmp[i]
	#DEVICE_ID
	tmp = DEVICE_ID.to_bytes(length = 4, byteorder = 'little', signed = False)
	for i in range(0, 4):
		data[4 + i] = tmp[i]
	#HW_VERSION
	for i in range(0, 4):
		data[8 + i] = HW_VERSION[3-i]
	#BOOT_VERSION
	for i in range(0, 4):
		data[12 + i] = BOOT_VERSION[3-i]
	#REV1-REV3
	for i in range(0, 12):
		data[16 + i] = 0
	#APP_VERSION
	for i in range(0, 4):
		data[28 + i] = APP_VERSION[3-i]
	
	#APP_SIZE
	tmp = app_size.to_bytes(length = 4, byteorder = 'little', signed = False)
	for i in range(0, 4):
		data[32 + i] = tmp[i]

	#APP_CRC32
	tmp = app_crc32.to_bytes(length = 4, byteorder = 'little', signed = False)
	for i in range(0, 4):
		data[36 + i] = tmp[i]
	
	#MIN_HW_VERSION
	for i in range(0, 4):
		data[40 + i] = MIN_HW_VERSION[3-i]
	#MAX_HW_VERSION
	for i in range(0, 4):
		data[44 + i] = MAX_HW_VERSION[3-i]
	#FIRM
	for i in range(0, len(FIRM)):
		data[48 + i] = FIRM[i]
	#SN
	for i in range(0, len(SN)):
		data[112 + i] = SN[i]
	
	#CRC16
	crc16 = cal_crc16(data, 112+96)
	print("crc16= 0x%x "%crc16)
	tmp = crc16.to_bytes(length=2, byteorder='little', signed=False)
	for i in range(0, 2):
		data[112+96 + i] = tmp[i]

	for i in range(0,cfg_size-240):
		data[240 + i] = 0xff	
	return data

def make_blank(size):
	data = bytearray(size)
	for i in range(0,size):
		data[0 + i] = 0xff
	return data
	
def version_to_string(version):
	ver_str = "V"
	ver_str += str(version[0]) + "."
	ver_str += str(version[1]) + "."
	ver_str += str(version[2]) + "."
	ver_str += str(version[3])
	
	return ver_str	
	
def get_system_date():
	return time.strftime("%Y%m%d%H%M%S", time.localtime())	

def del_old_upg_bin():
	#删除以IMG_HEAD为头的文件
	filenames = os.listdir(".\\release\\")
	for i in range(0, len(filenames)):
		tmp = filenames[i][0:len(IMG_HEAD)]
		if(tmp == IMG_HEAD):
			os.remove(".\\release\\"+filenames[i])
	
def make_app_upg():
	#新文件名
	global UPG_NAME			
	IMG_BIN = ".\\release\\"+IMG_HEAD
	IMG_BIN += version_to_string(APP_VERSION)
	IMG_BIN += "-" + get_system_date()
	IMG_BIN += ".pkg"

	UPG_NAME = IMG_BIN
	print("UPG_NAME = %s" %(UPG_NAME))
    #打开文件
	print("open files ")
	f_app = open(APP_BIN,"rb")
	f_img = open(IMG_BIN,"ab")
	
	#读取文件
	print("read app bin file ...")
	app_data = f_app.read()

	#计算blank大小
	app_size = os.path.getsize(APP_BIN)
	print("app_size = %d bytes "%(app_size))
	
	#计算CRC
	app_crc32 = calc_app_crc32(app_data)
	print("app_crc32 = 0x%x "%app_crc32)
	upg_head_data = bytes(make_cfg(app_size, app_crc32, APP_UPG_HEAD_SIZE))
	
	#数据搬移
	print("filling upg_head_data ...")
	f_img.write(upg_head_data)
	
	#加密
	if(USE_AES_UPG):
		app_data = encrypt(app_data)

	print("filling app ...")
	f_img.write(app_data)
	
	#关闭文件
	print("close files.")
	f_app.close()
	f_img.close()


def make_boot_app_bin():
	global IMG_NAME
	#新文件名
	IMG_BIN = ".\\release\\"+IMG_HEAD
	IMG_BIN += "boot_" + version_to_string(BOOT_VERSION)
	IMG_BIN += "_app_" + version_to_string(APP_VERSION)
	IMG_BIN += "_" + get_system_date()
	IMG_BIN += ".bin"

	
    #打开文件
	print("open files ")
	f_boot = open(BOOT_BIN,"rb")
	f_app = open(APP_BIN,"rb")
	f_img = open(IMG_BIN,"ab")
	
	#读取文件
	print("read boot & app bin file ...")
	boot_data = f_boot.read()
	app_data = f_app.read()

	#计算blank大小
	boot_size = os.path.getsize(BOOT_BIN)
	app_size = os.path.getsize(APP_BIN)
	blank_size = CFG_OFFSET - boot_size
	print("boot_size = %d bytes, app_size = %d bytes"%(boot_size, app_size))
	
	#计算CRC
	app_crc32 = calc_app_crc32(app_data)
	print("app_crc32 = 0x%x"%app_crc32)
	cfg_data = bytes(make_cfg(app_size, app_crc32, CFG_SIZE))
	
	#数据搬移
	print("filling boot ...")
	f_img.write(boot_data)
	
	print("filling blank ...")
	blank_data = bytes(blank_size)
	f_img.write(blank_data)
	
	print("filling boot_cfg ...")
	f_img.write(cfg_data)

	print("filling app ...")
	f_img.write(app_data)
	
	#关闭文件
	print("close files.")
	f_boot.close()
	f_app.close()
	f_img.close()


def make_app_upg_decrypt():
	#新文件名
	global UPG_NAME			
			
	IMG_BIN = ".\\release\\"+IMG_HEAD+"decrypt-"
	IMG_BIN += version_to_string(APP_VERSION)
	IMG_BIN += "-" + get_system_date()
	IMG_BIN += ".bin"
	
    #打开文件
	print("open files ")
	print("UPG_NAME = %s" %(UPG_NAME))

	f_boot = open(BOOT_BIN,"rb")
	f_app = open(UPG_NAME,"rb")
	f_img = open(IMG_BIN,"ab")
	
	print("read boot & app bin file ...")
	boot_data = f_boot.read()
	#计算blank大小
	boot_size = os.path.getsize(BOOT_BIN)
	blank_size = CFG_OFFSET - boot_size

	print("filling boot ...")
	f_img.write(boot_data)
	
	print("filling blank ...")
	blank_data = bytes(blank_size)
	f_img.write(blank_data)
	#读取头部
	print("read app bin file ...")
	app_data = f_app.read(240)

	print("filling  head...")
	f_img.write(app_data) 

	#读取应用
	app_data = f_app.read()
	#解码
	if(USE_AES_UPG):
		app_data = decrypt(app_data)
	
	print("filling app ...")
	f_img.write(app_data)
	
	#关闭文件
	print("close files.")
	f_boot.close()
	f_app.close()
	f_img.close()

del_old_upg_bin()
print("del old file success !\r\n")


make_app_upg()
print("Make app_upg success !\r\n")

make_boot_app_bin()
print("Make boot_app_bin success !\r\n")

# make_app_upg_decrypt()
# print("Make app_upg decrypt success !\r\n")
	
#os.system("PAUSE")

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

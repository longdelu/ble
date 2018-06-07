/*******************************************************************************
*                                 Apollo
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief usbcore �̼��ӿڣ���ֲȦȦ����ѧUSB)
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-11-28  lgr, first implementation.
 * \endinternal
 */ 
 
#include "am_usbcore.h"

uint8_t Buffer[16];  //���˵�0�õĻ�����

//USB�豸����ĸ��ֶ�
uint8_t  bmRequestType;
uint8_t  bRequest;
uint16_t wValue;
uint16_t wIndex;
uint16_t wLength;
//��ǰ�������ݵ�λ��
uint8_t * pSendData;
//��Ҫ�������ݵĳ���
uint16_t SendLength;
//�Ƿ���Ҫ����0���ݰ��ı�־����USB���ƴ�������ݹ����У�
//�����ص����ݰ��ֽ�������������ʱ������Ϊ���ݹ��̽�����
//��������ֽ�����ʵ����Ҫ���ص��ֽ���������ʵ�ʷ��ص��ֽ�
//���ָպ��Ƕ˵�0��С��������ʱ������Ҫ����һ��0���ȵ����ݰ�
//���������ݹ��̡������������һ����־������������Ƿ���Ҫ����
//һ��0���ȵ����ݰ���
uint8_t NeedZeroPacket;

//��ǰ������ֵ��ֻ�������÷�0���ú�
uint8_t ConfigValue;

//�˵�1�����Ƿ�æ�ı�־������������������ʱ���ñ�־Ϊ�档
//���������п���ʱ���ñ�־Ϊ�١�
uint8_t Ep1InIsBusy;

//USB�豸�������Ķ���
uint8_t DeviceDescriptor[0x12]=  //�豸������Ϊ18�ֽ�
{
//bLength�ֶΡ��豸�������ĳ���Ϊ18(0x12)�ֽ�
 0x12,
 
//bDescriptorType�ֶΡ��豸�������ı��Ϊ0x01
 0x01,
 
//bcdUSB�ֶΡ��������ð汾ΪUSB1.1����0x0110��
//������С�˽ṹ�����Ե��ֽ����ȣ���0x10��0x01��
 0x10,
 0x01,
 
//bDeviceClass�ֶΡ����ǲ����豸�������ж����豸�࣬
//���ڽӿ��������ж����豸�࣬���Ը��ֶε�ֵΪ0��
 0x00,
 
//bDeviceSubClass�ֶΡ�bDeviceClass�ֶ�Ϊ0ʱ�����ֶ�ҲΪ0��
 0x00,
 
//bDeviceProtocol�ֶΡ�bDeviceClass�ֶ�Ϊ0ʱ�����ֶ�ҲΪ0��
 0x00,
 
//bMaxPacketSize0�ֶΡ�PDIUSBD12�Ķ˵�0��С��16�ֽڡ�
 0x10,
 
//idVender�ֶΡ�����ID�ţ���������ȡ0x8888������ʵ���á�
//ʵ�ʲ�Ʒ�������ʹ�ó���ID�ţ������USBЭ�����볧��ID�š�
//ע��С��ģʽ�����ֽ����ȡ�
 0x88,
 0x88,
 
//idProduct�ֶΡ���ƷID�ţ������ǵ�һ��ʵ�飬��������ȡ0x0001��
//ע��С��ģʽ�����ֽ�Ӧ����ǰ��
 0x01,
 0x00,
 
//bcdDevice�ֶΡ��������USB���տ�ʼ�����ͽ���1.0��ɣ���0x0100��
//С��ģʽ�����ֽ����ȡ�
 0x00,
 0x01,
 
//iManufacturer�ֶΡ������ַ���������ֵ��Ϊ�˷������͹���
//�ַ��������ʹ�1��ʼ�ɡ�
 0x01,

//iProduct�ֶΡ���Ʒ�ַ���������ֵ���ո�����1�������ȡ2�ɡ�
//ע���ַ�������ֵ��Ҫʹ����ͬ��ֵ��
 0x02,
 
//iSerialNumber�ֶΡ��豸�����к��ַ�������ֵ��
//����ȡ3�Ϳ����ˡ�
 0x03,

//bNumConfigurations�ֶΡ����豸�����е���������
//����ֻ��Ҫһ�����þ����ˣ���˸�ֵ����Ϊ1��
 0x01
};
//////////////////////////�豸���������//////////////////////////////

//USB�����������Ķ���
uint8_t ReportDescriptor[]=
{
 //ÿ�п�ʼ�ĵ�һ�ֽ�Ϊ����Ŀ��ǰ׺��ǰ׺�ĸ�ʽΪ��
 //D7~D4��bTag��D3~D2��bType��D1~D0��bSize�����·ֱ��ÿ����Ŀע�͡�
 
 //����һ��ȫ�֣�bTypeΪ1����Ŀ��ѡ����;ҳΪ��ͨ����Generic Desktop Page(0x01)
 //�����һ�ֽ����ݣ�bSizeΪ1����������ֽ����Ͳ�ע���ˣ�
 //�Լ�����bSize���жϡ�
 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
 
 //����һ���ֲ���bTypeΪ2����Ŀ��˵����������Ӧ�ü�����;�������
 0x09, 0x02, // USAGE (Mouse)
 
 //����һ������Ŀ��bTypeΪ0����Ŀ�������ϣ������������0x01��ʾ
 //�ü�����һ��Ӧ�ü��ϡ�����������ǰ������;ҳ����;����Ϊ
 //��ͨ�����õ���ꡣ
 0xa1, 0x01, // COLLECTION (Application)
 
 //����һ���ֲ���Ŀ��˵����;Ϊָ�뼯��
 0x09, 0x01, //   USAGE (Pointer)
 
 //����һ������Ŀ�������ϣ������������0x00��ʾ�ü�����һ��
 //�����ϣ���;��ǰ��ľֲ���Ŀ����Ϊָ�뼯�ϡ�
 0xa1, 0x00, //   COLLECTION (Physical)
 
 //����һ��ȫ����Ŀ��ѡ����;ҳΪ������Button Page(0x09)��
 0x05, 0x09, //     USAGE_PAGE (Button)
 
 //����һ���ֲ���Ŀ��˵����;����СֵΪ1��ʵ��������������
 0x19, 0x01, //     USAGE_MINIMUM (Button 1)
 
 //����һ���ֲ���Ŀ��˵����;�����ֵΪ3��ʵ����������м���
 0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
 
 //����һ��ȫ����Ŀ��˵�����ص����ݵ��߼�ֵ���������Ƿ��ص��������ֵ����
 //��СΪ0����Ϊ����������Bit����ʾһ�������������СΪ0�����Ϊ1��
 0x15, 0x00, //     LOGICAL_MINIMUM (0)
 
 //����һ��ȫ����Ŀ��˵���߼�ֵ���Ϊ1��
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 
 //����һ��ȫ����Ŀ��˵�������������Ϊ������
 0x95, 0x03, //     REPORT_COUNT (3)
 
 //����һ��ȫ����Ŀ��˵��ÿ��������ĳ���Ϊ1��bit��
 0x75, 0x01, //     REPORT_SIZE (1)
 
 //����һ������Ŀ��˵����3������Ϊ1bit�������������ͳ���
 //��ǰ�������ȫ����Ŀ�����壩������Ϊ���룬
 //����Ϊ��Data,Var,Abs��Data��ʾ��Щ���ݿ��Ա䶯��Var��ʾ
 //��Щ�������Ƕ����ģ�ÿ�����ʾһ����˼��Abs��ʾ����ֵ��
 //��������Ľ�����ǣ���һ��������bit0��ʾ����1��������Ƿ��£�
 //�ڶ���������bit1��ʾ����2���Ҽ����Ƿ��£�������������bit2��ʾ
 //����3���м����Ƿ��¡�
 0x81, 0x02, //     INPUT (Data,Var,Abs)
 
 //����һ��ȫ����Ŀ��˵������������Ϊ1��
 0x95, 0x01, //     REPORT_COUNT (1)
 
 //����һ��ȫ����Ŀ��˵��ÿ��������ĳ���Ϊ5bit��
 0x75, 0x05, //     REPORT_SIZE (5)
 
 //����һ������Ŀ�������ã���ǰ������ȫ����Ŀ��֪������Ϊ5bit��
 //����Ϊ1������������Ϊ�����������ص�����һֱ��0����
 //���ֻ��Ϊ�˴���һ���ֽڣ�ǰ������3��bit��������һЩ����
 //���ѣ���������û��ʵ����;�ġ�
 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
 
 //����һ��ȫ����Ŀ��ѡ����;ҳΪ��ͨ����Generic Desktop Page(0x01)
 0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
 
 //����һ���ֲ���Ŀ��˵����;ΪX��
 0x09, 0x30, //     USAGE (X)
 
 //����һ���ֲ���Ŀ��˵����;ΪY��
 0x09, 0x31, //     USAGE (Y)
 
 //����һ���ֲ���Ŀ��˵����;Ϊ����
 0x09, 0x38, //     USAGE (Wheel)
 
 //��������Ϊȫ����Ŀ��˵�����ص��߼���С�����ֵ��
 //��Ϊ���ָ���ƶ�ʱ��ͨ���������ֵ����ʾ�ģ�
 //���ֵ����˼���ǣ���ָ���ƶ�ʱ��ֻ�����ƶ�����
 //�����ƶ�ʱ��XֵΪ���������ƶ�ʱ��YֵΪ����
 //���ڹ��֣����������Ϲ�ʱ��ֵΪ����
 0x15, 0x81, //     LOGICAL_MINIMUM (-127)
 0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
 
 //����һ��ȫ����Ŀ��˵��������ĳ���Ϊ8bit��
 0x75, 0x08, //     REPORT_SIZE (8)
 
 //����һ��ȫ����Ŀ��˵��������ĸ���Ϊ3����
 0x95, 0x03, //     REPORT_COUNT (3)
 
 //����һ������Ŀ����˵��������8bit���������������õģ�
 //����Ϊ��Data,Var,Rel��Data˵�������ǿ��Ա�ģ�Var˵��
 //��Щ�������Ƕ����ģ�����һ��8bit��ʾX�ᣬ�ڶ���8bit��ʾ
 //Y�ᣬ������8bit��ʾ���֡�Rel��ʾ��Щֵ�����ֵ��
 0x81, 0x06, //     INPUT (Data,Var,Rel)
 
 //��������������Ŀ�����ر�ǰ��ļ����á�
 //���ǿ����������ϣ�����Ҫ�����Ρ�bSizeΪ0�����Ժ���û���ݡ�
 0xc0,       //   END_COLLECTION
 0xc0        // END_COLLECTION
};
//ͨ������ı����������Ķ��壬����֪�����ص����뱨�����4�ֽڡ�
//��һ�ֽڵĵ�3λ������ʾ�����Ƿ��µģ���5λΪ����0�����á�
//�ڶ��ֽڱ�ʾX��ĵı����������ֽڱ�ʾY��ĸı����������ֽڱ�ʾ
//���ֵĸı������������ж϶˵�1��Ӧ��Ҫ��������ĸ�ʽ����ʵ�ʵ�
//������ݡ�
///////////////////////////�������������////////////////////////////

//USB�������������ϵĶ���
//�����������ܳ���Ϊ9+9+9+7�ֽ�
uint8_t ConfigurationDescriptor[9+9+9+7]=
{
 /***************����������***********************/
 //bLength�ֶΡ������������ĳ���Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ��������������Ϊ0x02��
 0x02,
 
 //wTotalLength�ֶΡ��������������ϵ��ܳ��ȣ�
 //�������������������ӿ��������������������˵��������ȡ�
 sizeof(ConfigurationDescriptor)&0xFF, //���ֽ�
 (sizeof(ConfigurationDescriptor)>>8)&0xFF, //���ֽ�
 
 //bNumInterfaces�ֶΡ������ð����Ľӿ�����ֻ��һ���ӿڡ�
 0x01,
 
 //bConfiguration�ֶΡ������õ�ֵΪ1��
 0x01,
 
 //iConfigurationz�ֶΣ������õ��ַ�������������û�У�Ϊ0��
 0x00,
 
 //bmAttributes�ֶΣ����豸�����ԡ��������ǵİ��������߹���ģ�
 //�������ǲ���ʵ��Զ�̻��ѵĹ��ܣ����Ը��ֶε�ֵΪ0x80��
 0x80,
 
 //bMaxPower�ֶΣ����豸��Ҫ�������������������ǵİ���
 //��Ҫ�ĵ�������100mA�����������������Ϊ100mA������ÿ��λ
 //����Ϊ2mA��������������Ϊ50(0x32)��
 0x32,
 
 /*******************�ӿ�������*********************/
 //bLength�ֶΡ��ӿ��������ĳ���Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ��ӿ��������ı��Ϊ0x04��
 0x04,
 
 //bInterfaceNumber�ֶΡ��ýӿڵı�ţ���һ���ӿڣ����Ϊ0��
 0x00,
 
 //bAlternateSetting�ֶΡ��ýӿڵı��ñ�ţ�Ϊ0��
 0x00,
 
 //bNumEndpoints�ֶΡ���0�˵����Ŀ������USB���ֻ��Ҫһ��
 //�ж�����˵㣬��˸�ֵΪ1��
 0x01,
 
 //bInterfaceClass�ֶΡ��ýӿ���ʹ�õ��ࡣUSB�����HID�࣬
 //HID��ı���Ϊ0x03��
 0x03,
 
 //bInterfaceSubClass�ֶΡ��ýӿ���ʹ�õ����ࡣ��HID1.1Э���У�
 //ֻ�涨��һ�����ࣺ֧��BIOS�������������ࡣ
 //USB���̡�������ڸ����࣬�������Ϊ0x01��
 0x01,
 
 //bInterfaceProtocol�ֶΡ��������Ϊ֧���������������࣬
 //��Э���ѡ�����ͼ��̡����̴���Ϊ0x01��������Ϊ0x02��
 0x02,
 
 //iConfiguration�ֶΡ��ýӿڵ��ַ�������ֵ������û�У�Ϊ0��
 0x00,
 
 /******************HID������************************/
 //bLength�ֶΡ���HID��������ֻ��һ���¼������������Գ���Ϊ9�ֽڡ�
 0x09,
 
 //bDescriptorType�ֶΡ�HID�������ı��Ϊ0x21��
 0x21,
 
 //bcdHID�ֶΡ���Э��ʹ�õ�HID1.1Э�顣ע����ֽ����ȡ�
 0x10,
 0x01,
 
 //bCountyCode�ֶΡ��豸���õĹ��Ҵ��룬����ѡ��Ϊ����������0x21��
 0x21,
 
 //bNumDescriptors�ֶΡ��¼�����������Ŀ������ֻ��һ��������������
 0x01,
 
 //bDescritporType�ֶΡ��¼������������ͣ�Ϊ���������������Ϊ0x22��
 0x22,
 
 //bDescriptorLength�ֶΡ��¼��������ĳ��ȡ��¼�������Ϊ������������
 sizeof(ReportDescriptor)&0xFF,
 (sizeof(ReportDescriptor)>>8)&0xFF,
 
 /**********************�˵�������***********************/
 //bLength�ֶΡ��˵�����������Ϊ7�ֽڡ�
 0x07,
 
 //bDescriptorType�ֶΡ��˵����������Ϊ0x05��
 0x05,
 
 //bEndpointAddress�ֶΡ��˵�ĵ�ַ������ʹ��D12������˵�1��
 //D7λ��ʾ���ݷ�������˵�D7Ϊ1����������˵�1�ĵ�ַΪ0x81��
 0x81,
 
 //bmAttributes�ֶΡ�D1~D0Ϊ�˵㴫������ѡ��
 //�ö˵�Ϊ�ж϶˵㡣�ж϶˵�ı��Ϊ3������λ����Ϊ0��
 0x03,
 
 //wMaxPacketSize�ֶΡ��ö˵�����������˵�1��������Ϊ16�ֽڡ�
 //ע����ֽ����ȡ�
 0x10,
 0x00,
 
 //bInterval�ֶΡ��˵��ѯ��ʱ�䣬��������Ϊ10��֡ʱ�䣬��10ms��
 0x0A
};
////////////////////////�����������������//////////////////////////

/************************����ID�Ķ���********************/
uint8_t LanguageId[4]=
{
 0x04, //���������ĳ���
 0x03, //�ַ���������
 //0x0409Ϊ��ʽӢ���ID
 0x09,
 0x04
};
////////////////////////����ID���//////////////////////////////////

/**************************************************/
/*********        ��ת���������         **********/
/********* Http://computer00.21ic.org    **********/
/*********        ����: ����ȦȦ         **********/
/*********         ��ӭ���ʹ��          **********/
/*********    ��Ȩ���У�������д������   **********/
/**************************************************/

//http://computer00.21ic.org/user1/2198/archives/2007/42769.html
//�ַ���������ȦȦ��USBר�� Http://group.ednchina.com/93/����Unicode����
//8λС�˸�ʽ
uint8_t ManufacturerStringDescriptor[82]={
82,         //���������ĳ���Ϊ82�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
0x35, 0x75, //��
0x11, 0x81, //��
0x08, 0x57, //Ȧ
0x08, 0x57, //Ȧ
0x84, 0x76, //��
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x13, 0x4e, //ר
0x3a, 0x53, //��
0x20, 0x00, // 
0x48, 0x00, //H
0x74, 0x00, //t
0x74, 0x00, //t
0x70, 0x00, //p
0x3a, 0x00, //:
0x2f, 0x00, ///
0x2f, 0x00, ///
0x67, 0x00, //g
0x72, 0x00, //r
0x6f, 0x00, //o
0x75, 0x00, //u
0x70, 0x00, //p
0x2e, 0x00, //.
0x65, 0x00, //e
0x64, 0x00, //d
0x6e, 0x00, //n
0x63, 0x00, //c
0x68, 0x00, //h
0x69, 0x00, //i
0x6e, 0x00, //n
0x61, 0x00, //a
0x2e, 0x00, //.
0x63, 0x00, //c
0x6f, 0x00, //o
0x6d, 0x00, //m
0x2f, 0x00, ///
0x39, 0x00, //9
0x33, 0x00, //3
0x2f, 0x00  ///
};
/////////////////////////�����ַ�������/////////////////////////////

//�ַ�������ȦȦ������USB��֮USB��ꡱ��Unicode����
//8λС�˸�ʽ
uint8_t ProductStringDescriptor[34]={
34,         //���������ĳ���Ϊ34�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
0x0a, 0x30, //��
0x08, 0x57, //Ȧ
0x08, 0x57, //Ȧ
0x59, 0x65, //��
0x60, 0x4f, //��
0xa9, 0x73, //��
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x0b, 0x30, //��
0x4b, 0x4e, //֮
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x20, 0x9f, //��
0x07, 0x68  //��
};
////////////////////////��Ʒ�ַ�������////////////////////////////

//�ַ�����2008-07-07����Unicode����
//8λС�˸�ʽ
uint8_t SerialNumberStringDescriptor[22]={
22,         //���������ĳ���Ϊ22�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
0x32, 0x00, //2
0x30, 0x00, //0
0x30, 0x00, //0
0x38, 0x00, //8
0x2d, 0x00, //-
0x30, 0x00, //0
0x37, 0x00, //7
0x2d, 0x00, //-
0x30, 0x00, //0
0x37, 0x00  //7
};
//////////////////////��Ʒ���к��ַ�������/////////////////////////


/********************************************************************
�������ܣ�USB�Ͽ����Ӻ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbDisconnect(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("�Ͽ�USB���ӡ�\r\n");
#endif
 D12WriteCommand(D12_SET_MODE);  //д����ģʽ����
 D12WriteByte(0x06); //����ģʽ�ĵ�һ�ֽ�,�Ͽ���������������
 D12WriteByte(0x47); //����ģʽ�ĵڶ��ֽڣ�CLKOUTʱ��Ƶ��Ϊ6MHz
 DelayXms(1000);     //�ӳ�1��
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ�USB���Ӻ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbConnect(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("����USB��\r\n");
#endif
 D12WriteCommand(D12_SET_MODE);  //д����ģʽ����
 D12WriteByte(0x16); //����ģʽ�ĵ�һ�ֽ�,��������������������
 D12WriteByte(0x47); //����ģʽ�ĵڶ��ֽ�
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ����߹����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbBusSuspend(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("USB���߹���\r\n");
#endif
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ����߸�λ�жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbBusReset(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("USB���߸�λ��\r\n");
#endif
 Ep1InIsBusy=0; //��λ��˵�1���뻺�������С�
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ�����pData��SendLength�����ݷ��͵��˵�0�ĺ�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp0SendData(void)
{
 //������д���˵���ȥ׼������
 //д֮ǰҪ���ж�һ����Ҫ���͵������Ƿ�ȶ˵�0
 //��󳤶ȴ���������˵��С����һ��ֻ�ܷ���
 //�����������ݡ��˵�0����������DeviceDescriptor[7]
 if(SendLength>DeviceDescriptor[7])
 {
  //���������ȷ���
  D12WriteEndpointBuffer(1,DeviceDescriptor[7],pSendData);
  //���ͺ�ʣ���ֽ�������������
  SendLength-=DeviceDescriptor[7];
  //����һ�κ�ָ��λ��Ҫ����
  pSendData+= DeviceDescriptor[7];
 }
 else
 {
  if(SendLength!=0)
  {
   //����������������ֱ�ӷ���
   D12WriteEndpointBuffer(1,SendLength,pSendData);
   //������Ϻ�SendLength���ȱ�Ϊ0
   SendLength=0;
  }
  else //���Ҫ���͵����ݰ�����Ϊ0
  {
   if(NeedZeroPacket==1) //�����Ҫ����0��������
   {
    D12WriteEndpointBuffer(1,0,pSendData); //����0�������ݰ�
    NeedZeroPacket=0; //����Ҫ����0�������ݰ���־
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�0����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp0Out(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("USB�˵�0����жϡ�\r\n");
#endif
 //��ȡ�˵�0��������״̬���ò�������жϱ�־
 //���жϵ�5λ�Ƿ�Ϊ1������ǣ���˵���ǽ�����
 if(D12ReadEndpointLastStatus(0)&0x20)
 {
  D12ReadEndpointBuffer(0,16,Buffer); //��������������
  D12AcknowledgeSetup(); //Ӧ������
  D12ClearBuffer(); //�建����
  //������������豸����ĸ��ֶ���
  bmRequestType=Buffer[0];
  bRequest=Buffer[1];
  wValue=Buffer[2]+(((uint16_t)Buffer[3])<<8);
  wIndex=Buffer[4]+(((uint16_t)Buffer[5])<<8);
  wLength=Buffer[6]+(((uint16_t)Buffer[7])<<8);
  //����Ĵ����жϾ�������󣬲����ݲ�ͬ�����������ز���
  //���D7λΪ1����˵������������
  if((bmRequestType&0x80)==0x80)
  {
   //����bmRequestType��D6~5λɢת��D6~5λ��ʾ���������
   //0Ϊ��׼����1Ϊ������2Ϊ��������
   switch((bmRequestType>>5)&0x03)
   {
    case 0:  //��׼����
     #ifdef DEBUG0
      AM_DBG_INFO("USB��׼��������");
     #endif
     //USBЭ�鶨���˼�����׼������������ʵ����Щ��׼���󼴿�
     //����Ĵ�����bRequest�У��Բ�ͬ������������ɢת
     //��ʵ�ϣ����ǻ���Ҫ�Խ����߽���ɢת����Ϊ��ͬ�����������
     //�ǲ�һ���ġ���������bmRequestType��D4~D0λ�ж��塣
     //��������Ϊ�˼򻯲�������Щ��ʡ���˶Խ����ߵ��жϡ�
     //�����ȡ������������ֻ����������������������
     switch(bRequest)
     {
      case GET_CONFIGURATION: //��ȡ����
       #ifdef DEBUG0
        AM_DBG_INFO("��ȡ���á�\r\n");
       #endif
      break;
      
      case GET_DESCRIPTOR:  //��ȡ������
       #ifdef DEBUG0
        AM_DBG_INFO("��ȡ����������");
       #endif
       //�����������ͽ���ɢת������ȫ���豸��
       //��׼����ֻ֧�ַ��͵��豸���豸�����á��ַ�������������
       switch((wValue>>8)&0xFF)
        {
         case DEVICE_DESCRIPTOR: //�豸������
          #ifdef DEBUG0
           AM_DBG_INFO("�豸��������\r\n");
          #endif
          pSendData=DeviceDescriptor;  //��Ҫ���͵�����
          //�ж�������ֽ����Ƿ��ʵ����Ҫ���͵��ֽ�����
          //������������豸��������������ݳ��Ⱦ���
          //DeviceDescriptor[0]���������ı�ʵ�ʵĳ���
          //��ôֻ����ʵ�ʳ��ȵ�����
          if(wLength>DeviceDescriptor[0])
          {
           SendLength=DeviceDescriptor[0];
           if(SendLength%DeviceDescriptor[7]==0) //���Ҹպ������������ݰ�ʱ
           {
            NeedZeroPacket=1; //��Ҫ����0���ȵ����ݰ�
           }
          }
          else
          {
           SendLength=wLength;
          }
          //������ͨ��EP0����
          UsbEp0SendData();
         break;
         
         case CONFIGURATION_DESCRIPTOR:  //����������
          #ifdef DEBUG0
           AM_DBG_INFO("������������\r\n");
          #endif
          pSendData=ConfigurationDescriptor; //��Ҫ���͵�����Ϊ����������
          //�ж�������ֽ����Ƿ��ʵ����Ҫ���͵��ֽ�����
          //������������������������ϣ�������ݳ��Ⱦ���
          //ConfigurationDescriptor[3]*256+ConfigurationDescriptor[2]��
          //�������ı�ʵ�ʵĳ�����ôֻ����ʵ�ʳ��ȵ�����
          SendLength=ConfigurationDescriptor[3];
          SendLength=SendLength*256+ConfigurationDescriptor[2];
          if(wLength>SendLength)
          {
           if(SendLength%DeviceDescriptor[7]==0) //���Ҹպ������������ݰ�ʱ
           {
            NeedZeroPacket=1; //��Ҫ����0���ȵ����ݰ�
           }
          }
          else
          {
           SendLength=wLength;
          }
          //������ͨ��EP0����
          UsbEp0SendData();
         break;
         
         case STRING_DESCRIPTOR:  //�ַ���������
          #ifdef DEBUG0
           AM_DBG_INFO("�ַ���������");
          #endif
          switch(wValue&0xFF)  //����wValue�ĵ��ֽڣ�����ֵ��ɢת
          {
           case 0:  //��ȡ����ID
            #ifdef DEBUG0
             AM_DBG_INFO("(����ID)��\r\n");
            #endif
            pSendData=LanguageId;
            SendLength=LanguageId[0];
           break;
           
           case 1:  //�����ַ���������ֵΪ1����������Ϊ�����ַ���
           #ifdef DEBUG0
             AM_DBG_INFO("(��������)��\r\n");
            #endif
            pSendData=ManufacturerStringDescriptor;
            SendLength=ManufacturerStringDescriptor[0];
           break;
           
           case 2:  //��Ʒ�ַ���������ֵΪ2����������Ϊ��Ʒ�ַ���
           #ifdef DEBUG0
             AM_DBG_INFO("(��Ʒ����)��\r\n");
            #endif
            pSendData=ProductStringDescriptor;
            SendLength=ProductStringDescriptor[0];
           break;
           
           case 3:  //��Ʒ���кŵ�����ֵΪ3����������Ϊ���к�
           #ifdef DEBUG0
             AM_DBG_INFO("(��Ʒ���к�)��\r\n");
            #endif
            pSendData=SerialNumberStringDescriptor;
            SendLength=SerialNumberStringDescriptor[0];
           break;
           
           default :
            #ifdef DEBUG0
             AM_DBG_INFO("(δ֪������ֵ)��\r\n");
            #endif
            //����δ֪����ֵ�����󣬷���һ��0���ȵİ�
            SendLength=0;
            NeedZeroPacket=1;
           break;
          }
          //�ж�������ֽ����Ƿ��ʵ����Ҫ���͵��ֽ�����
          //�������ı�ʵ�ʵĳ�����ôֻ����ʵ�ʳ��ȵ�����
          if(wLength>SendLength)
          {
           if(SendLength%DeviceDescriptor[7]==0) //���Ҹպ������������ݰ�ʱ
           {
            NeedZeroPacket=1; //��Ҫ����0���ȵ����ݰ�
           }
          }
          else
          {
           SendLength=wLength;
          }
          //������ͨ��EP0����
          UsbEp0SendData();         
         break;

         case REPORT_DESCRIPTOR:  //����������
          #ifdef DEBUG0
           AM_DBG_INFO("������������\r\n");
          #endif
          pSendData=ReportDescriptor; //��Ҫ���͵�����Ϊ����������
          SendLength=sizeof(ReportDescriptor); //��Ҫ���ص����ݳ���         
          //�ж�������ֽ����Ƿ��ʵ����Ҫ���͵��ֽ�����
          //�������ı�ʵ�ʵĳ�����ôֻ����ʵ�ʳ��ȵ�����
          if(wLength>SendLength)
          {
           if(SendLength%DeviceDescriptor[7]==0) //���Ҹպ������������ݰ�ʱ
           {
            NeedZeroPacket=1; //��Ҫ����0���ȵ����ݰ�
           }
          }
          else
          {
           SendLength=wLength;
          }
          //������ͨ��EP0����
          UsbEp0SendData();
         break;
                 
         default:  //����������
          #ifdef DEBUG0
           AM_DBG_INFO("���������������������룺");
           AM_DBG_INFO("0x%x",(wValue>>8)&0xFF);
           AM_DBG_INFO("\r\n");
          #endif
         break;
        }
       break;
      
      case GET_INTERFACE: //��ȡ�ӿ�
       #ifdef DEBUG0
        AM_DBG_INFO("��ȡ�ӿڡ�\r\n");
       #endif
      break;
      
      case GET_STATUS: //��ȡ״̬
       #ifdef DEBUG0
        AM_DBG_INFO("��ȡ״̬��\r\n");
       #endif
      break;
      
      case SYNCH_FRAME: //ͬ��֡
       #ifdef DEBUG0
        AM_DBG_INFO("ͬ��֡��\r\n");
       #endif
      break;
      
      default:  //δ����ı�׼����
       #ifdef DEBUG0
        AM_DBG_INFO("����δ����ı�׼��������\r\n");
       #endif       
      break;
     }
    break;
    
    case 1:  //������
     #ifdef DEBUG0
      AM_DBG_INFO("USB����������\r\n");
     #endif
    break;
    
    case 2:  //��������
     #ifdef DEBUG0
      AM_DBG_INFO("USB������������\r\n");
     #endif
    break;
    
    default: //δ�������������ֻ��ʾһ��������Ϣ��
     #ifdef DEBUG0
      AM_DBG_INFO("����δ�������������\r\n");
     #endif
    break;
   }
  }
  //����˵�����������
  else //if(bmRequestType&0x80==0x80)֮else
  {
   //����bmRequestType��D6~5λɢת��D6~5λ��ʾ���������
   //0Ϊ��׼����1Ϊ������2Ϊ��������
   switch((bmRequestType>>5)&0x03)
   {
    case 0:  //��׼����
     #ifdef DEBUG0
      AM_DBG_INFO("USB��׼�������");
     #endif
     //USBЭ�鶨���˼�����׼�����������ʵ����Щ��׼���󼴿�
     //����Ĵ�����bRequest�У��Բ�ͬ������������ɢת
     switch(bRequest)
     {
      case CLEAR_FEATURE: //�������
       #ifdef DEBUG0
        AM_DBG_INFO("������ԡ�\r\n");
       #endif
      break;
      
      case SET_ADDRESS:  //���õ�ַ
       #ifdef DEBUG0
        AM_DBG_INFO("���õ�ַ����ַΪ��");
        AM_DBG_INFO("0x%x",wValue&0xFF); //��ʾ�����õĵ�ַ
        AM_DBG_INFO("\r\n");
       #endif
       D12SetAddress(wValue&0xFF); //wValue�еĵ��ֽ������õĵ�ֵַ
       //���õ�ַû�����ݹ��̣�ֱ�ӽ��뵽״̬���̣�����һ��0���ȵ����ݰ�
       SendLength=0;
       NeedZeroPacket=1;
       //������ͨ��EP0����
       UsbEp0SendData();
      break;
      
      case SET_CONFIGURATION: //��������
       #ifdef DEBUG0
        AM_DBG_INFO("�������á�\r\n");
       #endif
       //ʹ�ܷ�0�˵㡣��0�˵�ֻ��������Ϊ��0�����ú����ʹ�ܡ�
       //wValue�ĵ��ֽ�Ϊ���õ�ֵ�������ֵΪ��0������ʹ�ܷ�0�˵㡣
       //���浱ǰ����ֵ
       ConfigValue=wValue&0xFF;
       D12SetEndpointEnable(ConfigValue);
       //����һ��0���ȵ�״̬���ݰ�
       SendLength=0;
       NeedZeroPacket=1;
       //������ͨ��EP0����
       UsbEp0SendData();
      break;
      
      case SET_DESCRIPTOR: //����������
       #ifdef DEBUG0
        AM_DBG_INFO("������������\r\n");
       #endif
      break;
      
      case SET_FEATURE: //��������
       #ifdef DEBUG0
        AM_DBG_INFO("�������ԡ�\r\n");
       #endif
      break;

      case SET_INTERFACE: //���ýӿ�
       #ifdef DEBUG0
        AM_DBG_INFO("���ýӿڡ�\r\n");
       #endif
      break;      
      
      default:  //δ����ı�׼����
       #ifdef DEBUG0
        AM_DBG_INFO("����δ����ı�׼�������\r\n");
       #endif       
      break;
     }
    break;
    
    case 1:  //������
     #ifdef DEBUG0
      AM_DBG_INFO("USB���������");
     #endif
     switch(bRequest)
     {
      case SET_IDLE:
       #ifdef DEBUG0
        AM_DBG_INFO("���ÿ��С�\r\n");
       #endif
       //ֻ��Ҫ����һ��0���ȵ����ݰ�����
       SendLength=0;
       NeedZeroPacket=1;
       //������ͨ��EP0����
       UsbEp0SendData();
      break;
      
      default:
       #ifdef DEBUG0
        AM_DBG_INFO("δ֪����\r\n");
       #endif
      break;
     }
    break;
    
    case 2:  //��������
     #ifdef DEBUG0
      AM_DBG_INFO("USB�����������\r\n");
     #endif
    break;
    
    default: //δ�������������ֻ��ʾһ��������Ϣ��
     #ifdef DEBUG0
      AM_DBG_INFO("����δ������������\r\n");
     #endif
    break;
   }
  }
 }
 //��ͨ�������
 else //if(D12ReadEndpointLastStatus(0)&0x20)֮else
 {
  D12ReadEndpointBuffer(0,16,Buffer);
  D12ClearBuffer();
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�0�����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp0In(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("USB�˵�0�����жϡ�\r\n");
#endif
 //�������״̬���⽫����˵�0���жϱ�־λ
 D12ReadEndpointLastStatus(1);
 //����ʣ����ֽ���
 UsbEp0SendData();
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�1����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp1Out(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("USB�˵�1����жϡ�\r\n");
#endif
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�1�����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp1In(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("USB�˵�1�����жϡ�\r\n");
#endif
 //�������״̬���⽫����˵�1������жϱ�־λ
 D12ReadEndpointLastStatus(3);
 //�˵�1���봦�ڿ���״̬
 Ep1InIsBusy=0;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�2����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp2Out(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("USB�˵�2����жϡ�\r\n");
#endif
}
////////////////////////End of function//////////////////////////////

/********************************************************************
�������ܣ��˵�2�����жϴ�������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void UsbEp2In(void)
{
#ifdef DEBUG0
 AM_DBG_INFO("USB�˵�2�����жϡ�\r\n");
#endif
}
////////////////////////End of function//////////////////////////////


/** \brief �豸�ж����� */
void am_usbcore_int_cfg (am_pfnvoid_t pfn_isr, void *p_arg, int pin)
{
    /* ��������Ϊ��������ģʽ        */   
    am_gpio_pin_cfg(pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);  
    
    am_gpio_set(pin, 1);
    
    /* �����жϷ�����              */
    am_gpio_trigger_connect(pin, pfn_isr, p_arg);
        
    /* ���ô������ź��½��ش���ģʽ  */
    am_gpio_trigger_cfg(pin, AM_GPIO_TRIGGER_RISE);
        
    /* ʹ�����Ŵ����ж�              */
    am_gpio_trigger_on(pin);    
}

 
/**
 * \brief �豸�жϴ����� 
 * \param[in] flag   : �жϱ�־λ��ֵΪ0��1��0���ж�δ������1���ж��ѷ��� 
 * \return    ��
 */
void am_usbcore_intterupt_handle (void)
{
    uint8_t InterruptSource = 0;
        
    D12WriteCommand(READ_INTERRUPT_REGISTER);  //д���жϼĴ���������
    InterruptSource=D12ReadByte();             //���ص�һ�ֽڵ��жϼĴ���
    if(InterruptSource&0x80)UsbBusSuspend();   //���߹����жϴ���
    if(InterruptSource&0x40)UsbBusReset();     //���߸�λ�жϴ���
    if(InterruptSource&0x01)UsbEp0Out();       //�˵�0����жϴ���
    if(InterruptSource&0x02)UsbEp0In();        //�˵�0�����жϴ���
    if(InterruptSource&0x04)UsbEp1Out();       //�˵�1����жϴ���
    if(InterruptSource&0x08)UsbEp1In();        //�˵�1�����жϴ���
    if(InterruptSource&0x10)UsbEp2Out();       //�˵�2����жϴ���
    if(InterruptSource&0x20)UsbEp2In();        //�˵�2�����жϴ���    
              
    InterruptSource = 0;
} 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/* end of file */ 



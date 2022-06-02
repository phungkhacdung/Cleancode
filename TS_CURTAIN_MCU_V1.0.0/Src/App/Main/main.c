/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
*******************************************************************************
 *				 _ _                                             _ _
				|   |                                           (_ _)
				|   |        _ _     _ _   _ _ _ _ _ _ _ _ _ _   _ _
				|   |       |   |   |   | |    _ _     _ _    | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |_ _ _  |   |_ _|   | |   |   |   |   |   | |   |
				|_ _ _ _ _| |_ _ _ _ _ _| |_ _|   |_ _|   |_ _| |_ _|
								(C)2022 Lumi
 * Copyright (c) 2022
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File name: main.h
 *
 * Description:
 *
 *
 * Last Changed By:  $Author: dungpk $
 * Revision:         $Revision: $
 * Last Changed:     $Date: $June 1, 2022
 *
 * Code sample:
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include <stdint.h>
#include <stm32f401re_gpio.h>
#include <stm32f401re_exti.h>
#include <stm32f401re_syscfg.h>
#include <stm32f401re_rcc.h>
#include <misc.h>
#include <timer.h>
#include <typedefs.h>

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/
#define PRESS_TIMEOUT 						500 //ms
#define RELEASE_TIMEOUT						400 //ms

#define GPIO_PIN_SET						1
#define GPIO_PIN_RESET						0
#define GPIO_PIN_LOW						0
#define GPIO_PIN_HIGH						1

/*Dinh nghia chan Led Green1 */
#define LEDGREEN_PORT						GPIOA // Dinh nghia GPIA cua Port Led Green
#define LEDGREEN_GPIO_PIN					GPIO_Pin_0 // Dinh nghia Pin dung Led Green
#define LEDGREEN_PIN						0 // Pin Led Green
#define LEDGREEN_SetClock					RCC_AHB1Periph_GPIOA // Bus cap clock cho GPIOA

/*Dinh nghia chan Led Green2 */
#define LEDGREEN2_PORT						GPIOA // Dinh nghia GPIA cua Port Led Green
#define LEDGREEN2_GPIO_PIN					GPIO_Pin_11 // Dinh nghia Pin dung Led Green
#define LEDGREEN2_PIN						11 // Pin Led Green
#define LEDGREEN2_SetClock					RCC_AHB1Periph_GPIOA // Bus cap clock cho GPIOA

/*Dinh nghia chan Led Blue */
#define LEDBLUE_PORT						GPIOA // Dinh nghia GPIA cua Port Led Green
#define LEDBLUE_GPIO_PIN					GPIO_Pin_10 // Dinh nghia Pin dung Led Green
#define LEDBLUE_PIN							10 // Pin Led Green
#define LEDBLUE_SetClock					RCC_AHB1Periph_GPIOA // Bus cap clock cho GPIOA

/*Dinh nghia chan Led Red */
#define LEDRED_PORT							GPIOA // Dinh nghia GPIA cua Port Led Green
#define LEDRED_GPIO_PIN						GPIO_Pin_1 // Dinh nghia Pin dung Led Green
#define LEDRED_PIN							1 // Pin Led Green
#define LEDRED_SetClock						RCC_AHB1Periph_GPIOA // Bus cap clock cho GPIOA



/*Dinh nghia chan Buzzer*/
#define BUZZER_GPIO_PORT					GPIOC //Dinh nghia GPIO Port
#define BUZZER_GPIO_PIN						GPIO_Pin_9 // Dinh nghia Pin buzzer
#define BUZZER_PIN9							9 // Dinh nghia chan buzzer
#define	BUZZERControl_SetClock				RCC_AHB1Periph_GPIOC // Dinh nghia bus cap nguon cho buzzer

/*Dinh nghia chan button B2*/
#define BUTTONB2_IT_PORT					GPIOB //Dinh nghia GPIO Port
#define BUTTONB2_IT_GPIO_PIN				GPIO_Pin_3  // Dinh nghia Pin button
#define BUTTONIT2_SetClock					RCC_AHB1Periph_GPIOB // Dinh nghia bus cap nguon cho button

 /*Dinh nghia chan button B3*/
#define BUTTONB3_IT_PORT					GPIOA //Dinh nghia GPIO Port
#define BUTTONB3_IT_GPIO_PIN				GPIO_Pin_4  // Dinh nghia Pin button
#define BUTTONIT3_SetClock					RCC_AHB1Periph_GPIOA // Dinh nghia bus cap nguon cho button


/*Dinh nghia chan button B4*/
#define BUTTONB4_IT_PORT					GPIOB //Dinh nghia GPIO Port
#define BUTTONB4_IT_GPIO_PIN				GPIO_Pin_0  // Dinh nghia Pin button
#define BUTTONIT4_SetClock					RCC_AHB1Periph_GPIOB // Dinh nghia bus cap nguon cho

enum{
	NO_PRESS = 0x00,
	PRESS_1TIMES = 0x02,
	PRESS_2TIMES = 0x04,
	PRESS_3TIMES = 0x06
};

enum
{
	SET = 0x00,
	RESET = 0x01
};

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/*Cac bien can dung*/
bool_t boHoldFlag_LedBlue = 0; //Khai bao bien dieu khien led blue truong hop giu 500ms
bool_t boHold_LedRed = 0; //Khai bao bien dieu khien led red truong hop giu 500ms
i8_t byPressFlag_Blue = 0; //Khai bao bien dieu khien led blue truong hop nhan 2 lan lien tiep
i8_t byPressFlag_LedRed = 0; //Khai bao bien dieu khien led red truong hop nhan 2 lan lien tiep
i8_t byCountPressB3 = 0; //Khoi tao bien luu gia tri nhan phim B3 5 lan lien tiep
u32_t dwTimeInit_B2Hold = 0; //Khoi tao thoi gian ban dau truong hop nhan giu
u32_t dwTimeCurrent_B2Hold = 0; //Khoi tao thoi gian hien tai de so sanh voi thoi gian khoi tao ban dau truong hop nhan giu
u32_t dwTimeTotal_B2Hold = 0; // KHoi tao bien luu tong thoi gian nhan giu
u32_t dwTimeInit_B2Hold_B2Press = 0; //Khoi tao thoi gian ban dau truong hop nhan nut B2 2 lan lien tiep
u32_t dwTimeCurrent_B2Hold_B2Press = 0;//Khoi tao thoi gian hien tai de so sanh voi thoi gian khoi tao ban dau truong hop nhan nut B2 2 lan lien tiep
u32_t dwTimeTotal_B2Hold_B2Press = 0;// KHoi tao bien luu tong thoi gian ntruong hop nhan nut B2 2 lan lien tiep
u32_t dwTimeInit_B4Hold = 0; //Khoi tao thoi gian ban dau truong hop nhan nut B4 2 lan lien tiep
u32_t dwTimeCurrent_B4Hold = 0;//Khoi tao thoi gian hien tai de so sanh voi thoi gian khoi tao ban dau truong hop nhan nut B4 2 lan lien tiep
u32_t dwTimeTotal_B4Hold = 0;// KHoi tao bien luu tong thoi gian ntruong hop nhan nut B4 2 lan lien tiep

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
static void delayMs(u32_t milisecond);// Ham delay
static void LedBuzz_Init(void); // Ham khoi tao led va buzzer
static void buttonInit (void); // Ham khoi tao nut nhan
static void ledControl_SetState (i8_t led_id,i8_t led_state);//Ham dieu khien led
static void buzzerControl_SetBeep(void); // Ham dieu khien buzzer
static u32_t calculatorTime(u32_t dwTimeInit_B2Hold,u32_t dwTimeCurrent_B2Hold);//Ham tinh thoi gian
static void blinkled_StatusPower(void); // Ham nhay led 4 lan
void EXTI0_IRQHandler(void); // Ham ngat 0
void EXTI3_IRQHandler(void);//Ham ngat 3
void EXTI4_IRQHandler(void);//Ham ngat 4
static void ledControl_TimPress(void);//Ham dieu khien led sang nhan lien
static void ledControl_TimOutPress(void);//Ham dieu khien led nhan 2 lan lien tiep
static void appInitCommon(void);

/******************************************************************************/

int main(void)
{
	appInitCommon();
	while(1)
	{
		ledControl_TimPress(); //Ham dieu khien led sang nhan lien
		ledControl_TimOutPress();//Ham dieu khien led nhan 2 lan lien tiep
	}
}


/*
 * @func   appInitCommon
 * @brief  Initialize peripheral
 * @param  None
 * @retval None
 */
static void_t appInitCommon(void_t)
{
	SystemCoreClockUpdate();//cap xung clock
	TimerInit(); // Khoi tao thoi gian
	LedBuzz_Init();// KHoi tao chan dung led va buzzer
	buttonInit(); // Khoi tao nut nhan
	blinkled_StatusPower(); // Ham nhay led 5 lan bao hieu nhan tin hieu dau vao
}

/***********************
 ****** Ham Delay*******
 *** ******************/

static void delayMs(u32_t milisecond)
{
	for(u32_t i= 0;i<milisecond;i++)
	{
		for(u32_t j=0;j<5000;j++);
	}
}

/******************************
 * Ham khoi tao led va buzzer**
 * ****************************/

static
void LedBuzz_Init(void)
{
	//Khai bao khoi tao GIPO cua Led Green 1

	GPIO_InitTypeDef GPIO_InitStructureLed; //Khai báo biến thuộc kiểu dữ liệu struct GPIO.

	RCC_AHB1PeriphClockCmd(LEDGREEN_SetClock, ENABLE);//Cấp xung clock cho PortA.

	GPIO_InitStructureLed.GPIO_Mode = GPIO_Mode_OUT; //Chọn chân điều khiển led ở chế độ Output.
	GPIO_InitStructureLed.GPIO_Pin = LEDGREEN_GPIO_PIN; //Chọn chân sử dụng với chức năng điều khiển Led.
	GPIO_InitStructureLed.GPIO_Speed = GPIO_Speed_50MHz; //Tốc độ xử lý trên chân là 50MHz.
	GPIO_InitStructureLed.GPIO_OType = GPIO_OType_PP; //Chọn chế độ là đẩy kéo Push Pull.
	GPIO_InitStructureLed.GPIO_PuPd = GPIO_PuPd_DOWN; //Trạng thái ban đầu trên chân là kéo xuống GND Pull Down.

	GPIO_Init(LEDGREEN_PORT,&GPIO_InitStructureLed); //Khởi tạo tất cả các giá trị bên trên bằng cách sử dụng hàm GPIO_Init



	//Khai bao khoi tao GIPO cua Led Green 2

	RCC_AHB1PeriphClockCmd(LEDGREEN2_SetClock, ENABLE); //Cấp xung clock cho LEDGreen 2
	GPIO_InitStructureLed.GPIO_Pin = LEDGREEN2_GPIO_PIN; //Chọn chân sử dụng với chức năng điều khiển Led.
	GPIO_Init(LEDGREEN2_PORT,&GPIO_InitStructureLed); //Khởi tạo tất cả các giá trị bên trên bằng cách sử dụng hàm GPIO_Init

	//Khai bao khoi tao GIPO cua Led Blue

	RCC_AHB1PeriphClockCmd(LEDBLUE_SetClock, ENABLE); //Cấp xung clock cho LEDBlue
	GPIO_InitStructureLed.GPIO_Pin = LEDBLUE_GPIO_PIN; // Chọn chân sử dụng với chức năng điều khiển Led.
	GPIO_Init(LEDBLUE_PORT,&GPIO_InitStructureLed); //Khởi tạo tất cả các giá trị bên trên bằng cách sử dụng hàm GPIO_Init


	//Khai bao khoi tao GIPO cua Led Red

	RCC_AHB1PeriphClockCmd(LEDRED_SetClock, ENABLE); //Cấp xung clock cho LED RED
	GPIO_InitStructureLed.GPIO_Pin = LEDRED_GPIO_PIN; // Chọn chân sử dụng với chức năng điều khiển Led.
	GPIO_Init(LEDRED_PORT,&GPIO_InitStructureLed); //Khởi tạo tất cả các giá trị bên trên bằng cách sử dụng hàm GPIO_Init

	//Khai bao khoi tao GPIO cua buzzer
	GPIO_InitTypeDef GPIO_InitStructureBuzzer; //Khai báo biến thuộc kiểu dữ liệu struct GPIO.
	RCC_AHB1PeriphClockCmd(BUZZERControl_SetClock,ENABLE);  //Cấp xung clock cho Buzzer
	GPIO_InitStructureBuzzer.GPIO_Pin = BUZZER_GPIO_PIN; //Chọn chân sử dụng với chức năng điều khiển Buzzer
	GPIO_InitStructureBuzzer.GPIO_Mode = GPIO_Mode_OUT; ////Chọn chân điều khiển led ở chế độ Output.
	GPIO_InitStructureBuzzer.GPIO_Speed = GPIO_Speed_50MHz;  //Tốc độ xử lý trên chân là 50MHz.
	GPIO_InitStructureBuzzer.GPIO_OType = GPIO_OType_PP; //Chọn chế độ là đẩy kéo Push Pull
	GPIO_InitStructureBuzzer.GPIO_PuPd = GPIO_PuPd_DOWN; //Trạng thái ban đầu trên chân là kéo xuống GND Pull Down.

	GPIO_Init(BUZZER_GPIO_PORT,&GPIO_InitStructureBuzzer); //Khởi tạo tất cả các giá trị bên trên bằng cách sử dụng hàm GPIO_Init

}

/********************************
 **** Ham khoi tao nut nhan******
 ********************************/

static
void buttonInit (void)
{
	/* Cau hinh button nut B2*/

	GPIO_InitTypeDef 		GPIO_InitStructure; //Khai báo các biến thuộc kiểu dữ liệu cấu trúc của GPIO, EXTI, NVIC
	EXTI_InitTypeDef		EXTI_InitStructure;
	NVIC_InitTypeDef		NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(BUTTONIT2_SetClock,ENABLE); // Cấp clock cho Button 2

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //Chọn chế độ trên chân  là Input.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Chọn tốc tốc trên chân PB3 là 50MHz.
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //Chọn chế độ kéo trở lên dương nguồn Pull Up.

	GPIO_InitStructure.GPIO_Pin = BUTTONB2_IT_GPIO_PIN; //Chọn chân để sử dụng
	GPIO_Init(BUTTONB2_IT_PORT,&GPIO_InitStructure); //Sử dụng hàm GPIO_Init với đối số là BUTTONB2_IT_PORT, &amp;GPIO_InitStructure

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //Cấp clock cho ngoại vi Syscfg.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3); //GPIO nào và Line ngắt nào

	EXTI_InitStructure.EXTI_Line = EXTI_Line3; //khởi tạo Line3 được sử dụng chức năng ngắt.
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //Chọn chế độ ngắt.
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //Chọn sườn xuống phát hiện sự kiện ngắt Falling va Rising
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //Cho phép ngắt được hoạt động.
	EXTI_Init(&EXTI_InitStructure); //Sử dụng hàm EXTI_Init để khởi tạo tất cả các giá trị ở trên

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //Sử dụng trình phục vụ ngắt EXTI3_IRQn.
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //Cấu hình ưu tiên ngắt PreemptionPriority mức 0.
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // Cấu hình ưu tiên ngắt SubPriority mức 0.
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //Cho phép sử dụng trình phục vụ ngắt.
	NVIC_Init(&NVIC_InitStructure); //Sử dụng hàm NVIC_Init để cấu hình các giá trị ở trên


	/* Cau hinh button nut B3*/

	RCC_AHB1PeriphClockCmd(BUTTONIT3_SetClock,ENABLE); // Cấp clock cho Button 3

	GPIO_InitStructure.GPIO_Pin = BUTTONB3_IT_GPIO_PIN; //Chọn chân để sử dụng
	GPIO_Init(BUTTONB3_IT_PORT,&GPIO_InitStructure); //Sử dụng hàm GPIO_Init với đối số là BUTTONB2_IT_PORT, &amp;GPIO_InitStructure


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //Cấp clock cho ngoại vi Syscfg.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4); //GPIO nào và Line ngắt nào

	EXTI_InitStructure.EXTI_Line = EXTI_Line4; //khởi tạo Line 4 được sử dụng chức năng ngắt.
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //Chọn chế độ ngắt.
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //Chọn sườn xuống phát hiện sự kiện ngắt Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //Cho phép ngắt được hoạt động.
	EXTI_Init(&EXTI_InitStructure); //Sử dụng hàm EXTI_Init để khởi tạo tất cả các giá trị ở trên

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; //Sử dụng trình phục vụ ngắt EXTI4_IRQn.
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //Cấu hình ưu tiên ngắt PreemptionPriority mức 0.
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// Cấu hình ưu tiên ngắt SubPriority mức 0.
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //Cho phép sử dụng trình phục vụ ngắt.
	NVIC_Init(&NVIC_InitStructure); //Sử dụng hàm NVIC_Init để cấu hình các giá trị ở trên

	/* Cau hinh button nut B4*/

	RCC_AHB1PeriphClockCmd(BUTTONIT4_SetClock,ENABLE); // Cấp clock cho Button 4

	GPIO_InitStructure.GPIO_Pin = BUTTONB4_IT_GPIO_PIN;  //Chọn chân để sử dụng
	GPIO_Init(BUTTONB4_IT_PORT,&GPIO_InitStructure); ////Sử dụng hàm GPIO_Init

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  //Cấp clock cho ngoại vi Syscfg.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);//GPIO nào và Line ngắt nào

	EXTI_InitStructure.EXTI_Line = EXTI_Line0; //khởi tạo Line 0 được sử dụng chức năng ngắt.
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//Chọn chế độ ngắt
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; ////Chọn sườn xuống phát hiện sự kiện ngắt Falling va Rising
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  //Cho phép ngắt được hoạt động.
	EXTI_Init(&EXTI_InitStructure); //Sử dụng hàm EXTI_Init để khởi tạo tất cả các giá trị ở trên

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //Sử dụng trình phục vụ ngắt EXTI0_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //Cấu hình ưu tiên ngắt PreemptionPriority mức 0.
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // Cấu hình ưu tiên ngắt SubPriority mức 0.
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //Sử dụng hàm NVIC_Init để cấu hình các giá trị ở trên
	NVIC_Init(&NVIC_InitStructure);  //Sử dụng hàm NVIC_Init để cấu hình các giá trị ở trên




}
/********************************
 *** Ham khoi tao bat tat led****
 ********************************/

static
void ledControl_SetState (i8_t led_id,i8_t led_state)
{
	if(led_state == GPIO_PIN_SET)
	{
		GPIOA->BSRRL |= 1 << led_id; //Dat gia tri thanh ghi BSRRL len muc cao
	}
	if(led_state == GPIO_PIN_RESET)
	{
		GPIOA->BSRRH |= 1 << led_id; //Dat gia tri thanh ghi BSRRH len muc cao
	}
}

/***************************
 ***Ham dieu khien buzzer***
 ***************************/

static void buzzerControl_SetBeep(void)
{
	//Dieu khien buzzer keu 3 tieng

	GPIO_SetBits(GPIOC,BUZZER_GPIO_PIN); //Bat coi
	delayMs(100);
	GPIO_ResetBits(GPIOC,BUZZER_GPIO_PIN);//Tat coi
	delayMs(100);
	GPIO_SetBits(GPIOC,BUZZER_GPIO_PIN);
	delayMs(100);
	GPIO_ResetBits(GPIOC,BUZZER_GPIO_PIN);
}
/*******************************
 * Ham tinh thoi gian nhan nut**
 *******************************/

static u32_t calculatorTime(u32_t dwTimeInit_B2Hold,u32_t dwTimeCurrent_B2Hold)
{
	u32_t dwdwTimeTotal_B2Hold;
	if(dwTimeCurrent_B2Hold >= dwTimeInit_B2Hold)
	{
		dwdwTimeTotal_B2Hold = dwTimeCurrent_B2Hold - dwTimeInit_B2Hold;
	}
	else
	{
		dwdwTimeTotal_B2Hold = 0xFFFFFFFFU + dwTimeCurrent_B2Hold - dwTimeInit_B2Hold;
	}
	return dwdwTimeTotal_B2Hold;
}

/**************************
 * Ham nhay led 4 lan******
 **************************/

static void blinkled_StatusPower(void)
{
	for(int i=0;i<4;i++)
	{
		ledControl_SetState(LEDGREEN_PIN,1);//Bat led
		delayMs(200);
		ledControl_SetState(LEDGREEN_PIN,0);//Tat led
		delayMs(200);

	}
}

/**********************
 * Ham xu ly ngat 0****
 *********************/

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line0) == SET)
	{
		boHold_LedRed = !boHold_LedRed;//Dao bien gia tri
		dwTimeTotal_B2Hold = 0;//Khoi tao thoi gian bat dau

		byPressFlag_LedRed++ ;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
/***********************
 * Ham xu ly ngat 4*****
 ***********************/

void EXTI4_IRQHandler(void)
{

	if(EXTI_GetFlagStatus(EXTI_Line4) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		byCountPressB3++;
		if(byCountPressB3==5)
		{
			byCountPressB3=0;
			for(int i=0;i<5;i++)
			{
				ledControl_SetState(LEDGREEN_PIN,1); // bat led green 1
				ledControl_SetState(LEDGREEN2_PIN,1);// bat led green 2
				delayMs(200);
				ledControl_SetState(LEDGREEN_PIN,0);//tat led green 1
				ledControl_SetState(LEDGREEN2_PIN,0); // tat led green 2
				delayMs(200);
			}
			buzzerControl_SetBeep();// dieu khien coi keu
		}
	}
}

/***********************
 * Ham xu ly ngat 3*****
 ***********************/

void EXTI3_IRQHandler(void)
{

	if(EXTI_GetFlagStatus(EXTI_Line3) == SET)
	{
		// Xu ly nut nhan B2 giu
		boHoldFlag_LedBlue = !boHoldFlag_LedBlue;
		dwTimeTotal_B2Hold = 0;

		//Xu ly nhan nut B2 2 lan lien tiep
		byPressFlag_Blue++; // Tang bien ledblue len 1
		EXTI_ClearITPendingBit(EXTI_Line3); //Xoa dong ngat 3
	}
}

static void ledControl_TimPress(void)
{
	if(boHoldFlag_LedBlue == SET && byPressFlag_Blue == SET) //truong hop nhan lien
	{
		dwTimeInit_B2Hold = GetMilSecTick();//do thoi gian bat dau khoi tao
		delayMs(10); //tao thoi gian tre de tinh total
		dwTimeCurrent_B2Hold = GetMilSecTick(); // tinh thoi gian hien tai
		dwTimeTotal_B2Hold += calculatorTime(dwTimeInit_B2Hold,dwTimeCurrent_B2Hold); // tinh thoi gian tong nhan nut
		if(dwTimeTotal_B2Hold > PRESS_TIMEOUT) // neu nhan giu lau hon 500 ms thi bat den
		{
			ledControl_SetState(LEDBLUE_PIN,1);
		}
	}
	if(boHoldFlag_LedBlue == RESET && byPressFlag_Blue == PRESS_1TIMES) // truong hop khi nha nut
	{
		ledControl_SetState(LEDBLUE_PIN,0);
		dwTimeTotal_B2Hold = 0;
	}

	if(boHold_LedRed == SET && byPressFlag_LedRed == PRESS_1TIMES)//truong hop nhan lien
	{
		dwTimeInit_B2Hold = GetMilSecTick();//do thoi gian bat dau khoi tao
		delayMs(10);//tao thoi gian tre de tinh total
		dwTimeCurrent_B2Hold = GetMilSecTick(); // tinh thoi gian tong nhan nut
		dwTimeTotal_B2Hold += calculatorTime(dwTimeInit_B2Hold,dwTimeCurrent_B2Hold);// tinh thoi gian tong nhan nut
		if(dwTimeTotal_B2Hold > PRESS_TIMEOUT)// neu nhan giu lau hon 500 ms thi bat den
		{
			ledControl_SetState(LEDRED_PIN,1);
		}
	}
	if(boHold_LedRed == RESET && byPressFlag_LedRed == PRESS_1TIMES)
	{
		ledControl_SetState(LEDRED_PIN,0);
	}
}

static void ledControl_TimOutPress(void)
{
	//Nut nhan 2 lan B2
	if(byPressFlag_Blue == PRESS_1TIMES) // nhan nut nhan 1 lan roi nha
	{
		dwTimeInit_B2Hold_B2Press = GetMilSecTick(); // khoi tao thoi gian bat dau khi nha nut
		delayMs(10); // tao thoi gian tre de tinh thoi gian tong
		dwTimeCurrent_B2Hold_B2Press = GetMilSecTick(); // thoi gian hien tai
		dwTimeTotal_B2Hold_B2Press += calculatorTime(dwTimeInit_B2Hold_B2Press,dwTimeCurrent_B2Hold_B2Press); // tinh tong thoi gian giua 2 lan nhan nut
		if(dwTimeTotal_B2Hold_B2Press > RELEASE_TIMEOUT) // neu thoi gian giua 2 lan nhan nut lon hon 400 thi reset
		{
			byPressFlag_Blue = RESET;
		}
	}

	if(byPressFlag_Blue == PRESS_2TIMES) // nhan nut lan 2
	{
		ledControl_SetState(LEDBLUE_PIN,1); // dieu khien bat led
		dwTimeTotal_B2Hold_B2Press = RESET;// reset thoi gian tinh baang 0
	}

	if(byPressFlag_Blue == PRESS_3TIMES) // nhan nut lan 3
	{
		ledControl_SetState(LEDBLUE_PIN,0);// tat led
		byPressFlag_Blue = RESET;
		dwTimeTotal_B2Hold_B2Press = RESET;
	}

	//Nut nhan 2 lan B4
	if(byPressFlag_LedRed == PRESS_1TIMES) // nhan nut nhan 1 lan roi nha
	{
		dwTimeInit_B4Hold = GetMilSecTick();// khoi tao thoi gian bat dau khi nha nut
		delayMs(10); // khoi tao thoi gian bat dau khi nha nut
		dwTimeCurrent_B4Hold = GetMilSecTick();// thoi gian hien tai
		dwTimeTotal_B4Hold += calculatorTime(dwTimeInit_B2Hold_B2Press,dwTimeCurrent_B2Hold_B2Press);// tinh tong thoi gian giua 2 lan nhan nut
		if(dwTimeTotal_B4Hold > RELEASE_TIMEOUT)// neu thoi gian giua 2 lan nhan nut lon hon 400 thi reset
		{
			byPressFlag_LedRed = RESET;
			dwTimeTotal_B4Hold = RESET;
		}
	}

	if(byPressFlag_LedRed == PRESS_2TIMES) // nhan nut lan 2
	{
		ledControl_SetState(LEDRED_PIN,1); // bat led
		dwTimeTotal_B4Hold = RESET;
	}

	if(byPressFlag_LedRed == PRESS_3TIMES)// nhan nut lan 3
	{
		ledControl_SetState(LEDRED_PIN,0);//tat led
		byPressFlag_LedRed = RESET;
		dwTimeTotal_B4Hold = RESET;
	}

}

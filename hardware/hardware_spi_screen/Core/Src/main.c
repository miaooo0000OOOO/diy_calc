/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// #include "button.h"
// #include "key.h"
// #include "pwm.h"
#include "retarget.h"
// #include "digital_tube.h"

// MSP3526 libraries
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "GUI.h"
#include "test.h"

// GUI
#include "calc_gui.h"

// std
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

// math
#include "math_core.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void default_screen_test()
{
	main_test();		 // 测试主界面
	Test_Read();		 // 读ID和GRAM测试
	Test_Color();		 // 简单刷屏填充测试
	Test_FillRec();		 // GUI矩形绘图测试
	Test_Circle();		 // GUI画圆测试
	Test_Triangle();	 // GUI三角形绘图测试
	English_Font_test(); // 英文字体示例测试
	Chinese_Font_test(); // 中文字体示例测试
	Pic_test();			 // 图片显示示例测试
	Test_Dynamic_Num();	 // 动态数字显示
	Rotate_Test();		 // 旋转显示测试
	// 如果不带触摸，或者不需要触摸功能，请注释掉下面触摸屏测试项
	Touch_Test(); // 触摸屏手写测试
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_SPI2_Init();
	/* USER CODE BEGIN 2 */
	// retarget_init(&huart1);
	// printf("Hello World!\n");

	delay_init(72); // 延时初始化
	LCD_Init();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	// 触摸设备初始化
	if (tp_dev.init())
	{
		return;
	}

	BUTTON4048 btn1 = new_button(0, 240, BLUE, GRAY, WHITE, "1", true);
	BUTTON4048 btn2 = new_button(40, 240, BLUE, GRAY, WHITE, "2", true);
	BUTTON4048 btn3 = new_button(80, 240, BLUE, GRAY, WHITE, "3", true);
	BUTTON4048 btn4 = new_button(0, 240 + 48, BLUE, GRAY, WHITE, "4", true);
	BUTTON4048 btn5 = new_button(40, 240 + 48, BLUE, GRAY, WHITE, "5", true);
	BUTTON4048 btn6 = new_button(80, 240 + 48, BLUE, GRAY, WHITE, "6", true);
	BUTTON4048 btn7 = new_button(0, 240 + 48 * 2, BLUE, GRAY, WHITE, "7", true);
	BUTTON4048 btn8 = new_button(40, 240 + 48 * 2, BLUE, GRAY, WHITE, "8", true);
	BUTTON4048 btn9 = new_button(80, 240 + 48 * 2, BLUE, GRAY, WHITE, "9", true);
	BUTTON4048 btn0 = new_button(40, 240 + 48 * 3, BLUE, GRAY, WHITE, "0", true);
	BUTTON4048 btn_add = new_button(120, 240, BLUE, GRAY, WHITE, "+", true);
	BUTTON4048 btn_sub = new_button(120, 240 + 48, BLUE, GRAY, WHITE, "-", true);
	BUTTON4048 btn_div = new_button(0, 240 + 48 * 3, BLUE, GRAY, WHITE, "/", true);
	BUTTON4048 btn_mul = new_button(80, 240 + 48 * 3, BLUE, GRAY, WHITE, "*", true);
	BUTTON4048 btn_pow = new_button(120, 240 + 48 * 2, BLUE, GRAY, WHITE, "^", true);
	BUTTON4048 btn_eq = new_button(120, 240 + 48 * 3, BLUE, GRAY, WHITE, "=", true);
	BUTTON4048 btn_lp = new_button(0, 240 + 48 * 4, BLUE, GRAY, WHITE, "(", true);
	BUTTON4048 btn_rp = new_button(40, 240 + 48 * 4, BLUE, GRAY, WHITE, ")", true);
	BUTTON4048 btn_mod = new_button(80, 240 + 48 * 4, BLUE, GRAY, WHITE, "%", true);
	BUTTON4048 btn_x = new_button(120, 240 + 48 * 4, BLUE, GRAY, WHITE, "x", true);

	BUTTON4048 btn_back = new_button(160, 240, BLUE, GRAY, WHITE, "<-", true);
	BUTTON4048 btn_cl = new_button(160, 240 + 48, BLUE, GRAY, WHITE, "CL", true);
	BUTTON4048 btn_cs = new_button(160, 240 + 48 * 2, BLUE, GRAY, WHITE, "CS", true);

	BUTTON4048 btn_123 = new_button(280, 240, BROWN, GRAY, WHITE, "123", true);
	BUTTON4048 btn_fc = new_button(280, 240 + 48, GRAYBLUE, GRAY, WHITE, "fc", true);
	BUTTON4048 btn_abc = new_button(280, 240 + 48 * 2, GRAYBLUE, GRAY, WHITE, "abc", true);

	BUTTON4048 btn_calc = new_button(280, 240 + 48 * 4, GRAYBLUE, GRAY, WHITE, "calc", true);

	BUTTON4048 btn_sqrt = new_button(0, 240, BLUE, GRAY, WHITE, "sqrt", false);
	BUTTON4048 btn_sin = new_button(40, 240, BLUE, GRAY, WHITE, "sin", false);
	BUTTON4048 btn_cos = new_button(80, 240, BLUE, GRAY, WHITE, "cos", false);

	BUTTON4048 *char_buttons[] = {&btn1, &btn2, &btn3, &btn4, &btn5, &btn6, &btn7, &btn8, &btn9, &btn0,
								  &btn_add, &btn_sub, &btn_mul, &btn_div, &btn_pow, &btn_eq, &btn_lp, &btn_rp, &btn_mod, &btn_x};
	const u16 char_buttons_len = sizeof(char_buttons) / sizeof(char_buttons[0]);

	BUTTON4048 *cmd_buttons[] = {&btn_back, &btn_cl, &btn_cs, &btn_123, &btn_fc, &btn_abc, &btn_calc};
	const u16 cmd_buttons_len = sizeof(cmd_buttons) / sizeof(cmd_buttons[0]);

	BUTTON4048 *fc_buttons[] = {&btn_sqrt, &btn_sin, &btn_cos};
	const u16 fc_buttons_len = sizeof(fc_buttons) / sizeof(fc_buttons[0]);

	BUTTON4048 *all_buttons[] = {&btn1, &btn2, &btn3, &btn4, &btn5, &btn6, &btn7, &btn8, &btn9, &btn0,
								 &btn_add, &btn_sub, &btn_mul, &btn_div, &btn_pow, &btn_eq, &btn_lp, &btn_rp, &btn_mod, &btn_x,
								 &btn_back, &btn_cl, &btn_cs, &btn_123, &btn_fc, &btn_abc, &btn_calc,
								 &btn_sqrt, &btn_sin, &btn_cos};
	const u16 all_buttons_len = sizeof(all_buttons) / sizeof(all_buttons[0]);
	u16 i = 0;
	u16 keybroad_state = 1;
	bool refrash_keybroad = true;

	u8 input_str[40] = {0};
	u8 temp_str[40] = {0};
	u8 input_str_len = 0;
	float *fp;
	float tempf;
	int *ip;

	refrash_keybroad = false;
	LCD_Fill(0, 0, lcddev.width, lcddev.height, WHITE);
	POINT_COLOR = LGRAY;
	LCD_DrawFillRectangle(0, lcddev.height / 2, lcddev.width, lcddev.height);
	for (i = 0; i < all_buttons_len; i++)
	{
		draw_button4048(all_buttons[i]);
	}
	while (1)
	{
		if (refrash_keybroad)
		{
			refrash_keybroad = false;
			LCD_Fill(0, 0, lcddev.width, lcddev.height, WHITE);
			POINT_COLOR = LGRAY;
			LCD_DrawFillRectangle(0, lcddev.height / 2, lcddev.width, lcddev.height);
			for (i = 0; i < all_buttons_len; i++)
			{
				all_buttons[i]->edge = true;
				draw_button4048(all_buttons[i]);
			}
		}

		tp_dev.scan();

		// delay_ms(25);
		for (i = 0; i < all_buttons_len; i++)
		{
			button_pressed(all_buttons[i]);
		}

		// 清除整行
		if (btn_cl.pressed && btn_cl.edge)
		{
			LCD_ShowString(4, 8, 16, "                             ", 0);
			for (i = 0; i < input_str_len; i++)
				input_str[i] = 0;
			input_str_len = 0;
		}
		// 退格
		if (btn_back.pressed && btn_back.edge && input_str_len != 0)
		{
			LCD_ShowString(4, 8, 16, "                             ", 0);
			input_str[input_str_len - 1] = 0;
			input_str_len--;
			POINT_COLOR = BLACK;
			LCD_ShowString(4, 8, 16, input_str, 1);
		}
		// 添加字符
		for (i = 0; i < char_buttons_len; i++)
		{
			if (!(char_buttons[i]->pressed && char_buttons[i]->edge))
				continue;
			input_str[input_str_len] = char_buttons[i]->display_str[0];
			input_str_len++;
			POINT_COLOR = BLACK;
			LCD_ShowString(4, 8, 16, input_str, 1);
		}
		// 计算
		if (btn_calc.pressed && btn_calc.edge)
		{
			LCD_ShowString(4, 8 + 16, 16, "                             ", 0);
			int state_code = parse_to_token_list(input_str);
			if (state_code == -1)
				continue;
			AST_Node *node = parse_to_ast();
			if (node == NULL)
				continue;
			Token *res_token = calc(node);
			volatile int debug_value = 114514;
			if (res_token->type == Int)
			{
				ip = (int *)&res_token->value;
				debug_value = sprintf(temp_str, "%d", *ip);
				POINT_COLOR = BLACK;
				LCD_ShowString(4, 8 + 16, 16, temp_str, 1);
			}
			else if (res_token->type == Float)
			{
				fp = (float *)&res_token->value;
				tempf = *fp;
				debug_value = sprintf(temp_str, "%f", tempf);
				// printf("%f\n", tempf);
				// gcvt(tempf, 7, temp_str);
				POINT_COLOR = BLACK;
				LCD_ShowString(4, 8 + 16, 16, temp_str, 1);
			}
			recu_free_ast(node);
		}

		if (btn_fc.pressed && btn_fc.edge)
		{
			for (i = 0; i < all_buttons_len; i++)
			{
				all_buttons[i]->display = false;
			}
			for (i = 0; i < fc_buttons_len; i++)
			{
				fc_buttons[i]->display = true;
				fc_buttons[i]->edge = true;
			}
			for (i = 0; i < cmd_buttons_len; i++)
			{
				cmd_buttons[i]->display = true;
				cmd_buttons[i]->edge = true;
			}
			refrash_keybroad = true;
			continue;
		}
		else if (btn_123.pressed && btn_123.edge)
		{
			for (i = 0; i < all_buttons_len; i++)
			{
				all_buttons[i]->display = false;
			}
			for (i = 0; i < char_buttons_len; i++)
			{
				char_buttons[i]->display = true;
				char_buttons[i]->edge = true;
			}
			for (i = 0; i < cmd_buttons_len; i++)
			{
				cmd_buttons[i]->display = true;
				cmd_buttons[i]->edge = true;
			}
			refrash_keybroad = true;
			continue;
		}

		// 按钮状态
		for (i = 0; i < all_buttons_len; i++)
		{
			draw_button4048(all_buttons[i]);
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

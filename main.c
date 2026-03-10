#include "main.h"

volatile int portBPin1Counter;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

volatile int state = 0;

void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	if (state){ //checks if button is high
		GPIOC->BSRR = GPIO_BSRR_BS_6; // LED turn on
		state = !state;
	} else {
		GPIOC->BSRR = GPIO_BSRR_BR_6; // LED turn off
		state = !state;
	}

	TIM1->SR &= ~(TIM_SR_UIF);
	//TIM1->DIER ^= TIM_DIER_UIE;
}

void TIMER_SETUP_UP(){

	TIM1->PSC = 10000; // prescale to 10000

	TIM1->ARR = 800; //configures timer 1 period

	TIM1->DIER |= TIM_DIER_UIE;


	TIM1->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE | TIM_CR1_CMS; //timer control register | enable | auto reload preload enable
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}
void TIMER_SETUP_DOWN(){

	TIM1->PSC = 10000; // prescale to 10000

	TIM1->ARR = 800; //configures timer 1 period

	TIM1->DIER |= TIM_DIER_UIE;


	TIM1->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE | TIM_CR1_DIR; //timer control register | enable | auto reload preload enable
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}

void MANUAL_GPIO_INIT(){
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN | RCC_APB2ENR_TIM1EN;


	GPIOC->MODER &= ~(GPIO_MODER_MODER6_1);
	GPIOC->MODER |= GPIO_MODER_MODER6_0; // we want it as output pin
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6); // 0:OUTPUT PUSH-PULL (RESET STATE) 8.4.2 (134)
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR6_0); // make 00
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6);


	GPIOB->MODER &= ~(GPIO_MODER_MODER1);

	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR1);
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR1_0; // internal pull up


	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PB; //11.2.5 SHOWS EXTI mapping

	EXTI->PR |= EXTI_PR_PR1; //no stale pending bit
	EXTI->RTSR |= EXTI_RTSR_TR1;
	EXTI->FTSR |= EXTI_FTSR_TR1; // need falling edge so it detects when button is unpressed and led turns off
	EXTI->IMR |= EXTI_IMR_MR1;
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_SetPriority(EXTI0_1_IRQn,0);
}

void EXTI0_1_IRQHandler(void){
//	if (EXTI->PR & EXTI_PR_PR1) // checks if this is our line which is line 0 to line 1 (line 1 we're using)
//	{
	if (!(GPIOB->IDR & GPIO_IDR_1)){ //checks if button is high
		GPIOC->BSRR = GPIO_BSRR_BS_6; // LED turn on
	} else {
		GPIOC->BSRR = GPIO_BSRR_BR_6; // LED turn off
	}

	EXTI->PR |= EXTI_PR_PR1; // clear pending bit
	//}
}


int main(void)
{

  HAL_Init();

  SystemClock_Config();

  //MX_GPIO_Init();
  MANUAL_GPIO_INIT();
  TIMER_SETUP_UP();
  //TIMER_SETUP_DOWN();


  	while(1){
//  		if (!(GPIOB->IDR & 0x2)){
//  				  GPIOC->BSRR |= GPIO_BSRR_BS_6;
//  		}
//  		else {
//  		  GPIOC->BSRR |= GPIO_BSRR_BR_6;
//  		}
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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

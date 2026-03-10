# STM32-Hardware-Timers-and-Counters
This project configures the internal hardware timers on the STM32F030 R8T6 microcontroller. We focus on utilizing the clock cycles for precise timing and offloading tasks from the main execution loop to hardware interrupts.
### Technical Features
- **Clock Prescaling:** Configured a 10,000x prescale to reduce the internal 8MHz RC oscillator frequency which allowed for LED blinking and making the count more visible to the human eye.
- **Auto-Reload Logic (ARR):** Utilized the ARR to set precise count limits, enabling the timer to reset automatically upon reaching a target value.
- **Bidirectional Counting**: Implements both Up-Counting, Down-Counting, and Center-Aligned Mode (CMS) by configuring the `TIM1_CR1` controller registers.
- **Interrupt Service Routines (ISR):** Implemented `TIM1_BRK_UP_TRG_COM_IRQHandler` to toggle system LEDs based on timer update events (UIF flag).
### Hardware
- **Microcontroller:** STM32F030 R8T6
- **Outputs:** LEDs and external push-buttons
### Software
**Monitoring Tool:** STM32CubeMonitor (Used for real-time visualization of the TIM1_CNT register)
**Programming Tool:** STM32CubeIde
### Challenge & Solution
The purpose of this project was to prove that the timer could work independently of the main loop. The issue we faced was that the software `delay()` functions blocked the CPU and would stop all other logic. So, we ran Hardware Timers and created an LED blinker that continues to run at the exact same speed even if the `main()` loop was busy. 
### Project Structure
- main.c: Contained manual initialization of GPIO ports and the Timer 1 interrupt configuration
- `TIMER_SETUP_UP/DOWN`: Functions that configured the different timer counting modes
- `TIM1_Handler`: The interrupt routine that clears the update flag (UIF) and toggles the LED state
### Project Collaboration
This project was co-developed by Jessica Ly and Daniel Meija Mendez. We collaborated on register-level timer configuration and verified the counter values using STM32CubeMonitor.

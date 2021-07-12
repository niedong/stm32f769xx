#ifndef STM32F769XX_UART_ASSERT_H
#define STM32F769XX_UART_ASSERT_H

void assert_failed(const char *file, unsigned int line, const char *func, const char *expr);

#define assert(expr) \
  (void)(!!(expr) || (assert_failed(__FILE__, __LINE__, __FUNCTION__, #expr), 0))

#endif

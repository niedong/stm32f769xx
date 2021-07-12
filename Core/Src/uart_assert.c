#include "uart_assert.h"
#include "uart_printf.h"

void assert_failed(const char *file, unsigned int line, const char *func, const char *expr)
{
  UartPrintf("\nFile \"%s\", line %u, in function %s\n"
             "Assertion failed: %s\n", file, line, func, expr);
  SystemHalt();
}

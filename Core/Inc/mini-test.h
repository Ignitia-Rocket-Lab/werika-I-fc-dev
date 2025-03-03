#ifndef SIMPLE_TESTING_H
#define SIMPLE_TESTING_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "stm32f4xx_hal.h"
#include "usart.h"

#include "info_sequences.h"

/**
 * USAGE:
 * To use this mini-test library do the following:
 * - Create a file tests, using TEST_ASSERT as needed
 * - Declare REGISTER_TESTS and use REGISTER_TEST to add all the tests that you
 * want to run
 * - Call either TEST_EXPECTED_PASS or TEST_EXPECTED_FAIL
 * - Observe the results from the serial terminal
 */
extern void REGISTER_TESTS_FAIL(void);
extern void REGISTER_TESTS_PASS(void);

__attribute__((always_inline)) static inline void TEST_EXPECTED_PASS(void);
__attribute__((always_inline)) static inline void TEST_EXPECTED_FAIL(void);

__attribute__((always_inline)) inline void ASSERT_PASS(uint8_t *file,
                                                       uint32_t line);
__attribute__((always_inline)) inline void ASSERT_FAIL(uint8_t *file,
                                                       uint32_t line);
__attribute__((always_inline)) inline void TEST_ASSERT(uint8_t expr, char *file, int32_t line);
__attribute__((always_inline)) inline void NOTIFY_TEST_START(void);
__attribute__((always_inline)) inline void NOTIFY_TEST_END(void);
__attribute__((always_inline)) inline void PRINT_TEST_MSG(const char *msg);

static inline void TEST_EXPECTED_PASS(void) {
  // TODO add check to avoid running passing and failing tests at the same time
  NOTIFY_TEST_START();

  REGISTER_TESTS_PASS();

  NOTIFY_TEST_END();
}

static inline void TEST_EXPECTED_FAIL(void) {
  // TODO add check to avoid running passing and failing tests at the same time
  NOTIFY_TEST_START();

  REGISTER_TESTS_FAIL();

  NOTIFY_TEST_END();
}

inline void NOTIFY_TEST_START(void) {
  const char buffer[] = "\r\n---TESTING STARTING---\r\n";
  HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}

inline void NOTIFY_TEST_END(void) {
  const char buffer[] = "\r\n---TESTING ENDED---\r\n";
  HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}

inline void ASSERT_PASS(uint8_t *file, uint32_t line) {
  char buffer[100];
  snprintf(buffer, sizeof(buffer), " -- Pass: %s, line %lu\n", file, line);

  HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}

inline void ASSERT_FAIL(uint8_t *file, uint32_t line) {
  char buffer[100];
  snprintf(buffer, sizeof(buffer), " -- Fail: %s, line %lu\n", file, line);

  HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}

inline void PRINT_TEST_MSG(const char *msg) {
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

inline void TEST_ASSERT(uint8_t expr, char *file, int32_t line) {
  (expr) ? ASSERT_PASS((uint8_t *)file, (uint32_t)line)
         : ASSERT_FAIL((uint8_t *)file, (uint32_t)line);
}

#endif // SIMPLE_TESTING_H
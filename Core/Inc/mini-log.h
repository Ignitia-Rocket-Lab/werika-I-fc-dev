#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char message[256];
} LogEntry;

#define MAX_LOG_ENTRIES 100
LogEntry logEntries[MAX_LOG_ENTRIES];
int logIndex = 0;

/**
 * USAGE:
 * To use this mini-log library do the following:
 * - Use c_assert to include asserts in your code
 * - Inspect the results of the failing asserts (if any) by reading the
 * logEntries
 */

bool c_assert(bool condition, const char *expression, const char *file,
              int line) {
  if (!condition) {
    // Log the error to memory
    if (logIndex < MAX_LOG_ENTRIES) {
      snprintf(logEntries[logIndex].message,
               sizeof(logEntries[logIndex].message),
               "Assertion failed at %s:%d: Expression '%s' evaluated to false",
               file, line, expression);
      logIndex++;
    }
    return false;
  }
  return true;
}

#define ASSERT(condition) c_assert(condition, #condition, __FILE__, __LINE__)

// Function to print logs to a big buffer
void printLogsToBuffer(char *buffer, size_t bufferSize) {
  size_t offset = 0;
  for (int i = 0; i < logIndex; i++) {
    size_t messageLength =
        strlen(logEntries[i].message) + 1; // Include null terminator
    if (offset + messageLength <= bufferSize) {
      memcpy(buffer + offset, logEntries[i].message, messageLength);
      offset += messageLength;
      // Add newline character if there's space
      if (offset < bufferSize) {
        buffer[offset++] = '\n';
      }
    } else {
      // Handle buffer overflow
      break;
    }
  }
  // Ensure buffer is null-terminated
  if (offset < bufferSize) {
    buffer[offset] = '\0';
  }
}

void printLogEntries() {
  for (int i = 0; i < logIndex; i++) {
    printf("%s\n", logEntries[i].message);
  }
}
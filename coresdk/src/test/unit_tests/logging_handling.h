/**
 * @header  logging
 * @author  Olivia McKeon
 * @brief   Provides functions to disable and enable logging in unit test files.
 *
 */

#ifndef logging_handling_h
#define logging_handling_h

/**
 * Logging types
 *
 * @constant INFO       Info logging.
 * @constant WARNING    Warning logging.
 * @constant ERROR      Error logging.
 */
typedef enum logging_type
{
    INFO,
    WARNING,
    ERROR,
} logging_type;

/**
 * Disables the logging for a particular logging type (info, warning, error)
 *
 * Used to keep output clean for unit tests checking functions with logging type
 *
 * @param type  The logging type to disable
 */
void disable_logging(logging_type type);

/**
 * Enables the logging for a particular logging type (info, warning, error)
 *
 * Used to re-enable logging type at the end of unit tests checking functions with logging type
 *
 * @param type  The logging type to enable
 */
void enable_logging(logging_type type);

#endif /* logging_handling_h */
//
// logging handling.cpp
//
// For use in unit tests after the logging message has been checked.
//
// Using these functions helps keep the unit testing output cleaner and more targeted for new tests
//
#include "logging_handling.h"

#include "easylogging++.h"

void disable_logging(logging_type type)
{
    el::Logger *defaultLogger = el::Loggers::getLogger("default");

    // Disable log of given type
    switch (type)
    {
    case INFO:
        defaultLogger->configurations()->set(el::Level::Info, el::ConfigurationType::Enabled, "false");
        break;
    case WARNING:
        defaultLogger->configurations()->set(el::Level::Warning, el::ConfigurationType::Enabled, "false");
        break;
    case ERROR:
        defaultLogger->configurations()->set(el::Level::Error, el::ConfigurationType::Enabled, "false");
        break;

    default:
        LOG(DEBUG) << "Invalid logging type. Available types are: INFO, WARNING, and ERROR.";
        break;
    }
    // Reconfigure to disable log of given type
    defaultLogger->reconfigure();
}

void enable_logging(logging_type type)
{
    el::Logger *defaultLogger = el::Loggers::getLogger("default");

    // Enable log of given type
    switch (type)
    {
    case INFO:
        defaultLogger->configurations()->set(el::Level::Info, el::ConfigurationType::Enabled, "true");
        break;
    case WARNING:
        defaultLogger->configurations()->set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
        break;
    case ERROR:
        defaultLogger->configurations()->set(el::Level::Error, el::ConfigurationType::Enabled, "true");
        break;

    default:
        LOG(DEBUG) << "Invalid logging type. Available types are: INFO, WARNING, and ERROR.";
        break;
    }
    // Reconfigure to enable log of given type
    defaultLogger->reconfigure();
}
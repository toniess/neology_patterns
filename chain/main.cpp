#include <iostream>
#include <fstream>
#include <stdexcept>

enum class Type {
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage {
private:
    Type messageType;
    std::string messageContent;

public:
    LogMessage(Type type, const std::string& message) : messageType(type), messageContent(message) {}

    Type type() const {
        return messageType;
    }

    const std::string& message() const {
        return messageContent;
    }
};

class LoggerHandler {
protected:
    LoggerHandler* nextHandler = nullptr;

public:
    virtual void handleLog(const LogMessage& message) = 0;

    void setNextHandler(LoggerHandler* handler) {
        nextHandler = handler;
    }
};

class FatalErrorHandler : public LoggerHandler {
public:
    void handleLog(const LogMessage& message) override {
        if (message.type() == Type::FatalError) {
            throw std::runtime_error("Fatal Error: " + message.message());
        } else if (nextHandler != nullptr) {
            nextHandler->handleLog(message);
        }
    }
};

class ErrorHandler : public LoggerHandler {
private:
    std::ofstream fileStream;

public:
    ErrorHandler(const std::string& filePath) {
        fileStream.open(filePath);
    }

    ~ErrorHandler() {
        fileStream.close();
    }

    void handleLog(const LogMessage& message) override {
        if (message.type() == Type::Error) {
            fileStream << "Error: " << message.message() << std::endl;
        } else if (nextHandler != nullptr) {
            nextHandler->handleLog(message);
        }
    }
};

class WarningHandler : public LoggerHandler {
public:
    void handleLog(const LogMessage& message) override {
        if (message.type() == Type::Warning) {
            std::cout << "Warning: " << message.message() << std::endl;
        } else if (nextHandler != nullptr) {
            nextHandler->handleLog(message);
        }
    }
};

class UnknownHandler : public LoggerHandler {
public:
    void handleLog(const LogMessage& message) override {
        if (message.type() == Type::Unknown) {
            throw std::runtime_error("Unknown Message: " + message.message());
        } else if (nextHandler != nullptr) {
            nextHandler->handleLog(message);
        }
    }
};

int main() {
    FatalErrorHandler fatalErrorHandler;
    ErrorHandler errorHandler("error_log.txt");
    WarningHandler warningHandler;
    UnknownHandler unknownHandler;

    errorHandler.setNextHandler(&warningHandler);
    warningHandler.setNextHandler(&fatalErrorHandler);
    fatalErrorHandler.setNextHandler(&unknownHandler);

    LogMessage log1(Type::Warning, "Sample Warning");
    LogMessage log2(Type::Error, "Sample Error");
    LogMessage log3(Type::FatalError, "Sample Fatal Error");
    LogMessage log4(Type::Unknown, "Unknown Message");

    try {
        errorHandler.handleLog(log1);
        errorHandler.handleLog(log2);
        // errorHandler.handleLog(log3);
        // errorHandler.handleLog(log4);
    } catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}

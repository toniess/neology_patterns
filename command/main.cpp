#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << "Console Log: " << message << std::endl;
    }
};

class FileLogCommand : public LogCommand {
private:
    std::ofstream fileStream;
public:
    FileLogCommand(const std::string& filePath) {
        fileStream.open(filePath);
    }

    ~FileLogCommand() {
        fileStream.close();
    }

    void print(const std::string& message) override {
        fileStream << "File Log: " << message << std::endl;
    }
};

void executeLogCommand(LogCommand& command, const std::string& message) {
    command.print(message);
}

int main() {
    ConsoleLogCommand consoleLogger;
    FileLogCommand fileLogger("log.txt");

    executeLogCommand(consoleLogger, "Message for Console Logger");
    executeLogCommand(fileLogger, "Message for File Logger");

    return 0;
}

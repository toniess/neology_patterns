#include <iostream>
#include <fstream>
#include <vector>

class Observer {
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Observable {
private:
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void warning(const std::string& message) const {
        for (Observer* observer : observers) {
            observer->onWarning(message);
        }
    }

    void error(const std::string& message) const {
        for (Observer* observer : observers) {
            observer->onError(message);
        }
    }

    void fatalError(const std::string& message) const {
        for (Observer* observer : observers) {
            observer->onFatalError(message);
        }
    }
};

class ConsoleWarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Console Warning: " << message << std::endl;
    }
};

class FileErrorObserver : public Observer {
private:
    std::ofstream fileStream;
public:
    FileErrorObserver(const std::string& filePath) {
        fileStream.open(filePath);
    }

    ~FileErrorObserver() {
        fileStream.close();
    }

    void onError(const std::string& message) override {
        fileStream << "File Error: " << message << std::endl;
    }

    void onFatalError(const std::string& message) override {
        fileStream << "File Fatal Error: " << message << std::endl;
    }
};

class ConsoleFileFatalErrorObserver : public Observer {
private:
    std::ofstream fileStream;
public:
    ConsoleFileFatalErrorObserver(const std::string& filePath) {
        fileStream.open(filePath);
    }

    ~ConsoleFileFatalErrorObserver() {
        fileStream.close();
    }

    void onWarning(const std::string& message) override {
        std::cout << "Console Warning: " << message << std::endl;
    }

    void onError(const std::string& message) override {
        fileStream << "File Error: " << message << std::endl;
    }

    void onFatalError(const std::string& message) override {
        std::cout << "Console Fatal Error: " << message << std::endl;
        fileStream << "File Fatal Error: " << message << std::endl;
    }
};

int main() {
    Observable logger;

    ConsoleWarningObserver consoleWarningObserver;
    FileErrorObserver fileErrorObserver("error_log.txt");
    ConsoleFileFatalErrorObserver consoleFileFatalErrorObserver("fatal_error_log.txt");

    logger.addObserver(&consoleWarningObserver);
    logger.addObserver(&fileErrorObserver);
    logger.addObserver(&consoleFileFatalErrorObserver);

    logger.warning("Warning message");
    logger.error("Error message");
    logger.fatalError("Fatal error message");

    return 0;
}

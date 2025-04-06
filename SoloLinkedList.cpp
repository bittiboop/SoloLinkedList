#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

struct SmartDevice {
private:
    string deviceName;
    string deviceType;
    bool isPoweredOn;
    int batteryLevel;
    float temperature;
    string location;
    ofstream logFile;

public:
    SmartDevice() : deviceName("Unknown"), deviceType("Generic"), isPoweredOn(false), 
                    batteryLevel(100), temperature(20.0f), location("Not set") {
        initLogger("device_log.txt");
        log("Device created with default parameters");
    }

    SmartDevice(const string& name) : SmartDevice() {
        deviceName = name;
        log("Device name set to: " + name);
    }

    SmartDevice(const string& name, const string& type) : SmartDevice(name) {
        deviceType = type;
        log("Device type set to: " + type);
    }

    SmartDevice(const string& name, const string& type, const string& loc) 
        : SmartDevice(name, type) {
        location = loc;
        log("Device location set to: " + loc);
    }

    SmartDevice(const string& name, const string& type, bool powered, 
                int battery, float temp, const string& loc) 
        : deviceName(name), deviceType(type), isPoweredOn(powered), 
          batteryLevel(battery), temperature(temp), location(loc) {
        initLogger("device_log.txt");
        log("Device fully initialized with custom parameters");
    }

    ~SmartDevice() {
        log("Device " + deviceName + " destroyed");
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void setDeviceName(const string& name) {
        deviceName = name;
        log("Device name changed to: " + name);
    }

    void setDeviceType(const string& type) {
        deviceType = type;
        log("Device type changed to: " + type);
    }

    void setPoweredOn(bool powered) {
        isPoweredOn = powered;
        log(string("Device power state changed to: ") + (powered ? "ON" : "OFF"));
    }

    void setBatteryLevel(int level) {
        if (level < 0) level = 0;
        if (level > 100) level = 100;
        batteryLevel = level;
        log("Battery level set to: " + to_string(level) + "%");
    }

    void setTemperature(float temp) {
        temperature = temp;
        log("Temperature set to: " + to_string(temp) + "°C");
    }

    void setLocation(const string& loc) {
        location = loc;
        log("Location changed to: " + loc);
    }

    string getDeviceName() const {
        return deviceName;
    }

    string getDeviceType() const {
        return deviceType;
    }

    bool getIsPoweredOn() const {
        return isPoweredOn;
    }

    int getBatteryLevel() const {
        return batteryLevel;
    }

    float getTemperature() const {
        return temperature;
    }

    string getLocation() const {
        return location;
    }

    void initLogger(const string& filename) {
        logFile.open(filename, ios::app);
        if (!logFile.is_open()) {
            cerr << "Failed to open log file: " << filename << endl;
        }
    }

    void log(const string& message) {
        auto now = chrono::system_clock::now();
        auto now_time = chrono::system_clock::to_time_t(now);
        
        string timestamp = getCurrentTimestamp();
        
        if (logFile.is_open()) {
            logFile << "[" << timestamp << "] " << deviceName << ": " << message << endl;
        }
        
        cout << "[" << timestamp << "] " << deviceName << ": " << message << endl;
    }

    string getCurrentTimestamp() const {
        auto now = chrono::system_clock::now();
        auto now_time = chrono::system_clock::to_time_t(now);
        
        tm tm_now;
        localtime_s(&tm_now, &now_time);
        
        ostringstream oss;
        oss << put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    void powerToggle() {
        isPoweredOn = !isPoweredOn;
        log(string("Device toggled to: ") + (isPoweredOn ? "ON" : "OFF"));
    }

    bool chargeBattery(int amount) {
        if (!isPoweredOn) {
            log("Cannot charge - device is powered off");
            return false;
        }
        
        int newLevel = batteryLevel + amount;
        if (newLevel > 100) newLevel = 100;
        
        int charged = newLevel - batteryLevel;
        batteryLevel = newLevel;
        
        log("Battery charged by " + to_string(charged) + "%. New level: " + to_string(batteryLevel) + "%");
        return true;
    }

    void adjustTemperature(float delta) {
        if (!isPoweredOn) {
            log("Cannot adjust temperature - device is powered off");
            return;
        }
        
        float oldTemp = temperature;
        temperature += delta;
        
        log("Temperature adjusted from " + to_string(oldTemp) + 
            "°C to " + to_string(temperature) + "°C");
    }

    string getDeviceStatus() const {
        string status = "Device: " + deviceName + " (" + deviceType + ")\n";
        status += "Location: " + location + "\n";
        status += "Power: " + string(isPoweredOn ? "ON" : "OFF") + "\n";
        status += "Battery: " + to_string(batteryLevel) + "%\n";
        status += "Temperature: " + to_string(temperature) + "°C";
        return status;
    }

    void displayInfo() const {
        cout << getDeviceStatus() << endl;
    }

    bool isLowBattery() const {
        return batteryLevel < 20;
    }

    void performDiagnostics() {
        log("Starting diagnostics...");
        log("Checking power: " + string(isPoweredOn ? "OK" : "OFF"));
        
        log("Checking battery level: " + to_string(batteryLevel) + "%");
        if (isLowBattery()) {
            log("WARNING: Low battery detected!");
        } else {
            log("Battery level OK");
        }
        
        log("Checking temperature: " + to_string(temperature) + "°C");
        if (temperature < 0 || temperature > 40) {
            log("WARNING: Temperature outside normal operating range!");
        } else {
            log("Temperature OK");
        }
        
        log("Diagnostics completed");
    }
};

int main() {
    SmartDevice device1;
    SmartDevice device2("Living Room Thermostat");
    SmartDevice device3("Kitchen Light", "Light Switch");
    SmartDevice device4("Bedroom Camera", "Security Camera", "Bedroom");
    SmartDevice device5("Front Door Lock", "Security Lock", true, 85, 22.5f, "Front Door");
    
    device1.setDeviceName("Main Hub");
    device1.setDeviceType("Control Center");
    device1.setLocation("Living Room");
    device1.setPoweredOn(true);
    
    device1.performDiagnostics();
    device3.powerToggle();
    device5.chargeBattery(10);
    device4.adjustTemperature(-2.5f);
    
    cout << "\nDevice Information:" << endl;
    cout << "-------------------" << endl;
    device1.displayInfo();
    cout << "\n" << device5.getDeviceStatus() << endl;
    
    return 0;
}

/**
 * CF Wi-Fi base for IoT devices.
 * 
 * IoT - Configurable device using ThingsBoard as server.
 * 
 * @author  Caio Frota <caiofrota@gmail.com>
 * @version 1.0.0
 * @since   Oct, 2021
 */

//#define CF_USE_DISPLAY 0x3C                                                     // Uncomment to use display. Use the display address.

// Libraries.

#include <Logger.h>                                                             // Logger.
#include <CFWiFiManagerHelper.h>                                                // CF WiFiManager Helper.
#include <CFThingsBoardHelper.h>                                                // CF ThingsBoard Helper.

// Optional libraries.

#ifdef CF_USE_DISPLAY
    #include <CFIconSet.h>                                                      // CF Icon Set for display.
    #include <CFIoTDisplayHelper.h>                                             // Display.
    CFIoTDisplayHelper _display(128, 64, CF_USE_DISPLAY);
#endif

// Software info.
#define APP_CODE                        "cf-iot-app-code"                       // App code.
#define APP_VERSION                     "1.0.0"                                 // App version.

// WiFiManager parameters.
#define CF_WM_MAX_PARAMS_QTY            3
WiFiManagerParameter _params[] = {
    { "p_device_name", "Device Name", "", 50 },
    { "p_server_url", "Server URL", "", 50 },
    { "p_server_token", "Token", "", 50 }
};

// CF Helpers.
CFWiFiManagerHelper _cfWiFiManager;                                             // CF WiFiManager Helper.
CFThingsBoardHelper _cfThingsBoard(APP_CODE, APP_VERSION);                      // CF WiFiManager Helper.

void setup() {
    // Setup Serial.
    Serial.begin(115200);

    // Start display.
    #ifdef CF_USE_DISPLAY
        _display.begin();
    #endif

    // Setup logger.
    Logger::setLogLevel(Logger::NOTICE); // VERBOSE, NOTICE, WARNING, ERROR, FATAL, SILENT.

    // Config WiFiManager.
    _cfWiFiManager.setCustomParameters(_params, CF_WM_MAX_PARAMS_QTY);
    _cfWiFiManager.setOnSaveParametersCallback(onSaveParametersCallback);
    _cfWiFiManager.setOnConfigModeCallback(onConfigModeCallback);
    _cfWiFiManager.begin();

    // Config ThingsBoard.
    onSaveParametersCallback();                                                 // Call the callback once to update the first time.

    // Config ThingsBoard.
    _cfThingsBoard.setLocalIP(_cfWiFiManager.getLocalIP());
    _cfThingsBoard.setOnThingsBoardConnectCallback(onThingsBoardConnectCallback);
}

void loop() {
    _cfWiFiManager.loop();                                                      // Do WiFiManager loop.
    _cfThingsBoard.loop();                                                      // Do ThingsBoard loop.

    // Call render method.
    render();
}

/**
 * Callback to update parameters when they have been modified.
 */
void onSaveParametersCallback() {
    _cfThingsBoard.setServerURL(_cfWiFiManager.getParameter("p_server_url"));
    _cfThingsBoard.setToken(_cfWiFiManager.getParameter("p_server_token"));
    _cfThingsBoard.setAttributeValue("attr_device_name", _cfWiFiManager.getParameter("p_device_name"));
}

/**
 * Callback to be called when receive any attribute update from ThingsBoard.
 */
void ATTRCallback(const RPC_Data &data) {
    Logger::notice("Attr received.");

    // Update attributes.
    _cfWiFiManager.setParameter("p_device_name", data["attr_device_name"]);
}

/**
 * Callback to be called when receive default RPC from ThingsBoard.
 */
void RPCDefaultCallback(const RPC_Data &data, RPC_Response &resp) {
    Logger::notice("RPC default received.");
    
    // Process data.
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);
    
    int value = doc["value"];
    
    // Return value.
    JsonObject r  = resp.to<JsonObject>();
    r["value"] = value;
}

/**
 * Callbacks list to be called when receive a RPC from ThingsBoard.
 */
int RPCCallbackListSize = 1;
RPC_Callback RPCCallbackList[] = {
    { "default",              RPCDefaultCallback }
};

/**
 * Callback to subscribe to ThingsBoard RPC/Attr.
 */
void onThingsBoardConnectCallback() {
    _cfThingsBoard.ATTRSubscribe(ATTRCallback);
    _cfThingsBoard.RPCSubscribe(RPCCallbackList, RPCCallbackListSize);
}

/**
 * Callback to be called when Wi-Fi config mode is called.
 */
void onConfigModeCallback() {
    // Render if display is defined.
    #ifdef CF_USE_DISPLAY
        _display.clearDisplay();

        // Draw bitmaps.
        _display.drawBitmap(0, 0, CFIconSet::NETWORK_HIGH_BARS_8X8, 8, 7, 1);   // Network.
        _display.drawBitmap(96, 0, CFIconSet::PHONE_8X8, 8, 7, 1);              // Things Board.
        
        // Draw lines.
        _display.setCursor(0, 0);                                               // Line 1 Size 1
        _display.print("  AP STARTED      OFF");
    
        _display.setCursor(0, 24);                                              // Line 3 Size 1
        _display.print(" SSID: " + _cfWiFiManager.getDefaultSSID());
    
        _display.setCursor(0, 32);                                              // Line 4 Size 1
        _display.print(" PASS: " + _cfWiFiManager.getDefaultPassword());
        
        _display.setCursor(0, 40);                                              // Line 5 Size 1
        _display.print(" IP: " + _cfWiFiManager.getLocalIP());
        
        _display.display();
    #endif
}

void renderHeader() {
    _display.drawBitmap(0, 0, CFIconSet::NETWORK_HIGH_BARS_8X8, 8, 7, 1);   // Network.
    _display.drawBitmap(96, 0, CFIconSet::PHONE_8X8, 8, 7, 1);              // Things Board.
    
    _display.setCursor(0, 0);                                               // Line 1 Size 1.
    if (!_cfWiFiManager.isConnected()) {
        _display.print("  OFFLINE         OFF");
    } else {
        // Print SSID.
        String ssid = _cfWiFiManager.getSSID().substring(0, 13);
        for (int i = ssid.length(); i < 13; i++) {
            ssid += " ";
        }
        _display.print("  " + ssid + "   ");

        // Print if ThingsBoard is connected.
        if (_cfThingsBoard.isConnected()) {
            _display.print("ON");
        } else {
            _display.print("OFF");
        }

        // Print IP address.
        _display.setCursor(0, 8);                                           // Line 2 Size 1.
        _display.print("IP: " + _cfWiFiManager.getLocalIP());
    }
}

/**
 * Render if display is defined.
 */
void render() {
    #ifdef CF_USE_DISPLAY
        _display.clearDisplay();
        renderHeader();

        // Render body.
        
        _display.display();
    #endif
}
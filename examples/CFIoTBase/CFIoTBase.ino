/**
 * CF Wi-Fi base for IoT devices.
 * 
 * IoT - Configurable device using ThingsBoard as server.
 * 
 * @author  Caio Frota <caiofrota@gmail.com>
 * @version 1.0.0
 * @since   Oct, 2021
 */

#include <Logger.h>                                                             // Logger.
#include <CFWiFiManagerHelper.h>                                                // CF WiFiManager Helper.
#include <CFThingsBoardHelper.h>                                                // CF ThingsBoard Helper.

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

    // Setup logger.
    Logger::setLogLevel(Logger::VERBOSE);

    // Config WiFiManager.
    _cfWiFiManager.setCustomParameters(_params, CF_WM_MAX_PARAMS_QTY);
    _cfWiFiManager.setOnSaveParametersCallback(onSaveParametersCallback);
    _cfWiFiManager.begin();

    // Config ThingsBoard.
    onSaveParametersCallback();                                                 // Call the callback once to update the first time.

    // Config ThingsBoard.
    _cfThingsBoard.setLocalIP(_cfWiFiManager.getLocalIP());
}

void loop() {
    _cfWiFiManager.loop();                                                      // Do WiFiManager loop.
    _cfThingsBoard.loop();                                                      // Do ThingsBoard loop.
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
    _cfWiFiManager.setParameter("p_device_name", data["p_device_name"]);
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
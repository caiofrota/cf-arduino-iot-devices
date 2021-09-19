/**
 * CFThingsBoardHelper.h
 * 
 * A library for Arduino that helps to integrate with ThingsBoard.
 * 
 * @author  Caio Frota <caiofrota@gmail.com>
 * @version 1.0
 * @since   Sep, 2021
 */
 
#ifndef CFThingsBoardHelper_h
#define CFThingsBoardHelper_h

#include <Arduino.h>                                                            // Arduino library.
#include <Logger.h>                                                             // Logger.
#include <WiFiClient.h>                                                         // WIFiClient.
#include <ThingsBoard.h>                                                        // Things Board.

class CFThingsBoardHelper {
    private:
        // Aliases.
        using VoidCallback = void (*)();                                        // Alias for callback.

        // ThingsBoard and WiFiClient attributes.
        WiFiClient _wifiClient;                                                 // WiFi Client.
        ThingsBoard _thingsBoard;                                               // ThingsBoard.
        
        // Config attributes.
        String _appCode;                                                        // Software code.
        String _appVersion;                                                     // Software version.
        String _serverURL;                                                      // Server URL.
        String _token;                                                          // Device token to connect to ThingsBoard device.
        String _localIP;                                                        // Local IP.
        String _deviceName;                                                     // Device name.
        long _ttRetry;                                                          // Time between connection attempts.
        long _ttSend;                                                           // Time between submissions.
        long _tLastSent;                                                        // Last time data was sent.

        // JSON Data.
        DynamicJsonDocument _data;                                              // JSON telemetry data.
        DynamicJsonDocument _attributes;                                        // JSON attributes.

        // Callbacks.
        VoidCallback _onThingsBoardConnectCallback;                             // On ThingsBoard connect callback.

    public:
        CFThingsBoardHelper(String appCode, String appVersion);                 // Constructor.
        void loop();                                                            // Loop.
        void ATTRSubscribe(const Attr_Callback callback);
        void RPCSubscribe(const RPC_Callback *callbacks, size_t size);
        void setServerURL(String serverURL);                                    // Define server URL.
        void setToken(String token);                                            // Define token.
        void setLocalIP(String localIP);                                        // Define device name.
        void setTelemetryValue(String key, int value);                          // Set telemetry int value.
        void setTelemetryValue(String key, String value);                       // Set telemetry String value.
        void setAttributeValue(String key, int value);                          // Set attribute int value.
        void setAttributeValue(String key, String value);                       // Set attribute String value.
        void setOnThingsBoardConnectCallback(const VoidCallback);               // Define on ThingsBoard connect callback.
};

#endif
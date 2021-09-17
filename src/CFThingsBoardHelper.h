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
        // ThingsBoard and WiFiClient attributes.
        WiFiClient _wifiClient;                                                 // WiFi Client.
        ThingsBoard _thingsBoard;                                               // ThingsBoard.
        //bool _rpcSubscribed;                                                    // Flag that indicates if RPC is subscribed.
        //bool _attSubscribed;                                                    // Flag that indicates if Attributes is subscribed.
        
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

        // Available callbacks.
        //Attr_Callback _attCallbacks;                                            // Attributes subscription callback.
        //RPC_Callback _rpcCallbacks;                                             // RPC subscription Callbacks.
        //int _rpcCallbacksSize;                                                  // Quantity of RPC callbacks.

    public:
        CFThingsBoardHelper(String appCode, String appVersion);                 // Constructor.
        void loop();                                                            // Loop.
        void setServerURL(String serverURL);                                    // Define server URL.
        void setToken(String token);                                            // Define token.
        void setLocalIP(String localIP);                                        // Define device name.
        void setTelemetryValue(String key, int value);                          // Set telemetry int value.
        void setTelemetryValue(String key, String value);                       // Set telemetry String value.
        void setAttributeValue(String key, int value);                          // Set attribute int value.
        void setAttributeValue(String key, String value);                       // Set attribute String value.
};

#endif
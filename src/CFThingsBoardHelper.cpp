/**
 * CFThingsBoardHelper.cpp
 * 
 * A library for Arduino that helps to integrate with ThingsBoard.
 * 
 * @author  Caio Frota <caiofrota@gmail.com>
 * @version 1.0
 * @since   Sep, 2021
 */

#include <CFThingsBoardHelper.h>                                                // CF Wi-Fi Manager.

/**
 * Constructor.
 */
CFThingsBoardHelper::CFThingsBoardHelper(String appCode, String appVersion):
        _wifiClient(), _thingsBoard(_wifiClient),
        _ttRetry(60000), _ttSend(60000),
        _data(64), _attributes(1024),
        _appCode(appCode), _appVersion(appVersion) {
    
}

/**
 * Loop.
 */
void CFThingsBoardHelper::loop() {
    // Check if it's disconnected.
    if (!_thingsBoard.connected()) {
        // Check the last attempt.
        if (_tLastSent == 0 || (millis() - _tLastSent) > _ttRetry) {
            // In case of lose connection, set variables to indicate _thingsBoard is disconnected.
            //_rpcSubscribed = false;
            //_attSubscribed = false;

            // Connect to ThingsBoard.
            Logger::notice("Connecting to Things Board node.");
            Logger::verbose("ServerURL: " + _serverURL);
            Logger::verbose("Token: " + _token);
            
            char serverURL[50]; strcpy(serverURL, _serverURL.c_str());
            char token[50]; strcpy(token, _token.c_str());
            if (_thingsBoard.connect(serverURL, token)) {
                // Get chip id.
                char espChipId[6];
                sprintf(espChipId, "%06X", ESP.getChipId());
                
                // Send attributes to ThingsBoard.
                _thingsBoard.sendAttributeString("app_code", _appCode.c_str());
                _thingsBoard.sendAttributeString("app_version", _appVersion.c_str());
                _thingsBoard.sendAttributeString("device_chip_id", espChipId);
                _thingsBoard.sendAttributeString("device_local_ip", _localIP.c_str());
            } else {
                Logger::warning("Fail connecting Things Board. Retrying in " + String(_ttRetry / 1000) + " second(s).");
                _tLastSent = millis();
                return;
            }
        } else {
            // It's not time to retry.
            return;
        }
    }
    /*
    // Check attributes subscription.
    if (!_attSubscribed) {
        if (!_thingsBoard.Attr_Subscribe(_attCallbacks)) {
            //Fail subscribing for attributes.
            return;
        }
        _attSubscribed = true;
    }

    // Check RPC subscription.
    if (!_rpcSubscribed) {
        if (!thingsBoard.RPC_Subscribe(_rpcCallbacks, _rpcCallbacksSize)) {
            // Fail subscribing for RPC.
            return;
        }
        _rpcSubscribed = true;
    }
    */

    // Check the last submission.
    if (_tLastSent == 0 || (millis() - _tLastSent) > _ttSend) {
        Logger::notice("Sending data to Things Board.");
        
        // Send telemetry.
        char serializedJson[64]; serializeJson(_data, serializedJson);
        _thingsBoard.sendTelemetryJson(serializedJson);

        // Send attributes.
        for (JsonPair p : _attributes.as<JsonObject>()) {
            const char* key = p.key().c_str();
            if (p.value().is<const char*>()) {
                _thingsBoard.sendAttributeString(key, p.value());
            }
            if (p.value().is<int>()) {
                _thingsBoard.sendAttributeInt(key, p.value());
            }
        }

        // Update last sent time.
        _tLastSent = millis();
    }

    _thingsBoard.loop();
}

/**
 * Define server URL.
 *
 * @param serverURL Server URL.
 */
void CFThingsBoardHelper::setServerURL(String serverURL) {
    _serverURL = serverURL;
}

/**
 * Define token.
 *
 * @param token Token.
 */
void CFThingsBoardHelper::setToken(String token) {
    _token = token;
}
/**
 * Define local IP.
 *
 * @param localIP Local IP.
 */
void CFThingsBoardHelper::setLocalIP(String localIP) {
    _localIP = localIP;
}

/**
 * Set telemetry int value.
 *
 * @param key Key.
 * @param value Int value.
 */
void CFThingsBoardHelper::setTelemetryValue(String key, int value) {
    _data[key] = value;
}

/**
 * Set telemetry String value.
 *
 * @param key Key.
 * @param value String value.
 */
void CFThingsBoardHelper::setTelemetryValue(String key, String value) {
    _data[key] = value;
}

/**
 * Set attribute int value.
 *
 * @param key Key.
 * @param value Int value.
 */
void CFThingsBoardHelper::setAttributeValue(String key, int value) {
    _attributes[key] = value;
}

/**
 * Set attribute String value.
 *
 * @param key Key.
 * @param value String value.
 */
void CFThingsBoardHelper::setAttributeValue(String key, String value) {
    _attributes[key] = value;
}
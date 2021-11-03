

#ifdef ESP8266
    #include "EOTAUpdate.h"
    #include <MD5Builder.h>
    #include <StreamString.h>
    #include <Updater.h>
    #include <WiFiClient.h>
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WiFi.h>
#else
    #include <HTTPClient.h>
    #include <MD5Builder.h>
    #include <StreamString.h>
    #include <Update.h>
    #include <WiFi.h>
    #include <WiFiClient.h>

    #include "EOTAUpdate.h"
#endif

#define GRAY_UPDATE       1

WiFiClient wf_client;

EOTAUpdate::EOTAUpdate(
    const String &url,
    const String chipID,
    const unsigned currentVersion,
    const unsigned long updateIntervalMs)
    :
    _url(url),
    _forceSSL(url.startsWith("https://")),
    _currentVersion(currentVersion),
    _updateIntervalMs(updateIntervalMs),
    _lastUpdateMs(0)
{
    m_str8266ChipID = chipID;
}

bool EOTAUpdate::CheckAndUpdate(bool force)
{
    const bool hasEverChecked = _lastUpdateMs != 0;
    const bool lastCheckIsRecent = (millis() - _lastUpdateMs < _updateIntervalMs);
    if (!force && hasEverChecked && lastCheckIsRecent)
    {
        return false;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wifi not connected");
        return false;
    }

    Serial.println("Checking for updates");

    _lastUpdateMs = millis();
    String binURL;
    String binMD5;
    if (GetUpdateFWURL(binURL, binMD5))
    {
        Serial.println("Update found. Performing update");
        return PerformOTA(binURL, binMD5);
    }
    return false;
}

bool EOTAUpdate::GetUpdateFWURL(String &binURL, String &binMD5)
{
    return GetUpdateFWURL(binURL, binMD5, _url);
}

bool EOTAUpdate::GetUpdateFWURL(String &binURL, String &binMD5, const String &url, const uint16_t retries)
{
    Serial.printf("Fetching OTA config from: %s\n", url.c_str());

    if (retries == 0)
    {
        Serial.println("Too many retries/redirections");
        return false;
    }

    bool isSSL = url.startsWith("https");

    if (_forceSSL && !isSSL)
    {
        Serial.println("Trying to access a non-ssl URL on a secure update checker");
        return false;
    }

    HTTPClient httpClient;
    //WiFiClient client = WiFiClient();
    if (!httpClient.begin(wf_client,url))
    {
        Serial.println("Error initializing client");
        return false;
    }

    const char *headerKeys[] = {"Location"};
    httpClient.collectHeaders(headerKeys, 1);
    int httpCode = httpClient.GET();
    switch (httpCode)
    {
    case HTTP_CODE_OK:
        break;
    case HTTP_CODE_MOVED_PERMANENTLY:
        if (httpClient.hasHeader("Location"))
        {
            return GetUpdateFWURL(binURL, binMD5, httpClient.header("Location"), retries - 1);
        }
        // Do not break here
    default:
        Serial.printf("1-[HTTP] [ERROR] [%d] %s\n",httpCode,httpClient.errorToString(httpCode).c_str());
        if (httpCode >= 400) //404 会崩溃
        {
            httpClient.end();
            return false;
        }
        Serial.printf("Response:\n%s\n",httpClient.getString().c_str());
        httpClient.end();
        return false;
    }

    auto & payloadStream = httpClient.getStream();
    binURL = payloadStream.readStringUntil('\n');
    const unsigned newVersionNumber = payloadStream.readStringUntil('\n').toInt();
    binMD5 = payloadStream.readStringUntil('\n');
    const String newVersionString = payloadStream.readStringUntil('\n');
    unsigned nUpdateFlag =  payloadStream.readStringUntil('\n').toInt(); //0是全网升级，1是灰度升级
    String strUpdateList =  payloadStream.readStringUntil('\n');

    int nSub = binURL.lastIndexOf("\r");
    binURL = binURL.substring(0,nSub);

    nSub = binMD5.lastIndexOf("\r");
    binMD5 = binMD5.substring(0,nSub);

    nSub = strUpdateList.lastIndexOf("\r");
    strUpdateList = strUpdateList.substring(0,nSub);

    Serial.printf("binURL=%s\n", binURL.c_str());
    Serial.printf("binMD5=%s\n", binMD5.c_str());
    Serial.printf("newVersionNumber=%d\n",newVersionNumber);
    Serial.printf("newVersionString=%s\n", newVersionString.c_str());
    Serial.printf("nUpdateFlag=%d\n", nUpdateFlag);
    Serial.printf("strUpdateList=%s\n", strUpdateList.c_str());

    httpClient.end();

    if (binURL.length() == 0)
    {
        Serial.println("Error parsing remote path of new binary");
        return false;
    }

    if (newVersionNumber == 0)
    {
        Serial.println("Error parsing version number");
        return false;
    }

    // int nSub = binMD5.lastIndexOf("\r");
    // Serial.printf("nSub=%d\n",nSub);
    // binMD5 = binMD5.substring(0,nSub);
    if (binMD5.length() > 0 && binMD5.length() != 32)
    {
        Serial.printf("The MD5:%s is not 32 characters long. Aborting update! binMD5.length()=%d\n",binMD5.c_str(), binMD5.length());
        return false;
    }

    
    bool bIsUpdate =   (newVersionNumber > _currentVersion);
    Serial.printf("Need Update = %d\n", bIsUpdate);
    if (bIsUpdate)
    {
        if (nUpdateFlag == GRAY_UPDATE)
        {  
            int nFind = strUpdateList.indexOf(m_str8266ChipID);
            if(nFind != -1)
            {
                bIsUpdate =  true;
            }
            else
            {
                bIsUpdate = false;
            }
            
            Serial.printf("%s Gray Update: %d\n",m_str8266ChipID.c_str(), bIsUpdate);
        }
    }
    
    Serial.println("Fetched update information:");
    Serial.printf("File url:           %s\n",       binURL.c_str());
    Serial.printf("File MD5:           %s\n",       binMD5.c_str());
    Serial.printf("Current version:    %u\n",       _currentVersion);
    Serial.printf("Current ChipID:     %s\n",       m_str8266ChipID.c_str());
    Serial.printf("Published version:  [%u] %s\n",  newVersionNumber, newVersionString.c_str());
    Serial.printf("Update available:   %s\n",       bIsUpdate ? "YES" : "NO");
    Serial.printf("Update Flag:   %d\n",      nUpdateFlag);
    Serial.printf("Update List:   %s\n",      strUpdateList.c_str());

    return bIsUpdate;
}

bool EOTAUpdate::PerformOTA(String &binURL, String &binMD5)
{   
    Serial.printf("Fetching OTA from: %s\n", binURL.c_str());

    if (binURL.length() == 0)
    {
        return false;
    }

    bool isSSL = binURL.startsWith("https");
    if (_forceSSL && !isSSL)
    {
        Serial.println("Trying to access a non-ssl URL on a secure update checker");
        return false;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wifi not connected");
        return false;
    }

    HTTPClient httpClient;
    if (!httpClient.begin(wf_client,binURL))
    {
        Serial.println("Error initializing client");
        return false;
    }

    const auto httpCode = httpClient.GET();
    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("2-[HTTP] [ERROR] [%d] %s\n",
                httpCode,
                httpClient.errorToString(httpCode).c_str());
        Serial.printf("Response:\n%s\n",
                httpClient.getString().c_str());
        return false;
    }

    const auto payloadSize = httpClient.getSize();
    auto & payloadStream = httpClient.getStream();

    if (binMD5.length() > 0 &&
        !Update.setMD5(binMD5.c_str()))
    {
            Serial.println("Failed to set the expected MD5");
            return false;
    }

    const bool canBegin = Update.begin(payloadSize);

    if (payloadSize <= 0)
    {
        Serial.println("Fetched binary has 0 size");
        return false;
    }

    if (!canBegin)
    {
        Serial.println("Not enough space to begin OTA");
        return false;
    }

    const auto written = Update.writeStream(payloadStream);
    if (written != payloadSize)
    {
        Serial.printf("Error. Written %lu out of %lu\n", written, payloadSize);
        return false;
    }

    if (!Update.end())
    {
        StreamString errorMsg;
        Update.printError(errorMsg);
        Serial.printf("Error Occurred: %s\n", errorMsg.c_str());
        return false;
    }

    if (!Update.isFinished())
    {
        StreamString errorMsg;
        Update.printError(errorMsg);
        Serial.printf("Undefined OTA update error: %s\n", errorMsg.c_str());
        return false;
    }

    Serial.println("Update completed. Rebooting");
    ESP.restart();
    return true;
}

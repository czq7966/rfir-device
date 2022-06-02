#include "Arduino.h"
#include "rfir/util/interrupt.h"

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
        DEBUGER.println("Wifi not connected");
        return false;
    }

    DEBUGER.println("Checking for updates");

    _lastUpdateMs = millis();
    String binURL;
    String binMD5;
    bool result = false;
    GInterrupt.stop();
    if (GetUpdateFWURL(binURL, binMD5))
    {
        DEBUGER.println("Update found. Performing update");
        result = PerformOTA(binURL, binMD5);
    }
    GInterrupt.start();

    return result;
}

bool EOTAUpdate::GetUpdateFWURL(String &binURL, String &binMD5)
{
    return GetUpdateFWURL(binURL, binMD5, _url);
}

bool EOTAUpdate::GetUpdateFWURL(String &binURL, String &binMD5, const String &url, const uint16_t retries)
{
    DEBUGER.printf("Fetching OTA config from: %s\n", url.c_str());

    if (retries == 0)
    {
        DEBUGER.println("Too many retries/redirections");
        return false;
    }

    bool isSSL = url.startsWith("https");

    if (_forceSSL && !isSSL)
    {
        DEBUGER.println("Trying to access a non-ssl URL on a secure update checker");
        return false;
    }

    HTTPClient httpClient;
    //WiFiClient client = WiFiClient();
    if (!httpClient.begin(wf_client,url))
    {
        DEBUGER.println("Error initializing client");
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
        DEBUGER.printf("1-[HTTP] [ERROR] [%d] %s\n",httpCode,httpClient.errorToString(httpCode).c_str());
        if (httpCode >= 400) //404 会崩溃
        {
            httpClient.end();
            return false;
        }
        DEBUGER.printf("Response:\n%s\n",httpClient.getString().c_str());
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

    DEBUGER.printf("binURL=%s\n", binURL.c_str());
    DEBUGER.printf("binMD5=%s\n", binMD5.c_str());
    DEBUGER.printf("newVersionNumber=%d\n",newVersionNumber);
    DEBUGER.printf("newVersionString=%s\n", newVersionString.c_str());
    DEBUGER.printf("nUpdateFlag=%d\n", nUpdateFlag);
    DEBUGER.printf("strUpdateList=%s\n", strUpdateList.c_str());

    httpClient.end();

    if (binURL.length() == 0)
    {
        DEBUGER.println("Error parsing remote path of new binary");
        return false;
    }

    if (newVersionNumber == 0)
    {
        DEBUGER.println("Error parsing version number");
        return false;
    }

    // int nSub = binMD5.lastIndexOf("\r");
    // DEBUGER.printf("nSub=%d\n",nSub);
    // binMD5 = binMD5.substring(0,nSub);
    if (binMD5.length() > 0 && binMD5.length() != 32)
    {
        DEBUGER.printf("The MD5:%s is not 32 characters long. Aborting update! binMD5.length()=%d\n",binMD5.c_str(), binMD5.length());
        return false;
    }

    
    bool bIsUpdate =   (newVersionNumber > _currentVersion);
    DEBUGER.printf("Need Update = %d\n", bIsUpdate);
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
            
            DEBUGER.printf("%s Gray Update: %d\n",m_str8266ChipID.c_str(), bIsUpdate);
        }
    }
    
    DEBUGER.println("Fetched update information:");
    DEBUGER.printf("File url:           %s\n",       binURL.c_str());
    DEBUGER.printf("File MD5:           %s\n",       binMD5.c_str());
    DEBUGER.printf("Current version:    %u\n",       _currentVersion);
    DEBUGER.printf("Current ChipID:     %s\n",       m_str8266ChipID.c_str());
    DEBUGER.printf("Published version:  [%u] %s\n",  newVersionNumber, newVersionString.c_str());
    DEBUGER.printf("Update available:   %s\n",       bIsUpdate ? "YES" : "NO");
    DEBUGER.printf("Update Flag:   %d\n",      nUpdateFlag);
    DEBUGER.printf("Update List:   %s\n",      strUpdateList.c_str());

    return bIsUpdate;
}

bool EOTAUpdate::PerformOTA(String &binURL, String &binMD5)
{   
    DEBUGER.printf("Fetching OTA from: %s\n", binURL.c_str());

    if (binURL.length() == 0)
    {
        return false;
    }

    bool isSSL = binURL.startsWith("https");
    if (_forceSSL && !isSSL)
    {
        DEBUGER.println("Trying to access a non-ssl URL on a secure update checker");
        return false;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        DEBUGER.println("Wifi not connected");
        return false;
    }

    HTTPClient httpClient;
    if (!httpClient.begin(wf_client,binURL))
    {
        DEBUGER.println("Error initializing client");
        return false;
    }

    const auto httpCode = httpClient.GET();
    if (httpCode != HTTP_CODE_OK)
    {
        DEBUGER.printf("2-[HTTP] [ERROR] [%d] %s\n",
                httpCode,
                httpClient.errorToString(httpCode).c_str());
        DEBUGER.printf("Response:\n%s\n",
                httpClient.getString().c_str());
        return false;
    }

    const auto payloadSize = httpClient.getSize();
    auto & payloadStream = httpClient.getStream();

    if (binMD5.length() > 0 &&
        !Update.setMD5(binMD5.c_str()))
    {
            DEBUGER.println("Failed to set the expected MD5");
            return false;
    }

    const bool canBegin = Update.begin(payloadSize);

    if (payloadSize <= 0)
    {
        DEBUGER.println("Fetched binary has 0 size");
        return false;
    }

    if (!canBegin)
    {
        DEBUGER.println("Not enough space to begin OTA");
        return false;
    }

    const auto written = Update.writeStream(payloadStream);
    if (written != payloadSize)
    {
        DEBUGER.printf("Error. Written %lu out of %lu\n", written, payloadSize);
        return false;
    }

    if (!Update.end())
    {
        StreamString errorMsg;
        Update.printError(errorMsg);
        DEBUGER.printf("Error Occurred: %s\n", errorMsg.c_str());
        return false;
    }

    if (!Update.isFinished())
    {
        StreamString errorMsg;
        Update.printError(errorMsg);
        DEBUGER.printf("Undefined OTA update error: %s\n", errorMsg.c_str());
        return false;
    }

    DEBUGER.println("Update completed. Rebooting");
    ESP.restart();
    return true;
}

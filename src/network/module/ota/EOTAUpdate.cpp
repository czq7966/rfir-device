#include "Arduino.h"
#include "rfir/util/interrupt.h"
#include "rfir/util/util.h"

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
        GDebuger.println(F("Wifi not connected"));
        return false;
    }

    GDebuger.println(F("Checking for updates"));

    _lastUpdateMs = millis();
    String binURL;
    String binMD5;
    bool result = false;
    GInterrupt.stop();
    if (GetUpdateFWURL(binURL, binMD5))
    {
        GDebuger.println(F("Update found. Performing update"));
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
    GDebuger.print(F("Fetching OTA config from:"));
    GDebuger.println(url.c_str());

    if (retries == 0)
    {
        GDebuger.println(F("Too many retries/redirections"));
        return false;
    }

    bool isSSL = url.startsWith("https");

    if (_forceSSL && !isSSL)
    {
        GDebuger.println(F("Trying to access a non-ssl URL on a secure update checker"));
        return false;
    }

    HTTPClient httpClient;
    //WiFiClient client = WiFiClient();
    if (!httpClient.begin(wf_client,url))
    {
        GDebuger.println(F("Error initializing client"));
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
        GDebuger.print(F("1-[HTTP] [ERROR] "));
        GDebuger.println(httpCode);
        GDebuger.println(httpClient.errorToString(httpCode).c_str());
        if (httpCode >= 400) //404 会崩溃
        {
            httpClient.end();
            return false;
        }
        GDebuger.println(F("Response:"));
        GDebuger.println(httpClient.getString().c_str());

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

    GDebuger.print(F("binURL="));
    GDebuger.println(binURL.c_str());
    GDebuger.print(F("binMD5="));
    GDebuger.println(binMD5.c_str());
    GDebuger.print(F("newVersionNumber="));
    GDebuger.println(newVersionNumber);
    GDebuger.print(F("newVersionString="));
    GDebuger.println(newVersionString.c_str());
    GDebuger.print(F("nUpdateFlag="));
    GDebuger.println(nUpdateFlag);
    GDebuger.print(F("strUpdateList="));
    GDebuger.println(strUpdateList.c_str());

    httpClient.end();

    if (binURL.length() == 0)
    {
        GDebuger.println(F("Error parsing remote path of new binary"));
        return false;
    }

    if (newVersionNumber == 0)
    {
        GDebuger.println(F("Error parsing version number"));
        return false;
    }


    if (binMD5.length() > 0 && binMD5.length() != 32)
    {
        GDebuger.print(F("The MD5:"));
        GDebuger.print(binMD5.c_str());
        GDebuger.print(F(" is not 32 characters long. Aborting update! binMD5.length()="));
        GDebuger.println(binMD5.length());
        return false;
    }

    
    bool bIsUpdate =   (newVersionNumber > _currentVersion);
    GDebuger.print(F("Need Update = "));
    GDebuger.println(bIsUpdate);
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
        }
    }
    
    GDebuger.println(F("Fetched update information:"));
    GDebuger.print(F("File url:           "));
    GDebuger.println(binURL.c_str());
    GDebuger.print(F("File MD5:           "));
    GDebuger.println(binMD5.c_str());
    GDebuger.print(F("Current version:    "));
    GDebuger.println(_currentVersion);
    GDebuger.print(F("Current ChipID:     "));
    GDebuger.println(m_str8266ChipID.c_str());
    GDebuger.print(F("Update available:   "));
    GDebuger.println(bIsUpdate ? F("YES") : F("NO"));
    GDebuger.print(F("Update Flag:   "));
    GDebuger.println(nUpdateFlag);
    GDebuger.print(F("Update List:   "));
    GDebuger.println(strUpdateList.c_str());

    return bIsUpdate;
}

bool EOTAUpdate::PerformOTA(String &binURL, String &binMD5)
{   
    GDebuger.print(F("Fetching OTA from: "));
    GDebuger.println(binURL.c_str());

    if (binURL.length() == 0)
    {
        return false;
    }

    bool isSSL = binURL.startsWith("https");
    if (_forceSSL && !isSSL)
    {
        GDebuger.println(F("Trying to access a non-ssl URL on a secure update checker"));
        return false;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        GDebuger.println(F("Wifi not connected"));
        return false;
    }

    HTTPClient httpClient;
    if (!httpClient.begin(wf_client,binURL))
    {
        GDebuger.println(F("Error initializing client"));
        return false;
    }

    const auto httpCode = httpClient.GET();
    if (httpCode != HTTP_CODE_OK)
    {
        GDebuger.print(F("2-[HTTP] [ERROR] "));
        GDebuger.println(httpCode);
        GDebuger.println(httpClient.errorToString(httpCode).c_str());
        GDebuger.println(F("Response:"));
        GDebuger.println(httpClient.getString().c_str());
        return false;
    }

    const auto payloadSize = httpClient.getSize();
    auto & payloadStream = httpClient.getStream();

    if (binMD5.length() > 0 &&
        !Update.setMD5(binMD5.c_str()))
    {
            GDebuger.println(F("Failed to set the expected MD5"));
            return false;
    }

    const bool canBegin = Update.begin(payloadSize);

    if (payloadSize <= 0)
    {
        GDebuger.println(F("Fetched binary has 0 size"));
        return false;
    }

    if (!canBegin)
    {
        GDebuger.println(F("Not enough space to begin OTA"));
        return false;
    }

    const auto written = Update.writeStream(payloadStream);
    if (written != payloadSize)
    {
        GDebuger.print(F("Error. Written "));
        GDebuger.print(written);
        GDebuger.print(F(" out of "));
        GDebuger.println(payloadSize);
        return false;
    }

    if (!Update.end())
    {
        StreamString errorMsg;
        Update.printError(errorMsg);
        GDebuger.print(F("Error Occurred: "));
        GDebuger.println(errorMsg.c_str());
        return false;
    }

    if (!Update.isFinished())
    {
        StreamString errorMsg;
        Update.printError(errorMsg);
        GDebuger.print(F("Undefined OTA update error: "));
        GDebuger.println(errorMsg.c_str());
        return false;
    }

    GDebuger.println(F("Update completed. Rebooting"));
    rfir::util::Util::Reset();

    return true;
}

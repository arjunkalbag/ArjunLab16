/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/student/Desktop/IoT/ArjunLab16/src/ArjunLab16.ino"
#include "oled-wing-adafruit.h"

void setup();
void loop();
void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);
void resetDisplay();
#line 3 "/Users/student/Desktop/IoT/ArjunLab16/src/ArjunLab16.ino"
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

BleAdvertisingData datas;
OledWingAdafruit display;

void onDataReceived(const uint8_t *datas, size_t len, const BlePeerDevice &peer, void *context);

const size_t UART_TX_BUF_SIZE = 20;

void onDataReceived(const uint8_t *datas, size_t len, const BlePeerDevice &peer, void *context);

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");

const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);

uint8_t txBuf[10]; // an array of 10 bytes we can send

String charToStr;
bool DC = false;
bool RC = false;

void setup()
{
    BLE.on();
    RGB.control(true);
    Serial.begin(9600);
    display.setup();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);

    txCharacteristic.setValue(txBuf, 10); // here we are sending all 10 bytes

    datas.appendServiceUUID(serviceUuid);
    BLE.advertise(&datas);
    BLE.setDeviceName("Arjon!");
}

void loop()
{
    display.loop();
    if (DC)
    {
        BLE.disconnect();
        DC = false;
        RGB.color(255, 255, 0);
        resetDisplay();
        display.println("Disconnected");
        display.display();
        delay(3000);
        RC = false;
    }
    if (!BLE.connected())
    {
        RGB.color(255, 255, 0);
    }
    if (BLE.connected())
    {
        if (RC == false)
        {
            resetDisplay();
            display.display();
            RC = !RC;
        }
        RGB.color(0, 0, 255);
        uint8_t txBuf[UART_TX_BUF_SIZE];
        String message = "Hey!\n";
        message.toCharArray((char *)txBuf, message.length() + 1);
        txCharacteristic.setValue(txBuf, message.length() + 1);
    }
    delay(1000);
}

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)
{
    for (size_t i = 0; i < len; i++)
    {
        charToStr = String(data[i]);
        int strToInt = charToStr.toInt();
        Serial.write(data[i]);
        if (strToInt == 49)
        {
            resetDisplay();
            display.println("1!");
            display.display();
        }
        if (strToInt == 48)
        {
            DC = true;
        }
    }
}

void resetDisplay()
{
    display.setCursor(0, 0);
    display.clearDisplay();
}
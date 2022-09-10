#include <ModbusRTU.h>
#include <Arduino.h>
#include <MCP3208.h>
#include <SPI.h>

// Firmware FOR BOARD 16IN_16OUT
// Using Uart2 for RS_485
//#define SLAVE_ID 1
// Chỉnh sửa cho đúng với board
int input[] = {A8, A9, A10, A11, A12, A13, A14, A15,
               A3, A2, A1, A0, A4, A5, A6, A7};
int output[] = {37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22};
#pragma region Button
#define BT1UP 2
#define BT1DOWN 3

#define BT2UP 4
#define BT2DOWN 5

#define BT3UP 6
#define BT3DOWN 7

#define BT4UP 8
#define BT4DOWN 9

#define BT5UP 10
#define BT5DOWN 11

#define BT6UP 12
#define BT6DOWN 13

#define BT7UP 14
#define BT7DOWN 15

#define BT8UP 16
#define BT8DOWN 17

#define BT9UP 18
#define BT9DOWN 19

#define TO_REG 10
#define SLAVE_ID 1
#define PULL_ID 1
#define FROM_REG 20
#pragma endregion
long ModbusBaurate;
ModbusRTU mb;
bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void *data)
{
    Serial.print("Request result: 0x");
    Serial.print(event, HEX);
    return true;
}
void setup()
{
#pragma region Pinmode
    pinMode(BT1UP, INPUT_PULLUP);
    pinMode(BT1DOWN, INPUT_PULLUP);
    pinMode(BT2UP, INPUT_PULLUP);
    pinMode(BT2DOWN, INPUT_PULLUP);
    pinMode(BT3UP, INPUT_PULLUP);
    pinMode(BT3DOWN, INPUT_PULLUP);
    pinMode(BT4UP, INPUT_PULLUP);
    pinMode(BT4DOWN, INPUT_PULLUP);
    pinMode(BT5UP, INPUT_PULLUP);
    pinMode(BT5DOWN, INPUT_PULLUP);
    pinMode(BT6UP, INPUT_PULLUP);
    pinMode(BT6DOWN, INPUT_PULLUP);
    pinMode(BT7UP, INPUT_PULLUP);
    pinMode(BT7DOWN, INPUT_PULLUP);
    pinMode(BT8UP, INPUT_PULLUP);
    pinMode(BT8DOWN, INPUT_PULLUP);
    pinMode(BT9UP, INPUT_PULLUP);
    pinMode(BT9DOWN, INPUT_PULLUP);
#pragma endregion
    // 16Input
    for (int i = 0; i < 16; i++)
    {
        pinMode(input[i], INPUT_PULLUP);
    }
    // 16Output
    for (int i = 0; i < 16; i++)
    {
        pinMode(output[i], OUTPUT);
    }
    Serial.begin(9600, SERIAL_8N1);
    mb.begin(&Serial);
    mb.setBaudrate(9600);

    mb.addCoil(0, 0, 16); //  Thêm 100 Coils
    mb.addHreg(0, 0, 16); //  Thêm thanh ghi hoding register với địa chỉ bắt đầu = 0 và độ dài thanh ghi =100
    mb.addIsts(0, 0, 16); //  Thêm thanh ghi discrete với địa chỉ bắt đầu = 0, giá trị set ban đầu = false và độ dài thanh ghi = 100
    mb.addIreg(0, 0, 16); //  Thêm thanh ghi discrete với địa chỉ bắt đầu = 0, giá trị set ban đầu = false và độ dài thanh ghi = 100
                          //  mb.Ireg(0,1992);      //  Dùng cho xác thực board từ PLC
    /*----------------------------------------------------------------*/
    mb.master();
}

bool coils[20];
void loop()
{
    if (!mb.slave())
    {
        mb.readCoil(1, 1, coils, 20, cbWrite);
        //Đọc giá trị Analog từ biến trở
        // Từ 10 đến 100%
        int value = analogRead(A0);
        value = map(value, 0, 1023, 10, 100);
        // Save Value to HoldingRes
        if ((digitalRead(BT1UP) == HIGH) && (digitalRead(BT1DOWN) == LOW) && value != 0)
        {
            // Up
            mb.Hreg(1, value);
        }
        if ((digitalRead(BT1UP) == LOW) && (digitalRead(BT1DOWN) == HIGH) && value != 0)
        {
            // Down
            mb.Hreg(2, value);
        }
        if ((digitalRead(BT1UP) == LOW) && (digitalRead(BT1DOWN) == LOW) && value != 0)
        {
            // Stop
            mb.Hreg(0, 0);
            mb.Hreg(1, 0);
        }
    }
    mb.task();
    yield();
}
//
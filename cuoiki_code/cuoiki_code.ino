#include "TrafficLight.h"

// Chân kết nối các segment của LED 7 đoạn
const int segmentPins[7] = {2, 3, 4, 5, 6, 7, 8};

// Chân kết nối các chung của 6 LED 7 đoạn
const int digitPins[4] = {9, 10, 11, 12};
int x = 1;
int k = 1;
int currentLight = 1;         // Biến theo dõi đèn hiện tại
bool counting = true;         // Biến để theo dõi trạng thái đếm
bool displayEnabled = true;   // Biến theo dõi hiển thị led 7 đoạn
bool nightModeActive = false; // Biến theo dõi trạng thái chế độ night

// Mảng lưu các giá trị để hiển thị các chữ số 0-9 trên LED 7 đoạn (common cathode)
const byte digitCodes[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

unsigned long previousMillis = 0; // Biến để lưu thời điểm trước đó
const long interval = 1000;       // Khoảng thời gian giữa các lần cập nhật (300 ms)

int counter1 = 0; // Biến đếm đèn 1
int counter2 = 0;      // Biến đếm đèn 2

int dx1 = 30;
int dv1 = 3;
int dd1 = 31;

int dx2 = 27;
int dv2 = 3;
int dd2 = 34;

TrafficLight trafficLight_1(A0, A1, A2, dx1, dv1, dd1);
TrafficLight trafficLight_2(A3, A4, A5, dx2, dv2, dd2);

void setup()
{
  Serial.begin(9600); // Khởi tạo Serial
  trafficLight_1.begin();
  trafficLight_2.begin();

  // Thiết lập các chân segment là OUTPUT
  for (int i = 0; i < 7; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);
  }

  // Thiết lập các chân digit là OUTPUT
  for (int i = 0; i < 4; i++)
  {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // Tắt tất cả các digit ban đầu
  }

  previousMillis = millis();
}

void turnOffAllLEDs()
{
  displayEnabled = false;
  // Tắt tất cả các segment LED 7 đoạn
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(segmentPins[i], LOW);
  }

  // Tắt tất cả các digit
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(digitPins[i], HIGH); // Tắt (common cathode)
  }
}

void turnOnAllLEDs()
{
  displayEnabled = true;
}

void sendCurrentState()
{
  if (x == 2)
  {
    Serial.print("Green 1: ");
  }
  else if (x == 3)
  {
    Serial.print("Yellow 1: ");
  }
  else
  {
    Serial.print("Red 1: ");
  }
  Serial.println(counter1);

  if (k == 2)
  {
    Serial.print("Red 2: ");
  }
  else if (k == 3)
  {
    Serial.print("Green 2: ");
  }
  else
  {
    Serial.print("Yellow 2: ");
  }
  Serial.println(counter2);
}

void handleCommand(int command)
{
  switch (command)
  {
  case 1: // Chế độ stop
    counting = false;
    nightModeActive = false; // Tắt night mode
    Serial.println("Counting stopped.");
    break;
  case 2: // Chế độ night
    // trafficLight_1.yellowPinActivate();
    // trafficLight_2.yellowPinActivate();
    turnOffAllLEDs();       // Tắt tất cả LED
    counting = false;       // Dừng đếm
    nightModeActive = true; // Kích hoạt chế độ night
    Serial.println("Night mode activated.");
    break;
  case 3:                    // Chế độ normal
    counting = true;         // Quay lại chế độ đếm
    turnOnAllLEDs();         // Bật lại tất cả LED
    nightModeActive = false; // Tắt night mode
    x = 1;
    k = 1;
    trafficLight_1.normalInterval();
    trafficLight_2.normalInterval();
    trafficLight_1.greenPinActivate();
    trafficLight_2.redPinActivate();
    counter1 = 0;
    counter2 = 0;
    Serial.println("Normal mode activated.");
    break;
  case 4: // Chế độ peak hour
    trafficLight_1.peakHourMode();
    trafficLight_2.peakHourMode();
    x = 1;
    k = 1;
    counter1 = 0;
    counter2 = 0;
    counting = true;
    nightModeActive = false; // Tắt night mode
    Serial.println("Peak hour mode activated.");
    break;
  case 5:             // Chế độ manual
    counting = false; // Dừng đếm
    turnOffAllLEDs(); // Tắt tất cả LED
    // Chuyển đổi giữa các chế độ đèn
    if (currentLight == 1)
    {
      trafficLight_1.redPinActivate();
      trafficLight_2.greenPinActivate();
      currentLight = 2; // Đèn 2 đang sáng
    }
    else
    {
      trafficLight_1.greenPinActivate();
      trafficLight_2.redPinActivate();
      currentLight = 1; // Đèn 1 đang sáng
    }
    Serial.println("Light switched to manual mode.");
    break;
  default:
    Serial.println("Invalid command.");
  }
}

void loop()
{
  unsigned long currentMillis = millis();

  // Kiểm tra xem có đủ thời gian trôi qua chưa và trạng thái đếm đang bật
  if (counting && currentMillis - previousMillis >= interval)
  {
    sendCurrentState();
    previousMillis = currentMillis;
    counter1--;
    counter2--;
  }

  // Kiểm tra xem có dữ liệu từ Serial Monitor không
  if (Serial.available() > 0)
  {
    String commandString = Serial.readStringUntil('\n'); // Đọc chuỗi từ Serial
    int command = commandString.toInt();                 // Chuyển đổi chuỗi thành số
    handleCommand(command);
  }

  // Nhấp nháy đèn vàng trong chế độ night
  if (nightModeActive)
  {
    trafficLight_1.yellowPinActivate();
    delay(500); // Bật trong 0.5 giây
    trafficLight_1.pinDeactivate();
    trafficLight_2.yellowPinActivate();
    delay(500); // Tắt trong 0.5 giây
    trafficLight_2.pinDeactivate();
  }

  int digits[4];
  digits[0] = counter1 / 10;
  digits[1] = counter1 % 10;
  digits[2] = counter2 / 10;
  digits[3] = counter2 % 10;

  // Hiển thị từng digit
  if (displayEnabled)
  {
    for (int i = 0; i < 4; i++)
    {
      // Tắt tất cả các digit trước khi bật
      for (int j = 0; j < 4; j++)
      {
        digitalWrite(digitPins[j], HIGH); // Tắt (common cathode)
      }

      // Thiết lập các segment
      byte code = digitCodes[digits[i]];
      for (int s = 0; s < 7; s++)
      {
        digitalWrite(segmentPins[s], (code & (1 << s)) ? HIGH : LOW);
      }

      // Bật digit hiện tại
      digitalWrite(digitPins[i], LOW); // Bật (common cathode)

      // Đợi một khoảng thời gian ngắn để hiển thị
      delay(5);

      // Tắt digit hiện tại
      digitalWrite(digitPins[i], HIGH);
    }
  }

  // Điều khiển đèn giao thông
  if (counter1 < 0)
  {
    if (x == 1)
    {
      trafficLight_1.greenPinActivate();
      x = 2;
      counter1 = trafficLight_1.getGreenPinInterval();
    }
    else if (x == 2)
    {
      trafficLight_1.yellowPinActivate();
      x = 3;
      counter1 = trafficLight_1.getYellowPinInterval();
    }
    else if (x == 3)
    {
      trafficLight_1.redPinActivate();
      counter1 = trafficLight_1.getRedPinInterval();
      x = 1;
    }
  }

  if (counter2 < 0)
  {
    if (k == 1)
    {
      trafficLight_2.redPinActivate();
      k = 2;
      counter2 = trafficLight_2.getRedPinInterval();
    }
    else if (k == 2)
    {
      trafficLight_2.greenPinActivate();
      k = 3;
      counter2 = trafficLight_2.getGreenPinInterval();
    }
    else if (k == 3)
    {
      trafficLight_2.yellowPinActivate();
      counter2 = trafficLight_2.getYellowPinInterval();
      k = 1;
    }
  }
}
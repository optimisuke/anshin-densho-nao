#include <M5Atom.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"

const char* MESSAGE = "ただいま！！";
const int MAX_RETRIES = 5;

const uint8_t RING[] = {0,1,2,3,4, 9,14,19, 24,23,22,21,20, 15,10,5};

// 成功時パターン（255 = 終端）
//   インデックス = 行*5 + 列（0〜24）
// . . . . .
// . O . O .
// . . . . .
// O . . . O
// . O O O .
const uint8_t P_HAPPY[]    = {6,8, 15,19, 21,22,23, 255};

// . O . O .
// O O O O O
// O O O O O
// . O O O .
// . . O . .
const uint8_t P_HEART[]    = {1,3, 5,6,7,8,9, 10,11,12,13,14, 16,17,18, 22, 255};

// . . O . .
// . . O . .
// O O O O O
// . . O . .
// . . O . .
const uint8_t P_STAR[]     = {2, 7, 10,11,12,13,14, 17, 22, 255};

// O . O . O
// O O O O O
// . O O O .
// . O O O .
// . O O O .
const uint8_t P_CROWN[]    = {0,2,4, 5,6,7,8,9, 11,12,13, 16,17,18, 21,22,23, 255};

// . . O O .
// . . O . .
// . O O . .
// . O . . .
// O O . . .
const uint8_t P_LIGHTNING[]= {3,4, 7, 11,12, 16, 20,21, 255};

// . . O . .
// . O . O .
// O . . . O
// . O . O .
// . . O . .
const uint8_t P_DIAMOND[]  = {2, 6,8, 10,14, 16,18, 22, 255};

// . . O . .
// . O . O .
// O O O O O
// . O . O .
// . . O . .
const uint8_t P_FLOWER[]   = {2, 6,8, 10,11,12,13,14, 16,18, 22, 255};

// O . O . O
// . O . O .
// O . O . O
// . O . O .
// O . O . O
const uint8_t P_CHECKER[]  = {0,2,4, 6,8, 10,12,14, 16,18, 20,22,24, 255};

// . . O . .
// . O O O .
// O . O . O
// . . O . .
// . . O . .
const uint8_t P_UPARROW[]  = {2, 6,7,8, 10,12,14, 17, 22, 255};

// . O O O .
// O . . . O
// O . . . O
// O . . . O
// . O O O .
const uint8_t P_FRAME[]    = {1,2,3, 5,9, 10,14, 15,19, 21,22,23, 255};

const uint8_t* SUCCESS_PATTERNS[] = {
  P_HAPPY, P_HEART, P_STAR, P_CROWN, P_LIGHTNING,
  P_DIAMOND, P_FLOWER, P_CHECKER, P_UPARROW, P_FRAME
};
const uint32_t SUCCESS_COLORS[] = {
  0x003300, // 緑
  0x330033, // マゼンタ
  0x333300, // 黄
  0x003333, // シアン
  0x332200, // オレンジ
  0x000033, // 青
  0x113300, // 黄緑
  0x222222, // 白（暗め）
  0x001133, // 水色
  0x331100, // 琥珀
};
const int NUM_PATTERNS = 10;

// 失敗時：泣き顔（赤）
const uint8_t P_SAD[] = {6,8, 16,17,18, 20,24, 255};

void clearDisplay() {
  M5.dis.fillpix(0x000000);
}

void drawPattern(const uint8_t* pixels, uint32_t color) {
  M5.dis.fillpix(0x000000);
  for (int i = 0; pixels[i] != 255; i++) {
    M5.dis.drawpix(pixels[i], color);
  }
}

void setup() {
  M5.begin(true, false, true);
  randomSeed(esp_random());

  bool connected = false;
  int ringIdx = 0;

  for (int attempt = 0; attempt < MAX_RETRIES && !connected; attempt++) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < 20) {
      clearDisplay();
      M5.dis.drawpix(RING[ringIdx % 16], 0x000033);
      ringIdx++;
      delay(500);
      tries++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
    } else {
      WiFi.disconnect(true);
      delay(1000);
    }
  }

  if (connected) {
    HTTPClient http;
    http.begin(NTFY_URL);
    http.addHeader("Content-Type", "text/plain; charset=utf-8");
    int code = http.POST(MESSAGE);
    http.end();

    if (code == 200) {
      int idx = random(NUM_PATTERNS);
      drawPattern(SUCCESS_PATTERNS[idx], SUCCESS_COLORS[idx]);
    } else {
      drawPattern(P_SAD, 0x330000);
    }
  } else {
    drawPattern(P_SAD, 0x330000);
  }

  delay(2000);
  clearDisplay();
  delay(100);
  WiFi.disconnect(true);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0);
  esp_deep_sleep_start();
}

void loop() {}

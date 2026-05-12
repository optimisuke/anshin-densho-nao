#include <M5StickCPlus.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"

const char* MESSAGE = "ただいま！！";
const int MAX_RETRIES = 5;

void showEmoji(const char* emoji, uint32_t bgColor) {
  M5.Lcd.fillScreen(bgColor);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setTextSize(5);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.drawString(emoji, M5.Lcd.width() / 2, M5.Lcd.height() / 2);
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);

  // 接続中：黄色にぐるぐる的な表示
  showEmoji("...", TFT_NAVY);

  bool connected = false;
  for (int attempt = 0; attempt < MAX_RETRIES && !connected; attempt++) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < 20) {
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
      showEmoji(":)", TFT_DARKGREEN); // 成功：緑にニコニコ
    } else {
      showEmoji(":(", TFT_RED); // 送信失敗：赤
    }
  } else {
    showEmoji("NG", TFT_RED); // WiFi失敗：赤
  }

  WiFi.disconnect(true);
  delay(2000);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Axp.SetLDO2(false); // 画面オフ

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_37, 0); // ボタンA
  esp_deep_sleep_start();
}

void loop() {}

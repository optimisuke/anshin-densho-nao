# あんしんでんしょなお

M5StickC Plusのボタンを押すと、家族のAndroidスマホにntfy.sh経由で通知を送るデバイス。子供が帰宅した時に使う。

## 必要なもの

- M5StickC Plus
- 自宅WiFi環境
- Androidスマホ2台（ntfyアプリ）

## スマホのセットアップ

1. Google Playで `ntfy` アプリをインストール（2台とも）
2. トピック `anshindensho-xxxxxx` を購読（secrets.h の NTFY_URL 末尾を参照）

## 開発環境セットアップ

### arduino-cli のインストール

```bash
brew install arduino-cli
```

### ボードとライブラリのセットアップ

```bash
arduino-cli config init
arduino-cli config add board_manager.additional_urls https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json
arduino-cli core update-index
arduino-cli core install m5stack:esp32
arduino-cli lib install "M5StickCPlus"
```

### WiFi設定

`tadaima/secrets.h.example` をコピーして `tadaima/secrets.h` を作成し、WiFi情報を入力：

```bash
cp tadaima/secrets.h.example tadaima/secrets.h
```

```cpp
#define WIFI_SSID "your-ssid"
#define WIFI_PASS "your-password"
#define NTFY_URL  "https://ntfy.sh/anshindensho-k4m9p2"
```

## コンパイル・書き込み

```bash
# コンパイル
arduino-cli compile --fqbn m5stack:esp32:m5stack_stickc_plus tadaima

# ポート確認
arduino-cli board list

# 書き込み（ポートは環境によって異なる）
arduino-cli upload --fqbn m5stack:esp32:m5stack_stickc_plus --port /dev/cu.usbserial-XXXXXXXX tadaima
```

## 動作

- ボタンAを押す → WiFi接続 → 「ただいま」通知送信 → 画面表示 → Deep Sleep
- 成功：緑画面に `:)`
- 失敗：赤画面に `:(` または `NG`
- WiFi接続失敗時は5回リトライ
- Deep Sleep中はほぼ無消費（バッテリー約2ヶ月持つ）

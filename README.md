# あんしんでんしょなお

ボタンを押すと、家族のAndroidスマホにntfy.sh経由で通知を送るデバイス。子供が帰宅した時に使う。

対応デバイス：
- `tadaima/` — M5StickC Plus（画面あり）
- `tadaima-atom/` — M5Atom ATOM-1 / ATOM Lite（LED表示）

## 必要なもの

- M5StickC Plus または M5Atom ATOM-1（ATOM Lite）
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
```

M5StickC Plus の場合：

```bash
arduino-cli lib install "M5StickCPlus"
```

M5Atom ATOM-1 の場合：

```bash
arduino-cli lib install "M5Atom"
arduino-cli lib install "FastLED"
```

### WiFi設定

それぞれのフォルダで `secrets.h.example` をコピーして `secrets.h` を作成し、WiFi情報を入力：

```bash
# M5StickC Plus
cp tadaima/secrets.h.example tadaima/secrets.h

# M5Atom
cp tadaima-atom/secrets.h.example tadaima-atom/secrets.h
```

```cpp
#define WIFI_SSID "your-ssid"
#define WIFI_PASS "your-password"
#define NTFY_URL  "https://ntfy.sh/anshindensho-k4m9p2"
```

## コンパイル・書き込み

### M5StickC Plus

```bash
# コンパイル
arduino-cli compile --fqbn m5stack:esp32:m5stack_stickc_plus tadaima

# ポート確認
arduino-cli board list

# 書き込み（ポートは環境によって異なる）
arduino-cli upload --fqbn m5stack:esp32:m5stack_stickc_plus --port /dev/cu.usbserial-XXXXXXXX tadaima
```

### M5Atom ATOM-1

```bash
# コンパイル
arduino-cli compile --fqbn m5stack:esp32:m5stack_atom tadaima-atom

# ポート確認
arduino-cli board list

# 書き込み（ポートは環境によって異なる）
arduino-cli upload --fqbn m5stack:esp32:m5stack_atom --port /dev/cu.usbserial-XXXXXXXX tadaima-atom
```

## 動作

### M5StickC Plus

- ボタンAを押す → WiFi接続 → 「ただいま！！」通知送信 → 画面表示 → Deep Sleep
- 成功：緑画面に `:)`
- 失敗：赤画面に `:(` または `NG`
- WiFi接続失敗時は5回リトライ
- Deep Sleep中はほぼ無消費（バッテリー約2ヶ月持つ）

### M5Atom ATOM-1

- ボタンを押す → WiFi接続 → 「ただいま！！」通知送信 → LED表示 → Deep Sleep
- 接続中：外周を青い点がぐるぐる回るアニメーション
- 成功：10種類のパターンからランダムで1つ表示（毎回色も変わる）
- 失敗：赤の泣き顔
- WiFi接続失敗時は5回リトライ
- Deep Sleep中はほぼ無消費

#### 成功時パターン一覧

| パターン | 色 |
|---|---|
| 笑顔 | 緑 |
| ハート | マゼンタ |
| 十字星 | 黄 |
| 王冠 | シアン |
| 稲妻 | オレンジ |
| ダイヤ | 青 |
| 花 | 黄緑 |
| 市松模様 | 白 |
| 上矢印 | 水色 |
| フレーム | 琥珀 |

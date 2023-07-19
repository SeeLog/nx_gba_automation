# nx_gba_automation
NX  Macro Controller を使って GBA 自動化する

# 使い方
## 必要なもの
- GBA
- エナメル線など
  - 電気を流せればなんでもいいけど、太い被覆線とかだとかなり邪魔
  - エナメル線を使う場合は被覆を確実に剥がして使いましょう
- ダイオード10個
  - 表面実装のものにするとコンパクトに収まるかも

## マイコンを使って回路を組む
回路図のような感じの回路を頑張って組みます。

ちゃんと基板設計してないので現状だとフレキシブル基板とかで頑張らないといけません。

- マイコンは ProMicro を使っています
  - 他のものでも良いと思います。その場合、ピンアサインなどが変わってくる可能性があるのでプログラム側を書き換えたりしてください。(`#define` を適当に変更すれば動く気がします)
- リセットボタンはつけなくても良いです。
- 電源は外部からとっても良いし、マイコンの 3.3v と GND を繋いでも良いと思います
  - 動作に必要な力が確保できない場合は外部電源を使ってください
  - 外部から電源を取る場合は GBA とマイコンの GND を繋いでください
  - 例えば電池ボックスの負極側とかに繋ぐと良いと思います。
- ダイオードを GBA の TP からマイコンのデジタルピンに向かって電流が流れるように繋げてください

## プログラムを書き込む
Arduino IDE などを使って `nx_gba_controller.ino` を書き込んでください。Platform IO などを使う場合は適宜がんばってください。

# 仕組み
GBA は TP(または各ボタンのパッドの GND ではない方) から GND に向かって電流が流れるようにショートしてやるとボタンを押した判定になります。

よって通常時は各 IO のピン出力を HIGH にして TP の電位を打ち消すようにしておきます。

ボタンを押したいときはピン出力を LOWER にして TP の電位がそのままピンに向かって流れるようにします。
内部で GND に繋がっているので結果的にボタンが押された判定になります。

HIGH にしている際に電流が逆流しないようにダイオードを使っています。
必須かどうかは不明だけど、あったほうが多分安全です。

# 参考
- [CH55xSwitchSerialControl](https://github.com/608/CH55xSwitchSerialControl)
- [Nintendo Switchを操作するプログラムの仕様を調べる](https://scrapbox.io/yatsuna827827-12010999/Nintendo_Switch%E3%82%92%E6%93%8D%E4%BD%9C%E3%81%99%E3%82%8B%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0%E3%81%AE%E4%BB%95%E6%A7%98%E3%82%92%E8%AA%BF%E3%81%B9%E3%82%8B)

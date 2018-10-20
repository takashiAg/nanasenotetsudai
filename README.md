# nanaseのやつのプログラム
------
# 使い方
とりあえず、ESP32にこのプログラムを書き込む
 
### 配線は以下の通り
|ULN2003Aモジュールのコネクタ|ESP32側のピン|
|--|--|
|IN1|16|
|IN2|17|
|IN3|18|
|IN4|19|
|+|5v|
|-|GND|
# プログラムのAPI 的なやつ
### 順方向に回る
```
motor_step(1)
```
### 逆方向に回る
```
motor_step(-1)
```
---
### **hueの現在のステータスを取得する**
JSON形式でとってきてStringで返す関数。パースはしない。
```
String result = get_hue_status()
```
---
### hueの色を変える
resultはアクセスした結果が入ってるけど、あんまりきにしなくていいと思う。一応resultがJSONで帰ってくる。
```
  String result = hue_change_color( id,  hue,  bri,  sat);
```
５番のhueの電球をHSV表色系で(H,S,V)=(2000,100,100)に買えたいときはこんな感じ
```
  String result = hue_change_color( 5,  2000,  100,  100);
```

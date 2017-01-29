# [WIP] VGS Keyboard

## これは何
- VGSの技術 + MIDIキーボードを使った音ゲーを作ろうとしているものです
- まだ作りかけです _(ある程度遊べるようになったらWIPを外します)_

> これはまだ検討中のことですが, 楽曲データの販売でマネタイズしようと思っているので, ゲーム本体はオープンソース＆完全無料で開発する方針です

## License
[3 Clause BSD](LICENSE.txt)

## Requirements 
完成した段階で Windows にも移植するつもりですが, 開発段階では Mac限定 で公開する方針です。（私のメイン環境が Mac なので）
- Mac OSX / mac OS
- XCode
- CocoaPods
- VGS mk-II CLIs
- MIDI keyboard (24 keys)

> MIDI keyboardは24鍵以上のものであれば何でも問題ありませんが, 推奨機種は Korg の [nanoKEY2](http://www.korg.com/jp/products/computergear/nanokey2/) です

## How to build 
```
git clone https://github.com/suzukiplan/vgs-keyboard.git
cd vgs-keyboard
pod update
pod install
cd romdata
make
cd ..
open VGSKeyboard.xcworkspace
```
あとは、XCodeでビルドすれば起動できます。


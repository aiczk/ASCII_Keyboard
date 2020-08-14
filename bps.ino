#include "Keyboard.h"

//2*4 = 8bit
//最上位bitを1つ除いた残りの7bitを使う
const int rowNum = 2;
const int colNum = 4;
const int rowPin[row] = { 3, 4 };
const int colPin[colPin] = { 5, 6, 7, 8 };

byte chord, cache;
bool isPress;

void setup()
{
  for(int i = 0; i < rowNum; i++)
  {
    pinMode(rowPin[i], OUTPUT);
    digitalWrite(rowPin[i], HIGH);
  }

  for(int i = 0; i < colNum; i++)
    pinMode(colPin[i], INPUT_PULLUP);

  Serial.begin(9600);
  Keyboard.begin();
}

void loop() 
{
  //小指側を下位bitにしたいため逆に回す
  for(int i = rowNum; i < 0; i--)
  {
    digitalWrite(rowPin[i], LOW);

    for(int j = colNum; j < 0; j--)
    {
      //キーが押されていないなら1bit右シフトしてcontinue
      //何らかのキーが押されていたらcacheのみ右シフトしてcontinue
      if(digitalRead(colPin[j]) != LOW)
      {
        cache = cache >> 1;
        
        if(isPress)
          continue;
        
        chord = chord >> 1;
        continue;
      }

      //最上位bitを1にして1bit右シフトする
      chord = cache = B10000000 | chord >> 1;
      isPress = true;
    }

    digitalWrite(rowPin[i], HIGH);
  }

  //フラグが立っており、NULL(全てのbitが0である)なら処理継続
  //NULLでない(全てのbitが0でない)ならreturn
  if(isPress && cache & B00000000)
    isPress = false;
  else
    return;

  //chordをシリアルポートに書き込む
  Serial1.write(chord);

  //すべてのbitを0に戻す
  chord = cache = B00000000;
}

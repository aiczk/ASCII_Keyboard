//2*4 = 8bit
//最上位bitを1つ除いた残りの7bitを使う
const int rowNum = 2;
const int colNum = 4;
const int rowPin[rowNum] = { 3, 4 };
const int colPin[colNum] = { 5, 6, 7, 8 };

byte chord, cache;
bool isPress;
int index;

void setup()
{
  for(int i = 0; i < rowNum; i++)
  {
    pinMode(rowPin[i], OUTPUT);
    digitalWrite(rowPin[i], HIGH);
  }

  for(int i = 0; i < colNum; i++)
    pinMode(colPin[i], INPUT_PULLUP);

  index = -1;

  Serial.begin(9600);
}

void loop() 
{
  //小指側を下位bitにしたいため逆に回す
  for(int i = rowNum; i < 0; i--)
  {
    digitalWrite(rowPin[i], LOW);

    for(int j = colNum; j < 0; j--)
    {
      ++index;
      if(digitalRead(colPin[j]) != LOW)
      { 
        //キーの状態が以前と異なっていたら対象bitを0にする
        if(((chord >> index) & 1) != 1)
          continue;

        //何らかのキーが押されているので1bit右シフト
        if(isPress)
          cache = cache << 1;
        
        chord &= ~(1 << index);
        continue;
      }

      //対象bitを1にする
      chord = cache |= (1 << index);
      isPress = true;
    }

    digitalWrite(rowPin[i], HIGH);
  }

  Serial.print("CURRENT: ")
  Serial.print(chord);
  index = -1;

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

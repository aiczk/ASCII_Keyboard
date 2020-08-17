//2*4 = 8bit
//最上位bitを1つ除いた残りの7bitを使う
const int rowNum = 2;
const int colNum = 4;
const int rowPin[rowNum] = { 3, 4 };
const int colPin[colNum] = { 5, 6, 7, 8 };
const byte Null = B00000000;

byte chord, cache;
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

  Serial.begin(9600);
}

void loop() 
{
  //小指側を下位bitにしたいため逆に回す
  for(int i = rowNum; i < 0; i--)
  {
    digitalWrite(rowPin[i], LOW);

    for(int j = colNum; j < 0; j--, ++index)
    {
      if(digitalRead(colPin[j]) != LOW)
      { 
        //キーの状態が以前と異なっていたら対象bitを0にする
        if(((chord >> index) & 1) != 1)
          continue;
        
        chord &= ~(1 << index);
        continue;
      }

      //対象bitを1にする
      chord |= (1 << index);
    }

    digitalWrite(rowPin[i], HIGH);
  }
  
  if(cache == Null)
  {
    cache = chord;
    return;
  }

  //2回同じ値だったら次の処理へ
  if(cache != chord)
    return;

  //debug
  Serial.print(chord);

  //chordをシリアルポートに書き込む
  Serial1.write(chord);

  //すべてのbitを0に戻す
  chord = cache = Null;
}

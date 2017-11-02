#define R1 12
#define Y1 11
#define G1 10  //상하 신호등
#define R2 15
#define Y2 16
#define G2 17 //좌우 신호등

#define TimeY 5 // sec단위로 적고
#define TimeG 10000 //sec*1000 해서 적기
#define TimeS 10

#define buttonA 2

const int col1[8] = { 28,29,26,27,24,25,22,23 };
const int row1[8] = { 38,39,36,37,34,35,32,33 };  //사람
const int col2[8] = { 46,47,44,45,42,43,40,41};
const int row2[8] = { 60,61,62,63,64,65,66,67};   //숫자

int numberd = 0;//??
int timec = 0;  //추가시간?
int bstate = 0; // <보행자신호> 0 = 아무것도 안눌림 1 = 1번 라인일때 2 = 2번 라인일때
int sstate = 0; //  <상하 // 좌우> 신호 구분
int times=0;

int signImage[2][8][8] = {
  { //stop
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 0, 0, 1, 0, 0, 1, 0, 0},
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 0, 0, 1, 1, 1, 1, 0, 0},
    { 0, 1, 0, 1, 1, 0, 1, 0},
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 0, 0, 1, 0, 0, 1, 0, 0},
    { 0, 0, 1, 0, 0, 1, 0, 0}
  },

  { //go
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 0, 0, 1, 0, 0, 1, 0, 0},
    { 0, 0, 0, 1, 1, 0, 0, 0},
    { 1, 0, 1, 1, 1, 1, 1, 0},
    { 0, 1, 0, 1, 1, 0, 0, 1},
    { 0, 0, 1, 0, 0, 1, 0, 0},
    { 0, 1, 0, 0, 0, 0, 1, 0},
    { 0, 0, 1, 0, 0, 0, 0, 1}
  }

};
int walkSignNumber[10][8][4] = {

  { //0
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //1
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 1, 1, 1, 1 }
  },

  { //2
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 0, 0, 1 },
    { 0, 0, 1, 0 },
    { 0, 1, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 1 }
  },

  { //3
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 0, 0, 1 },
    { 0, 1, 1, 1 },
    { 0, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //4
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 },
    { 1, 0, 1, 0 },
    { 1, 1, 1, 1 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 }
  },

  { //5
    { 1, 1, 1, 1 },
    { 1, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 1 },
    { 0, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //6
    { 0, 1, 1, 1 },
    { 1, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //7
    { 1, 1, 1, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 0, 0, 1 },
    { 0, 0, 1, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 }
  },

  { //8
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  },

  { //9
    { 0, 1, 1, 0 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 1 },
    { 0, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 0 }
  }
};
//////////////////////////////////////////////////////////////////

void delay_a(int sec);
bool bCheck();

void setup() {
 for(int i = 0; i<8 ; i++){
  pinMode(col1[i], OUTPUT);
  pinMode(row1[i], OUTPUT);
  pinMode(col2[i], OUTPUT);
  pinMode(row2[i], OUTPUT);
 }
  pinMode(R1, OUTPUT);
  pinMode(Y1, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(Y2, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(buttonA, INPUT_PULLUP); //기본값이 HIGH
}

void loop() {
  sstate = 1;                     // 1번라인
  digitalWrite(R2, HIGH);
  digitalWrite(R1, LOW);
  digitalWrite(G1, HIGH);
  delay_a(TimeG);         
  digitalWrite(G1, LOW);
  if(bstate == 0){                 //보행자 신호를 입력 받지않았을 경우
    for(int i = 0; i<TimeY; i++){
     digitalWrite(Y1, HIGH);
     delay_a(500);
     digitalWrite(Y1, LOW);
     delay_a(500);
   }
  }else if(bstate == 1){    //보행자 신호를 입력 받았을 경우 스위치가 눌렸을 경우 bstate=1;
    for(int i = 0; i<3; i++){
     digitalWrite(Y1, HIGH);            //이 부분이 필요가 없는거 같은데
     delay_a(500);
     digitalWrite(Y1, LOW);
     delay_a(500);
   }
   bstate = 0;
  }
  digitalWrite(R1, HIGH);
  //전환
  sstate = 2; // 2번라인             
  numberd = 10;                     //이게 숫자 신호등의 시간인거 같아
  digitalWrite(R2, LOW);
  digitalWrite(G2, HIGH);
  delay_a(TimeG);
  if(bstate == 2){                  //버튼이 눌러진 상태일경우
    numberd = 10;                   //돌아가는 딜레이네
    delay_a(TimeG);                 //이게 리셋이구나
  }
  digitalWrite(G2, LOW);
  
  if(bstate == 0){
   for(int i = 0; i<TimeY; i++){     //즉 i 하나에 1초
      digitalWrite(Y2, HIGH);
      delay_b(500);
      digitalWrite(Y2, LOW);
      delay_b(500);
    }
   }
}
void delay_a(int sec){        //검사 딜레이
  int k = 30;
  for(int i = sec / k ; i>0 ; i--){
    if(sstate == 1){            //상하 신호가 초록불일 때
      printSignalMatrix(0);     //정지 신호등 (좌우) 
      cMat2();                  //숫자 신호등 끄기
    }
    else if(sstate == 2){       //좌우 신호가 초록불일 때
      printSignalMatrix(1);     //가는 신호등
      printNumMatrix(numberd);  //남은 시간 신호
      
      if(timec <1000) timec += k;   //지나간 시간 계산해 주는 부분
      else if(timec > 999){         //10000 보다 클 경우 잖아
       timec = 0;                   //다 돌았다고 생각하는 거니깐 time을 리셋 해주고  
       if(numberd == 0) numberd = 0;//이게 머지
       else numberd -= 1;
      }
    }
    delay(k);
    if(bstate == 0){        //스위치 입력이 안 들어와 있는 경우 스위치 입력 들어왔는지 검사
      bool a = bCheck();    //입력이 있다면 true 값 반환 후 -> a
     if(a == true){       
        return;
     }
   }
  }
}
void delay_b(int sec){        //검사안하는 딜레이
  int k = 30;
  for(int i = sec / k ; i>0 ; i--){
    if(sstate == 1){            //상하 신호가 초록불일 때
      printSignalMatrix(0);     //정지 신호등 (좌우) 
      cMat2();                  //숫자 신호등 끄기
    }
    else if(sstate == 2){       //좌우 신호가 초록불일 때
      printSignalMatrix(1);     //가는 신호등
      printNumMatrix(numberd);  //남은 시간 신호
      
      if(timec <1000) timec += k;   //지나간 시간 계산해 주는 부분
      else if(timec > 999+times){   //10000 보다 클 경우 잖아
       timec = 0;                   //다 돌았다고 생각하는 거니깐 time을 리셋 해주고  
       if(numberd == 0) numberd = 0;
       else numberd -= 1;
       bstate=0;
      }
    }
    delay(k);
   times=0;
  }
}


bool bCheck(){
  int a = digitalRead(buttonA);

  if(a == LOW){         //스위치 입력 받았을 경우
    if(sstate == 1){    //1번 라인 실행중일 경우   근데 사실 상 1번라인 실행 중에 스위치가?
      bstate = 1;
      delay(TimeS);
     return true;
    }else if(sstate == 2){  //2번 라인 실행중일 경우
      bstate = 2;
      return true;
    }
  }
  return false;
}

void cMat(){  //1번 다키는거?
  for(int i= 0; i<8; i++){
    digitalWrite(col1[i], HIGH);
    digitalWrite(row1[i], LOW);
  }
}

void cMat2(){ //2번
  for(int i= 0; i<8; i++){
    digitalWrite(col2[i], HIGH);
    digitalWrite(row2[i], LOW);
  }
}


void printSignalMatrix(int num) {           //정지(0)인지 통행 가능(1)인지 아이콘 출력
  for (int r = 0; r < 8; r++) {
    cMat();                                 //다꺼주는거
    digitalWrite(row1[r], HIGH);            //row를 high로 바꾼 뒤
    for (int c = 0; c < 8; ++c) {
      if (signImage[num][r][c]) digitalWrite(col1[c], LOW);  //키고 싶은 부분만 col을 low로 바꾸네
    }
    delay(1);
    cMat();
  }
}
void printNumMatrix(int num) { //남은 시간 출력
  for (int r = 0; r < 8; r++) {
    digitalWrite(row2[r], HIGH);
    for (int c = 0; c < 4; ++c) { //10의 자리
      if (walkSignNumber[num / 10][r][c]) digitalWrite(col2[c], LOW);
    }
    for (int c = 0; c < 4; ++c) { //1의 자리
      if (walkSignNumber[num % 10][r][c]) digitalWrite(col2[c + 4], LOW);
    }
    delay(1); //시간지연을 조금이라도 줘야 도트가 깨끗하게 보임
    cMat2();
  }
}

/*void delay_b(int sec){
  int k = 30;
    if(sstate == 1){            
        for(int i = sec / k ; i> (sec / k)/3 ; i--){
             printSignalMatrix(0);     //정지 신호등 (좌우) 
             cMat2();                  //숫자 신호등 끄기
             if(timec <1000) timec += k;   //지나간 시간 계산해 주는 부분
             else if(timec > 999){         //10000 보다 클 경우 잖아
             timec = 0;                   //다 돌았다고 생각하는 거니깐 time을 리셋 해주고  
             if(numberd == 0) numberd = 0;//이게 머지
             else numberd -= 1;
               delay(k);
               if(bstate == 0){        //스위치 입력이 안 들어와 있는 경우 스위치 입력 들어왔는지 검사
                  bool a = bCheck();    //입력이 있다면 true 값 반환 후 -> a
                  if(a == true)  return;    
               }
        }
    }
    else if(sstate == 2){
      for(int i = sec / k ; i>0 ; i--){
             printSignalMatrix(0);     //정지 신호등 (좌우) 
             cMat2();                  //숫자 신호등 끄기
             delay(k);
             if(bstate == 0){        //스위치 입력이 안 들어와 있는 경우 스위치 입력 들어왔는지 검사
                bool a = bCheck();    //입력이 있다면 true 값 반환 후 -> a
                if(a == true){         
                    return;
                }   
             }
        }
    }
}*/
  

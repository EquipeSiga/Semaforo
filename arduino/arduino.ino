const int PIN = 9;
const int TURN_RED = LOW;
const int TURN_GREEN = HIGH;

const char RED = 'r';
const char GREEN = 'g';
const char UNFORTUNATELY_RED = 'R';
const char FINALLY_GREEN = 'G';

const int SOUND_PIN = 13;

#define OCTAVE_OFFSET 0
#define isdigit(n) (n >= '0' && n <= '9')

// These values can also be found as constants in the Tone library (Tone.h)
int notes[] = { 0,
262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
};

char *finallyGreenSong = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
char *unfortunatelyRedSong = "policeala:d=4,o=6,b=120:a5,f5,a5,f5,a5,f5,a5,f5,a5,f5,a5,f5,a5,f5,a5,f5,a5,f5,a5,f5,a5";

//"StarwarsI:d=16,o=5,b=100:4e,4e,4e,8c,p,g,4e,8c,p,g,4e,4p,4b,4b,4b,8c6,p,g,4d#,8c,p,g, 4e,8p";

void play_rtttl(char *p)
{
  // Absolutely no error checking in here
  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)
  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  //Serial.print("ddur: "); Serial.println(default_dur, 10);

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  //Serial.print("doct: "); Serial.println(default_oct, 10);

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  //Serial.print("bpm: "); Serial.println(bpm, 10);

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  //Serial.print("wn: "); Serial.println(wholenote, 10);


  // now begin note loop
  while(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

    if(note)
    {
      //Serial.print("Playing: ");
      //Serial.print(scale, 10); Serial.print(' ');
      //Serial.print(note, 10); Serial.print(" (");
      //Serial.print(notes[(scale - 4) * 12 + note], 10);
      //Serial.print(") ");
      //Serial.println(duration, 10);
      tone(SOUND_PIN, notes[(scale - 4) * 12 + note]);
      delay(duration);
      noTone(SOUND_PIN);
    }
    else
    {
      //Serial.print("Pausing: ");
      //Serial.println(duration, 10);
      delay(duration);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(PIN, OUTPUT);
  Serial.print('x');
}

void loop()
{
  if (Serial.available())
  {
    char state = (char) Serial.read(); 
    if (state == RED)
    {
      digitalWrite(PIN, TURN_RED);
      
    }
    else if (state == GREEN)
    {
      digitalWrite(PIN, TURN_GREEN);
    }
    else if (state == FINALLY_GREEN)
    {
      play_rtttl(finallyGreenSong);
      delay(100);
      play_rtttl(finallyGreenSong);
      delay(100);
    }
    else if (state == UNFORTUNATELY_RED)
    {
      play_rtttl(unfortunatelyRedSong);
      delay(100);
      play_rtttl(unfortunatelyRedSong);
      delay(100);
    }
  }
}

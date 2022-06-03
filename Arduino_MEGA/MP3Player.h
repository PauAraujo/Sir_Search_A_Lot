#include <DFRobotDFPlayerMini.h>
DFRobotDFPlayerMini useMP3Player;
SoftwareSerial MP3PlayerSerial(18, 19); // RX, TX

#define MP3_PLAYING_PIN 52

// Detect whether a mp3 is currently playing
boolean isPlayingMP3(){
  return digitalRead(MP3_PLAYING_PIN) == 0;
}

boolean playTrackWithDelay(void* trackToPlay){
  useMP3Player.play(trackToPlay);
  return false;
}

/**
 * 1 = R2D2 sound
 * 2 = Let's go!
 * 3 = Don't forget, time is ticking.
 * 4 = Running out of time!
 * 5 = 10 seconds left!
 * 6 = Losing sound
 * 7 = We didn't lose the game, we just ran out of time.
 * 8 = Victory sound
 * 9 = We did it!
 * 10 = Dance music
 * 11 = Help me, i am stuck.
 * 12 = Oh no, where do i go now?
 * 13 = Time for a new round. Give me directions when you are ready.
 * 14 = I can't hear you, please come closer.
 */
void playMP3(int track, int timeDelay = 0){
  // MP3 somehow starts playing file 8 if we pass track 1 for the first 13 files
  // Multiple DF player libraries have been tried, none of them work correctly.
  // After some research it seems that we have a chinese replica chip which can act odd
  // To prevent waisting more time, i added some silly logic to deal with it.
  
  int trackToPlay = track;
  if(track <= 13){
    int startsPlayingAt = 8;
    if(track < startsPlayingAt){
      trackToPlay = track + (13 - startsPlayingAt) + 1; 
    }else{
      trackToPlay = track - startsPlayingAt + 1;
    }
  }

  if(timeDelay > 0){
    // play track in x amount of miliseconds
    timer.in(timeDelay, playTrackWithDelay, trackToPlay);
    return;
  }

  useMP3Player.play(trackToPlay);
}

void mp3PlayerSetup()
{
  pinMode(MP3_PLAYING_PIN, INPUT);

  MP3PlayerSerial.begin(9600);
  useMP3Player.begin(MP3PlayerSerial, true);

  // Give the player a moment to connect
  delay(1000);
  useMP3Player.volume(26); // Set volume
  playMP3(1);  //Play the first mp3
}

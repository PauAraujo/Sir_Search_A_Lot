# Sir_Search_A_Lot

The robot agent, Sir_Search_A_Lot (SSAL, for short), is an autonomous car that needs to find a red parking spot located within the environment. The player, a human, can help the agent to find the parking spot using voice commands. The environment has a count-down timer and LED lights that change color depending on how much time is left. The goal of the game is for Sir_Search_A_Lot to find a parking spot before the time is up (see appendix , video 1). 

SSAL is autonomous in his ability to navigate the environment without getting hurt. He has obstacle avoidance capabilities which enable him to determine when something is blocking his path. SSAL was devised as a behavior-based agent with a reactive controller. A reactive system was seen as an appropriate choice given the time-dependent nature of the game. SSAL’s priorities of action can be represented in five layers: 


<img src="https://github.com/PauAraujo/Sir_Search_A_Lot/blob/master/other/Behavior-based%20architecture%20of%20SSAL.png?raw=true" width="500">

Figure 1. Behavior-based architecture of SSAL

The actions within the lower layers take precedence over those at the higher layers. For instance, the agent will opt to avoid an obstacle than drive into a wall. In practice, this means that SSAL always listens to human voice commands, as long as he does not hurt himself in the process. 

The social presence of the agent was heightened by simulating emotions of insecurity and joy. When the robot faces failure after repeatedly missing his target, he will pause for a few seconds and move his head left and right to evaluate the situation. This is a behavioral cue for confusion and insecurity. SSAL will then verbally ask for help. Conversely, if the agent successfully reaches his target, he will display emotions of joy. SSAL celebrates victory with a happy dance, which is rhythmically aligned to a song played through his speaker (appendix, video 3). SSAL will then verbally communicate his excitement by announcing to the world “We did it!”. Victory is reflected in the LED strip, which lights up displaying rainbow colors (see appendix, image 2). This wireless interaction exemplifies this original IoT concept I had in mind, where the actions of the agent have a direct effect on the environment. If SSAL had otherwise lost, the LED lights would have turned red (see appendix, image 3).. Thus, the environmental lights visually express the emotional state of the agent. Another wireless element interacting with the robot is the count-down timer. Sir_Search_A_Lot displays an understanding of time constraints. The less time is left, the more desperate he grows.

The agent can effectively gather knowledge from his surroundings owing to his multimodal nature. Indeed, SSAL has multiple sensors that enable him to take in different forms of information. One such sensor is the color detection module mounted on his underbelly. When SSAL encounters the color red, he very well knows it. We may call this a gut feeling. 

I sought ways to personify the agent, to increase his social presence. One such addition was his humorous propeller hat (see appendix, image 1), which stands in stark contrast with his mature British accent. I found that most people are enlivened by his whimsical appearance. SSAL’s endearing qualities give the illusion that he is a fully conscious being with a personality of his own. This anthropomorphization of the agent serves to increase the presence of the human player in the game. Another function of the propeller hat was to keep track of SSAL’s battery levels. We can determine how tired Sir_Search_A_Lot is by observing how fast his propeller swirls. 

# Practical implementation 
SSAL’s head is mounted on a servo motor representing his neck. When encountering an obstacle, SSAL will look left and right. If there are obstacles on both sides, then Sir_Search_A_Lot will drive backward. The agent detects these obstacles with his ultrasonic sensor, which was programmed to work within a 30 cm range. His mouth is displayed on an OLED screen, and it animates when the robot is speaking (appendix A, assets 1). The robot’s voice is broadcasted from a speaker mounted nearby his mouth. The speaker connects to a DFPlayer mini-module which holds a micro-SD card containing all mp3 audio files. Table 1 shows the pre-recorded lines SSAL speaks under different conditions. 

The agent receives voice commands through an Android application called AMR_Voice that works with Google’s automatic speech recognition software (appendix, video 2). Nine nine voice commands were implemented: go, drive, left, right, forward, backward, reverse, stop, and dance. 

The color detection module has a LED light built-in, which can create variation in the way a color is detected. Therefore, manual testing was done to determine an optimal range of RGB values that would correspond to the color hue of the parking lot. The agent detects the color red if the following conditions are met: red channel intensity is greater than 200, green channel intensity is lower than 40, and blue channel intensity is lower than 40. 

SSAL’s conception of time constraints comes from his Bluetooth LE module which connects wirelessly to the timer. The timer and the lights are mounted to a dual-core ESP-WROOM-32 module. I placed another Bluetooth LE on one core and the LED lights and timer on the other. This setup was essential to keep one module from interrupting the processes of the other. The timer displays in an 8x8 LED matrix, which was programmed to do a count down from 30 to 0 seconds. If the timer did not yet connect to the agent, a square dot shows up at the lower-right corner of the LED matrix. This dot is there for error prevention (see appendix, image 4). 

# Appendix

Video 1:

https://vimeo.com/716814306

Video 2:

https://vimeo.com/716817045

Video 3:

https://vimeo.com/716835842

Note: in the video the robot’s mouth looks glitchy due to difference between the number of frames per second of the camera and the frames per second (refresh rate) of the OLED screen. In real life, this glitch is not there.

Image 1: 

<img src="https://github.com/PauAraujo/Sir_Search_A_Lot/blob/master/other/image_1.png?raw=true" width="500">

Image 2:

<img src="https://github.com/PauAraujo/Sir_Search_A_Lot/blob/master/other/image_2.png?raw=true" width="500">

Image 3:

<img src="https://github.com/PauAraujo/Sir_Search_A_Lot/blob/master/other/image_3.png?raw=true" width="500">

Image 4:

<img src="https://github.com/PauAraujo/Sir_Search_A_Lot/blob/master/other/image_4.png?raw=true" width="500">

Table 1.
This table explains the different situations for when a certain mp3 file is triggered to play.

| #  | Type of sound | Description of when it is played |
|----|---------------|-----------------------------------|
| 1  | **Startup sound** | When the robot is turned on |
| 2  | Let's go!  | After the command “Go”, which starts the game |
| 3  | Don't forget, time is ticking.  | 20 seconds left |
| 4  | Running out of time! | 15 seconds left |
| 5  | 10 seconds left! | 10 seconds left |
| 6  | **Losing sound** | 7 seconds left |
| 7  | We didn't lose the game, we just run out of time. | When robot lost, and is in denial |
| 8  | ** Victory sound** | When the robot wins |
| 9  | We did it! | When the robot wins |
| 10 | ** Dance music** | When the robot wins |
| 11 | Help me, I am stuck. | When the robot is voice controlled, and he encounters an obstacle where he is able to go either left and right, he will ask to be given a direction. If no advice is given within 10, the agent will choose a random direction. |
| 12 | Oh no, where do I go now? | When the robot is self-driving in a wandering manner after being given the “drive” command, it will ask for human help every 60 seconds. |
| 13 | Time for a new round. Give me directions when you are ready. | When the game has finished (when the timer is either frozen or down to 0). |
| 14 | I can't hear you, please come closer. | When the robot’s Bluetooth Classic gets disconnected loses connection to the phone app. |

Pinout diagrams

<img src="https://github.com/PauAraujo/Sir_Search_A_Lot/blob/master/other/Arduino%20Mega.png?raw=true" width="500">

<img src="https://github.com/PauAraujo/Sir_Search_A_Lot/blob/master/other/ESP32-WROOM.png?raw=true" width="500">



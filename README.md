# LED Candle
An arduino led candle that flickers when something goes near it. This project is inspired by My New Flame by Moritz Waldemeyer for Ingo Maurer. Technically, this project is adapted from Phillip Burgess/paintyourdragon's Animated Flame Pendant:
https://learn.adafruit.com/animated-flame-pendant.


## Parts
- Pro Trinket microcontroller (adafruit.com/product/2010 or 2000) (#2010 = 3V/12MHz for longest battery life, but 5V/16MHz works OK)
- Charlieplex LED Matrix Driver (Adafruit product id: 2946)
- Charlieplex LED Matrix (Adafruit product id: 2947, 2948, 2972, 2973 or 2974)
- 350 mAh LiPoly battery (Adafruit product id: 2750)
- LiPoly backpack (Adafruit product id: 2124)
- SPDT Slide Switch (Adafruit product id: 805)
- Ultrasonic Distance Sensor (Adafruit product id: 4007)


## Prerequisites
- Read through Phillip Burgess' Animated Flame Pendant docs: https://learn.adafruit.com/animated-flame-pendant
- Read through the Ultrasonic Distance Sensor docs: https://learn.adafruit.com/ultrasonic-sonar-distance-sensors
- Have the Arduino IDE installed on your computer
- Have the parts listed in the Parts section, plus wire, solder, flux & soldering iron
- If you want to generate your own animations, have Python & FFmpeg installed on your computer (the leading multimedia framework): https://ffmpeg.org/


## How to Install Software

Clone this repo
`git clone https://github.com/snphillips/led-candle.git`

TODO: fill out this section

## Generate Your Own Animations
This project requires two short animations. If you don't like mine you can make your own.
_You'll need python(https://www.python.org/about/gettingstarted/) & FFmeg(https://ffmpeg.org/) installed on your computer._
1) Take a verticle video with you phone then airdrop to your computer.
2) Edit the video to your liking. Remove sound track, make greyscale, crop it. Keep in mind we have limited space on the small Pro Trinket. Your final animations can't be longer than about 15 seconds total.
3) Export the two source videos you'd like to turn into animations. Use a low resolution as you'll eventually be resizing each frame to 9px x 16px.
4) Place each video in their own folders named **flame-normal-source-mp4** and **flame-flicker-source-mp4**
5) Name the normal flame video `flame-normal-source.mp4`, and name the flicker flame video `flame-flicker-source.mp4`
6) Rotate the videos in the finder so they're vertical
7) Navigate into the **flame-normal-source-mp4** folder. Using the `ffmpeg` library convert video to pngs (30 frames/second)

```
ffmpeg -i flame-normal-source.mp4 -r 30/1 $flame-normal%03d.png
```

8) Navigate into the flame-flicker folder. Using the `ffmpeg` library convert video to pngs (30 frames/second)

```
ffmpeg -i flame-flicker-source.mp4 -r 30/1 $flame-flicker%03d.png
```

9) Our LED matrix is only 9 pixels by 16 pixels so we need to resize the images to by tiny 9x16 using ffmpeg (TODO: insert ffmpeg instructions) or this online tool: https://bulkresizephotos.com/.
10) Remove the full size pngs from the flame-normal & flame-flicker folders so only the tiny pngs are left. You can discard them or set them aside for safe keeping.
11) Navigate into the flame-normal folder. Run the following python script to generate an h file. If you see a file called `data-flame-normal.h` in the folder, the script worked.

```
python3 convert-flame-normal.py *.png > data-flame-normal.h
```

12) Navigate into the flame-flicker folder. Run the following python script to generate an h file. If you see a file called `data-flame-flicker.h` in the folder, the script worked.

```
python3 convert-flame-flicker.py *.png > data-flame-flicker.h
```

13) Move the two h files `data-flame-normal.h` & `data-flame-flicker.h` into the root of the project folder: led-candle.
14) Confirm that the project folder contains the three files needed to upload to the Adafruit Pro Trinket: `led-candle.ino`, `data-flame-normal.h`, `data-flame-flicker.h`.



## Hardware
TODO: write this section

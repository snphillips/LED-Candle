# LED Candle
An arduino led candle that flickers when someone goes near it. This project is adapted from Phillip Burgess/paintyourdragon's Animated Flame Pendant:
https://learn.adafruit.com/animated-flame-pendant

## Parts
- Pro Trinket microcontroller (adafruit.com/product/2010 or 2000) (#2010 = 3V/12MHz for longest battery life, but 5V/16MHz works OK)
- Charlieplex LED Matrix Driver (Adafruit product id: 2946)
- Charlieplex LED Matrix (Adafruit product id: 2947, 2948, 2972, 2973 or 2974)
- 350 mAh LiPoly battery (Adafruit product id: 2750)
- LiPoly backpack (Adafruit product id: 2124)
- SPDT Slide Switch (Adafruit product id: 805)
- Ultrasonic Distance Sensor (Adafruit product id: 4007)


## Getting Started
TODO


## Generating Your Own Animations
1) take a video with you phone then airdrop to your computer
2) edit video a bit (make greyscale, crop)
4) export the two source videos you'd like to turn into animations (low resolution, low settings)
5) name the normal flame video `flame-normal`, and name the flicker flame video `flame-flicker`
6) rotate the videos in the finder so they're vertical
7) using the `ffmpeg` library convert video to pngs (30 frames/second)
```
ffmpeg -i flame-normal.mp4 -r 30/1 $flamenormal%03d.png
```
```
ffmpeg -i flame-flicker.mp4 -r 30/1 $flameflicker%03d.png
```
8) resize photos to 9x16 using ffmpeg (TODO: insert ffmpeg instructions) or this online tool: https://bulkresizephotos.com/


## Hardware
TODO

# LED Proximity Sensing Flickering Candle
An arduino led candle that flickers when something goes near it. This project is inspired by My [New Flame](https://www.ingo-maurer.com/en/products/my-new-flame/) by Moritz Waldemeyer for Ingo Maurer. Technically, this project is adapted from Phillip Burgess/paintyourdragon's [Animated Flame Pendant](https://learn.adafruit.com/animated-flame-pendant).


## Prerequisites
- Read through Phillip Burgess' Animated Flame Pendant [Animated Flame Pendant docs](https://learn.adafruit.com/animated-flame-pendant).
- Read through the [Ultrasonic Distance Sensor docs](https://learn.adafruit.com/ultrasonic-sonar-distance-sensors)
- Have the [Arduino IDE](https://www.arduino.cc/en/software) installed on your computer
- Have the parts listed in the Parts section below, plus wire, solder, flux & soldering iron
- If you want to generate your own animations, have Python & FFmpeg installed on your computer (the leading multimedia framework): https://ffmpeg.org/


## How to Install Software onto Pro Trinket

First, install the [Arduino IDE](https://www.arduino.cc/en/software).

Clone this repo:

`git clone https://github.com/snphillips/led-candle.git`

Connect the Pro Trinket into your computer. The instructions below assume you connected it using USB.

Using the Arduino IDE, open `led-candle.ino`.

Select the Pro Trinket 5V/16MHz (USB) or Pro Trinket 3V/12MHz (USB) board from the Tools->Board menu.
Next go into the Tools -> Programmer menu and select the USBtinyISP programmer.

Upload `led-candle.ino` to the Pro Trinket board by pressing the tiny button on the Pro Trinket. Within eight seconds, select Sketch > Upload Using Programmer. Wait until you see the message "Done uploading". 
If your code uploaded successfully, at the end of the output message you'll see a paragraph like this:

`Sketch uses 21632 bytes (75%) of program storage space. Maximum is 28672 bytes.
Global variables use 506 bytes of dynamic memory.`

Nothing will happen yet, but now the code is on your Pro Trinket.

## How to Assemble Hardware
The big picutre is to follow the directions detailed in Phillip Burgess' Animated Flame Pendant docs, minus the section about 3D printing. Then, add in the ultrasonic distance sensor.

TODO: flush out this section further


## How to Generate Your Own Animations
This project requires two short animations. If you don't like mine you can make your own.
_You'll need [python](https://www.python.org/about/gettingstarted/) & [FFmpeg](https://ffmpeg.org/) installed on your computer._
1) Take a verticle video with you phone then airdrop to your computer.
2) Edit the video to your liking. Remove sound track, adjust contrast, crop it. Keep in mind we have limited space on the small Pro Trinket. Your final animations can't be longer than about 15 seconds total. I use the mac's iMovie for this step.
3) Export the two source videos you'd like to turn into animations. Use a low resolution as you'll eventually be resizing each frame to 9px x 16px.
4) Place each video in their own folders named **flame-normal-source-mp4** and **flame-flicker-source-mp4**
5) Name the normal flame video `flame-normal-source.mp4`, and name the flicker flame video `flame-flicker-source.mp4`

It's now time to use `FFmpeg` to further edit the video more and create PNGs. Navigate into the **flame-flicker-source-mp4** folder. 

```
cd flame-normal-source-mp4
```

6) Using the command line, use `FFmpeg` to make the video grayscale and rotate the videos so they're vertical. I also had to make my video upside down so that it was oriented properly when assembled.

This command will take the input file `flame-flicker-source.mp4`, apply the greyscale effect using the colorspace filter, and then rotate the video 180 degrees using the transpose filter. The output will be saved as `flame-flicker-grayscale-rotate.mp4`.


```
ffmpeg -i flame-flicker-source.mp4 -vf "colorspace=gray,transpose=2,transpose=2" flame-flicker-grayscale-rotate.mp4
```


7) Our LED matrix is only 9 pixels by 16 pixels so we need to resize the video to have tiny dimensions of 9x16 using `FFmpeg`. The following command scales the video to an aspect ratio of 9:16 while maintaining even dimensions and then adds padding to bring the output dimensions to 9x16. The shortest=1 option tells the pad filter to use the shortest input duration, which should prevent the output from being all black.


```
ffmpeg -i flame-flicker-grayscale-rotate.mp4 -vf "scale=w=9:h=16:force_original_aspect_ratio=decrease,pad=9:16:(ow-iw)/2:(oh-ih)/2:color=black:shortest=1" -aspect 9:16 flame-flicker-resized.mp4
```

8) Using the `FFmpeg` library convert video to pngs (30 frames/second). Explanation of the following command:

`-i flame-flicker-resized.mp4`: specifies the input video file name.
`-r 30`: specifies the frame rate of the output PNG images.
`flame-flicker_%03d.png`: specifies the naming convention of the output PNG files. %03d is a placeholder that will be replaced by a three-digit number that represents the frame number.
This command will extract the frames from the video and save them as PNG images with names like flame-flicker_001.png, flame-flicker_002.png, etc. in the same directory as the input video file.Explanation of the command:


```
ffmpeg -i flame-flicker-resized.mp4 -r 30 flame-flicker_%03d.png

```

8) Navigate into the flame-flicker folder. Using the `ffmpeg` library convert video to pngs (30 frames/second)

```
cd ..
cd flame-flicker-source-mp4
ffmpeg -i flame-flicker-source.mp4 -r 30/1 $flameflicker%03d.png
```

9) Our LED matrix is only 9 pixels by 16 pixels so we need to resize the images to by tiny 9x16 using ffmpeg (TODO: insert ffmpeg instructions on shrinking video before breaking the images into pngs) or this online tool: https://bulkresizephotos.com/.
10) Remove the full size pngs from the flame-normal & flame-flicker folders so only the tiny pngs are left. You can discard them or set them aside for safe keeping.
11) Navigate into the flame-normal folder. Run the following python script to generate an h file. If you see a file called `data-flame-normal.h` in the folder, the script worked.

```
cd flame-normal-source-pngs
python3 convert-flame-normal.py *.png > data-flame-normal.h
```

12) Navigate into the flame-flicker folder. Run the following python script to generate an h file. If you see a file called `data-flame-flicker.h` in the folder, the script worked.

```
cd ..
cd flame-flicker-source-pngs
python3 convert-flame-flicker.py *.png > data-flame-flicker.h
```

13) Move the two `h` files `data-flame-normal.h` & `data-flame-flicker.h` into the root of the project folder: led-candle.
14) Confirm that the project folder contains the three files needed to upload to the Adafruit Pro Trinket: `led-candle.ino`, `data-flame-normal.h`, `data-flame-flicker.h`.


## Parts
- Pro Trinket microcontroller [2010](https://www.adafruit.com/product/2010) or [2000](https://www.adafruit.com/product/2000)) (2010 = 3V/12MHz for longest battery life, but 5V/16MHz works OK)
- Charlieplex LED Matrix Driver (Adafruit product id: [2946](https://www.adafruit.com/product/2946))
- Charlieplex LED Matrix (Adafruit product id: 2947, [2948](https://www.adafruit.com/product/2948), 2972, 2973 or 2974)
- 350 mAh LiPoly battery (Adafruit product id: [2750](https://www.adafruit.com/product/2750))
- LiPoly backpack (Adafruit product id: [2124](https://www.adafruit.com/product/2224))
- On-Off Power Button / Pushbutton Toggle Switch (Adafruit product id: [1683](https://www.adafruit.com/product/1683))
- Ultrasonic Distance Sensor (Adafruit product id: [4007](https://www.adafruit.com/product/4007)

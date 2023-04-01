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

Select the Pro Trinket 5V/16MHz (USB) or Pro Trinket 3V/12MHz (USB) board (depending on which version you have) from the Tools->Board menu.
Next go into the Tools -> Programmer menu and select the USBtinyISP programmer.

Upload `led-candle.ino` to the Pro Trinket board by pressing the tiny button on the Pro Trinket. Within eight seconds, select Sketch > Upload Using Programmer. Wait until you see the message "Done uploading". 
If your code uploaded successfully, at the end of the output message you'll see a paragraph like this:


`Sketch uses 21632 bytes (75%) of program storage space. Maximum is 28672 bytes.
Global variables use 506 bytes of dynamic memory.`


Nothing will happen yet, but now the code is on your Pro Trinket.

## How to Assemble Hardware
The big picture is to follow the directions detailed in Phillip Burgess' Animated Flame Pendant docs, minus the section about 3D printing. Then, add in the ultrasonic distance sensor.

TODO: flush out this section further


## How to Generate Your Own Animations
This project requires two short animations. If you don't like mine you can make your own.

You'll need [python](https://www.python.org/about/gettingstarted/) & [FFmpeg](https://ffmpeg.org/) installed on your computer._

- Take a vertical video with you phone then airdrop to your computer.
- Edit the video to your liking. Remove sound track, adjust contrast, crop it. Keep in mind we have limited space on the small Pro Trinket. Your final animations can't be longer than about 15 seconds total. I use the mac's iMovie for this step.
- Export the two source videos you'd like to turn into animations. Use a low resolution as you'll eventually be resizing each frame to 9px x 16px.
- Place each video in their own folders named **flame-normal-source-mp4** and **flame-flicker-source-mp4**
- Name the normal flame video `flame-normal-source.mp4`, and name the flicker flame video `flame-flicker-source.mp4`
- We're going to use `FFmpeg` to further edit the video more and create PNGs. Navigate into the **flame-flicker-source-mp4** folder. 


```
cd flame-flicker-source-mp4
```

- Using the command line, use `FFmpeg` to make the video grayscale with the following command:

```
ffmpeg -i "flame-flicker-source.mp4" -vf "format=gray" "flame-flicker-grayscale.mp4"
```


- Now we need to rotate the video so it's upside down so that it is oriented properly when assembled. This command takes the input file "flame-flicker-grayscale.mp4", applies the transpose filter twice (which rotates the video 90 degrees each time), resulting in a 180-degree rotation, and saves the output as "flame-flicker-rotated.mp4".


```
ffmpeg -i flame-flicker-grayscale.mp4 -vf "transpose=2,transpose=2" flame-flicker-rotated.mp4
```


- Our LED matrix is only 9 pixels by 16 pixels so we need to resize the video to have tiny dimensions of 9x16. The following command takes the input file "flame-flicker-rotated.mp4", applies the scale filter with the specified dimensions (9x16 pixels), sets the video codec to libvpx-vp9, specifies a bitrate of 1M, sets the Constant Rate Factor (CRF) to 31 for quality, uses 4 threads for encoding, and saves the output as "flame-flicker-9x16.webm". Note that the output file format is changed to .webm since VP9 is not typically used with .mp4 containers.


```
ffmpeg -i flame-flicker-rotated.mp4 -vf "scale=9:16" -c:v libvpx-vp9 -b:v 1M -crf 31 -threads 4 flame-flicker-9x16.webm
```


- Convert video to pngs (30 frames/second). The following command takes the input file "flame-flicker-9x16.webm", applies the fps filter to specify 30 frames per second, and saves the output as a series of PNG images with the naming pattern "flicker-frame-001.png", "flicker-frame-002.png", and so on. The %03d in the output file name is a placeholder for the frame number, which will be zero-padded to 3 digits (e.g., 001, 002, 003, etc.).

```
 ffmpeg -i flame-flicker-9x16.webm -vf fps=30 flicker-frame-%03d.png
```


- Move the tiny pngs into the folder **flame-flicker-source-pngs**.

- Navigate into the **flame-flicker-source-pngs** folder then run the following python script to generate an `h` file. After you've run the script, if you see a file called `data-flame-flicker.h` in the folder, the script worked.

```
python3 convert-flicker-flicker.py *.png > data-flame-flicker.h
```

- Move the `h` file `data-flame-flicker.h` into the root of the project folder: led-candle.


- Repeat the above steps for the normal video **flame-normal-source.mp4**. Here are all the comands with _flicker_ replaced with _normal_:

```
ffmpeg -i "flame-normal-source.mp4" -vf "format=gray" "flame-normal-grayscale.mp4"
```
```
ffmpeg -i flame-normal-grayscale.mp4 -vf "transpose=2,transpose=2" flame-normal-rotated.mp4
```
```
ffmpeg -i flame-normal-rotated.mp4 -vf "scale=9:16" -c:v libvpx-vp9 -b:v 1M -crf 31 -threads 4 flame-normal-9x16.webm
```
```
ffmpeg -i flame-normal-9x16.webm -vf fps=30 normal-frame-%03d.png
```

- Move the tiny pngs into the folder **flame-normal-source-pngs**.

- Navigate into the **flame-normal-source-pngs** folder then run the following python script to generate an `h` file. After you've run the script, if you see a file called `data-flame-normal.h` in the folder, the script worked.

```
python3 convert-flame-normal.py *.png > data-flame-normal.h
```
- Move the `h` file `data-flame-flicker.h` into the root of the project folder: led-candle.

- Confirm that the project folder contains the three files needed to upload to the Adafruit Pro Trinket: `led-candle.ino`, `data-flame-normal.h`, `data-flame-flicker.h`.


## Parts
- Pro Trinket microcontroller [2010](https://www.adafruit.com/product/2010) or [2000](https://www.adafruit.com/product/2000)) (2010 = 3V/12MHz for longest battery life, but 5V/16MHz works OK)
- Charlieplex LED Matrix Driver (Adafruit product id: [2946](https://www.adafruit.com/product/2946))
- Charlieplex LED Matrix (Adafruit product id: 2947, [2948](https://www.adafruit.com/product/2948), 2972, 2973 or 2974)
- 350 mAh LiPoly battery (Adafruit product id: [2750](https://www.adafruit.com/product/2750))
- LiPoly backpack (Adafruit product id: [2124](https://www.adafruit.com/product/2224))
- On-Off Power Button / Pushbutton Toggle Switch (Adafruit product id: [1683](https://www.adafruit.com/product/1683))
- Ultrasonic Distance Sensor (Adafruit product id: [4007](https://www.adafruit.com/product/4007))

# Kindness Flower
###### Developed by Drew Hatzung and Daniel Stromberg for Explora "Science of Kindness" exhibition, 2024
![Kindness Flower](https://github.com/stromberg-DS/KindnessFlower/blob/main/Graphics/KindFlower.JPEG)


## General Info
Kindness Flowers are a set of exhibits that encourage and track kind acts. They are large flowers made of acrylic tube, foam tubes, craft foam, and some electronics. They have 4 pressure sensors on them that activate LEDs inside the flower when they are pressed. When the flower is passed from one person to another (all 4 pressure pads are pressed) the entire flower stem lights up with a rainbow effect and adds to the total number of passes for the day. The total count is displayed at [Adafruit.io](https://io.adafruit.com/explora_exhibits/dashboards/kindnesskount)

## Parts of the exhibit

### Flowers
The flowers are the the parts that visitors interact with directly. They can carry them around the museum and pass them off to someone else. They wait for user input (pressing the pressure pads) and light up the flower accordingly. They report their number of passes to Adafruit.io whenever they are passed. Overnight they reset the passcount to zero to prepare for the next day.

### Flower Base
The Flower Base collects the passcounts of each flower from Adafruit.io. It adds up all of the passes into one total and reports that total back to Adafruit.io. Visitors do not see the Flower Base. It stays behind the scenes and only exists to total the passcounts. There is a small OLED screen on the Flower Base to show the current pass count.

### Charging Stations
The charging stations have a wireless charger under the "vase" that holds the flower. The flowers can charge while they sit in the vase. Some of the charging stations also have a display on them the shows the Adafruit.io dashboard with the total pass count reported by the flower base. The webpage is open on either a raspberry pi or a small stick pc.




## Folder Info

### Flower Base
This is code for the Flower Base. Open this folder in VS code (with the Particle workbench installed).

### Kindness_Flower
This is code for the Kindness Flowers themselves. Open this folder in VS code (with the Particle workbench installed). When flashing flowers, make sure to set the target device to the flower you want (SciKiFlower_1, SciKiFlower_2, SciKiFlower_3, etc...). Make sure to also uncomment *only* the Adafruit MQTT line for the flower you are flashing (lines 72-79 of the current code).

### Pressure Calibration
This is the code to recalibrate the pressure baselines for the flowers. You may want to recalibrate occasionally, particularly if the LEDs flash randomly or if the LEDs are difficult to turn on. To recalibrate, do the following:
   1. Open this folder in VS Code. Make sure the Particle workbench is installed.
   2. Make sure you are logged into Particle on your computer.
   3. Change the target device to the flower you want to flash. At the very bottom of VS Code will be text like "select device" or "SciKiFlower_1" Click here and set it to the flower you want to flash.
   4. Press CTRL+SHIFT+P
   5. Type "Cloud Flash"
   6. Select the option "Particl: Cloud Flash" and hit enter
   7. Make sure flower is held vertically with no pressure on the sides of the stem.

### Wiring Diagrams
Wiring diagrams for the Kindness Flowers and the Flower Base. Diagrams all made in Fritzing. 

### Graphics
Flowcharts for the different parts of the exhibit. Flowcharts made with Drawio.com.

### 3D_Files
Here are 3D files for the project. Mostly just the case for the Flower Base.

### PCB_Design
This includes the KiCAD files for the PCB design. If you just want more of the existing PCBs, upload KindFlowerGBR_V2.zip to a PCB manufacturer such as PCBway.com. If you want to make changes, open KindFlowerPCB.kicad_pro in KiCAD 8.0 or later. 

### BLE_Tests
BLE is unused in the current version. In the future, we would like the flowers to sense their proximity to places around the museum. BLE_Tests has 2 folders within it. These are BLE tests for the 2 (or more) beacons, and the one central hub. The beacons should have their colors changed in code, get flashed and spread around. The central device should be portable. Folders should be opened in VS Code (Particle workbench should be installed). 

### CapSensorTest
Originally, we were using capacitive sensors. We switched over to pressure sensors instead. CapSensorTest folder should also be opened in VS Code.

### Other Tests
Other random tests we did while getting things working.

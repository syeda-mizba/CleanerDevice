# CleanerDevice
hardware project
# Solar Panel Cleaning Robot

An ESP32-based portable solar panel cleaning robot designed to reduce manual maintenance of solar panels. The robot uses caterpillar tracks for movement, a roller brush for dry cleaning, a water sprayer for wet cleaning, ultrasonic sensors for distance/edge monitoring, and an ESP32-CAM for remote visual monitoring.

## Features

* Wireless robot control using ESP32
* Forward, backward, left, right and stop controls
* Caterpillar-track drive system
* Roller brush control
* Water sprayer control
* Front and rear ultrasonic distance measurement
* Web-based control interface
* ESP32-CAM live video monitoring
* JPEG-based camera streaming
* Rechargeable battery-powered operation
* Designed for rooftop and solar-farm applications

## System Architecture

```text
                         SOLAR PANEL CLEANING ROBOT
                                   |
                 +-----------------+-----------------+
                 |                                   |
          Robot Controller                       ESP32-CAM
             ESP32                                Camera
                 |                                   |
       +---------+---------+                         |
       |         |         |                         |
    Motors     Brush     Sprayer                     |
       |         |         |                         |
   Caterpillar Roller    Water                       |
     Tracks    Brush      Pump                       |
       |         |         |                         |
       +---------+---------+                         |
                 |                                   |
             Ultrasonic                         Live Video
              Sensors                               |
                 |                                   |
                 +--------------- Wi-Fi -------------+
                                   |
                                   v
                           Android / Computer
```

## Hardware

### Robot controller

* ESP32 development board
* Motor driver
* Two DC gear motors
* Caterpillar tracks
* Roller brush and brush motor
* Water pump
* Water tank
* Spray nozzle
* Two relay modules/channels
* Two ultrasonic sensors
* Rechargeable battery
* Solar charging system

### Camera

* ESP32-CAM compatible board
* Camera sensor
* Wi-Fi network

## Software

* Arduino IDE
* ESP32 Arduino board package
* `WiFi.h`
* `WebServer.h`
* `esp_camera.h`
* Appropriate ESP32-CAM board configuration

---

# 1. Robot Controller

The robot controller ESP32 handles the physical operation of the robot.

It controls:

* Left motor
* Right motor
* Roller brush
* Water sprayer
* Front ultrasonic sensor
* Rear ultrasonic sensor

It also creates a Wi-Fi access point and hosts a web control page.

### Main control flow

```text
Android phone
      |
      | Wi-Fi
      v
ESP32 Robot Controller
      |
      +---- Motor Driver ----> Caterpillar Tracks
      |
      +---- Relay -----------> Roller Brush
      |
      +---- Relay -----------> Water Pump
      |
      +---- Ultrasonic ------> Distance Monitoring
```

## Uploading the robot controller

1. Open Arduino IDE.
2. Install the ESP32 board package.
3. Connect the ESP32 to the computer using USB.
4. Open:

```text
robot-controller/robot-controller.ino
```

5. Select the correct ESP32 board under:

```text
Tools → Board
```

6. Select the correct COM/serial port.
7. Compile the program.
8. Upload it to the ESP32.
9. Open:

```text
Tools → Serial Monitor
```

10. Set the baud rate to:

```text
115200
```

The ESP32 will display its Wi-Fi information.

## Using the robot controller

After the ESP32 starts:

1. Enable Wi-Fi on your Android phone.
2. Connect to the Wi-Fi network created by the robot.
3. Open the ESP32's IP address in a web browser.
4. The robot control page will appear.

The page provides:

```text
Forward
Backward
Left
Right
Stop

Brush ON
Brush OFF

Sprayer ON
Sprayer OFF
```

It also displays the front and rear ultrasonic distance readings.

## Robot operation

A typical cleaning operation is:

```text
1. Connect phone to robot Wi-Fi
2. Open control webpage
3. Turn ON the roller brush
4. Turn ON the sprayer when wet cleaning is required
5. Move the robot using the directional controls
6. Monitor ultrasonic distance readings
7. Stop the robot after cleaning
8. Turn OFF the brush
9. Turn OFF the sprayer
```

---

# 2. ESP32-CAM

The ESP32-CAM provides remote visual monitoring.

The camera program:

1. Initializes the camera.
2. Configures the camera GPIO pins.
3. Selects JPEG image format.
4. Uses PSRAM when available.
5. Connects to Wi-Fi.
6. Starts the camera web server.
7. Provides an IP address for accessing the camera.

## Uploading the camera program

Open:

```text
esp32-camera/esp32-camera.ino
```

Make sure the correct camera model is selected in:

```text
board_config.h
```

The camera model and GPIO configuration must match the actual ESP32-CAM board being used.

Connect the ESP32-CAM to your computer using the appropriate USB-to-serial/programming arrangement.

Select the appropriate ESP32-CAM board in Arduino IDE and upload the program.

Open the Serial Monitor at:

```text
115200 baud
```

After successful Wi-Fi connection, the Serial Monitor will display something similar to:

```text
WiFi connected
Camera Ready! Use 'http://192.168.x.x' to connect
```

Open the displayed IP address in a browser.

The camera interface should appear.

---

# 3. Network Configuration

The robot controller and camera should be configured so that the phone can access both devices.

For a simple setup, use a common Wi-Fi router/access point:

```text
             Wi-Fi Router
              /       \
             /         \
            v           v
       Robot ESP32   ESP32-CAM
            \           /
             \         /
              \       /
               Android
                 Phone
```

The phone must be connected to the same network as the ESP32-CAM.

If the robot controller is configured as its own Wi-Fi access point while the camera connects to a different network, the phone may not be able to access both devices simultaneously.

---

# 4. Safety

The robot is intended to operate on solar panels and may be used at elevated locations.

Important precautions:

* Test the robot on the ground before placing it on a solar panel.
* Test motor directions before attaching the cleaning mechanism.
* Verify that the tracks maintain sufficient grip.
* Verify ultrasonic sensor positioning.
* Test the stop command before operation.
* Keep the water system away from exposed electronics.
* Use appropriate waterproofing.
* Do not rely solely on software for fall protection.
* Use a physical safety tether when testing on elevated panels.
* Verify the battery and motor-driver ratings.
* Never leave the robot operating unattended on a rooftop.

## Important note about edge detection

The current ultrasonic routine uses a distance threshold to identify a possible panel-edge condition.

The physical orientation and mounting height of the ultrasonic sensors must be tested carefully.

The safety behavior should be validated experimentally before the robot is deployed on an elevated solar panel.

---

# 5. Cleaning Mechanism

The robot uses two cleaning mechanisms.

### Dry cleaning

The roller brush removes:

* Dust
* Loose dirt
* Dry debris
* Pollen

Typical operation:

```text
Brush ON
    ↓
Robot moves
    ↓
Roller brush contacts panel
    ↓
Dust/debris removed
```

### Wet cleaning

The water pump supplies water through the spray nozzle.

```text
Water Tank
    ↓
Water Pump
    ↓
Spray Nozzle
    ↓
Solar Panel
```

The water helps loosen more strongly attached contaminants.

---

# 6. Current System Limitations

The current software is primarily designed for remote operation.

It does not currently provide:

* Automatic dirt detection
* Automatic complete-panel navigation
* Cloud-based IoT monitoring
* Battery percentage monitoring
* Automatic cleaning scheduling
* Computer-vision-based inspection
* Automatic return-to-start navigation

These features can be added in future versions.

---

# 7. Future Improvements

Possible improvements include:

* Automatic panel-edge detection
* Obstacle detection and avoidance
* Battery voltage monitoring
* Solar charging monitoring
* Automatic cleaning cycles
* Mobile Android application
* Cloud IoT dashboard
* GPS/GNSS for large solar farms
* Camera-based dirt detection
* Remote emergency stop
* Variable motor speed control
* Water-level monitoring
* Automatic water-pump control
* Cleaning-path optimization

---

# 8. Project Objective

The objective of the Solar Panel Cleaning Robot is to reduce the manual effort and safety risks associated with cleaning solar panels.

By combining:

* ESP32 control
* Wireless communication
* Caterpillar-track movement
* Roller-brush cleaning
* Water spraying
* Ultrasonic sensing
* Camera monitoring

the system provides a portable platform for remotely operated solar-panel maintenance.

---



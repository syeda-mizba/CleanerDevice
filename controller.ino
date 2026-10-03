```cpp
#include <WiFi.h>
#include <WebServer.h>

// ============================================================
// Wi-Fi Access Point Configuration
// ============================================================

const char* AP_SSID = "ESP32-Robot";
const char* AP_PASSWORD = "CHANGE_THIS_PASSWORD";

WebServer server(80);

// ============================================================
// Motor Pins
// ============================================================

const int L_IN1 = 26;
const int L_IN2 = 25;

const int R_IN1 = 33;
const int R_IN2 = 32;

// ============================================================
// Ultrasonic Sensor Pins
// ============================================================

#define TRIG_FWD 14
#define ECHO_FWD 27

#define TRIG_BWD 12
#define ECHO_BWD 13

// ============================================================
// Relay Pins
// ============================================================

const int RELAY_BRUSH = 5;
const int RELAY_SPRAY = 18;

// ============================================================
// Distance Threshold
// ============================================================

const int EDGE_LIMIT = 17;

// ============================================================
// Web Control Page
// ============================================================

const char index_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport"
        content="width=device-width,initial-scale=1">

  <title>Solar Panel Cleaning Robot</title>

  <style>
    body {
      font-family: Arial, Helvetica, sans-serif;
      text-align: center;
      padding: 20px;
      background: #f5f5f5;
    }

    .controls {
      display: flex;
      gap: 10px;
      justify-content: center;
      flex-wrap: wrap;
      margin: 16px;
    }

    button {
      padding: 16px 24px;
      font-size: 18px;
      border-radius: 8px;
      cursor: pointer;
    }

    .status {
      margin-top: 20px;
      font-size: 20px;
    }
  </style>
</head>

<body>

  <h1>Solar Panel Cleaning Robot</h1>

  <h2>Robot Movement</h2>

  <div class="controls">
    <button onclick="cmd('forward')">Forward</button>
    <button onclick="cmd('left')">Left</button>
    <button onclick="cmd('stop')">Stop</button>
    <button onclick="cmd('right')">Right</button>
    <button onclick="cmd('backward')">Backward</button>
  </div>

  <h2>Cleaning Mechanism</h2>

  <div class="controls">
    <button onclick="cmd('brush_on')">Brush ON</button>
    <button onclick="cmd('brush_off')">Brush OFF</button>

    <button onclick="cmd('spray_on')">Sprayer ON</button>
    <button onclick="cmd('spray_off')">Sprayer OFF</button>
  </div>

  <div class="status">

    <h2>Sensor Data</h2>

    Forward Distance:
    <span id="fwd">--</span> cm
    <br>

    Backward Distance:
    <span id="bwd">--</span> cm

  </div>

  <script>

    function cmd(action) {

      fetch(`/cmd?action=${action}`)
        .then(response => response.text())
        .then(data => console.log("Response:", data))
        .catch(error => console.error(error));

    }

    function updateDistance() {

      fetch('/distance')

        .then(response => response.json())

        .then(data => {

          document.getElementById('fwd').textContent =
            data.forward;

          document.getElementById('bwd').textContent =
            data.backward;

        })

        .catch(error => console.error(error));

    }

    setInterval(updateDistance, 1000);

    updateDistance();

  </script>

</body>
</html>
)rawliteral";

// ============================================================
// Motor Control
// ============================================================

void stopMotors() {

  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, LOW);

  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, LOW);
}

void moveForward() {

  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);

  digitalWrite(R_IN1, HIGH);
  digitalWrite(R_IN2, LOW);
}

void moveBackward() {

  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);

  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
}

void turnLeft() {

  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);

  digitalWrite(R_IN1, HIGH);
  digitalWrite(R_IN2, LOW);
}

void turnRight() {

  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);

  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
}

// ============================================================
// Cleaning Mechanism
// ============================================================

void brushOn() {
  digitalWrite(RELAY_BRUSH, HIGH);
}

void brushOff() {
  digitalWrite(RELAY_BRUSH, LOW);
}

void sprayOn() {
  digitalWrite(RELAY_SPRAY, HIGH);
}

void sprayOff() {
  digitalWrite(RELAY_SPRAY, LOW);
}

// ============================================================
// Ultrasonic Distance Measurement
// ============================================================

long readDistanceCM(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration =
    pulseIn(echoPin, HIGH, 25000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.034 / 2;
}

// ============================================================
// Main Web Page
// ============================================================

void handleRoot() {

  server.send_P(
    200,
    "text/html",
    index_html
  );
}

// ============================================================
// Command Handler
// ============================================================

void handleCmd() {

  String action = server.arg("action");

  String response = "OK";

  if (action == "forward") {

    moveForward();
    response = "forward";

  }

  else if (action == "backward") {

    moveBackward();
    response = "backward";

  }

  else if (action == "left") {

    turnLeft();
    response = "left";

  }

  else if (action == "right") {

    turnRight();
    response = "right";

  }

  else if (action == "stop") {

    stopMotors();
    response = "stop";

  }

  else if (action == "brush_on") {

    brushOn();
    response = "brush on";

  }

  else if (action == "brush_off") {

    brushOff();
    response = "brush off";

  }

  else if (action == "spray_on") {

    sprayOn();
    response = "spray on";

  }

  else if (action == "spray_off") {

    sprayOff();
    response = "spray off";

  }

  else {

    response = "unknown command";

  }

  server.send(
    200,
    "text/plain",
    response
  );
}

// ============================================================
// Distance API
// ============================================================

void handleDistance() {

  long distanceForward =
    readDistanceCM(TRIG_FWD, ECHO_FWD);

  long distanceBackward =
    readDistanceCM(TRIG_BWD, ECHO_BWD);

  String json = "{";

  json += "\"forward\":";
  json += String(distanceForward);

  json += ",\"backward\":";
  json += String(distanceBackward);

  json += "}";

  server.send(
    200,
    "application/json",
    json
  );
}

// ============================================================
// Edge Monitoring
// ============================================================
//
// IMPORTANT:
// The exact response to an edge condition depends on how
// the ultrasonic sensors are physically mounted.
//
// Test this routine on the ground before rooftop operation.
// ============================================================

void edgeCheck() {

  long distanceForward =
    readDistanceCM(TRIG_FWD, ECHO_FWD);

  long distanceBackward =
    readDistanceCM(TRIG_BWD, ECHO_BWD);

  if (distanceForward > 0 &&
      distanceForward > EDGE_LIMIT) {

    Serial.println(
      "Possible front edge detected!"
    );

    stopMotors();
  }

  if (distanceBackward > 0 &&
      distanceBackward > EDGE_LIMIT) {

    Serial.println(
      "Possible rear edge detected!"
    );

    stopMotors();
  }
}

// ============================================================
// Setup
// ============================================================

void setup() {

  Serial.begin(115200);

  // ----------------------------
  // Motor pins
  // ----------------------------

  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);

  pinMode(R_IN1, OUTPUT);
  pinMode(R_IN2, OUTPUT);

  stopMotors();

  // ----------------------------
  // Relay pins
  // ----------------------------

  pinMode(RELAY_BRUSH, OUTPUT);
  pinMode(RELAY_SPRAY, OUTPUT);

  brushOff();
  sprayOff();

  // ----------------------------
  // Ultrasonic pins
  // ----------------------------

  pinMode(TRIG_FWD, OUTPUT);
  pinMode(ECHO_FWD, INPUT);

  pinMode(TRIG_BWD, OUTPUT);
  pinMode(ECHO_BWD, INPUT);

  // ----------------------------
  // Create Wi-Fi Access Point
  // ----------------------------

  WiFi.mode(WIFI_AP);

  WiFi.softAP(
    AP_SSID,
    AP_PASSWORD
  );

  Serial.println(
    "ESP32 Robot Access Point started"
  );

  Serial.print(
    "Robot IP address: "
  );

  Serial.println(
    WiFi.softAPIP()
  );

  // ----------------------------
  // Web Server Routes
  // ----------------------------

  server.on(
    "/",
    HTTP_GET,
    handleRoot
  );

  server.on(
    "/cmd",
    HTTP_GET,
    handleCmd
  );

  server.on(
    "/distance",
    HTTP_GET,
    handleDistance
  );

  server.begin();

  Serial.println(
    "Robot HTTP server started"
  );
}

// ============================================================
// Main Loop
// ============================================================

void loop() {

  server.handleClient();

  edgeCheck();
}
```

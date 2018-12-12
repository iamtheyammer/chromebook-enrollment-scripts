/*
  Copyright (c) 2017, Amplified IT
  See the full description at http://labs.amplifiedit.com/centipede

  Support forums are available at https://plus.google.com/communities/100599537603662785064

  Published under an MIT License https://opensource.org/licenses/MIT

*/

#include <Keyboard.h>

/* Modify the following definitions to fit your wireless and enrollment credentials. */

#define wifi_name "ssid" // Define SSID for your wireless connection.
#define wifi_pass "pass" // Define the password for your wireless connection.
#define wifi_security 2 // Set to 0 for open, 1 for WEP, 2 for WPA
#define username "hello@example.com" // Define the user name for enrolling the device.
#define password "passwordforaboveemail" // The password for the enrollment GAFE account.

// Use these options to determine if you want to disable analytics, skip asset ID, or if you need to slow down the Centipede

#define sendUsageToGoogle 0 // Set to 0 if you want to un-check the box to send usage analytics to Google
#define skipAssetIdScreen 0 // Set to 0 if you want Centipede to stop at the asset ID and location screen
#define languageIsSelectedOnBootPre70 1 // Set to 1 if the language box is selected when you first power on the device [BELOW CHROME VERSION 70]
#define languageIsSelectedOnBootPost70 0 // Set to 1 if the language box is selected when you first power on the device [AFTER OR ON CHROME 70]

/* These are advanced options. The defaults should be fine, but feel free to tweak values below. */

// Use this area for advanced network setup options
#define advancedNetworkSetup false // Set to true for EAP configuration, and fill out the definitions below
#define eapMethod "LEAP" // Valid options are "LEAP" "PEAP" "EAP-TLS" or "EAP-TTLS" - note that they require the quotes to work properly
#define phaseTwoAuthentication 0 // Set to 0 for automatic, 1 for EAP-MD5, 2 for MSCHAPv2, 3 for MSCHAP, 4 for PAP, 5 for CHAP
#define serverCaCertificateCheck 0 // 0 is default, 1 is "Do not check"
#define subjectMatch "" // Fill in subject match here if needed for advanced wireless
#define identity "" // Fill in identity here if needed for advanced wireless
#define anonymousIdentity "" // Fill in anonymous identity here for advanced wireless
#define saveIdentityAndPassword 0 // Set to 1 to save identity and password. NOT RECOMMENDED

#define slowMode 0 // Set to 1 if Centipede appears to be moving too quickly at any screen. This will slow down the entire process

/* Do not modify anything below this line unless you're confident that you understand how to program Arduino or C */

// Special characters definition
#define KEY_LEFT_CTRL  0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT   0x82
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT    0x85
#define KEY_RIGHT_ALT  0x86
#define KEY_UP_ARROW   0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW    0xD7
#define KEY_BACKSPACE  0xB2
#define KEY_TAB        0xB3
#define KEY_ENTER 0xB0
#define KEY_ESC        0xB1
#define KEY_CAPS_LOCK  0xC1

int buttonPin = 2;  // Set a button to any pin

int dtechButtonPin = 10;

int RXLED = 17;
static uint8_t __clock_prescaler = (CLKPR & (_BV(CLKPS0) | _BV(CLKPS1) | _BV(CLKPS2) | _BV(CLKPS3)));


void wait(int cycles = 1);

void setup()
{
  setPrescaler(); // Set prescaler to highest clock speed
  Keyboard.begin(); // Start they keyboard emulator
  pinMode(buttonPin, INPUT);  // Set up the debugging pin. If you want to debug the code, use a length of wire to connect pins 2 and GND on the board
  pinMode(dtechButtonPin, INPUT);
  digitalWrite(buttonPin, HIGH);

  pinMode(RXLED, OUTPUT); // Configure the on-board LED
  digitalWrite(RXLED, LOW);
  TXLED1;
  if (digitalRead(buttonPin) == 0) {
    showSuccess();
  }
  wait(5); // Wait for all services to finish loading
}

void loop() {

  if (digitalRead(buttonPin) == 1 ) { // Check for debugging. If not debugging, run the program
    if (digitalRead(dtechButtonPin == 1)) {
      wifiConfigPost70();
    } else {
      wifiConfigPre70(); // Enter the wifi configuration method (written down below)
    }
    TXLED1; // Toggle the TX on-board LED
    wait(8); // Wait for the connection to be established
    while (digitalRead(buttonPin) != 1) {
      bootLoop();
    }
    TXLED0;
    while (digitalRead(dtechButtonPin) == LOW) {
      // wait for the button to be pressed for the credentials to be entered.
      digitalWrite(LED_BUILTIN, LOW);
      delay(125);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    enterCredentials(); // enters google credentials
    if (skipAssetIdScreen) {
      skipAssetId();
    }
    showSuccess();
  }
  bootLoop();
}

void bootLoop() {
  //      digitalWrite(RXLED, LOW);   // set the LED on
  TXLED0; //TX LED is not tied to a normally controlled pin
  delay(200);              // wait for a second
  TXLED1;
  delay(200);
  TXLED0; //TX LED is not tied to a normally controlled pin
  delay(200);              // wait for a second
  TXLED1;
  delay(800);
}

void showSuccess() {
  digitalWrite(RXLED, HIGH);    // set the LED off
  while (true) {
    bootLoop();
  }
}

void repeatKey(byte key, int num) {
  for (int i = 0; i < num; i++) {
    Keyboard.write(key);
    wait(1);
  }
}

void blink() {
  digitalWrite(RXLED, LOW);
  //  TXLED1;
  delay(250);
  digitalWrite(RXLED, HIGH);
  //  TXLED0;
  delay(250);
}

void wait(int cycles) {
  for (int i = 0; i < cycles; i++) {
    blink();
    if (slowMode) {
      delay(250);
    }
  }
}

void enterCredentials() {
  wait(5);
  Keyboard.print(username);
  wait(3);
  Keyboard.write(KEY_ENTER);
  wait(8);
  Keyboard.print(password);
  wait(3);
  Keyboard.write(KEY_ENTER);
  wait(3);
}

void enterEnrollment() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.write('e');
  Keyboard.release(KEY_LEFT_ALT);
  Keyboard.release(KEY_LEFT_CTRL);
  wait(1);
}

void wifiConfigPost70() {
  // Access the Network option from the system tray (Status Area).
  //  Keyboard.press(KEY_LEFT_SHIFT);
  //  Keyboard.press(KEY_LEFT_ALT);
  //  Keyboard.write('s');
  //  Keyboard.release(KEY_LEFT_ALT);
  //  Keyboard.release(KEY_LEFT_SHIFT);
  repeatKey(KEY_TAB, 5 - languageIsSelectedOnBootPost70); // enter system tray
  Keyboard.write(KEY_ENTER);
  wait(2);
  //to select the Network
  repeatKey(KEY_TAB, 4); // enter wifi options
  wait(1);
  Keyboard.write(KEY_ENTER);
  wait(1);
  //to select the 'add Wifi' icon
  repeatKey(KEY_TAB, 3);
  Keyboard.write(KEY_ENTER);
  wait(1);
  // SSID
  Keyboard.print(wifi_name);
  wait(1);
  // TAB
  Keyboard.write(KEY_TAB);
  wait(1);
  if (wifi_security == 0) {
    repeatKey(KEY_TAB, 3);
  } else {
    repeatKey(KEY_DOWN_ARROW, wifi_security);
    // TAB
    Keyboard.write(KEY_TAB);
    wait(1);
    // type wifi password
    Keyboard.print(wifi_pass);
    if (advancedNetworkSetup) {
      setupAdvancedNetworkConfig();
    } else {
      repeatKey(KEY_TAB, 3);
    }

  }
  wait(1);
  // Enter
  Keyboard.write(KEY_ENTER);
  wait(1);
  // Delay 10 seconds to connect
  wait(25);

  repeatKey(KEY_TAB, 3);
  Keyboard.write(KEY_ENTER);
  wait(1);
  repeatKey(KEY_TAB, 3);
  // After connecting, enter the enrollment key command to skip checking for update at this point in the process
  enterEnrollment();
  Keyboard.write(KEY_ENTER);
  // Terms of Service screen

  wait(1);
  repeatKey(KEY_TAB, 2);
  if (!sendUsageToGoogle) {
    Keyboard.write(KEY_ENTER);
    wait(1);
  }
  repeatKey(KEY_TAB, 3);
  wait(1);
  Keyboard.write(KEY_ENTER);
  wait(10);
}

void wifiConfigPre70() {
  // Access the Network option from the system tray (Status Area).
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.write('s');
  Keyboard.release(KEY_LEFT_ALT);
  Keyboard.release(KEY_LEFT_SHIFT);
  wait(2);
  //to select the Network
  repeatKey(KEY_TAB, 3);
  wait(1);
  Keyboard.write(KEY_ENTER);
  wait(1);
  //to select the 'add Wifi' icon
  repeatKey(KEY_TAB, 3);
  Keyboard.write(KEY_ENTER);
  wait(1);
  // SSID
  Keyboard.print(wifi_name);
  wait(1);
  // TAB
  Keyboard.write(KEY_TAB);
  wait(1);
  if (wifi_security == 0) {
    repeatKey(KEY_TAB, 3);
  } else {
    repeatKey(KEY_DOWN_ARROW, wifi_security);
    // TAB
    Keyboard.write(KEY_TAB);
    wait(1);
    // type wifi password
    Keyboard.print(wifi_pass);
    if (advancedNetworkSetup) {
      setupAdvancedNetworkConfig();
    } else {
      repeatKey(KEY_TAB, 3);
    }

  }
  wait(1);
  // Enter
  Keyboard.write(KEY_ENTER);
  wait(1);
  // Delay 10 seconds to connect
  wait(25);

  repeatKey(KEY_TAB, 3 - languageIsSelectedOnBootPre70);
  Keyboard.write(KEY_ENTER);
  wait(1);
  repeatKey(KEY_TAB, 2);
  // After connecting, enter the enrollment key command to skip checking for update at this point in the process
  enterEnrollment();
  Keyboard.write(KEY_ENTER);
  // Terms of Service screen

  wait(1);
  repeatKey(KEY_TAB, 2);
  if (!sendUsageToGoogle) {
    Keyboard.write(KEY_ENTER);
    wait(1);
  }
  repeatKey(KEY_TAB, 3);
  wait(1);
  Keyboard.write(KEY_ENTER);
  wait(10);
}

void skipAssetId() {
  wait(40);
  repeatKey(KEY_TAB, 2);
  repeatKey(KEY_ENTER, 2);
}

void setupAdvancedNetworkConfig() {
  repeatKey(KEY_TAB, 2);
  Keyboard.write(KEY_ENTER);
  // SSID
  Keyboard.print(wifi_name);
  Keyboard.write(KEY_TAB);

  if (eapMethod == "LEAP") {
    // Select LEAP method
    Keyboard.write(KEY_DOWN_ARROW);
    // Identity
    Keyboard.print(identity);
    Keyboard.write(KEY_TAB);
    Keyboard.print(wifi_pass);
    repeatKey(KEY_TAB, 3);
    Keyboard.write(KEY_ENTER);
  } else if (eapMethod == "PEAP") {
    // Select PEAP method
    repeatKey(KEY_DOWN_ARROW, 2);

    // If phase two authentication is defined, select it
    if (phaseTwoAuthentication) {
      repeatKey(KEY_DOWN_ARROW, phaseTwoAuthentication);
    }
    Keyboard.write(KEY_TAB);

    // Server CA Certificate
    if (serverCaCertificateCheck) {
      Keyboard.write(KEY_DOWN_ARROW);
    }

    // Identity
    Keyboard.print(identity);
    Keyboard.write(KEY_TAB);
    Keyboard.print(wifi_pass);
    repeatKey(KEY_TAB, 2);

    // Anonymous Identity
    Keyboard.print(anonymousIdentity);
    repeatKey(KEY_TAB, 2);
    Keyboard.write(KEY_ENTER);
  } else if (eapMethod ==  "EAP-TLS") {
    // Select EAP-TLS method
    repeatKey(KEY_DOWN_ARROW, 3);
    Keyboard.write(KEY_TAB);

    // Server CA Certificate
    if (serverCaCertificateCheck) {
      Keyboard.write(KEY_DOWN_ARROW);
    }
    Keyboard.write(KEY_TAB);

    // Subject match
    Keyboard.print(subjectMatch);
    Keyboard.write(KEY_TAB);

    // Identity
    Keyboard.print(identity);
    repeatKey(KEY_TAB, 3);

  } else if (eapMethod == "EAP-TTLS") {
    repeatKey(KEY_DOWN_ARROW, 4);
    Keyboard.write(KEY_TAB);

    // If phase two authentication is defined, select it
    if (phaseTwoAuthentication) {
      repeatKey(KEY_DOWN_ARROW, phaseTwoAuthentication);
    }
    Keyboard.write(KEY_TAB);

    // Server CA Certificate
    if (serverCaCertificateCheck) {
      Keyboard.write(KEY_DOWN_ARROW);
    }

    // Identity
    Keyboard.print(identity);
    Keyboard.write(KEY_TAB);
    Keyboard.print(wifi_pass);
    repeatKey(KEY_TAB, 2);

    // Anonymous Identity
    Keyboard.print(anonymousIdentity);
    repeatKey(KEY_TAB, 2);
    Keyboard.write(KEY_ENTER);
  } else if (eapMethod ==  "EAP-TLS") {
    // Select EAP-TLS method
    repeatKey(KEY_DOWN_ARROW, 3);
    Keyboard.write(KEY_TAB);

    // Server CA Certificate
    if (serverCaCertificateCheck) {
      Keyboard.write(KEY_DOWN_ARROW);
    }
    Keyboard.write(KEY_TAB);

    // Subject match
    Keyboard.print(subjectMatch);
    Keyboard.write(KEY_TAB);

    // Identity
    Keyboard.print(identity);
    repeatKey(KEY_TAB, 3);

    // Anonymous Identity
    Keyboard.print(anonymousIdentity);
    repeatKey(KEY_TAB, 2);
    Keyboard.write(KEY_ENTER);
  }
}

void setPrescaler() {
  // Disable interrupts.
  uint8_t oldSREG = SREG;
  cli();

  // Enable change.
  CLKPR = _BV(CLKPCE); // write the CLKPCE bit to one and all the other to zero

  // Change clock division.
  CLKPR = 0x0; // write the CLKPS0..3 bits while writing the CLKPE bit to zero

  // Copy for fast access.
  __clock_prescaler = 0x0;

  // Recopy interrupt register.
  SREG = oldSREG;
}

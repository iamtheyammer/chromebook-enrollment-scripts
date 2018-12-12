# chromebook-enrollment-scripts
Chromebook Enrolment Arduino Pro Micro Scripts

## Usage

Super simple. What you'll need for this is a Pro Micro and a button attached on pin 10 (although that's changable).

Plug it into your device and watch the magic:

1. To use it for Chrome 70, hold the button down while the system boots up. After you see the first key press, you can let go.
2. Depending on your settings, it may wait at the enrollment screen. If it does, press the button to have it enter your credentials at the top of the script or you can unplug it and be done.

## Installation onto Arduino

- Grab the script for your chrome version (currently it's chrome-58-and-up-enrollment.ino |[raw to copy and paste](https://github.com/iamtheyammer/chromebook-enrollment-scripts/raw/master/chrome-58-and-up.ino)|[view](https://github.com/iamtheyammer/chromebook-enrollment-scripts/blob/master/chrome-58-and-up.ino))
- Change the settings as instructed at the top of the script
- Upload to your Arduino device

## Credits
[AmplifiedIT](https://labs.amplifiedit.com/centipede/) wrote the script for Chrome version 58-69, and I modified it to work with Chrome version 70 and up. I use this every day and needed a version for the new chrome version.

Under the MIT license just like theirs.

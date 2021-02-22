// Set declination angle on your location and fix heading
// You can find your declination on: http://magnetic-declination.com/
// (+) Positive or (-) for negative
// For Berlin / Germany declination angle is 4'30E (positive)
// Formula: (deg + (min / 60.0)) / (180 / PI);
float declinationAngle = (4.0 + (35.0 / 60.0)) / (180 / PI);
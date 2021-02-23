#include <DFRobot_QMC5883.h>
#include <EEPROM.h>

DFRobot_QMC5883 compass;
// used to detect of eeprom is valid.
uint8_t structMagicKey = 0xAB;
void storeStruct(void *data_source, size_t size);
bool loadStruct(void *data_dest, size_t size);

void sensor_init(float declinationAngle)
{
  Serial.println("Initialize");
  while (!compass.begin())
  {
    Serial.println("Could not find a valid sensor, check wiring!");
    delay(500);
  }

  if (compass.isHMC())
  {
    Serial.println("Initialize HMC5883");
    // Set measurement range
    // +/- 0.88 Ga: HMC5883L_RANGE_0_88GA
    // +/- 1.30 Ga: HMC5883L_RANGE_1_3GA (default)
    // +/- 1.90 Ga: HMC5883L_RANGE_1_9GA
    // +/- 2.50 Ga: HMC5883L_RANGE_2_5GA
    // +/- 4.00 Ga: HMC5883L_RANGE_4GA
    // +/- 4.70 Ga: HMC5883L_RANGE_4_7GA
    // +/- 5.60 Ga: HMC5883L_RANGE_5_6GA
    // +/- 8.10 Ga: HMC5883L_RANGE_8_1GA
    compass.setRange(HMC5883L_RANGE_8_1GA);

    // Set measurement mode
    // Idle mode:              HMC5883L_IDLE
    // Single-Measurement:     HMC5883L_SINGLE
    // Continuous-Measurement: HMC5883L_CONTINOUS (default)
    compass.setMeasurementMode(HMC5883L_CONTINOUS);

    // Set data rate
    //  0.75Hz: HMC5883L_DATARATE_0_75HZ
    //  1.50Hz: HMC5883L_DATARATE_1_5HZ
    //  3.00Hz: HMC5883L_DATARATE_3HZ
    //  7.50Hz: HMC5883L_DATARATE_7_50HZ
    // 15.00Hz: HMC5883L_DATARATE_15HZ (default)
    // 30.00Hz: HMC5883L_DATARATE_30HZ
    // 75.00Hz: HMC5883L_DATARATE_75HZ
    compass.setDataRate(HMC5883L_DATARATE_75HZ);

    // Set number of samples averaged
    // 1 sample:  HMC5883L_SAMPLES_1 (default)
    // 2 samples: HMC5883L_SAMPLES_2
    // 4 samples: HMC5883L_SAMPLES_4
    // 8 samples: HMC5883L_SAMPLES_8
    compass.setSamples(HMC5883L_SAMPLES_8);
  }
  else if (compass.isQMC())
  {
    Serial.println("Initialize QMC5883");
    compass.setRange(QMC5883_RANGE_8GA);
    compass.setMeasurementMode(QMC5883_CONTINOUS);
    compass.setDataRate(QMC5883_DATARATE_50HZ);
    compass.setSamples(QMC5883_OVERSAMPLERATIO_512);
  }
  else if (compass.isVCM())
  {
    Serial.println("Initialize VCM5883L");
    //compass.setMeasurementMode(VCM5883L_CONTINOUS);
    //compass.setDataRate(VCM5883L_DATARATE_200HZ);
  }

  if (declinationAngle != 0)
  {
    Serial.print("DeclinationAngle: ");
    Serial.println(declinationAngle);
    compass.setDeclinationAngle(declinationAngle);
  }

  Calibration cal;
  if (loadStruct(&cal, sizeof(cal)))
  {
    Serial.println("Loaded calibration data");
    compass.setCalibration(cal);
  }
  else
  {
    Serial.println("No calibration data found");
  }
}

void sensor_checkSettings()
{
  Serial.print("Selected sensor: ");
  if (compass.isHMC())
  {
    Serial.println("HMC5883");
  }
  else if (compass.isQMC())
  {
    Serial.println("QMC5883");
  }
  else if (compass.isVCM())
  {
    Serial.println("VCM5883L");
  }

  Serial.print("Selected range: ");
  if (compass.isHMC())
  {
    switch (compass.getRange())
    {
    case HMC5883L_RANGE_0_88GA:
      Serial.println("0.88 Ga");
      break;
    case HMC5883L_RANGE_1_3GA:
      Serial.println("1.3 Ga");
      break;
    case HMC5883L_RANGE_1_9GA:
      Serial.println("1.9 Ga");
      break;
    case HMC5883L_RANGE_2_5GA:
      Serial.println("2.5 Ga");
      break;
    case HMC5883L_RANGE_4GA:
      Serial.println("4 Ga");
      break;
    case HMC5883L_RANGE_4_7GA:
      Serial.println("4.7 Ga");
      break;
    case HMC5883L_RANGE_5_6GA:
      Serial.println("5.6 Ga");
      break;
    case HMC5883L_RANGE_8_1GA:
      Serial.println("8.1 Ga");
      break;
    default:
      Serial.println("Bad range!");
    }
  }
  else if (compass.isQMC())
  {
    switch (compass.getRange())
    {
    case QMC5883_RANGE_2GA:
      Serial.println("2 Ga");
      break;
    case QMC5883_RANGE_8GA:
      Serial.println("8 Ga");
      break;
    default:
      Serial.println("Bad range!");
    }
  }
  else if (compass.isVCM())
  {
    switch (compass.getRange())
    {
    case VCM5883L_RANGE_8GA:
      Serial.println("8 Ga");
      break;
    default:
      Serial.println("Bad range!");
    }
  }

  Serial.print("Selected Measurement Mode: ");
  if (compass.isHMC())
  {
    switch (compass.getMeasurementMode())
    {
    case HMC5883L_IDLE:
      Serial.println("Idle mode");
      break;
    case HMC5883_SINGLE:
      Serial.println("Single-Measurement");
      break;
    case HMC5883L_CONTINOUS:
      Serial.println("Continuous-Measurement");
      break;
    default:
      Serial.println("Bad mode!");
    }
  }
  else if (compass.isQMC() || compass.isVCM())
  {
    switch (compass.getMeasurementMode())
    {
    case QMC5883_STANDBY:
      Serial.println("Standby");
      break;
    case QMC5883_CONTINOUS:
      Serial.println("Continuous-Measurement");
      break;
    default:
      Serial.println("Bad mode!");
    }
  }

  Serial.print("Selected Data Rate: ");
  if (compass.isHMC())
  {
    switch (compass.getDataRate())
    {
    case HMC5883L_DATARATE_0_75_HZ:
      Serial.println("0.75 Hz");
      break;
    case HMC5883L_DATARATE_1_5HZ:
      Serial.println("1.5 Hz");
      break;
    case HMC5883L_DATARATE_3HZ:
      Serial.println("3 Hz");
      break;
    case HMC5883L_DATARATE_7_5HZ:
      Serial.println("7.5 Hz");
      break;
    case HMC5883L_DATARATE_15HZ:
      Serial.println("15 Hz");
      break;
    case HMC5883L_DATARATE_30HZ:
      Serial.println("30 Hz");
      break;
    case HMC5883L_DATARATE_75HZ:
      Serial.println("75 Hz");
      break;
    default:
      Serial.println("Bad data rate!");
    }
  }
  else if (compass.isQMC())
  {
    switch (compass.getDataRate())
    {
    case QMC5883_DATARATE_10HZ:
      Serial.println("10 Hz");
      break;
    case QMC5883_DATARATE_50HZ:
      Serial.println("50 Hz");
      break;
    case QMC5883_DATARATE_100HZ:
      Serial.println("100 Hz");
      break;
    case QMC5883_DATARATE_200HZ:
      Serial.println("200 Hz");
      break;
    default:
      Serial.println("Bad data rate!");
    }
  }
  else if (compass.isVCM())
  {
    switch (compass.getDataRate())
    {
    case VCM5883L_DATARATE_10HZ:
      Serial.println("10 Hz");
      break;
    case VCM5883L_DATARATE_50HZ:
      Serial.println("50 Hz");
      break;
    case VCM5883L_DATARATE_100HZ:
      Serial.println("100 Hz");
      break;
    case VCM5883L_DATARATE_200HZ:
      Serial.println("200 Hz");
      break;
    default:
      Serial.println("Bad data rate!");
    }
  }

  if (compass.isHMC())
  {
    Serial.print("Selected number of samples: ");
    switch (compass.getSamples())
    {
    case HMC5883L_SAMPLES_1:
      Serial.println("1");
      break;
    case HMC5883L_SAMPLES_2:
      Serial.println("2");
      break;
    case HMC5883L_SAMPLES_4:
      Serial.println("4");
      break;
    case HMC5883L_SAMPLES_8:
      Serial.println("8");
      break;
    default:
      Serial.println("Bad number of samples!");
    }
  }
  else if (compass.isQMC())
  {

    Serial.print("Selected Over Sample Ratio: ");
    switch (compass.getSamples())
    {
    case QMC5883_OVERSAMPLERATIO_512:
      Serial.println("512");
      break;
    case QMC5883_OVERSAMPLERATIO_256:
      Serial.println("256");
      break;
    case QMC5883_OVERSAMPLERATIO_128:
      Serial.println("128");
      break;
    case QMC5883_OVERSAMPLERATIO_64:
      Serial.println("64");
      break;
    default:
      Serial.println("Bad number of samples!");
    }
  }
}

bool sensor_ready()
{
  return compass.isDataReady();
}

int counter = 0;

float sensor_readDegress()
{
  counter++;
  if (counter > 250)
  {
    // every 250 sensor readings check for new calibration.
    // webpage is updated every 1/4 second = one calibration check per minute.
    counter = 0;
    if (compass.hasNewCalibration())
    {
      Calibration cal = compass.readCalibration();
      storeStruct(&cal, sizeof(cal));
    }
  }

  return compass.readHeadingDegrees();
}

void sensor_printPlotter()
{
  if (compass.isDataReady())
  {
    // Vector raw = compass.readRaw();
    // Serial.print("xRaw:");
    // Serial.print(raw.XAxis);
    // Serial.print(" yRaw:");
    // Serial.print(raw.YAxis);
    // Serial.print(" ");

    VectorScaled s = compass.readScaled();
    Serial.print("x:");
    Serial.print(s.XAxis);
    Serial.print(" y:");
    Serial.print(s.YAxis);
    Serial.print(" ");

    // VectorScaled cal = compass.readCalibration();
    // Serial.print("calibrationx:");
    // Serial.print(cal.XAxis);
    // Serial.print(" calibrationy:");
    // Serial.print(cal.YAxis);
    // Serial.print(" ");

    float com = sensor_readDegress();
    Serial.print("compass:");
    Serial.print(com);
    Serial.print(" ");

    Serial.println();
  }
  else
  {
    Serial.println();
  }
}

void storeStruct(void *data_source, size_t size)
{
  EEPROM.begin(size * 2 + 1);
  EEPROM.write(0, structMagicKey);
  for (size_t i = 1; i < size; i++)
  {
    char data = ((char *)data_source)[i];
    EEPROM.write(i, data);
  }
  EEPROM.commit();
}

bool loadStruct(void *data_dest, size_t size)
{
  EEPROM.begin(size * 2 + 1);
  if (EEPROM.read(0) == structMagicKey)
  {
    for (size_t i = 1; i < size; i++)
    {
      char data = EEPROM.read(i);
      ((char *)data_dest)[i] = data;
    }
    return true;
  }
  else
  {
    return false;
  }
}
#include <SPI.h>
#include <SD.h>

// Pin definitions for SPI1
#define SD_CS   PA4    // CS
#define SD_SCK  PA5    // SCK
#define SD_MISO PA6    // MISO
#define SD_MOSI PA7    // MOSI

// Define Serial object for UART1 (PA9/PA10)
HardwareSerial Serial1(PA10, PA9);

void setup() {
  // Start serial communication
  Serial1.begin(9600);
  delay(1000);
  
  Serial1.println("\nSD Card Test - Level Shifter Version");

  // Configure SPI pins
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH); // Deselect SD card
  
  // Initialize SPI with very conservative settings
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128); // Very slow speed
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  
  Serial1.println("\nPower-up sequence:");
  
  // Power-up sequence
  digitalWrite(SD_CS, HIGH);
  delay(100);
  
  // Send dummy clock cycles with CS high
  for(int i = 0; i < 10; i++) {
    SPI.transfer(0xFF);
    Serial1.print(".");
  }
  Serial1.println(" Done");
  
  delay(100);

  Serial1.println("\nInitializing SD card...");
  Serial1.println("Using level-shifted configuration:");
  Serial1.println("CS   -> PA4  (with level shifter)");
  Serial1.println("SCK  -> PA5  (with level shifter)");
  Serial1.println("MISO -> PA6  (with level shifter)");
  Serial1.println("MOSI -> PA7  (with level shifter)");
  Serial1.println("VCC  -> 5V   (to module's VCC)");
  Serial1.println("GND  -> GND");
  
  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial1.println("\nSD card initialization failed!");
    Serial1.println("\nTroubleshooting for level-shifted module:");
    Serial1.println("1. Power supply:");
    Serial1.println("   - Connect module's VCC to 5V (not 3.3V)");
    Serial1.println("   - Ensure good ground connection");
    Serial1.println("2. Check voltage levels:");
    Serial1.println("   - Module VCC should read ~5V");
    Serial1.println("   - Card VCC should read ~3.3V (from LDO)");
    Serial1.println("3. Level shifter connections:");
    Serial1.println("   - All signals should be properly level shifted");
    Serial1.println("   - No additional pullup resistors needed");
    Serial1.println("4. Try these steps:");
    Serial1.println("   - Power cycle the entire system");
    Serial1.println("   - Inspect the level shifter ICs");
    Serial1.println("   - Check for any burnt components");
    while(1);
  }

  Serial1.println("SD card initialization successful!");

  // Test file operations
  File dataFile = SD.open("test.txt", FILE_WRITE);
  if (dataFile) {
    Serial1.println("\nWriting to test.txt...");
    dataFile.println("Testing SD card with level shifter");
    dataFile.println("Module is working properly!");
    dataFile.close();
    Serial1.println("Write successful!");

    // Read back the file
    dataFile = SD.open("test.txt");
    if (dataFile) {
      Serial1.println("\nReading file contents:");
      while (dataFile.available()) {
        Serial1.write(dataFile.read());
      }
      dataFile.close();
      Serial1.println("\nRead successful!");
    }
  } else {
    Serial1.println("Error opening test file!");
  }
}

void loop() {
  // Empty loop
}

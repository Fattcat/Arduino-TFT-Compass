#include <TinyGPS++.h>  // Knižnica pre GPS
#include <TFT_eSPI.h>   // Knižnica pre TFT displej (uprav podľa displeja a knižnice)

TinyGPSPlus gps;
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // GPS modul na RX/TX (16/17)
  
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // Načítavanie GPS dát
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }

  if (gps.location.isUpdated() && gps.speed.isValid() && gps.course.isValid()) {
    float heading = gps.course.deg();  // Azimut/smer pohybu v stupňoch

    // Aktualizácia displeja
    tft.fillScreen(TFT_BLACK);
    
    // Zobrazenie textu na displeji
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.print("Kompas");

    // Výpočet pozície šípky na displeji
    int x_center = 120, y_center = 160;  // Stred displeja
    float arrow_length = 50;
    float rad = heading * PI / 180;  // Prevod uhla na radiány

    // Koncové súradnice šípky smerom na sever
    int x_end = x_center + arrow_length * cos(rad - PI / 2);  // -PI/2 pre otočenie o 90°
    int y_end = y_center + arrow_length * sin(rad - PI / 2);

    // Vykreslenie šípky
    tft.drawLine(x_center, y_center, x_end, y_end, TFT_RED);  // Hlavná šípka
    tft.fillTriangle(x_end, y_end,
                     x_end - 5 * cos(rad - PI / 2 + PI / 4), y_end - 5 * sin(rad - PI / 2 + PI / 4),
                     x_end - 5 * cos(rad - PI / 2 - PI / 4), y_end - 5 * sin(rad - PI / 2 - PI / 4),
                     TFT_RED);

    // Zobrazenie svetových strán
    tft.setCursor(110, 30);
    tft.print("N");  // Sever
    tft.setCursor(200, 160);
    tft.print("E");  // Východ
    tft.setCursor(120, 290);
    tft.print("S");  // Juh
    tft.setCursor(40, 160);
    tft.print("W");  // Západ

    delay(1000);  // Obnovovacia frekvencia (1 sekunda)
  }
}

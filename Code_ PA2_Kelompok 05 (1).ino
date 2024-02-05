#include <Wire.h>                 // Mengimpor library Wire untuk komunikasi I2C
#include <LiquidCrystal_I2C.h>    // Mengimpor library LCD yang menggunakan I2C

#define UVOUT A0                  // Pin analog A0 terhubung ke sensor UV
#define REF_3V3 A1                // Pin referensi 3.3V untuk pembacaan sensor UV
#define Relay1 13                  // Pin tempat relay terhubung
#define Relay2 11                  // Pin tempat relay terhubung

LiquidCrystal_I2C lcd(0x27, 16, 2);   // Menginisialisasi objek lcd untuk komunikasi I2C dengan alamat 0x27, dan ukuran layar 16x2

void setup() {
  Serial.begin(9600);                 // Memulai komunikasi serial pada baudrate 9600
  pinMode(UVOUT, INPUT);              // Menetapkan pin UVOUT sebagai input
  pinMode(REF_3V3, INPUT);            // Menetapkan pin REF_3V3 sebagai input
  pinMode(Relay1, OUTPUT);             // Menetapkan pin relay sebagai output
  pinMode(Relay2, OUTPUT);             // Menetapkan pin relay sebagai output

  lcd.init();                         // Menginisialisasi lcd
  lcd.backlight();                    // Menyalakan lampu latar lcd
  lcd.clear();                        // Membersihkan lcd
  lcd.setCursor(0,0);                 // Menyetel kursor pada posisi awal
  lcd.print("PDLC GLASS WITH");       // Menampilkan teks pada lcd
  lcd.setCursor(0,3);                 // Menyetel kursor pada posisi berikutnya
  lcd.print("UV DETECTION");          // Menampilkan teks lain pada lcd
  delay(2000);                        // Memberikan jeda sebelum mencetak lagi
  lcd.clear();                        // Membersihkan lcd
  lcd.print("BY");                    // Menampilkan teks baru pada lcd
  lcd.setCursor(0,3);                 // Menyetel kursor pada posisi berikutnya
  lcd.print("GROUP 05");              // Menampilkan teks baru lagi pada lcd
  delay (3000);                       // Memberikan jeda sebelum mencetak lagi
  lcd.clear();                         // Membersihkan lcd
}

void loop() {
  int uvLevel = averageAnalogRead(UVOUT);                   // Membaca rata-rata pembacaan sensor UV
  int refLevel = averageAnalogRead(REF_3V3);                // Membaca rata-rata pembacaan level referensi

  float outputVoltage = 3.3 / refLevel * uvLevel;        // Menghitung tegangan keluaran berdasarkan pembacaan UV dan level referensi

  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);  // Mengubah tegangan keluaran menjadi intensitas UV

  lcd.setCursor(0,0);              // Menyetel kursor pada posisi awal
  lcd.print("LEVEL UV: ");         // Menampilkan teks pada lcd

  lcd.setCursor(0,3);              // Menyetel kursor pada posisi berikutnya
  lcd.print(uvIntensity);          // Menampilkan intensitas UV pada lcd
  lcd.print(" mW/cm^2");

  Serial.println();               // Mengirim baris baru ke serial monitor

  if (uvLevel > 300) {            // Jika pembacaan UV melebihi 300
    digitalWrite(Relay1, HIGH);    // Nyalakan relay
    digitalWrite(Relay2, HIGH);    // Nyalakan relay
  } else {
    digitalWrite(Relay1, LOW);     // Matikan relay
    digitalWrite(Relay2, LOW);     // Matikan relay
  }

  delay(2000);                   // Beri jeda sebelum membaca lagi
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;        // Menentukan berapa kali pembacaan yang akan diambil
  unsigned int runningValue = 0;    // Variabel untuk menyimpan total pembacaan

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);        // Membaca nilai dari pin dan menambahkannya ke total
  runningValue /= numberOfReadings;               // Menghitung rata-rata

  return(runningValue);                           // Mengembalikan rata-rata pembacaan  
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; // Fungsi map untuk float, mengubah range input ke range output yang diinginkan
}
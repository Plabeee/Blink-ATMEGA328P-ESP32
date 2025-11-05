#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Configuración de Sensores ---

// Acelerómetro
Adafruit_MPU6050 mpu;

// Pantalla OLED
#define SCREEN_WIDTH 128 // Ancho de la pantalla OLED en píxeles
#define SCREEN_HEIGHT 64 // Alto de la pantalla OLED en píxeles
#define OLED_RESET -1    // Pin de Reset (-1 si se comparte el reset de Arduino)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Pines I2C del XIAO ESP32-S3 ---
// Según tu diagrama, estás usando los pines I2C por defecto
// D4 (SDA) y D5 (SCL)
const int I2C_SDA_PIN = D9;
const int I2C_SCL_PIN = D8;
void setup() {
  Serial.begin(115200);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  Serial.println("");
  Serial.println("Hello, XIAO ESP32-S3!");
  Serial.println("Welcome to Wokwi :-)");
  Serial.begin(115200);

  // Inicializar el bus I2C con los pines correctos
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // --- Inicializar el Acelerómetro (MPU-6050) ---
  if (!mpu.begin()) {
    Serial.println("Error al encontrar el sensor MPU-6050. Revisa las conexiones.");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU-6050 encontrado!");

  // Configurar el rango del acelerómetro (puedes cambiarlo)
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // Opciones: 2_G, 4_G, 8_G, 16_G

  // --- Inicializar la Pantalla OLED (SSD1306) ---
  // La dirección I2C más común es 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Error al inicializar la pantalla SSD1306."));
    while (1) {
      delay(10);
    }
  }
  Serial.println("Pantalla OLED inicializada.");

  // Limpiar la pantalla al inicio
  display.clearDisplay();
  display.display();
}

void loop() {
  Serial.println("Red");
  digitalWrite(D2, HIGH);
  delay(500);
  digitalWrite(D2, LOW);

  Serial.println("Green");
  digitalWrite(D3, HIGH);
  delay(500);
  digitalWrite(D3, LOW);

  Serial.println("Blue");
  digitalWrite(D4, HIGH);
  delay(500);
  digitalWrite(D4, LOW);
  // Obtener nuevos eventos (datos) de los sensores
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // --- Actualizar la Pantalla OLED ---

  // 1. Limpiar el búfer de la pantalla
  display.clearDisplay();

  // 2. Configurar el texto
  display.setTextSize(1);      // Tamaño de fuente pequeño
  display.setTextColor(SSD1306_WHITE); // Color del texto

  // 3. Escribir los datos en el búfer
  display.setCursor(0, 0); // Posición (x, y)
  display.println("Aceleracion (m/s^2):");
  display.println(""); // Línea en blanco

  display.setCursor(0, 24);
  display.print("X: ");
  display.print(a.acceleration.x);

  display.setCursor(0, 34);
  display.print("Y: ");
  display.print(a.acceleration.y);

  display.setCursor(0, 44);
  display.print("Z: ");
  display.print(a.acceleration.z);

  // 4. Enviar el búfer a la pantalla para mostrarlo
  display.display();

  // Pequeña pausa para que la pantalla sea legible
  delay(100);
}

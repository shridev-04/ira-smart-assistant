// ==================================================================================
//  ██╗██████╗  █████╗      █████╗ ██╗
//  ██║██╔══██╗██╔══██╗    ██╔══██╗██║
//  ██║██████╔╝███████║    ███████║██║
//  ██║██╔══██╗██╔══██║    ██╔══██║██║
//  ██║██║  ██║██║  ██║    ██║  ██║██║
//  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝    ╚═╝  ╚═╝╚═╝
// ==================================================================================
//  [PROJECT METADATA & INTELLECTUAL PROPERTY REGISTER]
// ==================================================================================
//  📌 PROJECT NAME:      I.R.A. (Intelligent Real-time Automation Client Node)
//  📌 CHIEF ARCHITECT:   Shridev Kumar
//  📌 DESIGN PURPOSE:    Open-Source Advanced Voice-Controlled Embedded System
//  📌 TARGET HARDWARE:   ESP32 DevKit V1 (with I2S Microphone & Speaker Modules)
//  📌 CORE FEATURES:     4-Channel Appliance Control, EEPROM State Cache, 
//                        Dynamic Token Routing, 100% DRY Configuration Panel.
// ==================================================================================
//  © 2026 Shridev Kumar. Co-developed with AI. Distributed for global open-source 
//  community usage. Unauthorized rebranding without explicit author credits is prohibited.
// ==================================================================================

#include <WiFi.h>
#include <WebSocketsClient.h>
#include <driver/i2s.h>
#include <EEPROM.h>

// ==================================================================================
// ⭐⭐⭐ [MASTER CONTROL PANEL - GENERAL STATIC SETTINGS] ⭐⭐⭐
// ==================================================================================
namespace Config {
    // 1. Network Connectivity Core (Users will enter their credentials here)
    const char* wifi_ssid      = "YOUR_WIFI_SSID";         // <-- Personal data removed safely
    const char* wifi_password  = "YOUR_WIFI_PASSWORD";     // <-- Personal data removed safely

    // 2. Telemetry and Routing Gateways (Users will put their hosted server URL here)
    const char* server_host    = "your-app-name.onrender.com"; // <-- Placeholder for users
    const uint16_t server_port = 443;                     // Always 443 for Cloud WSS

    // 3. Status Visualizer Matrix Hooks
    const int status_led       = 2;                        

    // 4. Hardware Appliance Relay Allocations
    const int RELAY_1_PIN      = 19;                       
    const int RELAY_2_PIN      = 18;                       
    const int RELAY_3_PIN      = 21;                       
    const int RELAY_4_PIN      = 23;                       

    // 5. System Tokens (Synchronized flawlessly with Python CONFIG Object)
    const String R1_ON         = "R1_ON_SIGNAL";
    const String R1_OFF        = "R1_OFF_SIGNAL";
    const String R2_ON         = "R2_ON_SIGNAL";
    const String R2_OFF        = "R2_OFF_SIGNAL";
    const String R3_ON         = "R3_ON_SIGNAL";
    const String R3_OFF        = "R3_OFF_SIGNAL";
    const String R4_ON         = "R4_ON_SIGNAL";
    const String R4_OFF        = "R4_OFF_SIGNAL";
}

// ==================================================================================
// [MEMORY SECTOR ALLOCATION & CACHE DEF]
// ==================================================================================
#define EEPROM_SIZE 4 

#define ADDR_R1 0 
#define ADDR_R2 1
#define ADDR_R3 2
#define ADDR_R4 3

// ==================================================================================
// [I2S SIGNAL INTEGRITY AUDIO MATRIX CONSTANTS]
// ==================================================================================
#define I2S_SPEAKER_BCLK 26
#define I2S_SPEAKER_LRC  25
#define I2S_SPEAKER_DOUT 22

#define I2S_MIC_SD       32
#define I2S_MIC_WS       15
#define I2S_MIC_SCK      14

#define BLOCK_SIZE       512           

// Global Objects and Automation Volatiles
WebSocketsClient webSocket;
bool is_playing          = false; 
bool ws_connected        = false;
unsigned long prevMillis = 0; 
const long blinkInterval = 300; 
bool ledState            = LOW;

// ==================================================================================
// 🎧 [AUDIO SIGNAL EXTRACTION LAYER & REPRODUCTION DRIVERS]
// ==================================================================================
void setupI2S() {
    // 1. Output Pipeline Setup (Speaker Driver)
    i2s_config_t tx_conf = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 24000, 
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 128,
        .use_apll = false
    };
    i2s_pin_config_t tx_pins = {
        .bck_io_num = I2S_SPEAKER_BCLK,
        .ws_io_num = I2S_SPEAKER_LRC,
        .data_out_num = I2S_SPEAKER_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    i2s_driver_install(I2S_NUM_0, &tx_conf, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &tx_pins);

    // 2. Input Capture Pipeline Setup (Microphone Driver)
    i2s_config_t rx_conf = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 16000, 
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 128,
        .use_apll = false
    };
    i2s_pin_config_t rx_pins = {
        .bck_io_num = I2S_MIC_SCK,
        .ws_io_num = I2S_MIC_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_MIC_SD
    };
    i2s_driver_install(I2S_NUM_1, &rx_conf, 0, NULL);
    i2s_set_pin(I2S_NUM_1, &rx_pins);
}

// ==================================================================================
// 📡 [WEBSOCKET REAL-TIME TRAFFIC EVENTS HANDLER RECEPTOR]
// ==================================================================================
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    size_t bytes_written;
    switch(type) {
        case WStype_DISCONNECTED:
            ws_connected = false;
            is_playing = false;
            Serial.println("[ROUTER INTERCEPT] Connection Terminated. Client Offline.");
            break;

        case WStype_CONNECTED:
            ws_connected = true;
            Serial.println("[ROUTER INTERCEPT] Synchronization Active. Secure Stream Handshake Achieved.");
            break;

        case WStype_TEXT:
            {
                String msg = String((char*)payload);
                if (msg == "AUDIO_END") {
                    is_playing = false;
                    i2s_zero_dma_buffer(I2S_NUM_0);
                    Serial.println("[CORE PROCESSOR] Response Broadcast Complete.");
                }
                
                // Appliance State Engine Matrix Decoding
                else if (msg == Config::R1_ON) {
                    digitalWrite(Config::RELAY_1_PIN, HIGH);
                    EEPROM.write(ADDR_R1, 1); EEPROM.commit();
                    Serial.println("[AUTOMATION CORE] Executed: Channel 01 -> ON");
                }
                else if (msg == Config::R1_OFF) {
                    digitalWrite(Config::RELAY_1_PIN, LOW);
                    EEPROM.write(ADDR_R1, 0); EEPROM.commit();
                    Serial.println("[AUTOMATION CORE] Executed: Channel 01 -> OFF");
                }
                else if (msg == Config::R2_ON) {
                    digitalWrite(Config::RELAY_2_PIN, HIGH);
                    EEPROM.write(ADDR_R2, 1); EEPROM.commit();
                    Serial.println("[AUTOMATION CORE] Executed: Channel 02 -> ON");
                }
                else if (msg == Config::R2_OFF) {
                    digitalWrite(Config::RELAY_2_PIN, LOW);
                    EEPROM.write(ADDR_R2, 0); EEPROM.commit();
                    Serial.println("[AUTOMATION CORE] Executed: Channel 02 -> OFF");
                }
                else if (msg == Config::R3_ON) {
                    digitalWrite(Config::RELAY_3_PIN, HIGH);
                    EEPROM.write(ADDR_R3, 1); EEPROM.commit();
                    Serial.println("[AUTOMATION CORE] Executed: Channel 03 -> ON");
                }
                else if (msg == Config::R3_OFF) {
                    digitalWrite(Config::RELAY_3_PIN, LOW);
                    EEPROM.write(ADDR_R3, 0); EEPROM.commit();
                    Serial.println("[AUTOMATION CORE] Executed: Channel 03 -> OFF");
                }
                else if (msg == Config::R4_ON) {
                    digitalWrite(Config::RELAY_4_PIN, HIGH);
                    EEPROM.write(ADDR_R4, 1); EEPROM.commit();
                    Serial.println("[AUTOMATION CORE] Executed: Channel 04 -> ON");
                }
                else if (msg == Config::R4_OFF) {
                    digitalWrite(Config::RELAY_4_PIN, LOW);
                    EEPROM.write(ADDR_R4, 0); EEPROM.commit();
                    Serial.println("[AUTOMATION CORE] Executed: Channel 04 -> OFF");
                }
            }
            break;

        case WStype_BIN:
            is_playing = true;
            i2s_write(I2S_NUM_0, payload, length, &bytes_written, portMAX_DELAY);
            break;
            
        default:
            break;
    }
}

// ==================================================================================
// ⚡ [HARDWARE INIT ENGINE & CONTEXT LINK SETUP]
// ==================================================================================
void setup() {
    Serial.begin(115200);
    
    pinMode(Config::status_led, OUTPUT);
    pinMode(Config::RELAY_1_PIN, OUTPUT);
    pinMode(Config::RELAY_2_PIN, OUTPUT);
    pinMode(Config::RELAY_3_PIN, OUTPUT);
    pinMode(Config::RELAY_4_PIN, OUTPUT);

    EEPROM.begin(EEPROM_SIZE);
    
    digitalWrite(Config::RELAY_1_PIN, EEPROM.read(ADDR_R1) == 1 ? HIGH : LOW);
    digitalWrite(Config::RELAY_2_PIN, EEPROM.read(ADDR_R2) == 1 ? HIGH : LOW);
    digitalWrite(Config::RELAY_3_PIN, EEPROM.read(ADDR_R3) == 1 ? HIGH : LOW);
    digitalWrite(Config::RELAY_4_PIN, EEPROM.read(ADDR_R4) == 1 ? HIGH : LOW);

    setupI2S();

    Serial.print("[NETWORK ARCHITECTURE] Establishing WiFi Link...");
    WiFi.begin(Config::wifi_ssid, Config::wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[NETWORK ARCHITECTURE] Connection Anchored Successfully!");

    // Secure SSL Connection Engine for Render Cloud Deployment
    webSocket.setInsecure(); 
    webSocket.beginSSL(Config::server_host, Config::server_port, "/ws/ira"); 
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(3000);
}

void loop() {
    webSocket.loop();

    if (ws_connected) {
        unsigned long currentMillis = millis();
        if (currentMillis - prevMillis >= blinkInterval) {
            prevMillis = currentMillis;
            ledState = !ledState;
            digitalWrite(Config::status_led, ledState); 
        }
    } else {
        digitalWrite(Config::status_led, WiFi.status() == WL_CONNECTED ? HIGH : LOW); 
    }

    if (ws_connected && !is_playing) {
        uint8_t mic_buffer[BLOCK_SIZE];
        size_t bytes_read = 0;
        i2s_read(I2S_NUM_1, &mic_buffer, BLOCK_SIZE, &bytes_read, 10);
        if (bytes_read > 0) {
            webSocket.sendBIN(mic_buffer, bytes_read);
        }
    }
}

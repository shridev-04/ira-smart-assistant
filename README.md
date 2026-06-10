# I.R.A. (Intelligent Real-time Automation) Smart Assistant

I.R.A. is an advanced, ultra-low latency, voice-controlled smart home assistant powered by an **ESP32** microcontroller and Google's cutting-edge **Gemini 3.1 Flash Live (Preview) API**. 

Unlike traditional smart assistants that rely on heavy cloud platforms or simple text commands, this project implements a full **bidirectional audio streaming pipeline** over WebSockets, enabling full duplex conversational AI and simultaneous 4-channel industrial hardware appliance control with non-volatile memory persistence.

---

## 🚀 Key Features & Architecture

### 1. Hardware & Embedded Layer (ESP32 Node)

* **Full-Duplex Audio Pipeline:** Uses dual I2S channels. One channel captures raw audio data at 16kHz (16-bit) via an I2S Microphone, and the other reproduces 24kHz audio via an I2S Speaker driver.

* **DRY Configuration Panel:** Flawlessly synchronized 4-channel appliance relay allocations directly matching the backend registers.

* **EEPROM Cache State Engine:** Built-in power-cut resilience. The appliance states (ON/OFF) are committed into the physical non-volatile sector of the ESP32. Upon unexpected reboot, it auto-restores the last state.

* **Visual Status Matrix:** Implements real-time telemetry blinking sequences to show connection status (Blinking = WebSocket Synchronized, Solid = Local WiFi Stable).

### 2. Backend Orchestration Layer (FastAPI Server)

* **Neural Stream Processor:** Built using Python's FastAPI framework with an asynchronous execution loop (`asyncio.gather`) to handle concurrent streams.

* **Live Connect Protocol Wrapper:** Hooks directly into the `gemini-3.1-flash-live-preview` API protocol, supporting continuous audio modalities with the "Aoede" prebuilt voice config.

* **Intelligent Token Routing Sub-Routine:** Processes the live model turn-by-turn. If a Hinglish verbal trigger is matched (e.g., *"light band karo"*), the server seamlessly embeds a custom token inside the text payload and dispatches it over WebSockets to trigger the physical relay on the hardware node.

* **Acoustic Filter & Wake-Word Isolation:** Prompt-engineered architecture to prevent accidental triggers in noisy environments, keeping the device in silent standby unless the wake-word "Ira" is distinctly addressed.

---

## 🛠️ System Design & Data Flow

[User Audio] ──> (I2S Mic) ──> [ESP32 Client] ──(Binary WS)──> [FastAPI Server]
│
(Google GenAI Live)
│
▼
[Relay Trigger] <──(Text Token)── [ESP32 Client] <──(Binary WS)── [Gemini AI Engine]
[Audio Response] <── (I2S Speaker) ┘

---

## ⚙️ Technical Specifications & Optimization Focus

During the 20-day development lifecycle, intense focus was given to optimization and solving architectural constraints:

* **Memory Management:** Carefully sized DMA buffers (`dma_buf_count = 8`, `dma_buf_len = 128` for Tx) to maximize the limited RAM headroom available on the ESP32 without causing audio cracking or stack overflows.

* **Chunk Streaming:** Chunked the binary audio stream into strict `512-byte` blocks with precisely calculated sleep timers (`0.008s`) to sustain smooth, non-blocking playback.

* **Ping-Pong Telemetry:** Implemented custom WebSocket keep-alive handshakes to ensure the hosted server does not prematurely recycle the execution process.

---

## ⚠️ Challenges Faced & Current Project Status

While the local WebSocket server-client synchronization between the ESP32 and the FastAPI server was fully achieved, the project hit a roadblock in the production/cloud staging environment:

1.  **Preview API Instability:** The `gemini-3.1-flash-live-preview` protocol is in an early development phase. During testing, the Live session connection repeatedly ran into handshake termination and unexpected token expiration errors (`Connection Closed`), causing intermittent service breaks

2.  **Hosting Environment Timeouts:** Deploying a real-time persistent audio streaming protocol on serverless/free cloud hosting tiers (like Render or Railway) introduced network jitter and cold-start delays that clashed with the Gemini Live protocol's aggressive timeout rules.

**Current Verdict:** The hardware-to-backend communication matrix is **100% complete and functionally verified**. The project is currently paused at the production API integration layer, awaiting stable non-preview releases of the Google Live API endpoints.

---

## 🎓 Author Note & Intellectual Property
* **Chief Architect:** Shridev Kumar
* **Affiliation:** First-Year Student (Electronics Engineering), Government Polytechnic Gulzarbagh, Patna-07.
* **Co-developer:** AI Assistant (Collaborative logic structure, error resolution, and baseline code compilation).

*This project was built entirely out of passion to explore real-time edge computing combined with generative intelligence.*

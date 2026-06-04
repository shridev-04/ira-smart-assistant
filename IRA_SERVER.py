# ==================================================================================
#  ██╗██████╗  █████╗      █████╗ ██╗
#  ██║██╔══██╗██╔══██╗    ██╔══██╗██║
#  ██║██████╔╝███████║    ███████║██║
#  ██║██╔══██╗██╔══██║    ██╔══██║██║
#  ██║██║  ██║██║  ██║    ██║  ██║██║
#  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝    ╚═╝  ╚═╝╚═╝
# ==================================================================================
#  [PROJECT METADATA & INTELLECTUAL PROPERTY REGISTER]
# ==================================================================================
#  📌 PROJECT NAME:      I.R.A. Central Processing Server Core
#  📌 CHIEF ARCHITECT:   Shridev Kumar
#  📌 SYSTEM UTILITY:    FastAPI Engine Layer for Real-Time Neural Stream Processing
#  📌 MODEL COGNITION:   Gemini-3.1-Flash-Live API Protocol Wrapper
# ==================================================================================
#  © 2026 Shridev Kumar. Co-developed with AI. All Rights Reserved.
#  This dynamic configuration matrix compiles prompts instantly based on user control logs.
# ==================================================================================

import os
import json
import asyncio
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from google import genai
from google.genai import types

# ==================================================================================
# ⭐⭐⭐ [MASTER CONTROL PANEL] ⭐⭐⭐
# ==================================================================================
CONFIG = {
    # 1. Artificial Intelligence Access Credentials
    "GEMINI_API_KEY": os.environ.get("GEMINI_API_KEY", "YOUR_GEMINI_API_KEY_HERE"),      
    
    # 2. Identity Customization
    "AI_NAME": "Ira",                                  
    "CREATOR_TITLE": "Shridev Kumar",                  
    
    # 3. Geo-Localization Anchor
    "TARGET_CITY": "Patna, Bihar",                     
    
    # 4. Behavioral Attributes & Prompt Injection Nature
    "AI_BEHAVIOR_DESCRIPTION": (
        "You are highly intelligent, lightning-fast, and function as a lively, friendly companion. "
        "Your responses must be ultra-short, highly conversational, and formatted entirely in crisp Hinglish text."
    ),

    # -----------------------------------------------------------------------------
    # ⚙️ MULTI-APPLIANCE CONFIGURATION DIRECTORY (4-Channels Dynamic Variable Array)
    # -----------------------------------------------------------------------------
    "DEV_1_NAME": "television",               
    "DEV_1_ON_CMD": "tv chalao",     
    "DEV_1_OFF_CMD": "tv band karo",   

    "DEV_2_NAME": "bedroom light",       
    "DEV_2_ON_CMD": "light on karo",   
    "DEV_2_OFF_CMD": "light band karo", 

    "DEV_3_NAME": "ceiling fan",        
    "DEV_3_ON_CMD": "pankha chalao",     
    "DEV_3_OFF_CMD": "pankha band karo",    

    "DEV_4_NAME": "air conditioner",   
    "DEV_4_ON_CMD": "ac on karo",      
    "DEV_4_OFF_CMD": "ac band karo",    

    # -----------------------------------------------------------------------------
    # 🔒 FIXED HANDSHAKE TOKENS (Do NOT touch, matches ESP32 physical register)
    # -----------------------------------------------------------------------------
    "T_1_ON": "R1_ON_SIGNAL",  "T_1_OFF": "R1_OFF_SIGNAL",
    "T_2_ON": "R2_ON_SIGNAL",  "T_2_OFF": "R2_OFF_SIGNAL",
    "T_3_ON": "R3_ON_SIGNAL",  "T_3_OFF": "R3_OFF_SIGNAL",
    "T_4_ON": "R4_ON_SIGNAL",  "T_4_OFF": "R4_OFF_SIGNAL",
}

# ==================================================================================
# [DYNAMIC CONTEXT COMPILER ENGINE - INTERNAL MATRIX OPERATIONS DO NOT ALTER]
# ==================================================================================
os.environ["GEMINI_API_KEY"] = CONFIG["GEMINI_API_KEY"]
MODEL_NAME = "gemini-3.1-flash-live-preview"
client = genai.Client()

# System Instruction Pipeline Auto-Compiler
SYSTEM_INSTRUCTION = (
    f"You are {CONFIG['AI_NAME']}, a real-time smart home companion engineered and created by {CONFIG['CREATOR_TITLE']}.\n\n"
    f"AI PERSONALITY & TONAL SPECS:\n- {CONFIG['AI_BEHAVIOR_DESCRIPTION']}\n\n"
    
    "LANGUAGE AND SCRIPT INTERFACES:\n"
    "- Always communicate using standard conversational Hinglish via the Latin/Roman text script (WhatsApp typing style).\n"
    "- Under no circumstances should you ever render or output Devanagari characters.\n\n"
    
    "STRICT VOICE FILTER & WAKE-WORD PROTOCOLS:\n"
    f"1. You are operating in a room where multiple people might be talking to each other. YOU MUST REMAIN COMPLETELY SILENT and ignore all audio input UNLESS the user explicitly addresses you by your name '{CONFIG['AI_NAME']}' or says 'Hey {CONFIG['AI_NAME']}'.\n"
    f"2. If you hear any background conversation, chatter, or sentences that do not start with or explicitly name '{CONFIG['AI_NAME']}', DO NOT generate any audio output, DO NOT send any tokens, and DO NOT interrupt. Act as if you are in standby mode.\n"
    f"3. The moment you detect your name '{CONFIG['AI_NAME']}' associated with a command, instantly process the request, embed the correct hardware token, and reply briefly.\n\n"
    
    "INDUSTRIAL INTERNET OF THINGS INTERACTION ARCHITECTURE:\n"
    f"You possess direct root access over exactly 4 connected household appliances: '{CONFIG['DEV_1_NAME']}', '{CONFIG['DEV_2_NAME']}', '{CONFIG['DEV_3_NAME']}', and '{CONFIG['DEV_4_NAME']}'.\n\n"
    
    "INTELLIGENT AMBIGUITY RESOLUTION PROTOCOLS:\n"
    "1. If multiple connected devices belong to the exact same category and the user issues an ambiguous/generic call such as 'light jala do' WITHOUT defining the name clearly, DO NOT EMBED ANY TOKEN. Instead, respond immediately with a question asking for clarity.\n"
    "2. If all configured devices have fully unique categorical names, execute the token immediately without asking clarifying questions.\n"
    "3. If the user commands an absolute global execution block (e.g., 'turn off everything'), you are authorized to embed ALL respective matching tokens together inside your text response string.\n\n"
    
    "EXPLICIT TOKEN EXECUTION MAPPING SUB-ROUTINE:\n"
    f"- Device 1 [{CONFIG['DEV_1_NAME']}]: Catch phrase '{CONFIG['DEV_1_ON_CMD']}' -> write '{CONFIG['T_1_ON'].lower()}'. Catch phrase '{CONFIG['DEV_1_OFF_CMD']}' -> write '{CONFIG['T_1_OFF'].lower()}'.\n"
    f"- Device 2 [{CONFIG['DEV_2_NAME']}]: Catch phrase '{CONFIG['DEV_2_ON_CMD']}' -> write '{CONFIG['T_2_ON'].lower()}'. Catch phrase '{CONFIG['DEV_2_OFF_CMD']}' -> write '{CONFIG['T_2_OFF'].lower()}'.\n"
    f"- Device 3 [{CONFIG['DEV_3_NAME']}]: Catch phrase '{CONFIG['DEV_3_ON_CMD']}' -> write '{CONFIG['T_3_ON'].lower()}'. Catch phrase '{CONFIG['DEV_3_OFF_CMD']}' -> write '{CONFIG['T_3_OFF'].lower()}'.\n"
    f"- Device 4 [{CONFIG['DEV_4_NAME']}]: Catch phrase '{CONFIG['DEV_4_ON_CMD']}' -> write '{CONFIG['T_4_ON'].lower()}'. Catch phrase '{CONFIG['DEV_4_OFF_CMD']}' -> write '{CONFIG['T_4_OFF'].lower()}'.\n\n"
    
    f"GEOGRAPHIC GEOMETRY CAPTURE:\n- Provide localized contexts matching the environment coordinates of: {CONFIG['TARGET_CITY']}."
)

app = FastAPI()

@app.websocket("/ws/ira")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    print(f"[CENTRAL ROUTER] Synchronization Active. Running Instance: {CONFIG['AI_NAME']} under {CONFIG['CREATOR_TITLE']}.")
    
    config = types.LiveConnectConfig(
        system_instruction=types.Content(parts=[types.Part.from_text(text=SYSTEM_INSTRUCTION)]),
        response_modalities=["AUDIO"],
        speech_config=types.SpeechConfig(
            voice_config=types.VoiceConfig(
                prebuilt_voice_config=types.PrebuiltVoiceConfig(voice_name="Aoede")
            )
        )
    )

    audio_queue = asyncio.Queue()
    clear_stream = False  

    try:
        async with client.aio.live.connect(model=MODEL_NAME, config=config) as session:
            async def receive_from_esp32_and_send_to_gemini():
                nonlocal clear_stream
                while True:
                    try:
                        message = await websocket.receive()
                        if "bytes" in message:
                            await session.send(input=message["bytes"], end_of_turn=False)
                        # ⭐ PING-PONG handler jo ESP32 connection ko zinda rakhega
                        elif "text" in message and message["text"] == "PING":
                            await websocket.send_text("PONG")
                    except WebSocketDisconnect:
                        break
                    except:
                        break

            async def receive_from_gemini():
                while True:
                    try:
                        async for response in session.receive():
                            server_content = getattr(response, 'server_content', None)
                            if server_content:
                                if getattr(server_content, 'turn_complete', False):
                                    await websocket.send_text("AUDIO_END")
                                    
                                if server_content.model_turn:
                                    for part in server_content.model_turn.parts:
                                        if part.inline_data and part.inline_data.data:
                                            await audio_queue.put(part.inline_data.data)
                                            
                                        if part.text:
                                            text_lower = part.text.lower()
                                            for state in ['_ON', '_OFF']:
                                                for i in range(1, 5):
                                                    token_key = f"T_{i}{state}"
                                                    if CONFIG[token_key].lower() in text_lower:
                                                        print(f"[ROUTING CONTROLLER CH {i}] Dispatched Data Token -> {CONFIG[token_key]}")
                                                        await websocket.send_text(CONFIG[token_key])
                        await asyncio.sleep(0.05)
                    except:
                        await asyncio.sleep(0.5)

            async def send_to_esp32_streamer():
                nonlocal clear_stream
                while True:
                    try:
                        # ⭐ FIXED: Timeout checking lagaya taaki khali baithe rehne par loop crash na ho
                        try:
                            audio_data = await asyncio.wait_for(audio_queue.get(), timeout=1.0)
                        except asyncio.TimeoutError:
                            continue 

                        chunk_size = 512
                        for i in range(0, len(audio_data), chunk_size):
                            if clear_stream:
                                break
                            chunk = audio_data[i:i + chunk_size]
                            try:
                                await asyncio.wait_for(websocket.send_bytes(chunk), timeout=0.05)
                                await asyncio.sleep(0.008)
                            except asyncio.TimeoutError:
                                break 
                        audio_queue.task_done()
                    except:
                        await asyncio.sleep(0.1)

            await asyncio.gather(
                receive_from_esp32_and_send_to_gemini(),
                receive_from_gemini(),
                send_to_esp32_streamer()
            )

    except WebSocketDisconnect:
        print("[ROUTER RECYCLE LOG] Process terminated. Socket cleaned.")
        

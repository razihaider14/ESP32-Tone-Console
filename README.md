# ESP32 Tone Console

### Bluetooth-controlled buzzer sequencing for active and passive buzzers
> A tiny ESP32 firmware that lets you build custom beep/tone sequences entirely over Bluetooth serial, from any Bluetooth terminal app on your phone.

---

## Features

- **Two Types**: active buzzer (on/off timing) and passive buzzer (on/off timing + per-tone frequency)
- **Build Sequence**: specify number of tones, each with its own timing (and pitch, for passive buzzer), then a repeat count
- **Cancel Anytime**: `stop` during setup returns to idle instead of finishing the prompts
- **Bluetooth Serial**: works with any generic BT terminal app, no companion app needed

---

## Hardware

- ESP32
- Active Buzzer
- Passive Buzzer

---

## Wiring

| Component | ESP32 Pin |
|---|---|
| Active Buzzer | 25 |
| Passive Buzzer | 26 |

---

## Command Reference

| Command | Description |
|---|---|
| `active` | Start building a sequence for the active buzzer |
| `passive` | Start building a sequence for the passive buzzer |
| `stop` | Stop playback immediately, or cancel setup and return to idle |
| `help` | List available commands |

---

## Example Session

```
> active
Active buzzer selected.
How many tones?
> 2
Tone 1/2, how long ON (ms)?
> 200
Tone 1/2, how long OFF (ms)?
> 200
Tone 2/2, how long ON (ms)?
> 400
Tone 2/2, how long OFF (ms)?
> 300
How many times to repeat?
> 3
Playing... (send 'stop' to cancel).
Done.
```

```
> passive
Passive buzzer selected.
How many tones?
> 2
Tone 1/2, what frequency (Hz)?
> 500
Tone 1/2, how long ON (ms)?
> 1000
Tone 1/2, how long OFF (ms)?
> 1000
Tone 2/2, what frequency (Hz)?
> 1500
Tone 2/2, how long ON (ms)?
> 200
Tone 2/2, how long OFF (ms)?
> 400
How many times to repeat?
> 3
Playing... (send 'stop' to cancel).
Done.
```

---

## Setup
### 1. Flash
Open `firmware/firmware.ino` in Arduino IDE. No external libraries are required. Select your board and port, then flash the code into the ESP32.

### 2. Connect
Pair your phone's Bluetooth with `ESP32-Tone-Console`. Open any BT serial terminal and connect to the same, and start typing commands.

---
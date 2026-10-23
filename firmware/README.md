# Firmware (ESP32)

Two PlatformIO projects. Open each folder **on its own** in VS Code with the PlatformIO extension, and upload with the **→ Upload** arrow (never F5 / Run and Debug).

| Folder | What it does |
| --- | --- |
| `rfid_tester/` | Reads a tag and prints its UID on the Serial Monitor. Use it to check the wiring. |
| `rfid_reader/` | Connects to Wi-Fi and sends every tag to Firebase Firestore (`readers/counter-1`). This is the one the app uses. |

## Before uploading `rfid_reader`

1. Copy `rfid_reader/src/secrets.example.h` to `rfid_reader/src/secrets.h`.
2. Fill in your Wi-Fi name and password, Firebase Project ID and Web API Key.

`secrets.h` is in `.gitignore`, so it is never uploaded to GitHub.

Wiring: see [`../hardware/README.md`](../hardware/README.md).

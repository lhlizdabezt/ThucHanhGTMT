# Computer Interfacing and BLE Data Acquisition Lab

<p align="center">
  <a href="https://github.com/lhlizdabezt/ThucHanhGTMT/releases/latest"><img src="https://img.shields.io/github/v/release/lhlizdabezt/ThucHanhGTMT?style=for-the-badge&logo=github&label=Release" alt="Latest release for ThucHanhGTMT" /></a>
  <a href="https://github.com/lhlizdabezt/ThucHanhGTMT/tags"><img src="https://img.shields.io/github/v/tag/lhlizdabezt/ThucHanhGTMT?style=for-the-badge&logo=git&label=Tag" alt="Latest tag for ThucHanhGTMT" /></a>
  <img src="https://img.shields.io/badge/Portfolio-English%20review%20ready-0f766e?style=for-the-badge" alt="English portfolio ready" />
</p>

<p align="center">
  <img src="assets/portfolio-motion.svg" alt="Animated engineering portfolio visual for ThucHanhGTMT" width="100%" />
</p>

## Overview

`ThucHanhGTMT` documents a computer interfacing lab where an AHT20 temperature and humidity sensor is read through I2C on a Silicon Labs BLE SoC, displayed locally on an LCD, advertised through BLE payload data, and logged on a Windows host through UART/VCOM and SQLite.

The repository is written for HR screening and engineering review. It keeps the project scope clear: this is a student lab and portfolio evidence package, not a commercial data-acquisition product.

## Repository Status

| Field | Status |
|---|---|
| Repository | [github.com/lhlizdabezt/ThucHanhGTMT](https://github.com/lhlizdabezt/ThucHanhGTMT) |
| Portfolio category | Computer interfacing, embedded data acquisition, BLE sensor logging |
| Review status | Portfolio-ready academic lab snapshot |
| Latest release | [GitHub Releases](https://github.com/lhlizdabezt/ThucHanhGTMT/releases/latest) |
| Version tags | [Repository tags](https://github.com/lhlizdabezt/ThucHanhGTMT/tags) |
| Visual policy | English labels, ASCII-safe SVG text, no moving connector-line patterns |
| Hardware dependency | Silicon Labs BLE SoC, AHT20 sensor, LCD and UART/VCOM connection |
| Host dependency | Windows C build environment, SQLite source files and an available COM port |

## System Flow

| Stage | Evidence in Repository | Engineering Point |
|---|---|---|
| Sensor input | `LONG1_2-20260513T142225Z-3-001/LONG1_2/aht20.c` | Starts and reads AHT20 temperature and humidity measurements over I2C |
| Embedded processing | `LONG1_2-20260513T142225Z-3-001/LONG1_2/app.c` | Converts sensor values, updates advertising data and prints UART log frames |
| Local display | `LONG1_2-20260513T142225Z-3-001/LONG1_2/lcd_display.c` | Shows measured data on the LCD without relying on host-side parsing |
| BLE payload | `LONG1_2-20260513T142225Z-3-001/LONG1_2/custom_adv.c` | Packs manufacturer-specific BLE advertising bytes with BCD sensor values |
| Host logging | `TH GTMT-20260513T142230Z-3-001/TH GTMT/TEST.c` | Reads UART/VCOM frames and stores timestamped records in SQLite |
| Portfolio visual | `assets/gtmt-data-flow.gif` | Shows the sensor-to-database flow without moving connector lines |

## Evidence Highlights

- AHT20 sensor acquisition through I2C.
- Silicon Labs BLE SoC firmware path with LCD display, BLE advertising and UART/VCOM logging.
- Windows-side C logger using SQLite for timestamped sensor records.
- Release-backed GIF and SVG assets designed for GitHub README rendering.
- Reports and slide material preserved as reviewable course artifacts.

## Repository Structure

| Path | Purpose |
|---|---|
| `assets/` | README-ready SVG and GIF visuals for portfolio review |
| `LONG1_2-20260513T142225Z-3-001/LONG1_2/` | Silicon Labs BLE SoC firmware project and sensor/display source files |
| `TH GTMT-20260513T142230Z-3-001/TH GTMT/` | Windows C/SQLite UART logger source tree |
| `22207056_report_DoAn.pdf` | Project report artifact |
| `22207056_report_DoAn (2).pptx` | Project presentation artifact |
| `22207056_report_lab6.pdf` | Supporting lab report artifact |
| `scripts/render_gtmt_data_flow.py` | Repeatable renderer for the English GIF visual |

## How to Review

1. Start with this README to understand the project boundary and evidence map.
2. Open the latest release to confirm the preserved review snapshot and release assets.
3. Inspect `app.c`, `aht20.c`, `lcd_display.c` and `custom_adv.c` for the embedded sensor path.
4. Inspect `TEST.c` for the Windows host logger, COM-port handling and SQLite insert flow.
5. Check `assets/gtmt-data-flow.gif` and `assets/portfolio-motion.svg` for the portfolio-facing visual summary.
6. Use the reports for course context, diagrams and lab deliverables.

## Run or Inspect Locally

The firmware side requires the Silicon Labs toolchain and compatible hardware. The host logger can be inspected without hardware, but live logging requires the board to stream UART frames.

### Firmware Review Path

```text
Open LONG1_2-20260513T142225Z-3-001/LONG1_2/LONG1_2.slcp in Simplicity Studio.
Review app.c for the timer, BLE event and UART log flow.
Review aht20.c for the I2C command sequence and conversion formula.
Review lcd_display.c for local display formatting.
```

### Host Logger Build Path

```powershell
cd "TH GTMT-20260513T142230Z-3-001\TH GTMT"
gcc TEST.c sqlite3.c -o App.exe
```

Before running the logger, edit `COM_PORT_NAME` in `TEST.c` to match the board's actual COM port. The source currently uses a Windows serial path format such as `\\\\.\\COM10`.

```powershell
.\App.exe
```

## FAQ

| Question | Answer |
|---|---|
| Is this a production data-acquisition system? | No. It is a lab workflow and portfolio evidence package. |
| What should a reviewer focus on first? | The AHT20 I2C read flow, BLE advertising payload, UART/VCOM logger and SQLite insert path. |
| Why are the visuals English-only? | The repository is intended for international HR and engineering review, and SVG/GIF text must avoid mojibake or unreadable labels. |
| Why avoid connector-line animation? | GitHub profile cards can become visually noisy at small widths. The current visuals use cards and pulses instead. |
| Can the project run without hardware? | The source can be reviewed and parts can be built, but live sensor logging requires the board, sensor, LCD and COM port. |

## Scope and Boundaries

This repository demonstrates embedded firmware, interface debugging and host-side sensor logging in a lab setting. It does not claim industrial calibration, production security hardening, cloud telemetry, field deployment, or long-term reliability testing.

## Professional Links

| Channel | Link |
|---|---|
| GitHub profile | [github.com/lhlizdabezt](https://github.com/lhlizdabezt) |
| Resume | [Luong Hai Long CV](https://github.com/lhlizdabezt/lhlizdabezt/blob/main/resume/Luong_Hai_Long_CV.pdf) |
| LinkedIn | [linkedin.com/in/lhlizdabezt](https://www.linkedin.com/in/lhlizdabezt) |
| Work email | [luonghailong.work@gmail.com](mailto:luonghailong.work@gmail.com) |
| Student email | [22207056@student.hcmus.edu.vn](mailto:22207056@student.hcmus.edu.vn) |
| Phone | [+84 988 114 708](tel:+84988114708) |
| Facebook | [facebook.com/wageseadrake](https://www.facebook.com/wageseadrake) |
| Instagram | [instagram.com/lhlizdabezt](https://www.instagram.com/lhlizdabezt) |
| YouTube | [youtube.com/@lhlizdabezt](https://www.youtube.com/@lhlizdabezt) |
| TikTok | [tiktok.com/@wageseadrake](https://www.tiktok.com/@wageseadrake) |

## Writing Standard

The README follows an evidence-first style: direct technical nouns, clear academic boundaries, release-backed artifacts, reviewable source paths and no inflated claims beyond what the repository can support.

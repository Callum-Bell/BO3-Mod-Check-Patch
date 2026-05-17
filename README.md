<h1 align="center">BO3 Mod Check Bypass</h1>
<p align="center"><b>Bypass BO3 Mod Integrity Check (Rank Up Plugin)</b></p>

<p align="center">
    <a href="https://dl.insanux.com/?mod=Ym2UJAN1uSlsIMu">
        <img alt="Download" src="https://img.shields.io/badge/Download-Here-blue?style=for-the-badge">
    </a>
</p>

## 📦 Overview
**BO3 Mod Check Bypass** is a plugin for *Call of Duty: Black Ops III* that patches the game in-memory to **bypass the mod integrity check**, allowing you to gain XP and rank up while playing with mods loaded.

## 🧩 Installation

### 1. Install T7Patch
T7Patch is required for this plugin to work correctly.

- Download the latest release from [T7Patch - GitHub](https://github.com/Scroptss/T7Patch/releases)
- Open Steam, right-click BO3 and go to **Manage > Browse Local Files**
- Extract `t7patch.dll`, `t7patchloader.dll` and `t7patch.conf` into your BO3 folder
- Rename `t7patchloader.dll` to `t7patchloader.asi`
- Open `t7patch.conf` and set a username and network password

### 2. Install BO3 Mod Check Bypass
- Extract all files from `BO3-Modded-Rankup-Plugin.zip` into your BO3 folder

### 3. Launch
- Launch BO3 as normal via Steam
- XP and rank progression will now work in modded lobbies

## 🖥 How It Works
- Injects into the game via a `.asi` plugin loader.
- Hooks the mod check function and all known call sites at runtime.
- Redirects the mod check to always return `false` (no mods loaded) for rank/XP purposes.
- Modded maps and content continue to work normally.

## ⚙️ Features
- **Compatible with the Feb 2026 game update**.
- **Works alongside T7Patch**.

## 👥 Credits
- [InsaneCallum](https://github.com/Callum-Bell)
- [Scroptss](https://github.com/Scroptss) — Updated offsets for Feb 2026
- [Baldywaldy09](https://github.com/Baldywaldy09)
- BOIII Client - Resources and reference

## ⚠️ Disclaimer
This project is provided as-is for educational and personal use. The author is not responsible for any bans, account issues, or unintended side effects that may arise from using this plugin. Use at your own risk. (We have had no reported issues)

# Roguelike Dungeon Crawler — Terminal Edition

Game roguelike 2D berbasis terminal, dibangun sepenuhnya dengan C++17 murni tanpa library eksternal.

## Fitur

- **3 Lantai Dungeon**: Easy → Medium → Boss, dengan kesulitan meningkat
- **Procedural Map Generation**: Room-based dungeon yang unik setiap game baru
- **Combat System**: Pertarungan berbasis giliran dengan formula damage yang jelas
- **Enemy AI**: Enemy mengejar pemain, boss dengan AI agresif
- **Loot System**: Drop rate probabilistik terpusat untuk enemy dan boss
- **Weapon System**: 4 tier rarity (Common, Rare, Epic, Legendary) + sistem upgrade
- **Inventory System**: Simpan dan gunakan item (potion, dll)
- **Save/Load System**: Format teks terstruktur, validasi file lengkap
- **Checkpoint System**: Auto-save saat menyentuh checkpoint, respawn setelah game over
- **ANSI Color**: Pewarnaan terminal untuk entity, loot, dan tile

## Kontrol

| Tombol | Aksi |
|--------|------|
| `W` | Gerak atas |
| `A` | Gerak kiri |
| `S` | Gerak bawah |
| `D` | Gerak kanan |
| `I` | Buka inventory |
| `E` | Buka menu senjata |
| `U` | Buka menu upgrade senjata |
| `P` | Save game manual |
| `Q` | Quit |
| `0-9` | Pilih item / senjata di menu |
| `ESC` | Tutup menu |

## Simbol Map

| Simbol | Arti |
|--------|------|
| `P` | Player (hijau) |
| `E` | Enemy (merah) |
| `B` | Boss (magenta) |
| `G` | Gold (kuning) |
| `W` | Weapon Drop (cyan) |
| `C` | Checkpoint (biru) |
| `>` | Stairs ke lantai berikutnya |
| `+` | Potion (hijau) |
| `#` | Wall |
| `.` | Floor |

## Build

### Linux

```bash
# Debug build
make

# Release build (optimized)
make release

# Build dan jalankan
make run

# Bersihkan build artifacts
make clean
```

### Windows (MinGW)

```batch
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -o roguelike.exe ^
    src/utility/random_manager.cpp ^
    src/world/map.cpp src/world/map_generator.cpp ^
    src/inventory/item.cpp src/inventory/inventory.cpp ^
    src/weapon/weapon.cpp src/weapon/weapon_upgrade.cpp ^
    src/entity/entity.cpp src/entity/player.cpp ^
    src/entity/enemy.cpp src/entity/boss.cpp ^
    src/ai/ai_controller.cpp ^
    src/combat/combat_system.cpp ^
    src/loot/loot_config.cpp src/loot/loot_table.cpp ^
    src/save/serializer.cpp src/save/save_manager.cpp ^
    src/checkpoint/checkpoint_system.cpp ^
    src/core/input_handler.cpp src/core/renderer.cpp ^
    src/core/game.cpp src/main.cpp
```

### Windows (MSVC)

```batch
cl /std:c++17 /W4 /EHsc /Fe:roguelike.exe src\**\*.cpp src\main.cpp
```

## Struktur Proyek

```
src/
├── main.cpp            # Entry point
├── core/               # Game loop, input, rendering
├── world/              # Map, tiles, procedural generation
├── entity/             # Player, Enemy, Boss
├── combat/             # Damage calculation, rewards
├── ai/                 # Enemy pathfinding & behavior
├── loot/               # Drop rates & loot tables
├── inventory/          # Items & inventory management
├── weapon/             # Weapons, rarity, upgrades
├── save/               # Save/load serialization
├── checkpoint/         # Auto-save checkpoints
└── utility/            # Constants, types, random manager
```

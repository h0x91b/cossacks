# Механика захвата зданий — Cossacks: Back to War 1.42

## Логика кода

### Основные функции

| Функция | Файл | Строка | Описание |
|---|---|---|---|
| `CheckCapture(OBJ)` | NewMon.cpp | 14854 | Основная проверка захвата, вызывается каждый кадр |
| `TestCapture(OBJ)` | NewMon.cpp | 14783 | Проверка для UI (показ команды захвата) |
| `SearchCapturers(cell, mask)` | NewMon.cpp | 14656 | Поиск вражеских юнитов-захватчиков |
| `SearchProtectors(cell, mask)` | NewMon.cpp | 14681 | Поиск союзных юнитов-защитников |

### Условие вызова (`NewMon.cpp:7938`)

```cpp
if ((tmtmt & 31) == mm && (OB->newMons->Capture || !OB->Ready))
    CheckCapture(OB);
```

Два пути для захвата:
1. **Флаг `CAPTURE`** в файле данных юнита — здание может быть захвачено в любом состоянии
2. **`!Ready`** — любое здание может быть захвачено, пока **не достроено**

### Процесс захвата

1. Проверка правил `CaptState` (настройка игры)
2. Поиск вражеских юнитов в радиусе 5x5 клеток (дистанция < 250*16 = 4000 единиц)
3. Захватчик **не должен** иметь флаг `Capture` и не должен быть заблокирован (`LockType`)
4. Поиск союзных защитников в радиусе 7x7 клеток — если найдены, захват отменяется
5. Передача владения: смена нации, перерегистрация на карте

### Ключевые файлы

- `src/Main executable/NewMon.cpp` — основная логика (строки 14656–15167)
- `src/Main executable/MapDiscr.h` — определения ID типов (строки 1979–2070), флаг `Capture` (строка 480)
- `src/Main executable/3DRandMap.cpp` — глобальная переменная `CaptState` (строка 3318)
- `src/Main executable/Interface.cpp` — чтение настройки из опций (строка 11574)
- `src/Main executable/Brigade.cpp` — захват шахт бригадой (строки 1148–1412)

---

## Здания С флагом CAPTURE

Эти здания захватываются **всегда** (когда достроены и при наличии вражеских юнитов рядом без союзных защитников).

| Тип здания | USAGE | Файлы данных (по нациям) | Кол-во |
|---|---|---|---|
| Академия | - | AKA* (AU,BA,BR,DA,FR,GE,HO,PI,PL,PO,R,SA,SP,SV,U,VE + SWZ,VNG) | 18 |
| Артиллерийское депо | - | ART* + ADET (все нации + SWZ,VNG) | 17 |
| Кузница | - | KUZ* + KUT (все нации + SWZ,VNG) | 19 |
| Ратуша / Центр | CENTER | CE* + CAL,CAU + RUS,EVR + SWZ/VNGCEN | 20 |
| Ферма / Дом | FARM | DO* + DAL,DAU + DPL,DPO,DPR,DSW + EUD,MDO,KPU,UKD + SWZ/VNGDOM | 22 |
| Мельница | MELNICA | MEL, MELN2, MT | 3 |
| Склад | SKLAD | SKL1, SKL2, SKL3, SKL4 | 4 |
| Шахта | MINE | SHA, SHAFE, SHAUG | 3 |
| Рынок | - | RINN, RINR, RINS, RIT | 4 |
| Минарет | - | MIT | 1 |
| Казарма (только BR, FR) | - | KAZBR, KAZFR | 2 |
| Пушка | PUSHKA | PUS | 1 |
| Мортира | MORTIRA | MOR | 1 |
| Супер-мортира | SUPERMORTIRA | MORBIG | 1 |
| Многостволка | MCANNON | PSM | 1 |
| Крестьяне | PEASANT | KRE, KRS, KRT, KRP, KR3, KPO | 6 |
| Прочее | - | GRUZZ | 1 |

**Итого: ~124 записи** (по всем нациям, включая варианты для каждой нации)

---

## Здания БЕЗ флага CAPTURE

Эти здания захватываются **только пока строятся** (`!Ready`). После достройки — не захватываются.

| Тип здания | USAGE | Файлы данных | Кол-во |
|---|---|---|---|
| Казарма 17 века | - | KA1* (AU,BA,DA,GE,HO,PI,PL,PO,SA,SP,SV,VE) | 12 |
| Казарма 18 века | - | KA2* (AU,BA,BR,DA,FR,GE,HO,PI,PL,PO,RU,SA,SP,SV,VE) | 15 |
| Конюшня | - | KON* (AU,BA,BR,DA,FR,GE,HO,PI,PL,PO,SA,SP,SV,U,VE + SWZ,VNG) + KOT | 17 |
| Церковь / Храм | - | HRA* (AU,BA,BR,DA,GE,HO,PI,PL,PO,SA,SP,SV,VE) + HRA,CERU,CRP | 16 |
| Дипломатический центр | - | DIP* (AU,BA,BR,DA,FR,GE,HO,PI,PL,PO,R,SA,SP,SV,T,U,VE + SWZ,VNG) | 20 |
| Башня | TOWER | TOW, TO2, TO3, BGAUZ, BGAUZ2, ATL_B1 | 6 |
| Порт | PORT | PORT, PORE, PORPO, PORR, PORU | 5 |
| Стены | - | WALL_EV, WALL_KR, WALL_TU | 3 |
| Прочие казармы | - | BAT, RST, UKS, SKA | 4 |
| Мечеть / Собор | - | MET, NOTRDAM | 2 |
| Декоративные объекты | - | MOST, MOST2, KOLODEC*, SUNDUK*, MUSOR, PALATKA, KOTEL, URTA | ~10 |
| Корабли (здания-спавнеры) | FREGAT/GALERA/etc. | FREG, FRN, SHE, GAL, GLS, KECH, LINK, VIC, YAH, KUTT, YAHTU | 11 |
| Гренадёры (здания?) | GRENADER | GRE, GREDIP, MDA, SAG, TAT, SWZEGR | 6 |
| Порты (RU/TU/UK) | - | PORTRU, PORTTU, PORTUK | 3 |

---

## Настройка CaptState

Глобальная переменная `CaptState` (задаётся в опциях игры, `Interface.cpp:11574`):

| CaptState | Название | Правило |
|---|---|---|
| **0** | По умолчанию | Всё с флагом CAPTURE захватывается, юниты внутри тоже |
| **1** | No Peasants | Крестьяне исключены из захвата |
| **2** | No Peasants & Centers | Крестьяне + Центры (CenterID) + Шахты (MineID) исключены |
| **3** | Only Artillery | Только артиллерия (`Artilery`) и стены (`Wall`) захватываются |

### Код (`NewMon.cpp:14856–14878`):

```cpp
switch (CaptState) {
    case 1: // No Peasants
        if (OBJ->newMons->Peasant) return;
        break;
    case 2: // No Peasants and City Centers
        if (OBJ->newMons->Peasant || OBJ->newMons->Usage == CenterID || OBJ->newMons->Usage == MineID) return;
        break;
    case 3: // Only Artillery
        if (!(OBJ->newMons->Artilery || OBJ->Wall)) return;
        break;
}
```

---

## Дополнительные флаги

| Флаг | Файл | Строка | Описание |
|---|---|---|---|
| `bool Capture : 1` | MapDiscr.h | 480 | Этот юнит/здание МОЖЕТ быть захвачен |
| `bool CanBeCapturedWhenFree : 1` | MapDiscr.h | 496 | Может быть захвачен когда свободен (для шахт, Ctrl+клик) |
| `bool Building : 1` | MapDiscr.h | 465 | Является зданием |
| `bool Artilery : 1` | MapDiscr.h | 488 | Является артиллерией |
| `bool Wall` | OneObject | - | Является стеной (стены < 1/3 HP тоже теряют защиту) |

---

## Определения Usage ID (`MapDiscr.h:1979–2070`)

```
MelnicaID       0x01    // Мельница
FarmID          0x02    // Ферма
CenterID        0x03    // Ратуша
SkladID         0x04    // Склад
TowerID         0x05    // Башня
FieldID         0x06    // Поле
MineID          0x07    // Шахта
FastHorseID     0x08    // Быстрая конница
MortiraID       0x09    // Мортира
PushkaID        0x0A    // Пушка
GrenaderID      0x0B    // Гренадёр
HardWallID      0x0C    // Крепкая стена
WeakWallID      0x0D    // Слабая стена
LinkorID        0x0E    // Линкор
WeakID          0x0F    // Слабый юнит
FisherID        0x10    // Рыбак
ArtDepoID       0x11    // Артдепо
SupMortID       0x12    // Супер-мортира
PortID          0x13    // Порт
LightInfID      0x14    // Лёгкая пехота
StrelokID       0x15    // Стрелок
HardHorceID     0x16    // Тяжёлая конница
PeasantID       0x17    // Крестьянин
HorseStrelokID  0x18    // Конный стрелок
FregatID        0x19    // Фрегат
GaleraID        0x1B    // Галера
IaxtaID         0x1C    // Яхта
ShebekaID       0x1E    // Шебека
ParomID         0x1F    // Паром
ArcherID        0x20    // Лучник
MultiCannonID   0x1E    // Многоствольная пушка
```

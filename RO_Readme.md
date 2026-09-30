# NanoVI Editor

Un editor text minimalist inspirat de **vi/vim**, implementat în C folosind biblioteca `nano_libc`.

## Caracteristici

- Editare text în memorie
- Moduri multiple de operare:
  - Normal
  - Insert
  - Visual
  - Command
- Salvare fișier
- Ștergere caractere, cuvinte și linii
- Copiere (Yank) și lipire (Put)
- Selecție vizuală cu evidențiere
- Protecție la ieșirea cu modificări nesalvate
- Interfață bazată pe terminal și secvențe ANSI

## Limitări

- Maximum 100 de linii
- Maximum 80 caractere pe linie
- Clipboard limitat la 1024 caractere
- Fișiere limitate la aproximativ 4 KB

---

# Pornire

```bash
editor nume_fisier.txt
```

Dacă nu este specificat niciun fișier:

```bash
editor
```

editorul va deschide automat:

```text
test.txt
```

---

# Moduri de lucru

## NORMAL Mode

Modul implicit la pornirea editorului.

În partea de jos apare:

```text
-- NORMAL --
```

### Navigare

| Tastă | Acțiune |
|---------|----------|
| h | Mută cursorul la stânga |
| l | Mută cursorul la dreapta |
| j | Mută cursorul în jos |
| k | Mută cursorul în sus |

### Comenzi

| Tastă | Acțiune |
|---------|----------|
| i | Intră în Insert Mode |
| v | Intră în Visual Mode |
| d | Începe o comandă de ștergere |
| y | Începe o comandă de copiere |
| p | Lipește conținutul clipboard-ului |
| x | Șterge caracterul curent |
| s | Salvează fișierul |
| : | Intră în Command Mode |

---

## INSERT Mode

Permite introducerea și modificarea textului.

Indicator:

```text
-- INSERT --
```

### Comenzi

| Tastă | Acțiune |
|---------|----------|
| ESC | Revenire în Normal Mode |
| Ctrl+A | Revenire în Normal Mode |
| Enter | Creează o linie nouă |
| Backspace | Șterge caracterul precedent |
| Orice caracter | Inserează caracterul la poziția cursorului |

---

## VISUAL Mode

Permite selectarea textului.

Indicator:

```text
-- VISUAL --
```

Textul selectat este evidențiat prin efectul **Reverse Video** ANSI.

### Comenzi

| Tastă | Acțiune |
|---------|----------|
| h | Mutare stânga |
| j | Mutare jos |
| k | Mutare sus |
| l | Mutare dreapta |
| y | Copiază selecția |
| v | Ieșire din Visual Mode |
| ESC | Ieșire din Visual Mode |

După copiere:

```text
Text yanked
```

---

## COMMAND Mode

Se activează apăsând:

```text
:
```

Indicator:

```text
:
```

Comanda introdusă apare pe ultima linie a ecranului.

### Comenzi disponibile

#### Salvare și ieșire

```text
:wq
```

Salvează fișierul și închide editorul.

#### Ieșire

```text
:q
```

Închide editorul doar dacă nu există modificări nesalvate.

Dacă există modificări:

```text
No write since last change (add ! to override)
```

#### Forțare ieșire

```text
:q!
```

Închide editorul fără salvare.

---

# Operații de ștergere

## Ștergere caracter

În Normal Mode:

```text
x
```

Șterge caracterul aflat sub cursor.

---

## Ștergere linie

În Normal Mode:

```text
dd
```

Șterge întreaga linie curentă.

---

## Ștergere cuvânt

În Normal Mode:

```text
dw
```

Șterge cuvântul de la poziția cursorului.

---

# Operații de copiere și lipire

## Copiere linie

În Normal Mode:

```text
yy
```

Copiază întreaga linie în clipboard.

Mesaj afișat:

```text
Line yanked (yy)
```

---

## Copiere selecție

1. Intră în Visual Mode:

```text
v
```

2. Selectează textul cu:

```text
h j k l
```

3. Copiază:

```text
y
```

---

## Lipire

În Normal Mode:

```text
p
```

Lipește conținutul clipboard-ului la poziția cursorului.

Mesaj afișat:

```text
Text pasted
```

---

# Indicatori de stare

## Fișier modificat

Dacă fișierul conține modificări nesalvate apare:

```text
[+]
```

Exemplu:

```text
-- NORMAL -- [+] File: main.c
```

---

## Fișier salvat

După salvare:

```text
"main.c" [Saved]
```

---

# Structura internă

## Buffer de text

```c
char lines[100][80];
```

- 100 linii maxime
- 80 caractere per linie

---

## Clipboard

```c
char clipboard[1024];
```

Utilizat pentru:

- `yy`
- Visual Yank
- `p`

---

## Moduri editor

```c
typedef enum {
    MODE_NORMAL,
    MODE_INSERT,
    MODE_COMMAND,
    MODE_DELETE_PENDING,
    MODE_VISUAL,
    MODE_YANK_PENDING
} EditorMode;
```

---

# Scurt ghid rapid

```text
i      -> Insert mode
ESC    -> Normal mode

h j k l -> Navigare

x      -> Șterge caracter
dd     -> Șterge linie
dw     -> Șterge cuvânt

yy     -> Copiază linie
v      -> Visual mode
y      -> Copiază selecție
p      -> Lipește

s      -> Salvează

:q     -> Ieșire
:q!    -> Ieșire forțată
:wq    -> Salvează și ieșire
```

---

# Exemplu de utilizare

```text
editor notes.txt

i
Introdu un text nou...
ESC

yy
p

s

:wq
```

Editorul va salva conținutul în `notes.txt` și se va închide.

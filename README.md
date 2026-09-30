# NanoVI Editor

A lightweight text editor inspired by **vi/vim**, implemented in C.

## Features

- In-memory text editing
- Multiple editing modes:
  - Normal
  - Insert
  - Visual
  - Command
- File saving
- Character, word, and line deletion
- Yank (copy) and Put (paste)
- Visual text selection with highlighting
- Protection against quitting with unsaved changes
- Terminal-based interface using ANSI escape sequences

## Limitations

- Maximum 100 lines
- Maximum 80 characters per line
- Clipboard limited to 1024 characters
- Files limited to approximately 4 KB

---

# Getting Started

```bash
editor filename.txt
```

If no filename is specified:

```bash
editor
```

the editor will automatically open:

```text
test.txt
```

---

# Editor Modes

## NORMAL Mode

This is the default mode when the editor starts.

Status line:

```text
-- NORMAL --
```

### Navigation

| Key | Action |
|------|---------|
| h | Move cursor left |
| l | Move cursor right |
| j | Move cursor down |
| k | Move cursor up |

### Commands

| Key | Action |
|------|---------|
| i | Enter Insert Mode |
| v | Enter Visual Mode |
| d | Start a delete command |
| y | Start a yank command |
| p | Paste clipboard contents |
| x | Delete current character |
| s | Save file |
| : | Enter Command Mode |

---

## INSERT Mode

Allows entering and editing text.

Status line:

```text
-- INSERT --
```

### Commands

| Key | Action |
|------|---------|
| ESC | Return to Normal Mode |
| Ctrl+A | Return to Normal Mode |
| Enter | Create a new line |
| Backspace | Delete previous character |
| Any character | Insert character at cursor position |

---

## VISUAL Mode

Allows text selection.

Status line:

```text
-- VISUAL --
```

Selected text is highlighted using ANSI **Reverse Video** mode.

### Commands

| Key | Action |
|------|---------|
| h | Move left |
| j | Move down |
| k | Move up |
| l | Move right |
| y | Copy selected text |
| v | Exit Visual Mode |
| ESC | Exit Visual Mode |

After copying text:

```text
Text yanked
```

---

## COMMAND Mode

Activated by pressing:

```text
:
```

Status line:

```text
:
```

The command being typed is displayed on the bottom line.

### Available Commands

#### Save and Quit

```text
:wq
```

Saves the file and exits the editor.

#### Quit

```text
:q
```

Exits only if there are no unsaved changes.

If changes exist:

```text
No write since last change (add ! to override)
```

#### Force Quit

```text
:q!
```

Exits without saving.

---

# Delete Operations

## Delete Character

In Normal Mode:

```text
x
```

Deletes the character under the cursor.

---

## Delete Line

In Normal Mode:

```text
dd
```

Deletes the entire current line.

---

## Delete Word

In Normal Mode:

```text
dw
```

Deletes the word starting at the cursor position.

---

# Copy and Paste Operations

## Copy Line

In Normal Mode:

```text
yy
```

Copies the entire current line into the clipboard.

Displayed message:

```text
Line yanked (yy)
```

---

## Copy Selection

1. Enter Visual Mode:

```text
v
```

2. Select text using:

```text
h j k l
```

3. Copy selection:

```text
y
```

---

## Paste

In Normal Mode:

```text
p
```

Pastes clipboard contents at the cursor position.

Displayed message:

```text
Text pasted
```

---

# Status Indicators

## Modified File

When the file contains unsaved changes, the following indicator appears:

```text
[+]
```

Example:

```text
-- NORMAL -- [+] File: main.c
```

---

## File Saved

After a successful save:

```text
"main.c" [Saved]
```

---

# Internal Structure

## Text Buffer

```c
char lines[100][80];
```

- Maximum 100 lines
- Maximum 80 characters per 

# tereminder
Reminders in the terminal.

## Requirements
To build you'll need:
- GCC
- GNU Make

## Building
To build tereminder, simply use:
```bash
make
```

To install tereminder, use:
```bash
make install
```

To uninstall tereminder, use:
```bash
make uninstall
```

To clean up build artifacts (objects + binary):
```bash
make clean
```

## Usage
Currently, you will need to manually edit your `.reminders` file
and there is no way to set where your `reminders` file is other
than modifying the source code and rebuilding (there are plans
to change this). It will currently default to `$HOME/.reminders`.

The format of a reminder is as follows:
```
YYYY-MM-DDTHH:MM:SS <R|L|M|H> <title>:<description>
|_________________| |_______|        |
|                   |                |_splits the title
|_date              |_severity         from the description
|_24 hour format    |_R: routine       on the first colon
                    |_L: low
                    |_M: medium
                    |_H: high
```

As an example:
```
2024-11-02T16:00:00 M Collect parcel: From the post office on Example St.
```

You can then use:
```bash
tereminder
```
To view your reminders.

Setting up `tereminder` to show up every time you open a new shell is simple,
```bash
#bash
echo "tereminder" >> ~/.bashrc

# zsh
echo "tereminder" >> ~/.zshrc
```
just add `tereminder` to the bottom of your shell's run commands.

# Notice
`tereminder` is still in development, updates may break your reminder file
and could introduce bugs.

# Todo
- Recurring reminders
- CLI or TUI for creating, updating, and deleting reminders
- Packaging for AUR and other distro repositories
- Potential optimisations
- More options for reminders (i.e. todo lists, no description, no title)

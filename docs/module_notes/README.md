# docs/module_notes

One evidence note per module. Module 2's filename is set by the assignment:

    module_02_instrument_pieces.md    <- evidence for C2, demonstrated S6, Wed 16 September

## What a note is

The argument built on the evidence, not the evidence itself. It links to code, embeds figures,
quotes numbers and explains them. Sketches are linked, never pasted — the assignment says so
explicitly, and a note full of code is unreadable.

Sections with no measurement behind them say so in one line rather than carrying blanks. §3 is
currently the whole of Part 3, marked not performed.

## Relative links from this folder

    ![caption](../../figures/module_02/m02_fig01_thermistor_divider.jpg)
    [sketch](../../arduino/m02_thermistor_serial/m02_thermistor_serial.ino)

Two levels up, because this file sits two folders deep. Check every link renders on GitHub
before exporting.

## Exporting

    cd docs/module_notes
    pandoc module_02_instrument_pieces.md -o C2_Lastname_Lastname.pdf \
      --pdf-engine=wkhtmltopdf --resource-path=.:../..

The `--resource-path` is what lets the relative image paths resolve; without it the figures
silently vanish from the PDF. Export **after** the final push so any cited commit hash is the
hash of the pushed checkpoint, then open the PDF and confirm the figures actually rendered.

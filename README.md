# QMentat - Mental Arithmetic Practice

**We're back! Everything updated to Qt6. Sorry for the absence.**

QMentat is a program to help you practice mental arithmetic. It uses arbitrary length fixed point arithmetic and can handle any size numbers, only limited by the size of your screen (and in the case of division with an integer result, the speed at which the number can be factored, and the square root of the largest number). It is also quite configurable, allowing you to tailor the questions to your needs.

For instructions on building and installing, please see the INSTALL file.

I'll fancy this up with a few screenshots shortly, when I have time.

The home page for QMentat is at: [QMentat Home Page](http://realgrep.github.io/QMentat/)

## Contributing Translations

QMentat currently ships with English, French, Spanish, Italian, and German. New languages and corrections to existing ones are very welcome!

The translation files are simple XML (`.ts` format) and no C++ knowledge is needed.

**To add or improve a translation:**

1. Find the relevant file in the repo root — e.g. `qmentat_fr.ts` for French
2. To start a new language, copy an existing `.ts` file and rename it (e.g. `qmentat_pt.ts` for Portuguese)
3. Open it in a text editor or [Qt Linguist](https://doc.qt.io/qt-6/linguist-translators.html) (free, purpose-built GUI for this)
4. Each `<message>` block contains a `<source>` (English) and a `<translation>` — fill in the translation and remove the `type="unfinished"` attribute
5. Submit a pull request, or simply [file an issue](../../issues) with the corrected strings if you prefer not to use git

To generate a new `.qm` binary from your `.ts` file: `lrelease qmentat_XX.ts`

Michel Dusseault <mike.dusseault@gmail.com>

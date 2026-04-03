# TF2-Medieval-chat-parser

  Replaces set words with their medieval counterpart
  The set words that get replaced and their replacements are in a dict.json (can be freely added)
  In TF2 the replacing has a chance built in, this swaps always :) (not counting prepend/append)
  Can be used as a basic present->medieval 'translator'

## How to use:
1. Download the latest release and extract it.
2. Ensure both `TF2-Medieval-chat-parser.exe` and `dict.json` are in the same folder.
3. Run the executable.
4. Type your sentences and press **Enter** to see the magic happen.

## Requirements:
* **OS:** Windows (64-bit)
* **Files:** `dict.json` (must be present in the application directory)

## Troubleshooting:
* Windows Defender might flag the executable as a "Trojan:Win32/Wacatac.C!ml" due to the lack of a digital signature and static linking (/MT).
  This is a false positive. You may need to allow the file in your antivirus settings.

# decrypt-chrome-passwords-and-cookies

Decrypt encrypted passwords and cookies saved in all versions of Chrome.

## Overview

This tool allows you to decrypt encrypted passwords and cookies saved in all versions of Chrome. It provides options to retrieve username and password information as well as cookies information for the URLs saved in Chrome.

## Usage

To use this tool, execute the following command: decrypt-chrome-passwords-and-cookies_x64.exe --help

Options:

- `-d`, `--credentials`: Get username and password information for the URL saved in Chrome.
- `-c`, `--cookies`: Get cookies information for the URL saved in Chrome.

## Requirements

To compile and use this tool, you need the following libraries:

- [Crypto++® Library 8.7](https://www.cryptopp.com/)
- [cJSON](https://github.com/DaveGamble/cJSON)
- [sqlite3](https://www.sqlite.org/index.html)

## Installation

1. Clone the repository: https://github.com/CaptanMoss/decrypt-chrome-passwords-and-cookies.git
2. Install the required libraries:
    - Crypto++® Library 8.7
    - cJSON
    - sqlite3

3. Build the executable using your preferred compiler.

## Example

Here is an example command to retrieve username and password information:

```bash
decrypt-chrome-passwords-and-cookies_x64.exe -d
```

This command will retrieve username and password information for the URLs saved in Chrome.

## Demo

![](2023-09-22-15-33-25.gif)

## Contributing

🤝 Contributions are welcome! If you'd like to contribute to this project, please open a pull request or create an issue to discuss your suggestions.


 




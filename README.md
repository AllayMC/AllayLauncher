# AllayLauncher

The launcher for [Allay](https://github.com/AllayMC/Allay) that aims to be small, fast and ready out of the box⚡

## Features

- [x] Automatically check java installation status.
- [x] Manage allay server.
- [x] Use a proxy to download updates.
- [x] Broad platform support.
- [ ] Managed java environment.

## Installation

Install from one command line:

Linux:

```bash
wget -qO- https://raw.githubusercontent.com/AllayMC/AllayLauncher/refs/heads/main/scripts/install_linux.sh | bash
```

Windows(PowerShell):

```powershell
Invoke-Expression (Invoke-WebRequest -Uri "https://raw.githubusercontent.com/AllayMC/AllayLauncher/refs/heads/main/scripts/install_windows.ps1").Content
```

MacOS:

```bash
wget -qO- https://raw.githubusercontent.com/AllayMC/AllayLauncher/refs/heads/main/scripts/install_macos.sh | bash
```

## Commands

Start the server (will download the latest version of allay if it is not installed):

```bash
allay
```

This is equal to:

```bash
allay --run --update
```

Allows concatenation of single-character arguments:

```bash
allay -ru
```

Available options:

- `-r`, `--run` Run allay server
- `-u`, `--update` Check for update before starting the server
- `-n`, `--nightly` Use nightly build
- `-d`, `--daemon` Use deamon mode, which the launcher will restart the server after server stopped
- `-a`, `--args` Pass arguments to java

Pass custom parameters to the JVM, such as memory limits:

> [!IMPORTANT]
> The "-" before the parameter will be automatically added.

```bash
allay -ru --args Xms1G Xmx4G
```

Start the server in daemon mode:

```bash
allay -rud
```

Start the server, but do not check for update:

```bash
allay --run
```

## License

Copyright **:copyright: 2023-2024 AllayMC**, all rights reserved.

If not otherwise specified, project content is open source under the LGPL-3.0 license.

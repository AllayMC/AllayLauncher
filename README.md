# AllayLauncher

Launcher of [Allay](https://github.com/AllayMC/Allay) which aims to be small and ready out of the box.

## Features

- [ ] Manage Allay
- [ ] Webhook support for auto restart

## Installation

Install from one command line:

```bash
WIP
```

## Commands

Start the server (will download the latest nightly version of allay if allay haven't been downloaded):
```bash
allay
```

This is equal to:
```bash
allay -u -r -n
```

Available options:

- `-u` Check for update before starting the server
- `-r` Run allay server
- `-n` Use nightly build (default)
- `-a` Pass arguments to java

Start the server with specified maximum memory limit:

```bash
allay -u -r -n -a "-Xms1G -Xmx4G"
```

## License

Copyright **:copyright: 2023-2024 AllayMC**, all rights reserved.

If not otherwise specified, project content is open source under the LGPL-3.0 license.
